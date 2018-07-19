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

from functools import reduce

# Language independent utilities

def list_str_prepend(pre, list_str):
    """
    Helper function that preprends an item to a stringified
    comma separated list of items.
    """
    return pre + ("" if list_str == "" else ", " + list_str)

def is_in_out(parm_desc):
    """Checks if the given parameter description is for an in-out parameter."""
    return "attributes" in parm_desc and "in_out" in parm_desc["attributes"]

def is_array(parm_desc):
    """Checks if the given parameter description is for an array parameter."""
    return "attributes" in parm_desc and "array" in parm_desc["attributes"]

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