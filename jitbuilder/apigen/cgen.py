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
A module for generating the JitBuilder C client API.
"""

import os
# import datetime
# import json
import shutil
import argparse
import cppgen
import genutils

class CGenerator(cppgen.CppGenerator):

    def __init__(self, api, headerdir):
        cppgen.CppGenerator.__init__(self, api, headerdir, [])

    def get_client_class_name(self, c):
        """
        Returns the name of a given class in the client API implementation,
        prefixed with the name of all contianing classes.
        """
        return "".join(c.containing_classes() + [c.name()])

    def get_client_type(self, t, ns=""):
        """
        Returns the C type to be used in the client API implementation
        for a given type name, prefixing with a given namespace if needed.

        Because C does not support the notion of namespaces, the namespace
        argument to this function is forced to be an empty string.
        """
        return cppgen.CppGenerator.get_client_type(self, t, "")

    def get_self_parm(self, service):
        return "{} self".format(self.get_client_type(service.owning_class().as_type()))

    def get_self_arg(self, service):
        return "self"

    # header utilities ###################################################

    def generate_field_decl(self, field, with_visibility = False):
        """
        Produces the declaration of a client API field from
        its description, specifying its visibility as required.
        """
        t = self.get_client_type(field.type())
        n = field.name()
        return "{type} {name};\n".format(type=t, name=n)

    def generate_service_parms(self, service):
        """
        Produces a stringified, comma separated list of parameter
        declarations for a service.
        """
        return genutils.list_str_prepend(self.get_self_parm(service), self.generate_parm_list(service.parameters()))

    def generate_service_vararg_parms(self, service):
        """
        Produces a stringified, comma separated list of parameter
        declarations for a service.
        """
        return genutils.list_str_prepend(self.get_self_parm(service), self.generate_vararg_parm_list(service.parameters()))

    def generate_service_args(self, service):
        """
        Produces a stringified, comma separated list of parameter
        declarations for a service.
        """
        return genutils.list_str_prepend(self.get_self_parm(service), self.generate_parm_list(service.parameters()))

    def generate_class_service_decl(self, service,is_callback=False):
        """
        Produces the declaration for a client API class service
        from its description.
        """
        vis = service.visibility() + ": "
        ret = self.get_client_type(service.return_type())
        class_prefix = service.owning_class().short_name()
        name = service.name()
        parms = self.generate_service_parms(service)

        decl = "{rtype} {cprefix}_{name}({parms});\n".format(rtype=ret, cprefix=class_prefix, name=name, parms=parms)
        if service.is_vararg():
            parms = self.generate_service_vararg_parms(service)
            decl = decl + "{rtype} {cprefix}_{name}_v({parms});\n".format(rtype=ret, cprefix=class_prefix, name=name,parms=parms)

        return decl

    def generate_ctor_decl(self, ctor_desc, class_name):
        """
        Produces the declaration of a client API class constructor
        from its description and the name of its class.
        """
        parms = self.generate_parm_list(ctor_desc.parameters())
        decls = "New{name}({parms});\n".format(name=class_name, parms=parms)
        return decls

    def generate_allocator_decl(self, class_desc):
        """Produces the declaration of a client API object allocator."""
        return 'extern void * {alloc}(void * impl);\n'.format(alloc=self.get_allocator_name(class_desc))

    def write_class_def(self, writer, class_desc):
        """Write the definition of a client API class from its description."""

        name = class_desc.name()
        has_extras = name in self.classes_with_extras

        writer.write("struct {name} {{\n".format(name=name))
        if class_desc.has_parent():
            # "inheritance" is represented using a data member called `super` that
            # points to the instance of the "parent" class
            writer.write("{} super;\n".format(self.get_client_type(class_desc.parent().as_type())))

        # write fields
        for field in class_desc.fields():
            decl = self.generate_field_decl(field)
            writer.write(decl)

        # write needed impl field
        if not class_desc.has_parent():
            writer.write("void * _impl;\n")

        if has_extras:
            writer.write(self.generate_include('{}ExtrasInsideClass.hpp'.format(class_desc.name())))

        writer.write("};\n")

        for ctor in class_desc.constructors():
            decls = self.generate_ctor_decl(ctor, name)
            writer.write(decls)

        # write impl constructor
        writer.write("New{name}(void * impl);\n".format(name=name))

        # write impl init service delcaration
        writer.write("void initializeFromImpl({} self, void * impl);\n".format(self.get_client_type(class_desc.as_type())))

        # dtor_decl = self.generate_dtor_decl(class_desc)
        # writer.write(dtor_decl)

        for callback in class_desc.callbacks():
            decl = self.generate_class_service_decl(callback, is_callback=True)
            writer.write(decl)

        service_names = [s.name() for s in class_desc.services()]
        for service in class_desc.services():
            if service.suffix() == "" or service.overload_name() not in service_names:
                # if a service already exists with the same overload name, don't declare the current one
                decl = self.generate_class_service_decl(service)
                writer.write(decl)

        # write nested classes
        for c in class_desc.inner_classes():
            self.write_class_def(writer, c)

    def write_class_header(self, writer, class_desc, namespaces, class_names):
        """Writes the header for a client API class from the class description."""

        has_extras = class_desc.name() in self.classes_with_extras

        writer.write(self.get_copyright_header())
        writer.write("\n")

        writer.write("#ifndef {}_INCL\n".format(class_desc.name()))
        writer.write("#define {}_INCL\n".format(class_desc.name()))

        if class_desc.has_parent():
            writer.write(self.generate_include("{}.hpp".format(class_desc.parent().name())))

        if has_extras:
            writer.write(self.generate_include('{}ExtrasOutsideClass.hpp'.format(class_desc.name())))
        writer.write("\n")

        writer.write("// forward declarations for all API classes\n")
        for c in class_names:
            writer.write("struct {};\n".format(c))
        writer.write("\n")

        self.write_class_def(writer, class_desc)
        writer.write("\n")

        self.write_allocator_decl(writer, class_desc)
        writer.write("\n")

        writer.write("#endif // {}_INCL\n".format(class_desc.name()))

    def write_class(self, header_dir, source_dir, class_desc, namespaces, class_names):
        """Generates and writes a client API class from its description."""

        cname = class_desc.name()
        header_path = os.path.join(header_dir, cname + ".h")
        source_path = os.path.join(source_dir, cname + ".cpp")
        with open(header_path, "w") as writer:
            self.write_class_header(writer, class_desc, namespaces, class_names)
        with open(source_path, "w") as writer:
            self.write_class_source(writer, class_desc, namespaces, class_names)

# main generator #####################################################

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
        api_description = genutils.APIDescription.load_json_file(api_src)

    generator = CGenerator(api_description, args.headerdir)

    namespaces = api_description.namespaces()
    class_names = api_description.get_class_names()

    for class_desc in api_description.classes():
        generator.write_class(args.headerdir, args.sourcedir, class_desc, namespaces, class_names)
    with open(os.path.join(args.headerdir, "JitBuilder.h"), "w") as writer:
        generator.write_common_decl(writer, api_description)
    with open(os.path.join(args.sourcedir, "JitBuilder.c"), "w") as writer:
        generator.write_common_impl(writer, api_description)

    extras_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "extras", "cpp")
    names = os.listdir(extras_dir)
    for name in names:
        if name.endswith(".hpp"):
            shutil.copy(os.path.join(extras_dir,name), os.path.join(args.headerdir,name))
