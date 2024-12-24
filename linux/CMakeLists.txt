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

# ---- Dependencies ----
# Enable FetchContent module
include(FetchContent)

# GoogleTest (Fetch latest from default branch)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG origin/main # Fetch latest from default branch
)
FetchContent_MakeAvailable(googletest)

# JSONCPP (Fetch latest from default branch)
FetchContent_Declare(
  jsoncpp
  GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
  GIT_TAG origin/master # Fetch latest from default branch
)
FetchContent_MakeAvailable(jsoncpp)

# SWIG
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# ---- Add Subdirectories ----
# Add subdirectories for internal libraries
add_subdirectory(../cmn cmn)    # Common utilities
add_subdirectory(../stl stl)    # STL bindings
add_subdirectory(../jsoncpp jsoncpp) # JSON library
add_subdirectory(../utl utl)    # Utility functions
add_subdirectory(../tmat tmat)  # TMAT module
add_subdirectory(../sig sig)    # Signal handling
add_subdirectory(../algo algo)  # Algorithm components
add_subdirectory(../rack rack)  # Netlist and design handling

# ---- Unit Tests ----
# Define a custom target for unit tests
enable_testing()

add_custom_target(unittests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
)

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
