# Introduction 
Electronic Design Automation industry builds very complex software systems. Every company ends up building a propitiatory netlist data model which conceptually stays the same across organizations. This is an attempt to create such a system which stays general purpose enough, similar to Standard Template Library in C++. Hence an attempt would be made to make it freely available.

# Getting Started
Simply download and compile along with your software systems. The software uses cmake build system
1.	Installation process
2.	Software dependencies
    - [Google Test](https://github.com/google/googletest)
    - [JsonCpp](https://github.com/open-source-parsers/jsoncpp)
3.	Latest releases
4.	API references

# Build Script
There is a build script ```linux/build.sh``` that you may find useful. Here is how you can use it:
```
 This utility must be run from the directory which has CMakeLists.txt
Usage:
    -c|--config <Debug|Release>           Choose build config
    -t|--target <getdep|                  Download all external dependancies
                 googletest|              Compile googletest
                 jsoncpp|                 Compile jsoncpp
                 swig|                    Compile swig
                 genscript|               Generate path script
                 generate|                cmake generate for the main product
                 compile|                 Compile main product
                 clean>                   Clean main product
    -j|--parallel <number>                Number of parallel compile jobs
    -h|--help                             This help message
Examples:
    ./build.sh --config Debug --target getdep
    ./build.sh --config Debug --target googletest
    ./build.sh --config Debug --target jsoncpp
    ./build.sh --config Debug --target swig
    ./build.sh --config Debug --target genscript
    ./build.sh --config Debug --target generate
    ./build.sh --config Debug --target compile
    ./build.sh --config Debug --target compile --parallel 4
    ./build.sh -c Debug -t compile -j 4
    ./build.sh --config Debug --target unittests
    ./build.sh --config Debug --target clean
```
## Option ```--config```
You can specify either ```Debug``` or ```Release``` to compile in these modes. This applies to all compile options that are cmake based.
## Option ```--target```
### Value ```getdep```
This option downloads all the dependencies in ```linux/libs```. This does not compile anything.
### Value ```googletest```
Runs compile on googletest, using Debug/Release config.
### Value ```jsoncpp```
Runs compile on jsoncpp, using Debug/Release config.
### Value ```swig```
Runs compile on swig, using using its own gnu config.
### Value ```genscript```
Generates a shell script that you can source to setup include and lib paths for compiles.
### Value ```generate```
Runs product cmake generate
### Value ```compile```
Runs product compile
### Value ```clean```
Cleans up the product build.
## Option ```--parallel```
Any numeric value after this option specifies how many parallel compiles are run.
## Option ```--help```
Prints the help message

# Build and Test
```
cd linux
mkdir build
cd build
cmake -G "Unix Makefiles" -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug --target all -- -j 4
cmake --build . --config Debug --target clean --

```

# Google Test
## Clone Google Test
```
export GTEST_DIR=~/<some-location>/libs/googletest

mkdir ${GTEST_DIR}
cd ${GTEST_DIR}
git clone https://github.com/google/googletest.git
```
## Run build
```
mkdir build             # Create a directory to hold the build output.
cd build
cmake ..                # Generate native build scripts.
# Or with testcases
cmake -Dgtest_build_samples=ON -D CMAKE_C_COMPILER=gcc -D CMAKE_CXX_COMPILER=g++ ..
# Compile
cmake --build . --config Debug --target all -- -j 4
```
## Library
```
${GTEST_DIR}/build/lib
```
## Headers
```
${GTEST_DIR}/googletest/include
${GTEST_DIR}/googlemock/include
```
## Refrences
[Google Test](https://github.com/google/googletest)
[Generic Build Instructions](https://github.com/google/googletest/blob/master/googletest/README.md)
[Googletest Primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md)

# JsonCpp
## Clone JsonCpp
```
git clone https://github.com/open-source-parsers/jsoncpp.git
```
## Building
### Debug
```
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
make
```
### Release
```
mkdir -p build/release
cd build/release
cmake -DCMAKE_BUILD_TYPE=release -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
make
```
## References
[Getting JsonCpp](https://github.com/open-source-parsers/jsoncpp)
[Documentation](http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html)
[Wiki](https://github.com/open-source-parsers/jsoncpp/wiki)
[Building](https://github.com/open-source-parsers/jsoncpp/wiki/Building)

## Getting SWIG
### Download SWIG
```
export SWIGVER=4.2.1
wget https://sourceforge.net/projects/swig/files/swig/swig-4.0.1/swig-${SWIGVER}.tar.gz
```
### Optionally clone SWIG
```
git clone https://github.com/swig/swig.git
mv swig swig-${SWIGVER}
```
## Run Autogen
```
cd swig-${SWIGVER}
./autogen.sh
```

## Run Configure
```
mkdir swig-${SWIGVER}-build
cd swig-${SWIGVER}-build
../swig-${SWIGVER}/configure --prefix=/opt/swig/${SWIGVER}
```

## Run build
```
make
make install
```

## Check build
```
make -k check
```
This could take up to an hour or longer. If you are interested in a particular language, just check the examples and test-suite for that language. For example, the Python tests:
```
make check-python-examples
make check-python-test-suite
```

## References
[SWIG](http://www.swig.org/)
[Building SWIG](http://www.swig.org/svn.html)
[CMake Example](https://github.com/Mizux/cmake-swig)

# Contribute
You are welcome to contribute to this code as long as the licensing is honored.

# Running tests
There are a number of test drivers available, testing different parts of the code
```
./linux/build/rack/test/racktest
./linux/build/utl/test/utltest
./linux/build/sig/test/sigtest
./linux/build/algo/test/algotest
./linux/build/tmat/test/tmattest
```
You can also run python module generated by swig.
```
cd ./linux/build/swig
ls
...
pyrack.py _pyrack.so
...

python
>>> import pyrack
help(pyrack)
```
You can now run commands from test.py seen in the top level swig directory
```
cd ./swig
ls
...
test.py  test.tcl ...
...
```
# IDE Setup
## Eclipse
Build process has been simplified with the help of build.sh script under ```linux/build.sh```
### C/C++ Build Example
#### Debug build
* Build command: ```${workspace_loc:/eda/linux}/build.sh```
* Build directory: ```${workspace_loc:/eda/linux}/```
* Build (Incremental build): ```--config Debug --target compile```
* Clean: ```--config Debug --target clean```

#### Configure debug
* Build command: ```${workspace_loc:/eda/linux}/build.sh```
* Build directory: ```${workspace_loc:/eda/linux}/```
* Build (Incremental build): ```--config Debug --target generate```

# About
*THIS IS AN UNPUBLISHED WORK, CREATED IN 2001*
 
Copyright (c) 2001-2020 Rohit Priyadarshi

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

- [Comparison of free and open-source software licenses](https://en.wikipedia.org/wiki/Comparison_of_free_and_open-source_software_licenses)
- [MIT License](https://en.wikipedia.org/wiki/MIT_License)
- [The MIT License](https://opensource.org/licenses/MIT)

_Rohit Priyadarshi_