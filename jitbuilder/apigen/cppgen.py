import sys
import os
import json
import unittest

copyright_header = """\
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

def needs_impl(t):
    return t in [ "BytecodeBuilder"
                , "IlBuilder"
                , "MethodBuilder"
                , "IlType"
                , "IlValue"
                , "TypeDictionary"
                , "VirtualMachineState"
                ]

def get_impl_type(t):
    return "TR::{} *".format(t) if needs_impl(t) else type_map[t]

def grab_impl(v, t):
    return "reinterpret_cast<{t}>({v} != NULL ? {v}->_impl : NULL)".format(v=v,t=get_impl_type(t)) if needs_impl(t) else v

def generate_parm_list(parms_desc):
    gen_parm = lambda p: "{t} {n}".format(t=type_map[p["type"]],n=p["name"])
    return ", ".join([ gen_parm(p) for p in parms_desc ])

def generate_arg_list(parms_desc):
    return ", ".join([ grab_impl(a["name"], a["type"]) for a in parms_desc ])

# header utilities ###################################################

def generate_field_decl(field, with_visibility = True):
    """Generate a field from its description"""
    t = type_map[field["type"]]
    n = field["name"]
    v = "public: " if with_visibility else ""
    return "{visibility}{type} {name};\n".format(visibility=v, type=t, name=n)

def generate_service_decl(service, with_visibility = True):
    """Generate a service from tis description"""
    vis = "" if not with_visibility else "protected: " if "protected" in service["flags"] else "public: "
    static = "static" if "static" in service["flags"] else ""
    ret = type_map[service["return"]]
    name = service["name"]
    parms = generate_parm_list(service["parms"])
    return "{visibility}{qualifier} {rtype} {name}({parms});\n".format(visibility=vis, qualifier=static, rtype=ret, name=name, parms=parms)

def generate_ctor_decl(ctor_desc, class_name):
    v = "protected: " if "protected" in ctor_desc["flags"] else "public: "
    parms = generate_parm_list(ctor_desc["parms"])
    decls = "{visibility}{name}({parms});\n".format(visibility=v, name=class_name, parms=parms)

    parms = "void * impl" + "" if parms == "" else ", " + parms
    return decls + "public: {name}({parms});\n".format(name=class_name, parms=parms)

def generate_dtor_decl(class_desc):
    return "public: ~{cname}();\n".format(cname=class_desc["name"])

def write_class_def(writer, class_desc):
    name = class_desc["name"]
    has_extras = "has_extras_header" in class_desc["flags"]

    writer.write("class {name} {{\n".format(name=name))

    # write fields
    for field in class_desc["fields"]:
        decl = generate_field_decl(field)
        writer.write(decl)

    # write needed impl field
    writer.write("public: void* _impl;\n")

    for ctor in class_desc["constructors"]:
        decls = generate_ctor_decl(ctor, name)
        writer.write(decls)

    # write impl init service delcaration
    writer.write("protected: void initializeFromImpl(void * impl);\n")

    dtor_decl = generate_dtor_decl(class_desc)
    writer.write(dtor_decl)

    for service in class_desc["services"]:
        decl = generate_service_decl(service)
        writer.write(decl)

    if has_extras:
        writer.write(generate_include('{}ExtrasInsideClass.hpp'.format(class_desc["name"])))

    writer.write('};\n')

# source utilities ###################################################

def write_service_impl(writer, desc, class_name):
    rtype = type_map[desc["return"]]
    name = desc["name"]
    parms = generate_parm_list(desc["parms"])
    writer.write("{rtype} {cname}::{name}({parms}) {{\n".format(rtype=rtype, cname=class_name, name=name, parms=parms))

    args = generate_arg_list(desc["parms"])#", ".join([ grab_impl(a["name"], a["type"]) for a in desc["parms"] ])
    impl_call = "reinterpret_cast<TR::{cname} *>(_impl)->{sname}({args})".format(cname=class_name,sname=name,args=args)
    if "none" == desc["return"]:
        writer.write(impl_call + ";\n")
    elif needs_impl(desc["return"]):
        writer.write("{rtype} implRet = {call};\n".format(rtype=get_impl_type(desc["return"]), call=impl_call))
        writer.write("GET_CLIENT_OBJECT(clientObj, {t}, implRet);\n".format(t=desc["return"]))
        writer.write("return clientObj;\n")
    else:
        writer.write("return " + impl_call + ";\n")

    writer.write("}\n")

def write_class_impl(writer, class_desc):
    cname = class_desc["name"]

    # write constructor definitions
    for ctor in class_desc["constructors"]:
        parms = generate_parm_list(ctor["parms"])
        writer.write("{cname}::{cname}({parms}) {{\n".format(cname=cname, parms=parms))
        args = generate_arg_list(ctor["parms"])
        writer.write("_impl = new TR::{cname}({args});\n".format(cname=cname, args=args))
        writer.write("reinterpret_cast<TR::{cname} *>(_impl)->setClient(this);\n".format(cname=cname));
        writer.write("initializeFromImpl(_impl);\n")
        writer.write("}\n\n")

        parms = "void * impl" + "" if parms == "" else "," + parms
        writer.write("{cname}::{cname}({parms}) {{\n".format(cname=cname, parms=parms))
        args = "impl" + "" if args == "" else "," + args
        writer.write("if (impl != NULL) {\n")
        writer.write("reinterpret_cast<TR::{cname} *>(impl)->setClient(this);\n".format(cname=cname));
        writer.write("initializeFromImpl(impl);\n")
        writer.write("}\n")
        writer.write("}\n")
    writer.write("\n")

    writer.write("void {cname}::initializeFromImpl(void * impl) {{\n".format(cname=cname))
    writer.write("_impl = impl;\n")
    for field in class_desc["fields"]:
        fmt = "GET_CLIENT_OBJECT(clientObj_{fname}, {ftype}, reinterpret_cast<TR::{cname} *>(_impl)->{fname});\n"
        writer.write(fmt.format(fname=field["name"], ftype=field["type"], cname=cname))
        writer.write("{fname} = clientObj_{fname};\n".format(fname=field["name"]))
    writer.write("}\n")

    # write destructor definition
    writer.write("{cname}::~{cname}() {{}}\n".format(cname=cname))
    writer.write("\n")

    # write service definitions
    for s in class_desc["services"]:
        write_service_impl(writer, s, cname)
        writer.write("\n")

# main generator #####################################################

def write_class_header(writer, class_desc, namespaces, class_names):
    has_extras = "has_extras_header" in class_desc["flags"]

    writer.write(copyright_header)
    writer.write("\n")

    if has_extras:
        writer.write(generate_include('{}ExtrasOutsideClass.hpp'.format(class_desc["name"])))
    writer.write("\n")

    # open each nested namespace
    for n in namespaces:
        writer.write("namespace {} {{\n".format(n))
    writer.write("\n")

    writer.write("// forward declarations for all API classes\n")
    for c in class_names:
        writer.write("class {};\n".format(c))
    writer.write("\n")

    write_class_def(writer, class_desc)
    writer.write("\n")

    # close each openned namespace
    for n in reversed(namespaces):
        writer.write("}} // {}\n".format(n))

def write_class_source(writer, class_desc, namespaces, class_names):
    writer.write(copyright_header)
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

    # open each nested namespace
    for n in namespaces:
        writer.write("namespace {} {{\n".format(n))
    writer.write("\n")

    write_class_impl(writer, class_desc)

    # close each openned namespace
    for n in reversed(namespaces):
        writer.write("}} // {}\n".format(n))

def write_class(dest_path, class_desc, namespaces, class_names):
    cname = class_desc["name"]
    header_path = os.path.join(dest_path, cname + ".hpp")
    source_path = os.path.join(dest_path, cname + ".cpp")
    with open(header_path, "w") as writer:
        write_class_header(writer, class_desc, namespaces, class_names)
    with open(source_path, "w") as writer:
        write_class_source(writer, class_desc, namespaces, class_names)

if __name__ == "__main__":
    api = {}
    with open("jitbuilder.api.json") as api_src:
        api = json.load(api_src)
    namespaces = api["namespace"]
    class_names = [ c["name"] for c in api["classes"] ]
    for class_desc in api["classes"]:
        write_class("./client", class_desc, namespaces, class_names)

