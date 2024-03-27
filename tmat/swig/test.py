# Setup import from build location
# https://docs.python.org/3/tutorial/modules.html
# https://pyformat.info
# https://www.geeksforgeeks.org/unit-testing-python-unittest/
# https://docs.python.org/3/library/unittest.html
# https://stackoverflow.com/questions/22923696/how-to-wrap-a-c-function-which-takes-in-a-function-pointer-in-python-using-swi
# https://rszalski.github.io/magicmethods/
import sys
import unittest
import pytmat

class TestTransformationMatrix(unittest.TestCase): 

    def setUp(self): 
        pass

    def checkAnyRotationTransformMatrix(self, t,
        v00, v01, v02,
        v10, v11, v12,
        v20, v21, v22,
        size) :
        error = 3 # 0.001
        self.assertAlmostEqual(v00, t.val00(), error)
        self.assertAlmostEqual(v01, t.val01(), error)
        self.assertAlmostEqual(v02, t.val02(), error)
     
        self.assertAlmostEqual(v10, t.val10(), error)
        self.assertAlmostEqual(v11, t.val11(), error)
        self.assertAlmostEqual(v12, t.val12(), error)
     
        self.assertAlmostEqual(v20, t.val20(), error)
        self.assertAlmostEqual(v21, t.val21(), error)
        self.assertAlmostEqual(v22, t.val22(), error)
     
        self.assertEqual(size, sys.getsizeof(t))


    def checkOrthoWith45TransformMatrix(self, t,
        v00, v01, v02,
        v10, v11, v12,
        v20, v21, v22,
        size) :
        error = 3 # 0.001
        self.assertAlmostEqual(v00, t.val00(), error)
        self.assertAlmostEqual(v01, t.val01(), error)
        self.assertAlmostEqual(v02, t.val02(), error)
     
        self.assertAlmostEqual(v10, t.val10(), error)
        self.assertAlmostEqual(v11, t.val11(), error)
        self.assertAlmostEqual(v12, t.val12(), error)
     
        self.assertAlmostEqual(v20, t.val20(), error)
        self.assertAlmostEqual(v21, t.val21(), error)
        self.assertAlmostEqual(v22, t.val22(), error)
     
        self.assertEqual(size, sys.getsizeof(t))


    def checkOrthoTransformMatrix(self, t,
        v00, v01, v02,
        v10, v11, v12,
        v20, v21, v22,
        size) :
        self.assertEqual(v00, t.val00())
        self.assertEqual(v01, t.val01())
        self.assertEqual(v02, t.val02())
     
        self.assertEqual(v10, t.val10())
        self.assertEqual(v11, t.val11())
        self.assertEqual(v12, t.val12())
     
        self.assertEqual(v20, t.val20())
        self.assertEqual(v21, t.val21())
        self.assertEqual(v22, t.val22())
     
        self.assertEqual(size, sys.getsizeof(t))


    def test_anyRotation(self) :
        size = 48 #64 #20
        t = pytmat.clAnyRotationTransformMatrixFloat(-0.0079, 1.0000, 0, -1.0000,-0.0079, 0, 49.7129, 36.3954, 1.0000)
        self.checkAnyRotationTransformMatrix(t, -0.0079, 1.0000, 0, -1.0000,    -0.0079, 0, 49.7129, 36.3954, 1.0000, size)
        t.val(-0.9999, -0.0159, 0, 0.0159, -0.9999, 0, 12.9244, 85.8181, 1.0000)
        self.checkAnyRotationTransformMatrix(t, -0.9999, -0.0159, 0, 0.0159, -0.9999, 0, 12.9244, 85.8181, 1.0000, size)
        t.identity()
        self.checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, 1, 0, 0, 0, 1, size)
        t.mirrorX()
        self.checkAnyRotationTransformMatrix(t, -1, 0, 0, 0, 1, 0, 0, 0, 1, size)
        t.mirrorY()
        self.checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, -1, 0, 0, 0, 1, size)
        t.rotation(1, 1)
        self.checkAnyRotationTransformMatrix(t, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size)
        t.translation(10, 100)
        self.checkAnyRotationTransformMatrix(t, 1, 0, 0, 0, 1, 0, 10, 100, 1, size)

        t1 = pytmat.clAnyRotationTransformMatrixFloat(-0.0079, 1.0000, 0, -1.0000, -0.0079, 0, 49.7129, 36.3954, 1.0000)
        t2 = pytmat.clAnyRotationTransformMatrixFloat()
        t2.rotation(1, 1)
        t1t2 = t1 * t2
        t2t1 = t2 * t1
        self.checkAnyRotationTransformMatrix(t1, -0.0079, 1, 0, -1, -0.0079, 0, 49.7129, 36.3954, 1, size)
        self.checkAnyRotationTransformMatrix(t2, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size)
        self.checkAnyRotationTransformMatrix(t1t2, -0.712693, 0.701521, 0, -0.701521, -0.712693, 0, 9.41689, 60.8878, 1, size)
        self.checkAnyRotationTransformMatrix(t2t1, -0.712693, 0.701521, 0, -0.701521, -0.712693, 0, 49.7129, 36.3954, 1, size)

        error = 3 #0.001
        x, y = t1t2.transform(100, 200)
  
        self.assertAlmostEqual(-202.157, x, error)
        self.assertAlmostEqual(-11.4988, y, error)
        x, y = t1t2.direction(100, 200)
  
        self.assertAlmostEqual(-211.573, x, error)
        self.assertAlmostEqual(-72.3865, y, error)
        x, y = t2t1.transform(100, 200)
  
        self.assertAlmostEqual(-161.861, x, error)
        self.assertAlmostEqual(-35.9911, y, error)
        x, y = t2t1.direction(100, 200)
  
        self.assertAlmostEqual(-211.573, x, error)
        self.assertAlmostEqual(-72.3865, y, error)

        m1 = pytmat.clAnyRotationTransformMatrixFloat()
        m1.translation(100, 200)
        m2 = pytmat.clAnyRotationTransformMatrixFloat()
        m2.translation(300, 400)
        m1m2 = pytmat.clAnyRotationTransformMatrixFloat()
        m1m2 = m1 * m2
        m2m1 = pytmat.clAnyRotationTransformMatrixFloat()
        m2m1 = m2 * m1
    
        self.checkAnyRotationTransformMatrix(m1, 1, 0, 0, 0, 1, 0, 100, 200, 1, size)
        self.checkAnyRotationTransformMatrix(m2, 1, 0, 0, 0, 1, 0, 300, 400, 1, size)
        self.checkAnyRotationTransformMatrix(m1m2, 1, 0, 0, 0, 1, 0, 400, 600, 1, size)
        self.checkAnyRotationTransformMatrix(m2m1, 1, 0, 0, 0, 1, 0, 400, 600, 1, size)

    def test_orthoWith45(self) :
        size = 48 #64 #12
        t1 = pytmat.clOrthoWith45TransformMatrixFloat()
        t1.rotation(1, 1)
        self.checkOrthoWith45TransformMatrix(t1, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size)
        t2 = pytmat.clOrthoWith45TransformMatrixFloat()
        t2.rotation(1, 1)
        self.checkOrthoWith45TransformMatrix(t2, 0.707107, 0.707107, 0, -0.707107, 0.707107, 0, 0, 0, 1, size)
        t1t2 = t1 * t2
        self.checkOrthoWith45TransformMatrix(t1t2, 0, 1, 0, -1, 0, 0, 0, 0, 1, size)
        t3 = pytmat.clOrthoWith45TransformMatrixFloat()
        t3.translation(10, 100)
        self.checkOrthoWith45TransformMatrix(t3, 1, 0, 0, 0, 1, 0, 10, 100, 1, size)
        t1t2t3 = t1 * t2 * t3
        self.checkOrthoWith45TransformMatrix(t1t2t3, 0, 1, 0, -1, 0, 0, 10, 100, 1, size)
    
    
    def test_ortho(self) :
        size = 48 #64 #24
        t1 = pytmat.clOrthoTransformMatrixFloat(-1, 0, 0, 0, 1, 0, 100, 200, 1)
        self.checkOrthoTransformMatrix(t1, -1, 0, 0, 0, 1, 0, 100, 200, 1, size)
        t2 = pytmat.clOrthoTransformMatrixFloat(1, 0, 0, 0, -1, 0, 200, 100, 1)
        self.checkOrthoTransformMatrix(t2, 1, 0, 0, 0, -1, 0, 200, 100, 1, size)
        t1t2 = t1*t2
        self.checkOrthoTransformMatrix(t1t2, -1, 0, 0, 0, -1, 0, 300, -100, 1, size)
        t2t1 = t2*t1
        self.checkOrthoTransformMatrix(t2t1, -1, 0, 0, 0, -1, 0, -100, 300, 1, size)


if __name__ == '__main__': 
    unittest.main() 
