################################################################################
# Copyright (c) 2001-2020 Rohit Priyadarshi
# 
# Permission is hereby granted, free of charge, to any person obtaining a 
# copy of this software and associated documentation files (the "Software"), 
# to deal in the Software without restriction, including without limitation 
# the rights to use, copy, modify, merge, publish, distribute, sublicense, 
# and/or sell copies of the Software, and to permit persons to whom the 
# Software is furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in 
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
# DEALINGS IN THE SOFTWARE.
# https://opensource.org/licenses/MIT
################################################################################

# Minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Set the project name and version
project(eda VERSION 1.0)
message(STATUS "Configuring project 'eda' version: ${PROJECT_VERSION}")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Check GCC version
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.2.0)
    message(FATAL_ERROR "GCC version is: ${CMAKE_CXX_COMPILER_VERSION}, need version >= 9.2.0")
endif()

# ---- Create 'libs' Subdirectory ----
set(LIBS_DIR "${CMAKE_BINARY_DIR}/libs")
file(MAKE_DIRECTORY ${LIBS_DIR})

# ---- Dependencies ----
# Enable FetchContent module
include(FetchContent)

# GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG origin/main
  SOURCE_DIR ${LIBS_DIR}/googletest
  BINARY_DIR ${LIBS_DIR}/googletest-build
)
FetchContent_MakeAvailable(googletest)

# JSONCPP
FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
  GIT_TAG origin/master
  SOURCE_DIR ${LIBS_DIR}/jsoncpp
  BINARY_DIR ${LIBS_DIR}/jsoncpp-build
)
FetchContent_MakeAvailable(jsoncpp)

# SWIG
FetchContent_Declare(
  swig
  GIT_REPOSITORY https://github.com/swig/swig.git
  GIT_TAG origin/master
  SOURCE_DIR ${LIBS_DIR}/swig-src
  BINARY_DIR ${LIBS_DIR}/swig-build
)

FetchContent_GetProperties(swig)
if(NOT swig_POPULATED)
    FetchContent_Populate(swig)
    message(STATUS "Building SWIG from ${swig_SOURCE_DIR}")
    execute_process(
        COMMAND cmake -S ${swig_SOURCE_DIR} -B ${swig_BINARY_DIR} -DCMAKE_INSTALL_PREFIX=${LIBS_DIR}/swig
        RESULT_VARIABLE SWIG_CONFIGURE_RESULT
    )
    if(SWIG_CONFIGURE_RESULT)
        message(FATAL_ERROR "SWIG configuration failed.")
    endif()

    execute_process(
        COMMAND cmake --build ${swig_BINARY_DIR} --target install
        RESULT_VARIABLE SWIG_BUILD_RESULT
    )
    if(SWIG_BUILD_RESULT)
        message(FATAL_ERROR "SWIG build failed.")
    endif()
endif()

# Validate SWIG Executable
set(SWIG_EXECUTABLE "${LIBS_DIR}/swig/bin/swig")
if(NOT EXISTS ${SWIG_EXECUTABLE})
    message(FATAL_ERROR "SWIG executable not found at ${SWIG_EXECUTABLE}. Please check the SWIG build process.")
endif()

# Add SWIG to CMake search paths
list(APPEND CMAKE_PREFIX_PATH "${LIBS_DIR}/swig")
find_package(SWIG REQUIRED)

find_package(Python3 COMPONENTS Interpreter Development)

# ---- Add Subdirectories ----
add_subdirectory(cmn cmn_build)
add_subdirectory(stl stl_build)
add_subdirectory(jsoncpp jsoncpp_build)
add_subdirectory(utl utl_build)
add_subdirectory(tmat tmat_build)
add_subdirectory(sig sig_build)
add_subdirectory(algo algo_build)
add_subdirectory(rack rack_build)

# ---- Unit Tests ----
enable_testing()
add_custom_target(unittests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

# Add individual test commands
add_test(NAME run_pyrackpytest COMMAND make run_pyrackpytest)
add_test(NAME run_pytmatpytest COMMAND make run_pytmatpytest)
add_test(NAME run_pyutlpydebug COMMAND make run_pyutlpydebug)
add_test(NAME run_pyutlpytest COMMAND make run_pyutlpytest)
add_test(NAME run_pyutltest COMMAND make run_pyutltest)
add_test(NAME run_racktest COMMAND make run_racktest)
add_test(NAME run_sigtest COMMAND make run_sigtest)
add_test(NAME run_tmattest COMMAND make run_tmattest)
add_test(NAME run_utltest COMMAND make run_utltest)

# Message for build completion
message(STATUS "Project 'eda' has been successfully configured.")
