################################################################################
##
## (c) Copyright IBM Corp. 2017, 2017
##
##  This program and the accompanying materials are made available
##  under the terms of the Eclipse Public License v1.0 and
##  Apache License v2.0 which accompanies this distribution.
##
##      The Eclipse Public License is available at
##      http://www.eclipse.org/legal/epl-v10.html
##
##      The Apache License v2.0 is available at
##      http://www.opensource.org/licenses/apache2.0.php
##
## Contributors:
##    Multiple authors (IBM Corp.) - initial implementation and documentation
################################################################################

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(jitbuilder LANGUAGES CXX)

set(OMR_COMPILER_TARGET_ARCHITECTURE "x")
set(OMR_COMPILER_TARGET_SUBARCHITECTURE "amd64")
set(OMR_COMPILER_HOST_ARCHITECTURE "x")
set(OMR_COMPILER_HOST_SUBARCHITECTURE "amd64")

set(OMRCompiler_DIR ../compiler)
find_package(OMRCompiler REQUIRED)

set(JIT_OMR_DIRTY_DIR ../compiler)
set(JIT_PRODUCT_DIR ./)

include_directories(
    ../include_core
    ${JIT_PRODUCT_DIR}/x/amd64
    ${JIT_PRODUCT_DIR}/x
    ${JIT_PRODUCT_DIR}
    ${JIT_OMR_DIRTY_DIR}/x/amd64
    ${JIT_OMR_DIRTY_DIR}/x
    ${JIT_OMR_DIRTY_DIR}
    ${JIT_OMR_DIRTY_DIR}/..
)

add_definitions(
    -fno-rtti
    -fno-threadsafe-statics
    -Wno-deprecated
    -Wno-enum-compare
    -Wno-invalid-offsetof
    -Wno-write-strings
    -O3
    -pthread
    -fomit-frame-pointer
    -fasynchronous-unwind-tables
    -Wreturn-type
    -fno-dollars-in-identifiers
    -m64
    -fPIC
    -fno-strict-aliasing

    -DBITVECTOR_BIT_NUMBERING_MSB
    -DUT_DIRECT_TRACE_REGISTRATION
    -DJITTEST
    -DJITBUILDER_SPECIFIC
    -DPROD_WITH_ASSUMES
    -DTR_HOST_X86
    -DTR_HOST_64BIT
    -DBITVECTOR_64BIT
    -DLINUX
    -DTR_TARGET_X86
    -DTR_TARGET_64BIT
    -DSUPPORTS_THREAD_LOCAL
    -D_LONG_LONG
    -DJ9HAMMER
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/TRBuildName.cpp
    COMMAND perl ${CMAKE_SOURCE_DIR}/build/scripts/generateVersion.pl tr.open.jitbuilder > ${CMAKE_BINARY_DIR}/TRBuildName.cpp
)

# common.mk
list(APPEND OMR_JIT_SOURCES
    ${JIT_PRODUCT_DIR}/compile/Method.cpp
    ${JIT_PRODUCT_DIR}/control/Jit.cpp
    ${JIT_PRODUCT_DIR}/env/FrontEnd.cpp
    ${JIT_PRODUCT_DIR}/ilgen/JBIlGeneratorMethodDetails.cpp
    ${JIT_PRODUCT_DIR}/optimizer/JBOptimizer.cpp
    ${JIT_PRODUCT_DIR}/runtime/JBCodeCacheManager.cpp
    ${JIT_PRODUCT_DIR}/runtime/JBJitConfig.cpp
)

if(OMR_COMPILER_HOST_ARCHITECTURE STREQUAL "x")
    list(APPEND OMR_JIT_SOURCES
        ${JIT_PRODUCT_DIR}/x/runtime/AsmUtil64.asm
    )
endif()

if(OMR_COMPILER_TARGET_ARCHITECTURE STREQUAL "x")
    list(APPEND OMR_JIT_SOURCES
        ${JIT_PRODUCT_DIR}/x/codegen/Evaluator.cpp
    )
endif()

add_library(jitbuilder STATIC
    ${CMAKE_BINARY_DIR}/TRBuildName.cpp
    ${OMR_JIT_SOURCES}
)
