#! /usr/bin/env python

###############################################################################
# Copyright (c) 2018, 2018 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at https://www.eclipse.org/legal/epl-2.0/
# or the Apache License, Version 2.0 which accompanies this distribution and
# is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following
# Secondary Licenses when the conditions for such availability set
# forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
# General Public License, version 2 with the GNU Classpath
# Exception [1] and GNU General Public License, version 2 with the
# OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
###############################################################################

"""
A modules for generating the C++ client API using the Listener pattern.
"""

import os
import datetime
import json
import shutil
import argparse
from genutils import *
import cppgen

def is_ilbuilder(c):
    # TODO: don't hardcode the class names
    return c.name() in ["IlBuilder", "MethodBuilder", "BytecodeBuilder"]

# listener interface generation ################################################

def generate_listener_class_name(class_desc):
    return class_desc.name() + "Listener"

def generate_listener_service(service):
    """
    Generate a string for the declaration of a service (method)
    as a listener.
    """
    name = service.name()
    parms = cppgen.generate_parm_list(service.parameters())
    return "virtual void {}({}) {{}}\n".format(name, parms)

def write_class_listener(writer, class_desc):
    """
    Write the interface defintion for a class listener.
    """

    if class_desc.has_parent():
        writer.write("class {}: public {} {{\n".format(generate_listener_class_name(class_desc), generate_listener_class_name(class_desc.parent())))
    else:
        writer.write("class {} {{\n".format(generate_listener_class_name(class_desc)))
    writer.write("public:\n")

    for service in class_desc.services():
        writer.write(generate_listener_service(service))
    for service in class_desc.callbacks():
        writer.write(generate_listener_service(service))

    # The cloneInto functions are used register a new instance of a listener
    # that coresponds to the given object. If the clone and registration
    # are successful, `true` is returned, `false` otherwise.
    #
    # TODO: find a better name than "clone"
    # TODO: don't hard code the acceptors, traverse the class description heirarchy
    #       instead and find all the classes than need an acceptor
    writer.write("virtual bool cloneInto(IlBuilder * b) { return false; }\n")
    writer.write("virtual bool cloneInto(BytecodeBuilder * b) { return false; }\n")
    writer.write("virtual bool cloneInto(MethodBuilder * b) { return false; }\n")

    # The "mute" hint is intended to singal a listener that it may ignore the
    # events that follow. This can be useful in cases where one event results
    # in several other events being triggered that a listener may wish to ignore.
    writer.write("void muteHint() { _muteHint = true; }\n")
    writer.write("void unmuteHint() { _muteHint = false; }\n")
    writer.write("bool isMuteHintSet() { return _muteHint; }\n")

    writer.write("private:\n")
    writer.write("bool _muteHint;\n")

    writer.write("};\n")

def write_listener_header(writer, class_desc, namespaces):
    """
    Write header file for a class listener
    """
    guard_name = generate_listener_class_name(class_desc).upper() + "_INCL"

    writer.write("#ifndef {}\n".format(guard_name))
    writer.write("#define {}\n".format(guard_name))

    for n in namespaces:
        writer.write("namespace {} {{\n".format(n))
    writer.write("\n")

    # TODO: don't reach into `api` field of class description,
    #       the information for what needs to be forward declared
    #       should be stored somewhere else
    writer.write("// forward declarations for all API classes\n")
    for c in class_desc.api.classes():
        writer.write("class {};\n".format(c.name()))
        # Because nested classes are now "unnested", we can forward declare them
        # TODO: find more elegant/general solution for dealing with nested/neighbour classes
        for c_ in c.inner_classes():
            writer.write("class {};\n".format(c_.name()))
    writer.write("\n")

    write_class_listener(writer, class_desc)
    writer.write("\n")

    for n in reversed(namespaces):
        writer.write("}} // namespace {}\n".format(n))

    writer.write("#endif // {}\n".format(guard_name))

# recorder generation ##########################################################

def generate_recorder_name(class_desc):
    return class_desc.name() + "Recorder"

def collect_services(class_desc):
    """
    Collect the description of all the services in the given class description 
    and all parent class descriptions.
    """
    services = class_desc.services()
    services += class_desc.callbacks()
    if class_desc.has_parent():
        services += collect_services(class_desc.parent())
    return services

def dedup(l):
    """
    Deduplicate the elements in an list (or an iterator really).
    """
    # TODO: find better names than l_, i, and l
    l_ = []
    for i in l:
        if i not in l_:
            l_.append(i)
    return l_

def generate_recorder_service_decl(service):
    """
    Write the declaraion of a service (method) of a recorder.
    """
    name = service.name()
    parms = cppgen.generate_parm_list(service.parameters())
    return 'virtual void {}({});\n'.format(name, parms)

def write_recorder_class_def(writer, class_desc):
    """
    Write recorder class defintion.
    """

    writer.write("class {}: public {} {{\n".format(generate_recorder_name(class_desc), generate_listener_class_name(class_desc)))
    writer.write("public:\n")

    # write service recorder implementation
    # TODO: find a better way than just collecting all the services and deduplicating them
    for s in dedup(collect_services(class_desc)):
        writer.write(generate_recorder_service_decl(s))
    writer.write("\n")

    # TODO: don't hard code the acceptors, traverse the class description heirarchy
    #       instead and find all the classes than need an acceptor
    writer.write("virtual bool cloneInto(IlBuilder * b);\n")
    writer.write("virtual bool cloneInto(BytecodeBuilder * b);\n")
    writer.write("virtual bool cloneInto(MethodBuilder * b);\n")

    writer.write("};\n")

def write_recorder_header(writer, class_desc, namespaces):
    """
    Write header file for recorder.
    """
    guard_name = generate_recorder_name(class_desc).upper() + "_INCL"

    writer.write("#ifndef {}\n".format(guard_name))
    writer.write("#define {}\n".format(guard_name))

    # TODO: figure out what actually needs to be included and include it
    writer.write(cppgen.generate_include("{}.hpp".format(generate_listener_class_name(class_desc))))

    for n in namespaces:
        writer.write("namespace {} {{\n".format(n))
    writer.write("\n")

    # TODO: don't reach into `api` field of class description,
    #       the information for what needs to be forward declared
    #       should be stored somewhere else
    writer.write("// forward declarations for all API classes\n")
    for c in class_desc.api.classes():
        writer.write("class {};\n".format(c.name()))
        # Because nested classes are now "unnested", we can forward declare them
        # TODO: find more elegant/general solution for dealing with nested/neighbour classes
        for c_ in c.inner_classes():
            writer.write("class {};\n".format(c_.name()))
    writer.write("\n")

    write_recorder_class_def(writer, class_desc)
    writer.write("\n")

    for n in reversed(namespaces):
        writer.write("}} // namespace {}\n".format(n))

    writer.write("#endif // {}\n".format(guard_name))

def write_recorder_service_def(writer, cname, service):
    """
    Write the definition of a service (method) of a recorder.
    """
    name = service.name()
    parms = cppgen.generate_parm_list(service.parameters())
    writer.write('void {}::{}({}) {{ std::cout << "{}\\n"; }}\n'.format(cname, name, parms, name))

def write_recorder_source(writer, headerdir, class_desc, namespaces):
    for f in cppgen.impl_include_files:
        writer.write(cppgen.generate_include(f))
    # TODO: don't hardcode includes
    writer.write(cppgen.generate_include(os.path.join(headerdir, "IlBuilderRecorder.hpp")))
    writer.write(cppgen.generate_include(os.path.join(headerdir, "BytecodeBuilderRecorder.hpp")))
    writer.write(cppgen.generate_include(os.path.join(headerdir, "MethodBuilderRecorder.hpp")))
    writer.write("#include <iostream>\n")
    writer.write("\n")

    for n in namespaces:
        writer.write("namespace {} {{\n".format(n))
    writer.write("\n")

    # write service recorder implementation
    # TODO: find a better way than just collecting all the services and deduplicating them
    for s in dedup(collect_services(class_desc)):
        write_recorder_service_def(writer, class_desc.name() + "Recorder", s)

    # TODO: don't hardcode the acceptors, traverse the class description heirarchy
    #       instead and find all the classes than need an acceptor
    # TODO: use string formatting instead of concatnation
    writer.write("bool "+class_desc.name()+"Recorder::cloneInto(IlBuilder * b) { b->RegisterListener(new IlBuilderRecorder()); return true; }\n")
    writer.write("bool "+class_desc.name()+"Recorder::cloneInto(BytecodeBuilder * b) { b->RegisterListener(new BytecodeBuilderRecorder()); return true; }\n")
    writer.write("bool "+class_desc.name()+"Recorder::cloneInto(MethodBuilder * b) { b->RegisterListener(new MethodBuilderRecorder()); return true; }\n")

    for n in reversed(namespaces):
        writer.write("}} // namespace {}\n".format(n))

# main program #################################################################

if __name__ == "__main__":
    default_dest = os.path.join(os.getcwd(), "client")
    parser = argparse.ArgumentParser()
    parser.add_argument("--sourcedir", type=str, default=default_dest,
                        help="destination directory for the generated source files")
    parser.add_argument("--headerdir", type=str, default=default_dest,
                        help="destination directory for the generated header files")
    parser.add_argument("description", help="path to the API description file")
    args = parser.parse_args()

    with open(args.description) as api_src:
        api_description = APIDescription.load_json_file(api_src)

    namespaces = api_description.namespaces()
    class_names = api_description.get_class_names()

    cppgen.impl_include_files = cppgen.gen_api_impl_includes(api_description.classes(), args.headerdir)

    for class_desc in api_description.classes():
        # TODO: don't just check if a class is an IlBuilder, check a property
        #       in the class description instead
        if is_ilbuilder(class_desc):
            fname = generate_listener_class_name(class_desc) + ".hpp"
            with open(os.path.join(args.headerdir, fname), "w") as writer:
                write_listener_header(writer, class_desc, namespaces)
    
    for class_desc in api_description.classes():
        # TODO: don't just check if a class is an IlBuilder, check a property
        #       in the class description instead
        if is_ilbuilder(class_desc):
            fname = generate_recorder_name(class_desc) + ".hpp"
            with open(os.path.join(args.headerdir, fname), "w") as writer:
                write_recorder_header(writer, class_desc, namespaces)
            fname = generate_recorder_name(class_desc) + ".cpp"
            with open(os.path.join(args.sourcedir, fname), "w") as writer:
                write_recorder_source(writer, args.headerdir, class_desc, namespaces)

    for class_desc in api_description.classes():
        cppgen.write_class(args.headerdir, args.sourcedir, class_desc, namespaces, class_names)
    with open(os.path.join(args.headerdir, "JitBuilder.hpp"), "w") as writer:
        cppgen.write_common_decl(writer, api_description)
    with open(os.path.join(args.sourcedir, "JitBuilder.cpp"), "w") as writer:
        cppgen.write_common_impl(writer, api_description)

    extras_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "extras", "cpp")
    names = os.listdir(extras_dir)
    for name in names:
        if name.endswith(".hpp"):
            shutil.copy(os.path.join(extras_dir,name), os.path.join(args.headerdir,name))
