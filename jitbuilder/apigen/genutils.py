#! /usr/bin/python

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
A collection of common utilities for generating JitBuilder client APIs
and language bindings.t
"""

import json
from functools import reduce

# Language independent utilities

def list_str_prepend(pre, list_str):
    """
    Helper function that preprends an item to a stringified
    comma separated list of items.
    """
    return pre + ("" if list_str == "" else ", " + list_str)

# API description handling utilities

class APIField:
    """A wrapper for a field API description."""

    def __init__(self, description, api):
        self.description = description
        self.api = api # not used but included for consistency

    ## Basic interface

    def name(self):
        """Returns the name of the field."""
        return self.description["name"]

    def type(self):
        """returns the type of the field."""
        return self.description["type"]

class APIService:
    """A wrapper for a service API description."""

    def __init__(self, description, api):
        self.description = description
        self.api = api

    def name(self):
        """Returns the base-name of the API service."""
        return self.description["name"]

    def suffix(self):
        """Returns the overload suffix of the API service."""
        return self.description["overloadsuffix"]

    def overload_name(self):
        """Returns the name of the API service as an overload."""
        return self.name() + self.suffix()

    def __flags(self):
        """Returns the list of flags set for this service."""
        return self.description["flags"]

    def sets_allocators(self):
        """Returns whether the service sets class allocators."""
        return "sets-allocators" in self.__flags()

    def is_static(self):
        """Returns true if this service is static."""
        return "static" in self.__flags()

    def is_impl_default(self):
        """Returns true if this service has the 'impl-default' flag set."""
        return "impl-default" in self.__flags()

    def visibility(self):
        """
        Returns the visibility of the service as a string.

        By default, a service is always public, since this is
        the common case in most APIs.
        """
        return "protected" if "protected" in self.__flags() else "public"

    def return_type(self):
        """Returns the name of the type returned by this service."""
        return self.description["return"]

    def parameters(self):
        """Returns a list of the services parameters."""
        return self.description["parms"]

    def is_vararg(self):
        """
        Checks if the given API service description can be
        implemented as a vararg.

        A service is assumed to be implementable as a vararg
        if one of its parameters contains the attribute
        `can_be_vararg`.
        """
        vararg_attrs = ["can_be_vararg" in p["attributes"] for p in self.parameters() if "attributes" in p]
        return reduce(lambda l,r: l or r, vararg_attrs, False)

class APIClass:
    """A wrapper for a class API description."""

    def __init__(self, description, api):
        self.description = description
        self.api = api

    ## Basic interface

    def name(self):
        """Returns the (base) name of the API class."""
        return self.description["name"]

    def has_parent(self):
        """Returns true if this class extends another class."""
        return "extends" in self.description

    def parent(self):
        """
        Returns the name of the parent class if it has one,
        an empy string otherwise.
        """
        return self.description["extends"] if self.has_parent() else ""

    def inner_classes(self):
        """Returns a list of innter classes descriptions."""
        return [APIClass(c, self.api) for c in self.description["types"]]

    def services(self):
        """Returns a list of descriptions of all contained services."""
        return [APIService(s, self.api) for s in self.description["services"]]

    def constructors(self):
        """Returns a list of the class constructor descriptions."""
        return self.description["constructors"]

    def callbacks(self):
        """Returns a list of descriptions of all class callbacks."""
        return [APIService(s, self.api) for s in self.description["callbacks"]]

    def fields(self):
        """Returns a list of descriptions of all class fields."""
        return [APIField(f, self.api) for f in self.description["fields"]]

    ## Extended interface

    def containing_classes(self):
        """Returns a list of classes containing the current class, from inner- to outer-most."""
        return self.api.containing_classes_of(self.name())

    def base(self):
        """
        Returns the base class of the current class. If the class does not
        extend another class, the current class is returned.
        """
        return self.api.get_class_by_name(self.api.base_of(self.name())) if self.has_parent() else self

class APIDescription:
    """A class abstract the details of how an API description is stored"""

    @staticmethod
    def load_json_string(desc):
        """Load an API description from a JSON string."""
        return APIDescription(json.loads(desc))

    @staticmethod
    def load_json_file(desc):
        """Load an API description from a JSON file."""
        return APIDescription(json.load(desc))

    def __init__(self, description):
        self.description = description

        # table mapping class names to class descriptions
        self.class_table = {}
        self.__init_class_table(self.class_table, self.classes())

        # table of classes and their contained classes
        self.containing_table = {}
        self.__init_containing_table(self.containing_table, self.classes())

        # table of base-classes
        self.inheritance_table = {}
        self.__init_inheritance_table(self.inheritance_table, self.classes())

    def __init_class_table(self, table, cs):
        """Generates a dictionary from class names class descriptions."""
        for c in cs:
            table[c.name()] = c
            self.__init_class_table(table, c.inner_classes())

    def __init_containing_table(self, table, cs, outer_classes=[]):
        """
        Generates a dictionary from class names to complete class names
        that include the names of containing classes from a list of
        client API class descriptions.
        """
        for c in cs:
            self.__init_containing_table(table, c.inner_classes(), outer_classes + [c.name()])
            table[c.name()] = outer_classes

    def __init_inheritance_table(self, table, cs):
        """
        Generates a dictionary from class names to base-class names
        from a list of API class descriptions.
        """
        for c in cs:
            self.__init_inheritance_table(table, c.inner_classes())
            if c.has_parent(): table[c.name()] = c.parent()

    ## Basic interface

    def project(self):
        """Returns the name of the project the API is for."""
        return self.description["project"]

    def namespaces(self):
        """Returns the namespace that the API is in."""
        return self.description["namespace"]

    def classes(self):
        """Returns a list of all the top-level classes defined in the API."""
        return [APIClass(c, self) for c in self.description["classes"]]

    def services(self):
        """Returns a list of all the top-level services defined in the API."""
        return [APIService(s, self) for s in self.description["services"]]

    @staticmethod
    def is_in_out(parm_desc):
        """Checks if the given parameter description is for an in-out parameter."""
        return "attributes" in parm_desc and "in_out" in parm_desc["attributes"]

    @staticmethod
    def is_array(parm_desc):
        """Checks if the given parameter description is for an array parameter."""
        return "attributes" in parm_desc and "array" in parm_desc["attributes"]

    # @staticmethod
    # def is_vararg(service_desc):
    #     """
    #     Checks if the given API service description can be
    #     implemented as a vararg.

    #     A service is assumed to be implementable as a vararg
    #     if one of its parameters contains the attribute
    #     `can_be_vararg`.
    #     """
    #     vararg_attrs = ["can_be_vararg" in p["attributes"] for p in service_desc["parms"] if "attributes" in p]
    #     return reduce(lambda l,r: l or r, vararg_attrs, False)

    ## Extended interface

    def get_class_names(self):
        """Retruns a list of the names of all the top-level classes defined in the API."""
        return [c.name() for c in self.classes()]

    def get_class_by_name(self, c):
        """Returns the description of a class from its name."""
        assert self.is_class(c), "'{}' is not a class in the {} API".format(c, self.project())
        return self.description["classes"][c]

    def is_class(self, c):
        """Returns true if the given string is the name of an API class."""
        return c in self.class_table

    def containing_classes_of(self, c):
        """
        Returns a list of the classes containing the specified class,
        from outmost to innermost.
        """
        assert self.is_class(c), "'{}' is not a class in the {} API".format(c, self.project())
        return self.containing_table[c]

    def has_parent(self, c):
        """Returns true if the class with name 'c' has a parent."""
        assert self.is_class(c), "'{}' is not a class in the {} API.".format(c, self.project())
        return c in self.inheritance_table

    def parent_of(self, c):
        """
        Returns the name of the parrent class for 'c'.
        If the 'c' does not have a parent, and empty string is returned.
        """
        return self.inheritance_table[c] if self.has_parent(c) else ""

    def base_of(self, c):
        """
        Returns the name of the base-class of a given class name `c`.
        If `c` does not extend any class, then `c` itself is returned.
        """
        assert self.is_class(c), "'{}' is not a class in the {} API".format(c, self.project())
        return self.base_of(self.inheritance_table[c]) if c in self.inheritance_table else c

# Implementation info helpers
#
# These helpers provide information about the JitBuilder Implementation.
# It is therefore acceptable for them to be C++ specific.

def callback_setter_name(callback_desc):
    """
    Produces the name of the function used to set a client callback
    on an implementation object.
    """
    return "setClientCallback_" + callback_desc.name()

def gen_inheritance_table(cs):
    """
    Generates a dictionary from class names to base-class names
    from a list of API class descriptions.

    The generated dictionary should be assigned to `inheritance_table`.
    """
    table = {}
    for c in cs:
        if "extends" in c: table[c["name"]] = c["extends"]
        table.update(gen_inheritance_table(c["types"]))
    return table

def get_impl_service_name(service):
    """
    Produces the name of the JitBuilder implementation of a
    "stand-alone" service (not an API class member).
    """
    return "internal_" + service.name()