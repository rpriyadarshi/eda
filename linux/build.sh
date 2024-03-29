#!/bin/bash

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

################################################################################
# Help message
################################################################################
run_usage() {
	PROG=`basename $1`
	echo " This utility must be run from the directory which has CMakeLists.txt"
	echo "Usage:"
	echo "    -c|--config <Debug|Release>           Choose build config"
	echo "    -t|--target <getdep|googletest|jsoncpp|genscript|generate|compile|clean>"
	echo "                                          Resolve dependencies, makefiles,"
	echo "                                          compie build or"
	echo "                                          clean build"
	echo "    -j|--parallel <number>                Number of parallel compile jobs"
	echo "    -h|--help                             This help message"
	echo "Examples:"
	echo "    ${PROG} --config Debug --target getdep"
	echo "    ${PROG} --config Debug --target googletest"
	echo "    ${PROG} --config Debug --target jsoncpp"
	echo "    ${PROG} --config Debug --target swig"
	echo "    ${PROG} --config Debug --target genscript"
	echo "    ${PROG} --config Debug --target generate"
	echo "    ${PROG} --config Debug --target compile"
	echo "    ${PROG} --config Debug --target compile --parallel 4"
	echo "    ${PROG} -c Debug -t compile -j 4"
	echo "    ${PROG} --config Debug --target clean"
}

################################################################################
# Set dependency paths
################################################################################
set_dependency_paths() {
	export BUILD_NAME=build
	export LIBS_NAME=libs
	export GOOGLETEST_NAME=googletest
	export GOOGLEMOCK_NAME=googlemock
	export JSONCPP_NAME=jsoncpp
	export SWIG_NAME=swig
	export LIB_PATHS=lib_paths.sh

	export GTEST_DIR=${BUILD_ROOT}/${LIBS_NAME}/${GOOGLETEST_NAME}
	export JSONCPP_DIR=${BUILD_ROOT}/${LIBS_NAME}/${JSONCPP_NAME}
	export SWIG_DIR=${BUILD_ROOT}/${LIBS_NAME}/${SWIG_NAME}
}

################################################################################
# Get dependencies
################################################################################
get_dependencies() {
	rm -rf ${LIBS_NAME}
	mkdir -p ${LIBS_NAME}
	pushd ${LIBS_NAME}
	git clone https://github.com/google/googletest.git
	git clone https://github.com/open-source-parsers/jsoncpp.git
	git clone https://github.com/swig/swig.git
	popd
}

################################################################################
# Build Googletest
################################################################################
build_googletest() {
	mkdir -p ${GTEST_DIR}/${BUILD_NAME}/${CONFIG}	# Create a directory to hold the build output.
	pushd ${GTEST_DIR}/${BUILD_NAME}/${CONFIG}

	cmake ..                	# Generate native build scripts.
	# Or with testcases
	# cmake -Dgtest_build_samples=ON -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ..
	cmake -Dgtest_build_samples=ON ../..
	# Compile
	cmake --build . --config ${CONFIG} --target all -- -j 4
	popd
}

################################################################################
# Build JSONCPP
################################################################################
build_jsoncpp() {
	mkdir -p ${JSONCPP_DIR}/${BUILD_NAME}/${CONFIG}
	pushd ${JSONCPP_DIR}/${BUILD_NAME}/${CONFIG}
	cmake -DCMAKE_BUILD_TYPE=${CONFIG} -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
	make
	popd
}

################################################################################
# Build swig
################################################################################
build_swig() {
	pushd ${SWIG_DIR}
	./autogen.sh

	## Run Configure
	mkdir -p ${SWIG_DIR}-${BUILD_NAME}
	pushd ${SWIG_DIR}-${BUILD_NAME}

	../${SWIG_NAME}/configure --prefix=${SWIG_DIR}/lib

	## Run build
	make
	make install
	popd
}

################################################################################
# Run cmake generator
################################################################################
run_generate() {
	rm -rf ${BUILD_NAME}
	mkdir -p ${BUILD_NAME}
	pushd ${BUILD_NAME}
	# cmake -G "Unix Makefiles" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=$1 ..
	cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$1 ..
	popd
}

################################################################################
# Run build
################################################################################
run_compile() {
	pushd ${BUILD_NAME}
	if [ $3 -eq 0 ]; then
		cmake --build . --config $1 --target $2 --
	else
		cmake --build . --config $1 --target $2 -- -j $3
	fi
	popd
}

################################################################################
# Clean build
################################################################################
run_clean() {
	pushd ${BUILD_NAME}
	cmake --build . --config $1 --target $2 --
	popd
}

################################################################################
# Process commandline options
################################################################################
process_options() {
	export BUILD_ROOT=`pwd`

	# Checks
	if [ $# -eq 0 ]; then
		echo "${ERRORPREFIX} Missing options"
		HELP=2
		RC=1
	fi
	
	# Commandline options processing 
	while [ $# -ne 0 ]; do
	    case $1 in
	        -c | --config )
	        	shift
	        	CONFIG=$1
	            ;;
	        -t | --target )
	        	shift
	        	TARGET=$1
	            ;;
	        -j | --parallel )
	        	shift
	        	PARALLEL=$1
	            ;;
	        -h | --help )
	        	HELP=1
	            ;;
	        * )
	        	HELP=2
	        	RC=2
	        	;;
	    esac
	    shift
	done
}

################################################################################
# Check values optained
################################################################################
check_values() {
	if [ ${RC} -ne 0 ]; then
		return 
	fi
	# Map to integers, for simplicity. Completes selector logic
	if [ "${TARGET}" = "getdep" ]; then
		GETDEP=1
	elif [ "${TARGET}" = "googletest" ]; then
		GOOGLETEST=1
	elif [ "${TARGET}" = "jsoncpp" ]; then
		JSONCPP=1
	elif [ "${TARGET}" = "swig" ]; then
		SWIG=1
	elif [ "${TARGET}" = "genscript" ]; then
		GENSCRIPT=1
	elif [ "${TARGET}" = "generate" ]; then
		GENERATE=1
	elif [ "${TARGET}" = "compile" ]; then
		COMPILE=1
	elif [ "${TARGET}" = "clean" ]; then
		CLEAN=1
	else
		echo "${ERRORPREFIX} Bad value (${TARGET}) for option -t|--target"
		HELP=2
		RC=3
	fi
	
	if [ "${CONFIG}" != "Debug" ] && [ "${CONFIG}" != "Release" ]; then
		echo "${ERRORPREFIX} Bad value (${CONFIG}) for option -c|--config"
		HELP=2
		RC=4
	fi
	
	re='^[0-9]+$'
	if ! [[ ${PARALLEL} =~ $re ]] ; then
		echo "${ERRORPREFIX} Bad value (${PARALLEL}) for option -j|--parallel"
		HELP=2
		RC=5
	fi
}

################################################################################
# Debug dump values
################################################################################
dump_vars() {
	echo "GETDEP=${GETDEP}"
	echo "GOOGLETEST=${GOOGLETEST}"
	echo "JSONCPP=${JSONCPP}"
	echo "SWIG=${SWIG}"
	echo "GENSCRIPT=${GENSCRIPT}"
	echo "GENERATE=${GENERATE}"
	echo "COMPILE=${COMPILE}"
	echo "CLEAN=${CLEAN}"
	echo "CONFIG=${CONFIG}"
	echo "TARGET=${TARGET}"
	echo "PARALLEL=${PARALLEL}"
	echo "HELP=${HELP}"
}

################################################################################
# Set compile paths
################################################################################
set_compile_paths() {
	export PATH=${PATH}:${SWIG_DIR}/lib/bin
	export C_INCLUDE_PATH=${GTEST_DIR}/${GOOGLETEST_NAME}/include:${GTEST_DIR}/${GOOGLEMOCK_NAME}/include:${JSONCPP_DIR}/include
	export CPLUS_INCLUDE_PATH=${GTEST_DIR}/${GOOGLETEST_NAME}/include:${GTEST_DIR}/${GOOGLEMOCK_NAME}/include:${JSONCPP_DIR}/include
	export LIBRARY_PATH=${GTEST_DIR}/${BUILD_NAME}/${CONFIG}/lib:${JSONCPP_DIR}/${BUILD_NAME}/${CONFIG}/lib
}

################################################################################
# generate path script
################################################################################
run_genscript() {
	echo "Wrote script ${LIB_PATHS} that you can source to set paths."
	echo "
# Googletest Paths:
#     ${GTEST_DIR}/${BUILD_NAME}/${CONFIG}/lib
#     ${GTEST_DIR}/${GOOGLETEST_NAME}/include
#     ${GTEST_DIR}/${GOOGLEMOCK_NAME}/include
# JSONCPP Paths:
#     ${JSONCPP_DIR}/${BUILD_NAME}/${CONFIG}
# Swig Paths:
#     ${SWIG_DIR}/lib/bin
# Please set:
export PATH=\${PATH}:${SWIG_DIR}/lib/bin
export C_INCLUDE_PATH=${GTEST_DIR}/${GOOGLETEST_NAME}/include:${GTEST_DIR}/${GOOGLEMOCK_NAME}/include:${JSONCPP_DIR}/include
export CPLUS_INCLUDE_PATH=${GTEST_DIR}/${GOOGLETEST_NAME}/include:${GTEST_DIR}/${GOOGLEMOCK_NAME}/include:${JSONCPP_DIR}/include
export LIBRARY_PATH=${GTEST_DIR}/${BUILD_NAME}/${CONFIG}/lib:${JSONCPP_DIR}/${BUILD_NAME}/${CONFIG}/lib
" > ${LIB_PATHS}
}

################################################################################
# Driver when everything checks out
################################################################################
run_func() {
	# Call selected functions
	if [ ${HELP} -gt 0 ]; then
		run_usage $0
	elif [ ${GETDEP} -eq 1 ]; then
		get_dependencies ${CONFIG}
	elif [ ${GOOGLETEST} -eq 1 ]; then
		build_googletest ${CONFIG}
	elif [ ${JSONCPP} -eq 1 ]; then
		build_jsoncpp ${CONFIG}
	elif [ ${SWIG} -eq 1 ]; then
		build_swig ${CONFIG}
	elif [ ${GENSCRIPT} -eq 1 ]; then
		run_genscript ${CONFIG}
	elif [ ${GENERATE} -eq 1 ]; then
		set_compile_paths
		run_generate ${CONFIG}
	elif [ ${COMPILE} -eq 1 ]; then
		set_compile_paths
		run_compile ${CONFIG} ${COMPILENAME} ${PARALLEL}
	elif [ ${CLEAN} -eq 1 ]; then
		run_clean ${CONFIG} ${CLEANNAME}
	fi
}

################################################################################
# Main entry point
################################################################################
main() {
	process_options $@
	set_dependency_paths
	check_values
	# dump_vars
	run_func
}

################################################################################
# Global state variables
################################################################################
GETDEP=0
GOOGLETEST=0
JSONCPP=0
SWIG=0
GENSCRIPT=0
GENERATE=0
COMPILE=0
CLEAN=0
CONFIG=""
TARGET=""
PARALLEL=0
HELP=0

# cmake names
COMPILENAME=all
CLEANNAME=clean

# Messaging, error etc.
ERRORPREFIX="Error:"
RC=0

################################################################################
# Main entry
################################################################################
main $@

################################################################################
# Return error state back to the shell
################################################################################
exit ${RC}
# Check using echo $?

