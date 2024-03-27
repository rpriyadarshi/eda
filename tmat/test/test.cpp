/*******************************************************************************
 * Copyright (c) 2001-2020 Rohit Priyadarshi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <iostream>
#include <bitset>
#include <math.h>

#include <gtest/gtest.h>

#include "common.h"
#include "trm.h"

using AnyRotType = float;
using OrthoWith45Type = float;
using OrthoType = long;

void checkAnyRotationTransformMatrix(utl::AnyRotationTransformMatrix<AnyRotType>& t,
    const AnyRotType v00, const AnyRotType v01, const AnyRotType v02,
    const AnyRotType v10, const AnyRotType v11, const AnyRotType v12,
    const AnyRotType v20, const AnyRotType v21, const AnyRotType v22,
    size_t size)
{
    AnyRotType error = 0.001;
    EXPECT_NEAR(v00, t.val00(), error);
    EXPECT_NEAR(v01, t.val01(), error);
    EXPECT_NEAR(v02, t.val02(), error);

    EXPECT_NEAR(v10, t.val10(), error);
    EXPECT_NEAR(v11, t.val11(), error);
    EXPECT_NEAR(v12, t.val12(), error);

    EXPECT_NEAR(v20, t.val20(), error);
    EXPECT_NEAR(v21, t.val21(), error);
    EXPECT_NEAR(v22, t.val22(), error);

    EXPECT_EQ(size, sizeof(t));
}

void checkOrthoWith45TransformMatrix(utl::OrthoWith45TransformMatrix<OrthoWith45Type>& t,
    const OrthoWith45Type v00, const OrthoWith45Type v01, const OrthoWith45Type v02,
    const OrthoWith45Type v10, const OrthoWith45Type v11, const OrthoWith45Type v12,
    const OrthoWith45Type v20, const OrthoWith45Type v21, const OrthoWith45Type v22,
    size_t size)
{
    AnyRotType error = 0.001;
    EXPECT_NEAR(v00, t.val00(), error);
    EXPECT_NEAR(v01, t.val01(), error);
    EXPECT_NEAR(v02, t.val02(), error);

    EXPECT_NEAR(v10, t.val10(), error);
    EXPECT_NEAR(v11, t.val11(), error);
    EXPECT_NEAR(v12, t.val12(), error);

    EXPECT_NEAR(v20, t.val20(), error);
    EXPECT_NEAR(v21, t.val21(), error);
    EXPECT_NEAR(v22, t.val22(), error);

    EXPECT_EQ(size, sizeof(t));
}

void checkOrthoTransformMatrix(utl::OrthoTransformMatrix<OrthoType>& t,
    const OrthoType v00, const OrthoType v01, const OrthoType v02,
    const OrthoType v10, const OrthoType v11, const OrthoType v12,
    const OrthoType v20, const OrthoType v21, const OrthoType v22,
    size_t size)
{
    EXPECT_EQ(v00, t.val00());
    EXPECT_EQ(v01, t.val01());
    EXPECT_EQ(v02, t.val02());

    EXPECT_EQ(v10, t.val10());
    EXPECT_EQ(v11, t.val11());
    EXPECT_EQ(v12, t.val12());

    EXPECT_EQ(v20, t.val20());
    EXPECT_EQ(v21, t.val21());
    EXPECT_EQ(v22, t.val22());

    EXPECT_EQ(size, sizeof(t));
}

TEST(anyRotation, matrix)
{
    const size_t size = 20;
    utl::AnyRotationTransformMatrix<AnyRotType> t(-0.0079, 1.0000, 0, -1.0000,
            -0.0079, 0, 49.7129, 36.3954, 1.0000);
    checkAnyRotationTransformMatrix(t, -0.0079, 1.0000, 0, -1.0000,    -0.0079, 0, 49.7129, 36.3954, 1.0000, size);
    t.val(-0.9999, -0.0159, 0, 0.0159, -0.9999, 0, 12.9244, 85.8181, 1.0000);
    checkAnyRotationTransformMatrix(t, -0.9999, -0.0159, 0, 0.0159, -0.9999, 0, 12.9244, 85.8181, 1.0000, size);
    t.identity();
    checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, 1, 0, 0, 0, 1, size);
    t.mirrorX();
    checkAnyRotationTransformMatrix(t, -1, 0, 0, 0, 1, 0, 0, 0, 1, size);
    t.mirrorY();
    checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, -1, 0, 0, 0, 1, size);
    t.rotation(1, 1);
    checkAnyRotationTransformMatrix(t, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size);
    t.translation(10, 100);
    checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, 1, 0, 10, 100, 1, size);

    utl::AnyRotationTransformMatrix<AnyRotType> t1(-0.0079, 1.0000, 0, -1.0000, -0.0079, 0, 49.7129, 36.3954, 1.0000);
    utl::AnyRotationTransformMatrix<AnyRotType> t2;
    t2.rotation(1, 1);
    utl::AnyRotationTransformMatrix<AnyRotType> t1t2 = t1 * t2;
    utl::AnyRotationTransformMatrix<AnyRotType> t2t1 = t2 * t1;
    checkAnyRotationTransformMatrix(t1, -0.0079, 1, 0, -1, -0.0079, 0, 49.7129, 36.3954, 1, size);
    checkAnyRotationTransformMatrix(t2, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size);
    checkAnyRotationTransformMatrix(t1t2, -0.712693, 0.701521, 0, -0.701521, -0.712693, 0, 9.41689, 60.8878, 1, size);
    checkAnyRotationTransformMatrix(t2t1, -0.712693, 0.701521, 0, -0.701521, -0.712693, 0, 49.7129, 36.3954, 1, size);

    AnyRotType x, y, error = 0.001;
    t1t2.transform(100, 200, x, y);
    //cout << x << " " << y << endl;
    EXPECT_NEAR(-202.157, x, error);
    EXPECT_NEAR(-11.4988, y, error);
    t1t2.direction(100, 200, x, y);
    //cout << x << " " << y << endl;
    EXPECT_NEAR(-211.573, x, error);
    EXPECT_NEAR(-72.3865, y, error);
    t2t1.transform(100, 200, x, y);
    //cout << x << " " << y << endl;
    EXPECT_NEAR(-161.861, x, error);
    EXPECT_NEAR(-35.9911, y, error);
    t2t1.direction(100, 200, x, y);
    //cout << x << " " << y << endl;
    EXPECT_NEAR(-211.573, x, error);
    EXPECT_NEAR(-72.3865, y, error);

    utl::AnyRotationTransformMatrix<AnyRotType> m1;
    m1.translation(100, 200);
    utl::AnyRotationTransformMatrix<AnyRotType> m2;
    m2.translation(300, 400);
    utl::AnyRotationTransformMatrix<AnyRotType> m1m2;
    m1m2 = m1 * m2;
    utl::AnyRotationTransformMatrix<AnyRotType> m2m1;
    m2m1 = m2 * m1;

    checkAnyRotationTransformMatrix(m1, 1, 0, 0, 0, 1, 0, 100, 200, 1, size);
    checkAnyRotationTransformMatrix(m2, 1, 0, 0, 0, 1, 0, 300, 400, 1, size);
    checkAnyRotationTransformMatrix(m1m2, 1, 0, 0, 0, 1, 0, 400, 600, 1, size);
    checkAnyRotationTransformMatrix(m2m1, 1, 0, 0, 0, 1, 0, 400, 600, 1, size);
}

TEST(orthoWith45, matrix)
{
    size_t size = 12;
    utl::OrthoWith45TransformMatrix<OrthoWith45Type> t1;
    t1.rotation(1, 1);
    checkOrthoWith45TransformMatrix(t1, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size);
    utl::OrthoWith45TransformMatrix<OrthoWith45Type> t2;
    t2.rotation(1, 1);
    checkOrthoWith45TransformMatrix(t2, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size);
    utl::OrthoWith45TransformMatrix<OrthoWith45Type> t1t2 = t1 * t2;
    checkOrthoWith45TransformMatrix(t1t2, 0, 1, 0, -1, 0, 0, 0, 0, 1, size);
    utl::OrthoWith45TransformMatrix<OrthoWith45Type> t3;
    t3.translation(10, 100);
    checkOrthoWith45TransformMatrix(t3, 1, 0, 0, 0, 1, 0, 10, 100, 1, size);
    utl::OrthoWith45TransformMatrix<OrthoWith45Type> t1t2t3 = t1 * t2 * t3;
    checkOrthoWith45TransformMatrix(t1t2t3, 0, 1, 0, -1, 0, 0, 10, 100, 1, size);
}

TEST(orthoTest, matrix)
{
    size_t size = 24;
    utl::OrthoTransformMatrix<OrthoType> t1(-1, 0, 0, 0, 1, 0, 100, 200, 1);
    checkOrthoTransformMatrix(t1, -1, 0, 0, 0, 1, 0, 100, 200, 1, size);
    utl::OrthoTransformMatrix<OrthoType> t2(1, 0, 0, 0, -1, 0, 200, 100, 1);
    checkOrthoTransformMatrix(t2, 1, 0, 0, 0, -1, 0, 200, 100, 1, size);
    utl::OrthoTransformMatrix<OrthoType> t1t2 = t1*t2;
    checkOrthoTransformMatrix(t1t2, -1, 0, 0, 0, -1, 0, 300, -100, 1, size);
    utl::OrthoTransformMatrix<OrthoType> t2t1 = t2*t1;
    checkOrthoTransformMatrix(t2t1, -1, 0, 0, 0, -1, 0, -100, 300, 1, size);
}
