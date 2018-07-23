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

def generate_field(writer, field, with_visibility = True):
    """Generate a field from its description"""
    t = type_map[field["type"]]
    n = field["name"]
    v = "public" if with_visibility else ""
    writer.write("{visibility}: {type} {name}; ".format(visibility=v, type=t, name=n))

def generate_fields(writer, fields, with_visibility = True):
    """Generate a list of fields"""
    for field in fields:
        generate_field(writer, field, with_visibility)
        writer.write("\n")
    writer.write("public: void* _impl\n")

def generate_service(writer, service, with_visibility = True):
    """Generate a service from tis description"""
    vis = "" if not with_visibility else "protected" if "protected" in service["flags"] else "public"
    static = "static" if "static" in service["flags"] else ""
    ret = type_map[service["return"]]
    name = service["name"]
    parms = ", ".join([ type_map[p["type"]] for p in service["parms"] ])
    writer.write("{visibility}: {qualifier} {rtype} {name}({parms});".format(visibility=vis, qualifier=static, rtype=ret, name=name, parms=parms))

def generate_services(writer, services, with_visibility = True):
    """Generate a list of services"""
    for service in services:
        generate_service(writer, service, with_visibility)
        writer.write("\n")

def generate_include(path):
    return '#include "{}"\n'.format(path)

def generate_class(writer, class_desc):
    name = class_desc["name"]
    has_extras = "has_extras_header" in class_desc["flags"]

    if has_extras:
        writer.write(generate_include('ExtrasOutsideClass.hpp'))
    writer.write("class {name} {{\n".format(name=name))
    generate_fields(writer, class_desc["fields"])
    generate_services(writer, class_desc["services"])
    if has_extras:
        writer.write(generate_include('ExtrasInsideClass.hpp'))
    writer.write('};\n')

def generate_class_forward_decl(writer, class_desc):
    name = class_desc["name"]
    writer.write("class {};\n".format(name))

with open("jitbuilder.api.json") as api_src:
    api = json.load(api_src)

    with open("JitBuilder.hpp", "w") as target:
        target.write(copyright_header)

        for n in api["namespace"]:
            target.write("namespace {} {{\n".format(n))

        for c in api["classes"]:
            generate_class_forward_decl(target, c)
        for c in api["classes"]:
            generate_class(target, c)

        for n in api["namespace"]:
            target.write("}} // {}\n".format(n))

    with open("JitBuilder.cpp", "w") as target:
        target.write(copyright_header)

        for n in api["namespace"]:
            target.write("namespace {} {{\n".format(n))

        for n in api["namespace"]:
            target.write("}} // {}\n".format(n))

