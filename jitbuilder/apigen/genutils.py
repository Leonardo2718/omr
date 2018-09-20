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

class BasicAPIDescription:
    """A thin wrapper around a raw API description."""

    def __init__(self, description):
        self.description = description

    def project(self):
        """Returns the name of the project the API is for."""
        return self.description["project"]

    def namespaces(self):
        """Returns the namespace that the API is in."""
        return self.description["namespace"]

    def classes(self):
        """Returns a list of all the top-level classes defined in the API."""
        return self.description["classes"]

    def services(self):
        """Returns a list of all the top-level services defined in the API."""
        return self.description["services"]

    @staticmethod
    def is_in_out(parm_desc):
        """Checks if the given parameter description is for an in-out parameter."""
        return "attributes" in parm_desc and "in_out" in parm_desc["attributes"]

    @staticmethod
    def is_array(parm_desc):
        """Checks if the given parameter description is for an array parameter."""
        return "attributes" in parm_desc and "array" in parm_desc["attributes"]

    @staticmethod
    def is_vararg(service_desc):
        """
        Checks if the given API service description can be
        implemented as a vararg.

        A service is assumed to be implementable as a vararg
        if one of its parameters contains the attribute
        `can_be_vararg`.
        """
        vararg_attrs = ["can_be_vararg" in p["attributes"] for p in service_desc["parms"] if "attributes" in p]
        return reduce(lambda l,r: l or r, vararg_attrs, False)

class APIDescription(BasicAPIDescription):
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
        BasicAPIDescription.__init__(self, description)

        # table mapping class names to raw class descriptions
        self.class_table = self.__gen_class_table(self.classes())

        # table of classes and their contained classes
        #
        # each class has a key in the table and the corresponding value
        # is a list of its outer/containing classes, from outmost to innermost
        self.containing_table = self.__gen_containing_table(self.classes())

        # table of base-classes
        self.inheritance_table = self.__gen_inheritance_table(self.classes())

    def get_class_names(self):
        """Retruns a list of the names of all the top-level classes defined in the API."""
        return [c["name"] for c in self.classes()]

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

    def __gen_class_table(self, cs):
        """Generates a dictionary from class names raw class descriptions."""
        classes = {}
        for c in cs:
            name = c["name"]
            classes[name] = c
            classes.update(self.__gen_class_table(c["types"]))
        return classes

    @staticmethod
    def __gen_containing_table(cs, outer_classes=[]):
        """
        Generates a dictionary from class names to complete class names
        that include the names of containing classes from a list of
        client API class descriptions.
        """
        classes = {}
        for c in cs:
            name = c["name"]
            classes[name] = outer_classes
            classes.update(APIDescription.__gen_containing_table(c["types"], outer_classes + [name]))
        return classes

    @staticmethod
    def __gen_inheritance_table(cs):
        """
        Generates a dictionary from class names to base-class names
        from a list of API class descriptions.
        """
        table = {}
        for c in cs:
            if "extends" in c: table[c["name"]] = c["extends"]
            table.update(APIDescription.__gen_inheritance_table(c["types"]))
        return table

# Implementation info helpers
#
# These helpers provide information about the JitBuilder Implementation.
# It is therefore acceptable for them to be C++ specific.

def callback_setter_name(callback_desc):
    """
    Produces the name of the function used to set a client callback
    on an implementation object.
    """
    return "setClientCallback_" + callback_desc["name"]

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
    return "internal_" + service["name"]