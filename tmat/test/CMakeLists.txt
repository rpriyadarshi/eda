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

set(UNIT_TEST tmattest)
set(UNIT_TEST_CMD tmattest)

# set the project name and version
project(${UNIT_TEST} VERSION 1.0)

# add the executable
add_executable(${UNIT_TEST} test.cpp)

target_link_libraries(${UNIT_TEST} tmatlib pthread jsoncpp_lib gtest gtest_main)

target_include_directories(${UNIT_TEST} PUBLIC
    ../../cmn/include
    ../../utl/include
    ../../tmat/include
)

# add_custom_command(TARGET ${UNIT_TEST} POST_BUILD
#     COMMAND ${UNIT_TEST_CMD}
# )

add_custom_target(run_${UNIT_TEST} #ALL
    DEPENDS RunUnitTests.touch ${UNIT_TEST}
)
add_custom_command(OUTPUT RunUnitTests.touch
    COMMAND ${UNIT_TEST_CMD}
)
