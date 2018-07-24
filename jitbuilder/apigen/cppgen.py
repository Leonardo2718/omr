import sys
import json
import unittest

copyright_header = """
/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/
"""

# Generic utilities ##################################################

# mapping between API type descriptions and C++ data types
type_map = { "none": "void"
           , "boolean": "bool"
           , "integer": "size_t"
           , "int8": "int8_t"
           , "int16": "int16_t"
           , "int32": "int32_t"
           , "int64": "int64_t"
           , "float": "float"
           , "double": "double"
           , "pointer": "void *"
           , "ppointer": "void **"
           , "unsignedInteger": "size_t" # really?! should be `usize_t`
           , "constString": "const char *"
           , "string": "char *"
           , "booleanArray": "bool *"
           , "int32Array": "int32_t *"
           #, "vararg": "..."
           , "BytecodeBuilder": "BytecodeBuilder *"
           , "BytecodeBuilderArray": "BytecodeBuilder **"
           , "BytecodeBuilderByRef": "BytecodeBuilder **"
           , "BytecodeBuilderImpl": "TR::BytecodeBuilder *"
           , "ppBytecodeBuilder": "BytecodeBuilder **"
           , "IlBuilder": "BytecodeBuilder *"
           , "IlBuilderArray": "IlBuilder **"
           , "IlBuilderByRef": "IlBuilder **"
           , "IlBuilderImpl": "TR::IlBuilder *"
           , "ppIlBuilder": "IlBuilder **"
           , "MethodBuilder": "MethodBuilder *"
           , "MethodBuilderImpl": "TR::MethodBuilder *"
           , "IlReference": "IlReference *"
           , "IlType": "IlType *"
           , "IlTypeArray": "IlType **"
           , "IlTypeImplr": "TR::IlType *"
           , "IlValue": "IlValue *"
           , "IlValueArray": "IlValue **"
           , "IlValueImpl": "TR::IlValue *"
           , "ThunkBuilder": "ThunkBuilder *"
           , "TypeDictionary": "TypeDictionary *"
           , "TypeDictionaryImpl": "TR::TypeDictionary *"
           , "VirtualMachineOperandArray": "VirtualMachineOperandArray *"
           , "VirtualMachineOperandStack": "VirtualMachineOperandStack *"
           , "VirtualMachineRegister": "VirtualMachineRegister *"
           , "VirtualMachineRegisterInStruct": "VirtualMachineRegisterInStruct *"
           , "VirtualMachineState": "VirtualMachineState *"
           , "VirtualMachineStateImpl": "TR::VirtualMachineState *"
           }

def generate_include(path):
    return '#include "{}"\n'.format(path)

# header utilities ###################################################

def write_field(writer, field, with_visibility = True):
    """Generate a field from its description"""
    t = type_map[field["type"]]
    n = field["name"]
    v = "public" if with_visibility else ""
    writer.write("{visibility}: {type} {name}; ".format(visibility=v, type=t, name=n))

def write_service(writer, service, with_visibility = True):
    """Generate a service from tis description"""
    vis = "" if not with_visibility else "protected" if "protected" in service["flags"] else "public"
    static = "static" if "static" in service["flags"] else ""
    ret = type_map[service["return"]]
    name = service["name"]
    parms = ", ".join([ type_map[p["type"]] for p in service["parms"] ])
    writer.write("{visibility}: {qualifier} {rtype} {name}({parms});".format(visibility=vis, qualifier=static, rtype=ret, name=name, parms=parms))

def write_ctor_decl(writer, ctor_desc, class_name):
    v = "protected" if "protected" in ctor_desc["flags"] else "public"
    parms = ", ".join([ type_map[p["type"]] for p in ctor_desc["parms"] ])
    writer.write("{visibility}: {name}({parms});\n".format(visibility=v, name=class_name, parms=parms))

def write_class(writer, class_desc):
    name = class_desc["name"]
    has_extras = "has_extras_header" in class_desc["flags"]

    if has_extras:
        writer.write(generate_include('ExtrasOutsideClass.hpp'))
    writer.write("class {name} {{\n".format(name=name))

    # write fields
    for field in class_desc["fields"]:
        write_field(writer, field)
        writer.write("\n")

    # write needed impl field
    writer.write("public: void* _impl\n")

    for ctor in class_desc["constructors"]:
        write_ctor_decl(writer, ctor, name)

    # write destructor declaration
    writer.write("public: ~{name}();\n".format(name=name))

    for service in class_desc["services"]:
        write_service(writer, service)
        writer.write("\n")

    if has_extras:
        writer.write(generate_include('ExtrasInsideClass.hpp'))

    writer.write('};\n')

def write_header(writer, api):
    writer.write(copyright_header)

    for n in api["namespace"]:
        writer.write("namespace {} {{\n".format(n))

    # write call forward declarations
    for c in api["classes"]:
        name = c["name"]
        writer.write("class {};\n".format(name))

    # write classes
    for c in api["classes"]:
        write_class(target, c)

    for n in api["namespace"]:
        target.write("}} // {}\n".format(n))

# source utilities ###################################################

def needs_impl(t):
    return t in [ "BytecodeBuilder"
                , "IlBuilder"
                , "MethodBuilder"
                , "IlType"
                , "IlValue"
                , "TypeDictionary"
                , "VirtualMachineState"
                ]

def grab_impl(v, t):
    return "{v} != NULL ? {v}->_impl : NULL".format(v=v) if needs_impl(t) else v

def write_service_impl(writer, desc, class_name):
    rtype = type_map[desc["return"]]
    name = desc["name"]
    gen_parm = lambda p: "{t} {n}".format(t=type_map[p["type"]],n=p["name"])
    parms = ", ".join([ gen_parm(p) for p in desc["parms"] ])
    writer.write("{rtype} {cname}::{name}({parms}) {{\n".format(rtype=rtype, cname=class_name, name=name, parms=parms))

    args = ", ".join([ grab_impl(a["name"], a["type"]) for a in desc["parms"] ])
    impl_call = "reinterpret_cast<TR::{cname} *>(_impl)->{sname}({args})".format(cname=class_name,sname=name,args=args)
    if "none" == desc["return"]:
        writer.write(impl_call + ";\n");
    else:
        writer.write("{rtype} implRet = {call};\n".format(rtype=rtype, call=impl_call))
        writer.write("GET_CLIENT_OBJECT(clientObj, {t}, implRet);\n".format(t=desc["return"]))
        writer.write("return clientObj;\n")

    writer.write("}\n")

def write_class_impl(writer, class_desc):
    for s in class_desc["services"]:
        write_service_impl(writer, s, class_desc["name"])
        writer.write("\n")

def write_source(writer, api):
    target.write(copyright_header)
    writer.write("\n")

    # don't bother checking what headers are needed, include everything
    trheaders = [ "ilgen/BytecodeBuilder.hpp"
                , "ilgen/IlBuilder.hpp"
                , "ilgen/IlType.hpp"
                , "ilgen/IlValue.hpp"
                , "ilgen/MethodBuilder.hpp"
                , "ilgen/ThunkBuilder.hpp"
                , "ilgen/TypeDictionary.hpp"
                , "ilgen/VirtualMachineOperandArray.hpp"
                , "ilgen/VirtualMachineOperandStack.hpp"
                , "ilgen/VirtualMachineRegister.hpp"
                , "ilgen/VirtualMachineRegisterInStruct.hpp"
                , "ilgen/VirtualMachineState.hpp"
                , "client/cpp/Callbacks.hpp"
                , "client/cpp/Macros.hpp"
                , "release/cpp/include/BytecodeBuilder.hpp"
                , "release/cpp/include/IlBuilder.hpp"
                , "release/cpp/include/IlReference.hpp"
                , "release/cpp/include/IlType.hpp"
                , "release/cpp/include/IlValue.hpp"
                , "release/cpp/include/MethodBuilder.hpp"
                , "release/cpp/include/ThunkBuilder.hpp"
                , "release/cpp/include/TypeDictionary.hpp"
                , "release/cpp/include/VirtualMachineOperandArray.hpp"
                , "release/cpp/include/VirtualMachineOperandStack.hpp"
                , "release/cpp/include/VirtualMachineRegister.hpp"
                , "release/cpp/include/VirtualMachineRegisterInStruct.hpp"
                , "release/cpp/include/VirtualMachineState.hpp"
                ]
    for h in trheaders:
        writer.write(generate_include(h))
    writer.write("\n")

    for n in api["namespace"]:
        target.write("namespace {} {{\n".format(n))
    writer.write("\n")

    for c in api["classes"]:
        write_class_impl(writer, c)

    for n in api["namespace"]:
        target.write("}} // {}\n".format(n))

# main generator #####################################################

if __name__ == "__main__":
    with open("jitbuilder.api.json") as api_src:
        api = json.load(api_src)
        with open("JitBuilder.hpp", "w") as target:
            write_header(target, api)
        with open("JitBuilder.cpp", "w") as target:
            write_source(target, api)

