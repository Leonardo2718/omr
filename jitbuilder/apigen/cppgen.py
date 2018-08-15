import sys
import os
import json
from functools import reduce
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
builtin_type_map = { "none": "void"
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
                   , "unsignedInteger": "size_t"
                   , "constString": "const char *"
                   , "string": "char *"
                   }

class_type_map = { "BytecodeBuilder": "BytecodeBuilder"
                 , "IlBuilder": "IlBuilder"
                 , "MethodBuilder": "MethodBuilder"
                 , "JBCase": "IlBuilder::JBCase"
                 , "JBCondition": "IlBuilder::JBCondition"
                 , "IlReference": "IlReference"
                 , "IlType": "IlType"
                 , "IlValue": "IlValue"
                 , "ThunkBuilder": "ThunkBuilder"
                 , "TypeDictionary": "TypeDictionary"
                 , "VirtualMachineOperandArray": "VirtualMachineOperandArray"
                 , "VirtualMachineOperandStack": "VirtualMachineOperandStack"
                 , "VirtualMachineRegister": "VirtualMachineRegister"
                 , "VirtualMachineRegisterInStruct": "VirtualMachineRegisterInStruct"
                 , "VirtualMachineState": "VirtualMachineState"
                 }

inheritance_table = { "MethodBuilder": "IlBuilder" }

def generate_include(path):
    return '#include "{}"\n'.format(path)

def is_class(t):
    return t in class_type_map

def get_class_name(t):
    assert is_class(t), "Cannot get name for non-class type {}".format(t)
    return class_type_map[t]

def get_impl_class_name(t):
    assert is_class(t), "Cannot get name for non-class type {}".format(t)
    return "TR::{}".format(class_type_map[t])

def as_client_type(t):
    return "{} *".format(get_class_name(t)) if is_class(t) else builtin_type_map[t]

def as_impl_type(t):
    return "{} *".format(get_impl_class_name(t)) if is_class(t) else builtin_type_map[t]

def base_of(c):
    return base_of(inheritance_table[c]) if c in inheritance_table else c

def to_impl_cast(c, v):
    assert is_class(c), "Can only generate cast to implementation type for class types, not {}".format(t)
    b = base_of(c)
    v = v if b == c else "static_cast<{b}>({v})".format(b=as_impl_type(b), v=v)
    return "static_cast<{t}>({v})".format(t=as_impl_type(c),v=v)

def to_opaque_cast(v, from_c):
    assert is_class(from_c), "Can only generate cast to implementation type for class types, not {}".format(t)
    b = base_of(from_c)
    v = v if b == from_c else "static_cast<{b}>({v})".format(b=as_impl_type(b), v=v)
    return "static_cast<void *>({v})".format(v=v)

def to_client_cast(t, v):
    assert is_class(t), "Can only generate cast to client type for class types, not {}".format(t)
    return "static_cast<{t}>({v})".format(t=as_client_type(t),v=v)

def grab_impl(v, t):
    return to_impl_cast(t, "{v} != NULL ? {v}->_impl : NULL".format(v=v)) if is_class(t) else v

def is_in_out(parm_desc):
    return "attributes" in parm_desc and "in_out" in parm_desc["attributes"]

def is_array(parm_desc):
    return "attributes" in parm_desc and "array" in parm_desc["attributes"]

def is_vararg(parm_desc):
    return reduce(lambda l,r: l or r, ["can_be_vararg" in p["attributes"] for p in parm_desc["parms"] if "attributes" in p], False)

def generate_parm(parm_desc):
    fmt = "{t}* {n}" if is_in_out(parm_desc) or is_array(parm_desc) else "{t} {n}"
    t = as_client_type(parm_desc["type"])
    return fmt.format(t=t,n=parm_desc["name"])

def generate_parm_list(parms_desc):
    return ", ".join([ generate_parm(p) for p in parms_desc ])

def generate_vararg_parm_list(parms_desc):
    return ", ".join(["..." if "attributes" in p and "can_be_vararg" in p["attributes"] else generate_parm(p) for p in parms_desc])

def generate_arg(parm_desc):
    n = parm_desc["name"]
    t = parm_desc["type"]
    return (n + "Arg") if is_in_out(parm_desc) or is_array(parm_desc) else grab_impl(n,t)

def generate_arg_list(parms_desc):
    return ", ".join([ generate_arg(a) for a in parms_desc ])

def callback_registrar_name(callback_desc):
    return "setClientCallback_" + callback_desc["name"]

def callback_thunk_name(class_desc, callback_desc):
    return "{cname}Callback_{callback}".format(cname=class_desc["name"], callback=callback_desc["name"])

def list_str_prepend(pre, list_str):
    return pre + ("" if list_str == "" else ", " + list_str)

# header utilities ###################################################

def generate_field_decl(field, with_visibility = True):
    """Generate a field from its description"""
    t = as_client_type(field["type"])
    n = field["name"]
    v = "public: " if with_visibility else ""
    return "{visibility}{type} {name};\n".format(visibility=v, type=t, name=n)

def generate_service_decl(service, with_visibility = True, is_callback=False):
    """Generate a service from tis description"""
    vis = "" if not with_visibility else "protected: " if "protected" in service["flags"] else "public: "
    static = "static" if "static" in service["flags"] else ""
    qual = ("virtual" if is_callback else " ") + static
    ret = as_client_type(service["return"])
    name = service["name"]
    parms = generate_parm_list(service["parms"])

    decl = "{visibility}{qualifier} {rtype} {name}({parms});\n".format(visibility=vis, qualifier=qual, rtype=ret, name=name, parms=parms)
    if is_vararg(service):
        parms = generate_vararg_parm_list(service["parms"])
        decl = decl + "{visibility}{qualifier} {rtype} {name}({parms});\n".format(visibility=vis,qualifier=qual,rtype=ret,name=name,parms=parms)

    return decl

def generate_ctor_decl(ctor_desc, class_name):
    v = "protected: " if "protected" in ctor_desc["flags"] else "public: "
    parms = generate_parm_list(ctor_desc["parms"])
    decls = "{visibility}{name}({parms});\n".format(visibility=v, name=class_name, parms=parms)
    return decls

def generate_dtor_decl(class_desc):
    return "public: ~{cname}();\n".format(cname=class_desc["name"])

def write_class_def(writer, class_desc):
    name = class_desc["name"]
    has_extras = "has_extras_header" in class_desc["flags"]

    inherit = ": public {parent}".format(parent=get_class_name(class_desc["extends"])) if "extends" in class_desc else ""
    writer.write("class {name}{inherit} {{\n".format(name=name,inherit=inherit))

    # write nested classes
    writer.write("public:\n")
    for c in class_desc["types"]:
        write_class_def(writer, c)

    # write fields
    for field in class_desc["fields"]:
        decl = generate_field_decl(field)
        writer.write(decl)

    # write needed impl field
    if "extends" not in class_desc:
        writer.write("public: void* _impl;\n")

    for ctor in class_desc["constructors"]:
        decls = generate_ctor_decl(ctor, name)
        writer.write(decls)

    # write impl constructor
    writer.write("public: explicit {name}(void * impl);\n".format(name=name))

    # write impl init service delcaration
    writer.write("protected: void initializeFromImpl(void * impl);\n")

    dtor_decl = generate_dtor_decl(class_desc)
    writer.write(dtor_decl)

    for callback in class_desc["callbacks"]:
        decl = generate_service_decl(callback, is_callback=True)
        writer.write(decl)

    for service in class_desc["services"]:
        decl = generate_service_decl(service)
        writer.write(decl)

    if has_extras:
        writer.write(generate_include('{}ExtrasInsideClass.hpp'.format(class_desc["name"])))

    writer.write('};\n')

# source utilities ###################################################

def write_ctor_impl(writer, ctor_desc, class_desc):
    parms = generate_parm_list(ctor_desc["parms"])
    name = class_desc["name"]
    full_name = get_class_name(name)
    inherit = ": {parent}(NULL)".format(parent=get_class_name(class_desc["extends"])) if "extends" in class_desc else ""

    writer.write("{cname}::{name}({parms}){inherit} {{\n".format(cname=full_name, name=name, parms=parms, inherit=inherit))
    args = generate_arg_list(ctor_desc["parms"])
    writer.write("auto * impl = new {cname}({args});\n".format(cname=get_impl_class_name(name), args=args))
    writer.write("{impl_cast}->setClient(this);\n".format(impl_cast=to_impl_cast(name,"impl")))
    writer.write("initializeFromImpl({});\n".format(to_opaque_cast("impl",name)))
    writer.write("}\n")

def write_impl_ctor_impl(writer, class_desc):
    name = class_desc["name"]
    full_name = get_class_name(name)
    inherit = ": {parent}(NULL)".format(parent=get_class_name(class_desc["extends"])) if "extends" in class_desc else ""

    writer.write("{cname}::{name}(void * impl){inherit} {{\n".format(cname=full_name, name=name, inherit=inherit))
    writer.write("if (impl != NULL) {\n")
    writer.write("{impl_cast}->setClient(this);\n".format(impl_cast=to_impl_cast(name,"impl")));

    impl = "impl" if "extends" not in class_desc else to_opaque_cast("static_cast<{}>(impl)".format(as_impl_type(name)),name)
    writer.write("initializeFromImpl({});\n".format(impl))

    writer.write("}\n")
    writer.write("}\n")

def write_impl_initializer(writer, class_desc):
    name = class_desc["name"]
    full_name = get_class_name(name)

    writer.write("void {cname}::initializeFromImpl(void * impl) {{\n".format(cname=full_name))

    if "extends" in class_desc:
        writer.write("{parent}::initializeFromImpl(impl);\n".format(parent=get_class_name(class_desc["extends"])))
    else:
        writer.write("_impl = impl;\n")

    for field in class_desc["fields"]:
        fmt = "GET_CLIENT_OBJECT(clientObj_{fname}, {ftype}, {impl_cast}->{fname});\n"
        writer.write(fmt.format(fname=field["name"], ftype=field["type"], impl_cast=to_impl_cast(name,"_impl")))
        writer.write("{fname} = clientObj_{fname};\n".format(fname=field["name"]))

    for callback in class_desc["callbacks"]:
        fmt = "{impl_cast}->{registrar}(reinterpret_cast<void*>(&{thunk}));\n"
        registrar = callback_registrar_name(callback)
        thunk = callback_thunk_name(class_desc, callback)
        writer.write(fmt.format(impl_cast=to_impl_cast(name,"_impl"),registrar=registrar,thunk=thunk))

    writer.write("}\n")

def write_arg_setup(writer, parm):
    if is_in_out(parm):
        assert is_class(parm["type"])
        t = get_class_name(parm["type"])
        writer.write("ARG_SETUP({t}, {n}Impl, {n}Arg, {n});\n".format(t=t, n=parm["name"]))
    elif is_array(parm):
        assert is_class(parm["type"])
        t = get_class_name(parm["type"])
        writer.write("ARRAY_ARG_SETUP({t}, {s}, {n}Arg, {n});\n".format(t=t, n=parm["name"], s=parm["array-len"]))

def write_arg_return(writer, parm):
    if is_in_out(parm):
        assert is_class(parm["type"])
        t = get_class_name(parm["type"])
        writer.write("ARG_RETURN({t}, {n}Impl, {n});\n".format(t=t, n=parm["name"]))
    elif is_array(parm):
        assert is_class(parm["type"])
        t = get_class_name(parm["type"])
        writer.write("ARRAY_ARG_RETURN({t}, {s}, {n}Arg, {n});\n".format(t=t, n=parm["name"], s=parm["array-len"]))

def write_service_impl(writer, desc, class_name):
    rtype = as_client_type(desc["return"])
    name = desc["name"]
    parms = generate_parm_list(desc["parms"])
    writer.write("{rtype} {cname}::{name}({parms}) {{\n".format(rtype=rtype, cname=class_name, name=name, parms=parms))

    for parm in desc["parms"]:
        write_arg_setup(writer, parm)

    args = generate_arg_list(desc["parms"])
    impl_call = "{impl_cast}->{sname}({args})".format(impl_cast=to_impl_cast(class_name,"_impl"),sname=name,args=args)
    if "none" == desc["return"]:
        writer.write(impl_call + ";\n")
        for parm in desc["parms"]:
            write_arg_return(writer, parm)
    elif is_class(desc["return"]):
        writer.write("{rtype} implRet = {call};\n".format(rtype=as_impl_type(desc["return"]), call=impl_call))
        for parm in desc["parms"]:
            write_arg_return(writer, parm)
        writer.write("GET_CLIENT_OBJECT(clientObj, {t}, implRet);\n".format(t=desc["return"]))
        writer.write("return clientObj;\n")
    else:
        writer.write("auto ret = " + impl_call + ";\n")
        for parm in desc["parms"]:
            write_arg_return(writer, parm)
        writer.write("return ret;\n")

    writer.write("}\n")

    if is_vararg(desc):
        writer.write("\n")
        write_vararg_service_impl(writer, desc, class_name)

def write_vararg_service_impl(writer, desc, class_name):
    rtype = as_client_type(desc["return"])
    name = desc["name"]
    vararg = desc["parms"][-1]
    vararg_type = as_client_type(vararg["type"])

    parms = generate_vararg_parm_list(desc["parms"])
    writer.write("{rtype} {cname}::{name}({parms}) {{\n".format(rtype=rtype,cname=class_name,name=name,parms=parms))

    args = ", ".join([p["name"] for p in desc["parms"]])
    writer.write("{t}* {arg} = new {t}[{num}];\n".format(t=vararg_type,arg=vararg["name"],num=vararg["array-len"]))
    writer.write("va_list vararg;\n")
    writer.write("va_start(vararg, {num});\n".format(num=vararg["array-len"]))
    writer.write("for (int i = 0; i < {num}; ++i) {{ {arg}[i] = va_arg(vararg, {t}); }}\n".format(num=vararg["array-len"],arg=vararg["name"],t=vararg_type))
    writer.write("va_end(vararg);\n")
    get_ret = "" if "none" == desc["return"] else "{rtype} ret = ".format(rtype=rtype)
    writer.write("{get_ret}{name}({args});\n".format(get_ret=get_ret,name=name,args=args))
    writer.write("delete[] {arg};\n".format(arg=vararg["name"]))
    if "none" != desc["return"]:
        writer.write("return ret;\n")
    writer.write("}\n")

def write_callback_thunk(writer, class_desc, callback_desc):
    rtype = as_client_type(callback_desc["return"])
    thunk = callback_thunk_name(class_desc, callback_desc)
    parms = list_str_prepend("void * clientObj", generate_parm_list(callback_desc["parms"]))
    ctype = as_client_type(class_desc["name"])
    callback = callback_desc["name"]
    args = generate_arg_list(callback_desc["parms"])
    writer.write("{rtype} {thunk}({parms}) {{\n".format(rtype=rtype,thunk=thunk,parms=parms))
    writer.write("{ctype} client = {clientObj};\n".format(ctype=ctype,clientObj=to_client_cast(class_desc["name"],"clientObj")))
    writer.write("return client->{callback}({args});\n".format(callback=callback,args=args))
    writer.write("}\n")

def write_class_impl(writer, class_desc):
    name = class_desc["name"]
    full_name = get_class_name(name)

    # write source for iner classes first
    for c in class_desc["types"]:
        write_class_impl(writer, c)

    # write constructor definitions
    for ctor in class_desc["constructors"]:
        write_ctor_impl(writer, ctor, class_desc)
    writer.write("\n")

    write_impl_ctor_impl(writer, class_desc)
    writer.write("\n")

    # write class initializer (called from all constructors)
    write_impl_initializer(writer, class_desc)
    writer.write("\n")

    # write destructor definition
    writer.write("{cname}::~{name}() {{}}\n".format(cname=full_name,name=name))
    writer.write("\n")

    # write service definitions
    for s in class_desc["services"]:
        write_service_impl(writer, s, get_class_name(class_desc["name"]))
        writer.write("\n")

    # write callback thunk definitions
    for callback in class_desc["callbacks"]:
        write_callback_thunk(writer, class_desc, callback)
        writer.write("\n")

# main generator #####################################################

def write_class_header(writer, class_desc, namespaces, class_names):
    has_extras = "has_extras_header" in class_desc["flags"]

    writer.write(copyright_header)
    writer.write("\n")

    writer.write("#ifndef {}_INCL\n".format(class_desc["name"]))
    writer.write("#define {}_INCL\n\n".format(class_desc["name"]))

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
    writer.write("\n")

    writer.write("#endif // {}_INCL\n".format(class_desc["name"]))

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

