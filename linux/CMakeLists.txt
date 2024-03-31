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

cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(eda VERSION 1.0)
message(STATUS "Project eda version: ${PROJECT_VERSION}")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.2.0)
    message(FATAL_ERROR "GCC version is: ${CMAKE_CXX_COMPILER_VERSION}, need version >= 9.2.0")
endif()

# add the UTL library
add_subdirectory(../cmn cmn)
add_subdirectory(../stl stl)
add_subdirectory(../jsoncpp jsoncpp)
add_subdirectory(../utl utl)
add_subdirectory(../tmat tmat)
add_subdirectory(../sig sig)
add_subdirectory(../algo algo)
add_subdirectory(../rack rack)

add_custom_target(unittests #ALL
    COMMAND make run_pyrackpytest
    COMMAND make run_pytmatpytest
    COMMAND make run_pyutlpydebug
    COMMAND make run_pyutlpytest
    COMMAND make run_pyutltest
    COMMAND make run_racktest
    COMMAND make run_sigtest
    COMMAND make run_tmattest
    COMMAND make run_utltest
)
