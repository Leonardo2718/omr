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
set(JIT_PRODUCT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_POSITION_INDEPENDENT_CODE True)

set(BUILD_NAME_FILE ${CMAKE_BINARY_DIR}/TRBuildName.cpp)
set_source_files_properties(${BUILD_NAME_FILE} PROPERTIES
    GENERATED True
)

add_custom_target(jitbuilder_name_file ALL
    COMMAND perl ${CMAKE_SOURCE_DIR}/build/scripts/generateVersion.pl tr.open.jitbuilder > ${BUILD_NAME_FILE}
    BYPRODUCTS ${BUILD_NAME_FILE}
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
    ${BUILD_NAME_FILE}
    ${OMR_COMPILER_SOURCES}
    ${OMR_JIT_SOURCES}
)

add_dependencies(jitbuilder jitbuilder_name_file)

target_include_directories(jitbuilder PRIVATE
    ../include_core
    ${JIT_PRODUCT_DIR}/x/amd64
    ${JIT_PRODUCT_DIR}/x
    ${JIT_PRODUCT_DIR}
    ${OMR_COMPILER_INCLUDE_DIRECTORIES}
)

add_definitions(
    ${OMR_COMPILER_CXX_BASE_FLAGS}
    ${OMR_COMPILER_CXX_ERROR_FLAGS}
    ${OMR_COMPILER_CXX_PLATFORM_FLAGS}
    -O3
)

target_compile_definitions(jitbuilder PRIVATE
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

