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
           , "ppBytecodeBuilder": "BytecdoeBuilder **"
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
    v = "public:" if with_visibility else ""
    writer.write(" ".join([v, t, n, ";"]))

def generate_fields(writer, fields, with_visibility = True):
    """Generate a list of fields"""
    for field in fields:
        generate_field(writer, field, with_visibility)
        writer.write("\n")

with open("jitbuilder.api.json") as f:
    obj = json.load(f)
    generate_fields(sys.stdout, obj["classes"][0]["fields"])
