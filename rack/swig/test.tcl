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

lappend auto_path .
package require rack_int

proc dumpMatrix {m s} {
    puts $s
    puts -nonewline    [$m val00]
    puts -nonewline "\t"
    puts -nonewline [$m val01]
    puts -nonewline     "\t"
    puts             [$m val02]
    
    puts -nonewline [$m val10]
    puts -nonewline "\t"
    puts -nonewline [$m val11]
    puts -nonewline     "\t"
    puts             [$m val12]
    
    puts -nonewline [$m val20]
    puts -nonewline "\t"
    puts -nonewline [$m val21]
    puts -nonewline "\t"
    puts             [$m val22]
}

proc testOrthoTransformMatrix {} {
    rkcOrthoTransformMatrixDouble t1;
    rkcOrthoTransformMatrixDouble t2;
    
    t1 val -1 0 0 0 1 0 100 200 1
    dumpMatrix t1 "Configuring orthogonal Matrix t1..."
    t2 val 1 0 0 0 -1 0 200 100 1
    dumpMatrix t2 "Configuring orthogonal Matrix t2..."
    
    set t1t2 [rkfOrthoTransformMatrixMultiplyDouble t1 t2]
    dumpMatrix $t1t2 "Multiplying Matrices t1*t2..."
    
    set t2t1 [rkfOrthoTransformMatrixMultiplyDouble t2 t1]
    dumpMatrix $t2t1 "Multiplying Matrices t2*t1..."
}

proc testOrthoWith45TransformMatrix {} {
    rkcOrthoWith45TransformMatrixDouble t1;
    t1 rotation 1 1
    dumpMatrix t1 "Configuring orthogonal Matrix with 45 deg. t1..."
    
    rkcOrthoWith45TransformMatrixDouble t2;
    t2 rotation 1 1
    dumpMatrix t2 "Configuring orthogonal Matrix with 45 deg. t2..."
    
    set t1t2 [rkfOrthoWith45TransformMatrixMultiplyDouble t1 t2]
    dumpMatrix $t1t2 "Multiplying Matrices t1*t2..."
    
    rkcOrthoWith45TransformMatrixDouble t3;
    t3 translation 10 100
    dumpMatrix t3 "Configuring orthogonal Matrix with 45 deg. t3..."
    
    set t1t2t3 [rkfOrthoWith45TransformMatrixMultiplyDouble $t1t2 t3]
    dumpMatrix $t1t2t3 "Multiplying Matrices t1*t2*t3..."
}

proc testString {} {
    rkcString s0 "Rohit"
    rkcString s1 "Priyadarshi"
    rkcString s2 "Rohit Priyadarshi"
    
    # Operations
    s0 size
    s1 size
    s2 size
    
    s2 find "Priyadarshi"
    s2 find s1
    s2 find s0
    s2 c_str
    s2 data
    s2 compare s1
    s2 compare s0
    s2 compare "Rohit Priyadarshi"
    #s2 compare [s0 + s1]
    
    rkcString s3 "Rohit"
    s3 add " "
    s3 size
    s3 add s1
    s2 compare s3
}

proc testMultistring {} {
    rkcString s0 "Rohit"
    rkcString s1 "Priyadarshi"
    rkcString s2 "Rohit Priyadarshi"
    
    rkcMultiString ms0 s0
    rkcMultiString ms1 s1
    rkcMultiString ms2 s2
    rkcMultiString ms3 s0
    
    ms0 size
    ms1 size
    ms2 size
    ms3 size
    
    [ms0 index 0] index 0
    [ms0 index 0] index 1
    [ms0 index 0] index 2    
    [ms0 index 0] index 3    
    [ms0 index 0] index 4
    
    [ms1 index 0] index 0
    
    ms0 compare ms1
    ms1 compare ms0
    ms0 compare ms3
    
    ms3 add s1
    ms3 size
    [ms3 index 0] size
    [ms3 index 1] size
    
    ms0 compare ms3
    ms3 compare ms1
}

proc testMask {} {
    rkcMask m0 11
    rkcMask m1 12
    rkcMask m2 13
    
    m0 size
    m1 size
    m2 size
    
    [m0 to_str] c_str
    m0 to_ulong
}

testOrthoWith45TransformMatrix
testOrthoTransformMatrix

testMultistring
testString
testMask

