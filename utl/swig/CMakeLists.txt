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

set(LIB_NAME pyutl)
set(PY_UNIT_TEST pyutlpytest)
set(PY_UNIT_TEST_CMD python test.py)
set(PY_DEBUG_TEST pyutlpydebug)
set(PY_DEBUG_TEST_CMD python debug.py)

set(UNIT_TEST pyutltest)
set(UNIT_TEST_CMD pyutltest)

# set the project name
project(edaint VERSION 1.0
    DESCRIPTION "EDA command interfaces"
    LANGUAGES CXX)

# Swig dependancy
include(${SWIG_USE_FILE})

set_property(SOURCE utl_int.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE utl_int.i PROPERTY SWIG_FLAGS "-builtin")
set_property(SOURCE utl_int.i PROPERTY SWIG_MODULE_NAME ${LIB_NAME})

swig_add_library(${LIB_NAME}
    TYPE SHARED
    LANGUAGE python
    SOURCES utl_int.i
)

target_include_directories(${LIB_NAME} PUBLIC
    ${Python3_INCLUDE_DIRS}
    ${LIBS_DIR}/googletest/googletest/include
    ${LIBS_DIR}/jsoncpp/include
    ${LIBS_DIR}/swig-build/include
    ../../cmn/include
    ../../utl/include
    ../../stl/interface
    ../../jsoncpp/interface
    ../../cmn/interface
    ../../utl/interface
)

set_property(TARGET ${LIB_NAME} PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)

swig_link_libraries(${LIB_NAME} PUBLIC
    jsoncpp_lib
    ${Python3_LIBRARIES}
)

add_custom_command(TARGET ${LIB_NAME} POST_BUILD
    # Useful when the platform does not support linking
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test.py $<TARGET_FILE_DIR:${LIB_NAME}>/.
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/debug.py $<TARGET_FILE_DIR:${LIB_NAME}>/.
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.py ${CMAKE_CURRENT_SOURCE_DIR}/.
#    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:${LIB_NAME}>/_${LIB_NAME}.so ${CMAKE_CURRENT_SOURCE_DIR}/.
    #https://stackoverflow.com/questions/35765106/symbolic-links-cmake
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/test.py $<TARGET_FILE_DIR:${LIB_NAME}>/test.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/debug.py $<TARGET_FILE_DIR:${LIB_NAME}>/debug.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:${LIB_NAME}>/${LIB_NAME}.py ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_NAME}.py
    COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE_DIR:${LIB_NAME}>/_${LIB_NAME}.so ${CMAKE_CURRENT_SOURCE_DIR}/_${LIB_NAME}.so
)

# add_custom_command(TARGET ${LIB_NAME} POST_BUILD
#     COMMAND ${PY_UNIT_TEST_CMD}
# )

add_custom_target(run_${PY_UNIT_TEST} #ALL
    DEPENDS RunPyUnitTests.touch ${LIB_NAME}
)
add_custom_command(OUTPUT RunPyUnitTests.touch
    COMMAND ${PY_UNIT_TEST_CMD}
)

# add_custom_command(TARGET ${LIB_NAME} POST_BUILD
#     COMMAND ${PY_DEBUG_TEST_CMD}
# )

add_custom_target(run_${PY_DEBUG_TEST} #ALL
    DEPENDS RunPyDebugTests.touch ${LIB_NAME}
)
add_custom_command(OUTPUT RunPyDebugTests.touch
    COMMAND ${PY_DEBUG_TEST_CMD}
)


# Python run executable
add_executable(${UNIT_TEST} test.cpp)

target_include_directories(${UNIT_TEST} PUBLIC
    ${Python3_INCLUDE_DIRS}
)

target_link_libraries(${UNIT_TEST} ${Python3_LIBRARIES} pthread gtest gtest_main)

add_dependencies(${UNIT_TEST} ${LIB_NAME})

# https://gitlab.com/inorton/cmake-custom-concurrency
# add_custom_command(TARGET ${UNIT_TEST} POST_BUILD
#     COMMAND ${UNIT_TEST_CMD}
# )

add_custom_target(run_${UNIT_TEST} #ALL
    DEPENDS RunUnitTests.touch ${UNIT_TEST}
)
add_custom_command(OUTPUT RunUnitTests.touch
    COMMAND ${UNIT_TEST_CMD}
)
