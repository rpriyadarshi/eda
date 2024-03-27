#ifndef __TRM_H__
#define __TRM_H__
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

namespace utl {

////////////////////////////////////////////////////////////////////////
// Transformations:
// ----------------
// A point (x,y) given in the symbol is transformed to a point (x',y')
// in a chip coordinate system by a 3x3 transform matrix T:
// 
//                   [x' y' 1] = [x y 1]T
// 
// The matrix T is itself the product of primitive transformations
// specified in the call: T = T1 T2 T3, where T1 is primitive 
// transformation matrix obtained from the first transformation 
// primitive given in the call, T2 from the second, and T3 from the 
// third (of course there may be fewer or more than three primitive 
// transformations specified in a call). These matrices are obtained 
// using the following templates for each kind of primitive 
// transformation.
// 
//                   Txy    Tn =  1   0   0
//                                0   1   0
//                                x   y   1
//
//                   MX     Tn = -1   0   0
//                                0   1   0
//                                0   0   1
// 
//                   MY     Tn =  1   0   0
//                                0  -1   0
//                                0   0   1
// 
//                   Tab    Tn =  a/c b/c 0
//                               -b/c a/c 0 where c = (a^2 + b^2)^1/2
//                                0   0   1
// 
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Template Class TransformMatrixInterface:
// 
// This class shows only the usable public interface to the transform 
// matrix classes. This class is necessary only to enable quick 
// switching of matrix type in the code, while keeping the interface 
// the same.
//////////////////////////////////////////////////////////////////////
template <class _Tp>
class TransformMatrixInterface {
public: // constructors/destructors
    TransformMatrixInterface() {}
    TransformMatrixInterface(
        const _Tp v00, const _Tp v01, const _Tp v02, 
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) { 
            assert(0); 
    }
    ~TransformMatrixInterface() {}

public: 
    // access functions
    const _Tp val00() const { assert(0); return 0; }
    const _Tp val01() const { assert(0); return 0; }
    const _Tp val02() const { assert(0); return 0; }
    const _Tp val10() const { assert(0); return 0; }
    const _Tp val11() const { assert(0); return 0; }
    const _Tp val12() const { assert(0); return 0; }
    const _Tp val20() const { assert(0); return 0; }
    const _Tp val21() const { assert(0); return 0; }
    const _Tp val22() const { assert(0); return 0; }

    // set functions
    void val(
        const _Tp v00, const _Tp v01, const _Tp v02, 
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) { 
            assert(0); 
    }

    // initialization functions.
    void identity() { assert(0); }
    void mirrorX() { assert(0); }
    void mirrorY() { assert(0); }
    void translation(const _Tp x, const _Tp y) { 
        assert(0); 
    }
    void rotation(const _Tp a, const _Tp b) { 
        assert(0); 
    }

    // interface functions.
#ifdef SWIG
    %apply _Tp& OUTPUT { _Tp& rx, _Tp& ry };
#endif /* SWIG */
    void transform(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const { 
        assert(0); 
    }
    void direction(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const { 
        assert(0); 
    }

    // utility functions.
    void dump(std::ostream& o) const { assert(0); }
};

//////////////////////////////////////////////////////////////////////
// Template Class AnyRotationTransformMatrix:
// 
// This class implements ability to keep any rotation for an object.
//////////////////////////////////////////////////////////////////////
template <class _Tp>
class AnyRotationTransformMatrix : 
    public TransformMatrixInterface<_Tp> {
public: // constructors/destructors
    AnyRotationTransformMatrix() 
        : m_sign00(0), m_sign01(0), m_sign10(0), m_sign11(0) {
            identity(); 
    }
    AnyRotationTransformMatrix(
        const _Tp v00, const _Tp v01, const _Tp v02,
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) 
        : m_sign00(0), m_sign01(0), m_sign10(0), m_sign11(0) {
            val(v00, v01, v02, v10, v11, 
                v12, v20, v21, v22); 
    }
    ~AnyRotationTransformMatrix() {}

public: // functions
    // access functions
    const _Tp val00() const { return sign00() == 0 ? rac() : -rac(); }
    const _Tp val01() const { return sign01() == 0 ? rbc() : -rbc(); }
    const _Tp val02() const { return 0; }
    const _Tp val10() const { return sign10() == 0 ? rbc() : -rbc(); }
    const _Tp val11() const { return sign11() == 0 ? rac() : -rac(); }
    const _Tp val12() const { return 0; }
    const _Tp val20() const { return tx(); }
    const _Tp val21() const { return ty(); }
    const _Tp val22() const { return 1; }

    // set functions
    void val(
        const _Tp v00, const _Tp v01, const _Tp v02, 
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) { 
            sign00(0); sign01(0); sign10(0); sign11(0); 
            val00(v00); val01(v01); val02(v02); 
            val10(v10); val11(v11); val12(v12); 
            val20(v20); val21(v21); val22(v22); 
    }

    // initialization functions.
    void identity() { val(1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorX() { val(-1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorY() { val(1, 0, 0, 0, -1, 0, 0, 0, 1); }
    void translation(const _Tp x, const _Tp y) { 
        val(1, 0, 0, 0, 1, 0, x, y, 1); 
    }
    void rotation(const _Tp a, const _Tp b) { 
         const double c = sqrt(a*a+b*b);
        val((const _Tp)(a/c), (const _Tp)(b/c), 0, 
            (const _Tp)(-b/c), (const _Tp)(a/c), 0, 0, 0, 1);
    }

    // interface functions.
#ifdef SWIG
    %apply _Tp& OUTPUT { _Tp& rx, _Tp& ry };
#endif /* SWIG */
    void transform(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10()+val20();
        ry = x*val01()+y*val11()+val21();
    }
    void direction(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10();
        ry = x*val01()+y*val11();
    }

    // utility functions.
    void dump(std::ostream& o) const;

#ifdef SWIG
    %extend {
        AnyRotationTransformMatrix<_Tp> __mul__(
            const AnyRotationTransformMatrix<_Tp>& m2) {
        	return AnyRotationTransformMatrixMultiply(*self, m2);
        }
    }
#endif /* SWIG */

private: // functions
    // data get functions
    const unsigned sign00() const { return m_sign00; }
    const unsigned sign01() const { return m_sign01; }
    const unsigned sign10() const { return m_sign10; }
    const unsigned sign11() const { return m_sign11; }
    const _Tp rac() const { return m_rac; }
    const _Tp rbc() const { return m_rbc; }
    const _Tp tx() const { return m_tx; }
    const _Tp ty() const { return m_ty; }

    // data set functions
    void sign00(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign00 = v; }
    void sign01(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign01 = v; }
    void sign10(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign10 = v; }
    void sign11(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign11 = v; }
    void rac(const _Tp v) { m_rac = v; }
    void rbc(const _Tp v) { m_rbc = v; }
    void tx(const _Tp v) { m_tx = v; }
    void ty(const _Tp v) { m_ty = v; }

    // set access functions
    void val00(const _Tp v) { 
        if (v < 0) { rac(-v); sign00(1); } else rbc(v); }
    void val01(const _Tp v) { 
        if (v < 0) { rbc(-v); sign01(1); } else rac(v); }
    void val02(const _Tp v) { }
    void val10(const _Tp v) { 
        if (v < 0) { rbc(-v); sign10(1); } else rbc(v); }
    void val11(const _Tp v) { 
        if (v < 0) { rac(-v); sign11(1); } else rac(v); }
    void val12(const _Tp v) { }
    void val20(const _Tp v) { tx(v); }
    void val21(const _Tp v) { ty(v); }
    void val22(const _Tp v) { }

private: // data
    unsigned m_sign00:1;
    unsigned m_sign01:1;
    unsigned m_sign10:1;
    unsigned m_sign11:1;
    _Tp m_rac;
    _Tp m_rbc;
    _Tp m_tx;
    _Tp m_ty;
};

template <class _Tp> 
inline void 
AnyRotationTransformMatrix<_Tp>::dump(std::ostream& o) const {
    o << "ARTM {" << std::endl;
        
    o << "  MATRIX {" << std::endl;
    o << "    " << val00() << "\t" << val01() << "\t" << val02() 
        << std::endl;
    o << "    " << val10() << "\t" << val11() << "\t" << val12() 
        << std::endl;
    o << "    " << val20() << "\t" << val21() << "\t" << val22() 
        << std::endl;
    o << "  }" << std::endl;

    o << "  SIGN_BITS {" << std::endl;
    o << "    ";
    o << sign11() << sign10() << sign01() << sign00() << std::endl;
    o << "  }" << std::endl;

    o << "  SIZE {" << std::endl;
    o << "    " << sizeof(*this) << std::endl;
    o << "  }" << std::endl;
    o << "}" << std::endl;

    o << "MATH {";
    o << "{" << val00() << ", " << val01() << ", " << val02() << "}, ";
    o << "{" << val10() << ", " << val11() << ", " << val12() << "}, ";
    o << "{" << val20() << ", " << val21() << ", " << val22() << "}";
    o << "}" << std::endl;
}

template <class _Tp>
inline AnyRotationTransformMatrix<_Tp> AnyRotationTransformMatrixMultiply(
    const AnyRotationTransformMatrix<_Tp>& m1, 
    const AnyRotationTransformMatrix<_Tp>& m2) {
  AnyRotationTransformMatrix<_Tp> m(
    m1.val00()*m2.val00()+m1.val01()*m2.val10(),
    m1.val00()*m2.val01()+m1.val01()*m2.val11(),
    0, 
    m1.val10()*m2.val00()+m1.val11()*m2.val10(),
    m1.val10()*m2.val01()+m1.val11()*m2.val11(),
    0, 
    m1.val20()*m2.val00()+m1.val21()*m2.val10()+m2.val20(),
    m1.val20()*m2.val01()+m1.val21()*m2.val11()+m2.val21(),
    1);
  return m;
}

// operator definitions
template <class _Tp>
inline AnyRotationTransformMatrix<_Tp> operator*(
    const AnyRotationTransformMatrix<_Tp>& m1, 
    const AnyRotationTransformMatrix<_Tp>& m2) {
    return AnyRotationTransformMatrixMultiply(m1, m2);
}
//////////////////////////////////////////////////////////////////////
// Template Class OrthoWith45TransformMatrix:
// 
// This class implements ability to keep only orthogonal rotation for 
// an object.
//////////////////////////////////////////////////////////////////////
template <class _Tp>
class OrthoWith45TransformMatrix : 
    public TransformMatrixInterface<_Tp> {
public: // constructors/destructors
    OrthoWith45TransformMatrix() 
        : m_sign00(0), m_sign01(0), m_sign10(0), m_sign11(0) {
            identity(); 
    }
    OrthoWith45TransformMatrix(
        const _Tp v00, const _Tp v01, const _Tp v02,
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) 
        : m_sign00(0), m_sign01(0), m_sign10(0), m_sign11(0) {
            val(v00, v01, v02, v10, v11, 
                v12, v20, v21, v22); }
    ~OrthoWith45TransformMatrix() {}

public: // functions
    // access functions
    const _Tp val00() const { 
         _Tp data = (int)(data00()&2) == 0 ? data00() 
            : (_Tp)cOneByRootTwo;
        return sign00() == 0 ? data : -data; 
    }
    const _Tp val01() const { 
         _Tp data = (int)(data01()&2) == 0 ? data01() 
            : (_Tp)cOneByRootTwo;
        return sign01() == 0 ? data : -data; 
    }
    const _Tp val02() const { return 0; }
    const _Tp val10() const { 
         _Tp data = (int)(data10()&2) == 0 ? data10() 
            : (_Tp)cOneByRootTwo;
        return sign10() == 0 ? data : -data; 
    }
    const _Tp val11() const { 
         _Tp data = (int)(data11()&2) == 0 ? data11() 
            : (_Tp)cOneByRootTwo;
        return sign11() == 0 ? data : -data; 
    }
    const _Tp val12() const { return 0; }
    const _Tp val20() const { return tx(); }
    const _Tp val21() const { return ty(); }
    const _Tp val22() const { return 1; }

    // set functions
    void val(
        const _Tp v00, const _Tp v01, const _Tp v02, 
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) { 
            sign00(0); sign01(0); sign10(0); sign11(0); 
            val00(v00); val01(v01); val02(v02); 
            val10(v10); val11(v11); val12(v12); 
            val20(v20); val21(v21); val22(v22); 
    }

    // initialization functions.
    void identity() { val(1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorX() { val(-1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorY() { val(1, 0, 0, 0, -1, 0, 0, 0, 1); }
    void translation(const _Tp x, const _Tp y) { 
        val(1, 0, 0, 0, 1, 0, x, y, 1); 
    }
    void rotation(const _Tp a, const _Tp b) { 
         const double c = sqrt(a*a+b*b);
        val((const _Tp)(a/c), (const _Tp)(b/c), 0, 
            (const _Tp)(-b/c), (const _Tp)(a/c), 0, 0, 0, 1);
    }

    // interface functions.
#ifdef SWIG
    %apply _Tp& OUTPUT { _Tp& rx, _Tp& ry };
#endif /* SWIG */
    void transform(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10()+val20();
        ry = x*val01()+y*val11()+val21();
    }
    void direction(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10();
        ry = x*val01()+y*val11();
    }

    // utility functions.
    void dump(std::ostream& o) const;

#ifdef SWIG
    %extend {
        OrthoWith45TransformMatrix<_Tp> __mul__(
            const OrthoWith45TransformMatrix<_Tp>& m2) {
        	return OrthoWith45TransformMatrixMultiply(*self, m2);
        }
    }
#endif /* SWIG */

private: // functions
    // data get functions
    const unsigned sign00() const { return m_sign00; }
    const unsigned sign01() const { return m_sign01; }
    const unsigned sign10() const { return m_sign10; }
    const unsigned sign11() const { return m_sign11; }
    const unsigned data00() const { return m_data00; }
    const unsigned data01() const { return m_data01; }
    const unsigned data10() const { return m_data10; }
    const unsigned data11() const { return m_data11; }
    const _Tp tx() const { return m_tx; }
    const _Tp ty() const { return m_ty; }

    // data set functions
    void sign00(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign00 = v; }
    void sign01(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign01 = v; }
    void sign10(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign10 = v; }
    void sign11(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign11 = v; }
    void data00(const unsigned v) { 
        assert(v == 0 || v == 1 || v == 2 || v == 3);
        m_data00 = v; }
    void data01(const unsigned v) { 
        assert(v == 0 || v == 1 || v == 2 || v == 3);
        m_data01 = v; }
    void data10(const unsigned v) { 
        assert(v == 0 || v == 1 || v == 2 || v == 3);
        m_data10 = v; }
    void data11(const unsigned v) { 
        assert(v == 0 || v == 1 || v == 2 || v == 3);
        m_data11 = v; }
    void tx(const _Tp v) { m_tx = v; }
    void ty(const _Tp v) { m_ty = v; }

    // set access functions
    void val00(const _Tp v) { 
        if (v < 0) { 
            data00((unsigned) // value being set must be unsigned.
                (-v > 0.6 && -v < 0.8 ? 3 : -floor(v))); 
            sign00(1); 
        } else { 
            data00((unsigned) // value being set must be unsigned.
                (v > 0.6 && v < 0.8 ? 3 : ceil(v))); 
        } 
    }
    void val01(const _Tp v) { 
        if (v < 0) { 
            data01((unsigned) // value being set must be unsigned.
                (-v > 0.6 && -v < 0.8 ? 3 : -floor(v))); 
            sign01(1); 
        } else {
            data01((unsigned) // value being set must be unsigned.
                (v > 0.6 && v < 0.8 ? 3 : ceil(v))); 
        }
    }
    void val02(const _Tp v) { }
    void val10(const _Tp v) { 
        if (v < 0) { 
            data10((unsigned) // value being set must be unsigned.
                (-v > 0.6 && -v < 0.8 ? 3 : -floor(v))); 
            sign10(1); 
        } else {
            data10((unsigned) // value being set must be unsigned.
                (v > 0.6 && v < 0.8 ? 3 : ceil(v))); 
        }
    }
    void val11(const _Tp v) { 
        if (v < 0) { 
            data11((unsigned) // value being set must be unsigned.
                (-v > 0.6 && -v < 0.8 ? 3 : -floor(v))); 
            sign11(1); 
        } else {
            data11((unsigned) // value being set must be unsigned.
                (v > 0.6 && v < 0.8 ? 3 : ceil(v))); 
        }
    }
    void val12(const _Tp v) { }
    void val20(const _Tp v) { tx(v); }
    void val21(const _Tp v) { ty(v); }
    void val22(const _Tp v) { }

private: // data
    unsigned m_sign00:1;
    unsigned m_sign01:1;
    unsigned m_sign10:1;
    unsigned m_sign11:1;
    unsigned m_data00:2;
    unsigned m_data01:2;
    unsigned m_data10:2;
    unsigned m_data11:2;
    _Tp m_tx;
    _Tp m_ty;
};

template <class _Tp>
inline void 
OrthoWith45TransformMatrix<_Tp>::dump(std::ostream& o) const {
    o << "O45TM {" << std::endl;
        
    o << "  MATRIX {" << std::endl;
    o << "    " << val00() << "\t" << val01() << "\t" << val02() 
        << std::endl;
    o << "    " << val10() << "\t" << val11() << "\t" << val12() 
        << std::endl;
    o << "    " << val20() << "\t" << val21() << "\t" << val22() 
        << std::endl;
    o << "  }" << std::endl;

    o << "  SIGN_BITS {" << std::endl;
    o << "    ";
    o << sign11() << sign10() << sign01() << sign00() << std::endl;
    o << "  }" << std::endl;

    o << "  DATA_BITS {" << std::endl;
    o << "    ";
    PrintBits(o, data11(), 2);
    o << " ";
    PrintBits(o, data10(), 2);
    o << " ";
    PrintBits(o, data01(), 2);
    o << " ";
    PrintBits(o, data00(), 2);
    o << std::endl;
    o << "  }" << std::endl;

    o << "  SIZE {" << std::endl;
    o << "    " << sizeof(*this) << std::endl;
    o << "  }" << std::endl;
    o << "}" << std::endl;

    o << "MATH {";
    o << "{" << val00() << ", " << val01() << ", " << val02() << "}, ";
    o << "{" << val10() << ", " << val11() << ", " << val12() << "}, ";
    o << "{" << val20() << ", " << val21() << ", " << val22() << "}";
    o << "}" << std::endl;
}

template <class _Tp>
inline OrthoWith45TransformMatrix<_Tp> OrthoWith45TransformMatrixMultiply(
    const OrthoWith45TransformMatrix<_Tp>& m1,
    const OrthoWith45TransformMatrix<_Tp>& m2) {
    OrthoWith45TransformMatrix<_Tp> m(
        m1.val00()*m2.val00()+m1.val01()*m2.val10(),
        m1.val00()*m2.val01()+m1.val01()*m2.val11(),
        0, 
        m1.val10()*m2.val00()+m1.val11()*m2.val10(),
        m1.val10()*m2.val01()+m1.val11()*m2.val11(),
        0, 
        m1.val20()*m2.val00()+m1.val21()*m2.val10()+
            m2.val20(),
        m1.val20()*m2.val01()+m1.val21()*m2.val11()+
            m2.val21(),
        1);
    return m;
}

template <class _Tp>
inline OrthoWith45TransformMatrix<_Tp> operator*(
    const OrthoWith45TransformMatrix<_Tp>& m1,
    const OrthoWith45TransformMatrix<_Tp>& m2) {
    return OrthoWith45TransformMatrixMultiply(m1, m2);
}
//////////////////////////////////////////////////////////////////////
// Template Class OrthoTransformMatrix:
// 
// This class implements ability to keep only orthogonal rotation for 
// an object.
//////////////////////////////////////////////////////////////////////
template <class _Tp>
class OrthoTransformMatrix : 
    public TransformMatrixInterface<_Tp> {
public: // constructors/destructors
    OrthoTransformMatrix() 
        : m_sign00(0), m_sign01(0), 
        m_sign10(0), m_sign11(0) { 
            identity(); 
    }
    OrthoTransformMatrix(
        const _Tp v00, const _Tp v01, const _Tp v02,
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) 
        : m_sign00(0), m_sign01(0), m_sign10(0), m_sign11(0) { 
        val(v00, v01, v02, v10, v11, 
            v12, v20, v21, v22); 
    }
    ~OrthoTransformMatrix() {}

public: // functions
    // access functions
    const _Tp val00() const { 
        return sign00() == 0 ? data00() : -(const _Tp)data00(); }
    const _Tp val01() const { 
        return sign01() == 0 ? data01() : -(const _Tp)data01(); }
    const _Tp val02() const { return 0; }
    const _Tp val10() const { 
        return sign10() == 0 ? data10() : -(const _Tp)data10(); }
    const _Tp val11() const { 
        return sign11() == 0 ? data11() : -(const _Tp)data11(); }
    const _Tp val12() const { return 0; }
    const _Tp val20() const { return tx(); }
    const _Tp val21() const { return ty(); }
    const _Tp val22() const { return 1; }

    // set functions
    void val(
        const _Tp v00, const _Tp v01, const _Tp v02, 
        const _Tp v10, const _Tp v11, const _Tp v12, 
        const _Tp v20, const _Tp v21, const _Tp v22) { 
        sign00(0); sign01(0); sign10(0); sign11(0); 
        val00(v00); val01(v01); val02(v02); 
        val10(v10); val11(v11); val12(v12); 
        val20(v20); val21(v21); val22(v22); 
    }

    // initialization functions.
    void identity() { val(1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorX() { val(-1, 0, 0, 0, 1, 0, 0, 0, 1); }
    void mirrorY() { val(1, 0, 0, 0, -1, 0, 0, 0, 1); }
    void translation(const _Tp x, const _Tp y) { 
        val(1, 0, 0, 0, 1, 0, x, y, 1); 
    }
    void rotation(const _Tp a, const _Tp b) { 
         const double c = sqrt(a*a+b*b);
        val(a/c, b/c, 0, -b/c, a/c, 0, 0, 0, 1);
    }

    // interface functions.
#ifdef SWIG
    %apply _Tp& OUTPUT { _Tp& rx, _Tp& ry };
#endif /* SWIG */
    void transform(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10()+val20();
        ry = x*val01()+y*val11()+val21();
    }
    void direction(const _Tp x, const _Tp y, 
        _Tp& rx, _Tp& ry) const {
        rx = x*val00()+y*val10();
        ry = x*val01()+y*val11();
    }

    // utility functions.
    void dump(std::ostream& o) const;

#ifdef SWIG
    %extend {
        OrthoTransformMatrix<_Tp> __mul__(
            const OrthoTransformMatrix<_Tp>& m2) {
        	return OrthoTransformMatrixMultiply(*self, m2);
        }
    }
#endif /* SWIG */

private: // functions
    // data get functions
    const unsigned sign00() const { return m_sign00; }
    const unsigned sign01() const { return m_sign01; }
    const unsigned sign10() const { return m_sign10; }
    const unsigned sign11() const { return m_sign11; }
    const unsigned data00() const { return m_data00; }
    const unsigned data01() const { return m_data01; }
    const unsigned data10() const { return m_data10; }
    const unsigned data11() const { return m_data11; }
    const _Tp tx() const { return m_tx; }
    const _Tp ty() const { return m_ty; }

    // data set functions
    void sign00(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign00 = v; }
    void sign01(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign01 = v; }
    void sign10(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign10 = v; }
    void sign11(const unsigned v) { 
        assert(v == 0 || v == 1); m_sign11 = v; }
    void data00(const unsigned v) { 
        assert(v == 0 || v == 1); m_data00 = v; }
    void data01(const unsigned v) { 
        assert(v == 0 || v == 1); m_data01 = v; }
    void data10(const unsigned v) { 
        assert(v == 0 || v == 1); m_data10 = v; }
    void data11(const unsigned v) { 
        assert(v == 0 || v == 1); m_data11 = v; }
    void tx(const _Tp v) { m_tx = v; }
    void ty(const _Tp v) { m_ty = v; }

    // set access functions
    void val00(const _Tp v) { 
        if (v < 0) { data00((const unsigned)(-v)); sign00(1); } 
        else data00((const unsigned)(v)); }
    void val01(const _Tp v) { 
        if (v < 0) { data01((const unsigned)(-v)); sign01(1); } 
        else data01((const unsigned)(v)); }
    void val02(const _Tp v) { }
    void val10(const _Tp v) { 
        if (v < 0) { data10((const unsigned)(-v)); sign10(1); } 
        else data10((const unsigned)(v)); }
    void val11(const _Tp v) { 
        if (v < 0) { data11((const unsigned)(-v)); sign11(1); } 
        else data11((const unsigned)(v)); }
    void val12(const _Tp v) { }
    void val20(const _Tp v) { tx(v); }
    void val21(const _Tp v) { ty(v); }
    void val22(const _Tp v) { }

private: // data
    unsigned m_sign00:1;
    unsigned m_sign01:1;
    unsigned m_sign10:1;
    unsigned m_sign11:1;
    unsigned m_data00:1;
    unsigned m_data01:1;
    unsigned m_data10:1;
    unsigned m_data11:1;
    _Tp m_tx;
    _Tp m_ty;
};

template <class _Tp>
inline void 
OrthoTransformMatrix<_Tp>::dump(std::ostream& o) const {
    o << "OTM {" << std::endl;
        
    o << "  MATRIX {" << std::endl;
    o << "    " << val00() << "\t" << val01() << "\t" << val02() 
        << std::endl;
    o << "    " << val10() << "\t" << val11() << "\t" << val12() 
        << std::endl;
    o << "    " << val20() << "\t" << val21() << "\t" << val22() 
        << std::endl;
    o << "  }" << std::endl;

    o << "  SIGN_BITS {" << std::endl;
    o << "    ";
    o << sign11() << sign10() << sign01() << sign00() << std::endl;
    o << "  }" << std::endl;

    o << "  DATA_BITS {" << std::endl;
    o << "    ";
    o << data11() << " " << data10() << " " 
        << data01() << " " << data00() << std::endl;
    o << "  }" << std::endl;

    o << "  SIZE {" << std::endl;
    o << "    " << sizeof(*this) << std::endl;
    o << "  }" << std::endl;
    o << "}" << std::endl;

    o << "MATH {";
    o << "{" << val00() << ", " << val01() << ", " << val02() << "}, ";
    o << "{" << val10() << ", " << val11() << ", " << val12() << "}, ";
    o << "{" << val20() << ", " << val21() << ", " << val22() << "}";
    o << "}" << std::endl;
}

template <class _Tp>
inline OrthoTransformMatrix<_Tp> OrthoTransformMatrixMultiply(
    const OrthoTransformMatrix<_Tp>& m1, 
    const OrthoTransformMatrix<_Tp>& m2) {
    OrthoTransformMatrix<_Tp> m(
        m1.val00()*m2.val00()+m1.val01()*m2.val10(),
        m1.val00()*m2.val01()+m1.val01()*m2.val11(),
        0, 
        m1.val10()*m2.val00()+m1.val11()*m2.val10(),
        m1.val10()*m2.val01()+m1.val11()*m2.val11(),
        0, 
        m1.val20()*m2.val00()+m1.val21()*m2.val10()+
            m2.val20(),
        m1.val20()*m2.val01()+m1.val21()*m2.val11()+
            m2.val21(),
        1);
  return m;
}

// operator definitions
template <class _Tp>
inline OrthoTransformMatrix<_Tp> operator*(
    const OrthoTransformMatrix<_Tp>& m1, 
    const OrthoTransformMatrix<_Tp>& m2) {
  return OrthoTransformMatrixMultiply(m1, m2);
}

}

#ifdef SWIG
%define TMN_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## TransformMatrixInterface ## type) utl::TransformMatrixInterface<_Tp>;
%template(prefix ## AnyRotationTransformMatrix ## type) utl::AnyRotationTransformMatrix<_Tp>;
%template(prefix ## OrthoWith45TransformMatrix ## type) utl::OrthoWith45TransformMatrix<_Tp>;
%template(prefix ## OrthoTransformMatrix ## type) utl::OrthoTransformMatrix<_Tp>;
%enddef

TMN_TEMPLATE_CL_WRAP(cl, float, Float);

%define TMN_TEMPLATE_FN_WRAP(prefix, _Tp, type)
%template(prefix ## AnyRotationTransformMatrixMultiply ## type) utl::AnyRotationTransformMatrixMultiply<_Tp>;
%template(prefix ## OrthoWith45TransformMatrixMultiply ## type) utl::OrthoWith45TransformMatrixMultiply<_Tp>;
%template(prefix ## OrthoTransformMatrixMultiply ## type) utl::OrthoTransformMatrixMultiply<_Tp>;
%enddef

TMN_TEMPLATE_FN_WRAP(fn, float, Float);
#endif // SWIG

#endif // __TRM_H__
