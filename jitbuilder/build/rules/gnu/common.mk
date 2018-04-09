###############################################################################
# Copyright (c) 2016, 2016 IBM Corp. and others
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

#
# Defines rules for compiling source files into object files
#
# Each of these rules will also add themselves to jit_cleanobjs and jit_cleandeps
# to clean build artifacts and dependecy files, respectively
#
include $(JIT_MAKE_DIR)/rules/gnu/filetypes.mk

# Convert the source file names to object file names
JIT_PRODUCT_BACKEND_OBJECTS=$(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(JIT_PRODUCT_BACKEND_SOURCES)))
JIT_PRODUCT_OBJECTS=$(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(JIT_PRODUCT_SOURCE_FILES)))

LANGUAGE_BINDINGS_OBJECTS=$(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(LANGUAGE_BINDINGS_SOURCES)))

#C_CLIENT_OBJECTS=$(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(C_GENERATED_CLIENT_SOURCES))) \
#                 $(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(C_CLIENT_SOURCES)))

CPP_CLIENT_OBJECTS=$(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(CPP_GENERATED_CLIENT_SOURCES))) \
                   $(patsubst %,$(FIXED_OBJBASE)/%.o,$(basename $(CPP_CLIENT_SOURCES))) \
             

# build the jitbuilder library with all the object files
#C_JIT_PRODUCT_BACKEND_LIBRARY=$(FIXED_DLL_DIR)/c/$(LIBPREFIX)$(PRODUCT_NAME).a
CPP_JIT_PRODUCT_BACKEND_LIBRARY=$(FIXED_DLL_DIR)/cpp/$(LIBPREFIX)$(PRODUCT_NAME).a

# Figure out the name of the executable file
JIT_PRODUCT_SONAME=$(FIXED_DLL_DIR)/$(PRODUCT_NAME)

# Add build name to test
JIT_PRODUCT_BUILDNAME_SRC=$(FIXED_OBJBASE)/$(JIT_OMR_DIRTY_DIR)/env/TRBuildName.cpp
JIT_PRODUCT_BUILDNAME_OBJ=$(FIXED_OBJBASE)/$(JIT_OMR_DIRTY_DIR)/env/TRBuildName.o
JIT_PRODUCT_BACKEND_OBJECTS+=$(JIT_PRODUCT_BUILDNAME_OBJ)

BINDINGS_GENERATOR=$(FIXED_OBJBASE)/jitbuilder/client/genlab
$(BINDINGS_GENERATOR): $(LANGUAGE_BINDINGS_OBJECTS)
	$(CXX) -o $@ $(LANGUAGE_BINDINGS_OBJECTS)

#../build/jitbuilder/client/cpp/BytecodeBuilder.cpp : ../build/jitbuilder/client/genlab
#	$(BINDINGS_GENERATOR) $(FIXED_OBJBASE)/..

#BINDINGS_OUTPUT=$(FIXED_OBJBASE)/jitbuilder/client/genlab.output
#$(CPP_CLIENT_SOURCES)) : $(BINDINGS_GENERATOR)
#	$(BINDINGS_GENERATOR) > $(BINDINGS_OUTPUT)

$(CPP_JIT_PRODUCT_BACKEND_LIBRARY): $(CPP_CLIENT_OBJECTS)
#jit: $(C_JIT_PRODUCT_BACKEND_LIBRARY) $(CPP_JIT_PRODUCT_BACKEND_LIBRARY)
jit: $(CPP_JIT_PRODUCT_BACKEND_LIBRARY)

$(C_JIT_PRODUCT_BACKEND_LIBRARY): $(JIT_PRODUCT_BACKEND_OBJECTS) $(C_CLIENT_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR_CMD) rcsv $@ $(JIT_PRODUCT_BACKEND_OBJECTS) $(C_CLIENT_OBJECTS)

$(CPP_JIT_PRODUCT_BACKEND_LIBRARY): $(JIT_PRODUCT_BACKEND_OBJECTS) $(CPP_CLIENT_OBJECTS)
	@mkdir -p $(dir $@)
	$(AR_CMD) rcsv $@ $(JIT_PRODUCT_BACKEND_OBJECTS) $(CPP_CLIENT_OBJECTS)

jit_clean::
	rm -f $(C_JIT_PRODUCT_BACKEND_LIBRARY) $(CPP_JIT_PRODUCT_BACKEND_LIBRARY)

jit_cleandll::
	rm -f $(JIT_PRODUCT_SONAME)

$(call RULE.cpp,$(JIT_PRODUCT_BUILDNAME_OBJ),$(JIT_PRODUCT_BUILDNAME_SRC))    
    
.phony: $(JIT_PRODUCT_BUILDNAME_SRC)
$(JIT_PRODUCT_BUILDNAME_SRC):
	@mkdir -p $(dir $@)
	$(PERL_PATH) $(GENERATE_VERSION_SCRIPT) $(PRODUCT_RELEASE) > $@

jit_clean::
	rm -f $(JIT_PRODUCT_BUILDNAME_SRC)

$(call RULE.cpp,$(GTEST_OBJ),$(GTEST_CC))

#
# This part calls the "RULE.x" macros for each source file
#
$(foreach SRCFILE,$(JIT_PRODUCT_BACKEND_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(JIT_PRODUCT_SOURCE_FILES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(LANGUAGE_BINDINGS_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(C_CLIENT_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(CPP_CLIENT_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(C_GENERATED_CLIENT_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_SRCBASE)/$(SRCFILE)) \
 )

$(foreach SRCFILE,$(CPP_GENERATED_CLIENT_SOURCES),\
    $(call RULE$(suffix $(SRCFILE)),$(FIXED_OBJBASE)/$(basename $(SRCFILE))$(OBJSUFF),$(FIXED_OBJBASE)/$(SRCFILE)) \
 )

$(FIXED_OBJBASE)/%.cpp: $(BINDINGS_GENERATOR)
	$(BINDINGS_GENERATOR) $(FIXED_OBJBASE)/..
