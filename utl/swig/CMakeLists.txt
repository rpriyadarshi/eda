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

# set the project name
project(edaint VERSION 1.0
    DESCRIPTION "EDA command interfaces"
    LANGUAGES CXX)

# Swig dependancy
# https://cmake.org/cmake/help/latest/module/UseSWIG.html
find_package(SWIG 4.0.1)
include(${SWIG_USE_FILE})

# Python dependancy
# https://cmake.org/cmake/help/latest/module/FindPython3.html
find_package(Python3 COMPONENTS Interpreter Development)

set_property(SOURCE utl_int.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE utl_int.i PROPERTY SWIG_FLAGS "-builtin")
set_property(SOURCE utl_int.i PROPERTY SWIG_MODULE_NAME pyutl)

swig_add_library(pyutl
    TYPE SHARED
    LANGUAGE python
    SOURCES utl_int.i
)

target_include_directories(pyutl PUBLIC
    ${Python3_INCLUDE_DIRS}
    ../../cmn/include
    ../../utl/include
    ../../stl/interface
    ../../jsoncpp/interface
    ../../cmn/interface
    ../../utl/interface
)

set_property(TARGET pyutl PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

swig_link_libraries(pyutl PUBLIC
    jsoncpp
    ${Python3_LIBRARIES}
)

add_custom_command(TARGET pyutl POST_BUILD
    # Useful when the platform does not support linking
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test.py $<TARGET_FILE_DIR:pyutl>/.
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/debug.py $<TARGET_FILE_DIR:pyutl>/.
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:pyutl>/pyutl.py ${CMAKE_CURRENT_SOURCE_DIR}/.
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:pyutl>/_pyutl.so ${CMAKE_CURRENT_SOURCE_DIR}/.
    #https://stackoverflow.com/questions/35765106/symbolic-links-cmake
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/test.py $<TARGET_FILE_DIR:pyutl>/test.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/debug.py $<TARGET_FILE_DIR:pyutl>/debug.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:pyutl>/pyutl.py ${CMAKE_CURRENT_SOURCE_DIR}/pyutl.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:pyutl>/_pyutl.so ${CMAKE_CURRENT_SOURCE_DIR}/_pyutl.so

    COMMAND python test.py
    COMMAND python debug.py
)

# Python run executable
add_executable(pyutltest test.cpp)

target_include_directories(pyutltest PUBLIC
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(pyutltest ${Python3_LIBRARIES} pthread gtest gtest_main)

add_dependencies(pyutltest pyutl)

# https://gitlab.com/inorton/cmake-custom-concurrency
add_custom_command(TARGET pyutltest POST_BUILD
    COMMAND pyutltest
)
