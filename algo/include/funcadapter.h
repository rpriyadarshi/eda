#ifndef __FUNC_ADAPTER_H__
#define __FUNC_ADAPTER_H__
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

namespace algo {

/* 
 * The ber function pointers are a very tricky subject. This is 
 * explained in some detail at the link shown below.
 * http://www.codeproject.com/cpp/FastDelegate.asp
 * Locally, the converted article is stored below:
 * file://../../../doc/technical/
 * In anycase, the syntax is strange, almost looking like a regular
 * expression.
 */

/* 
 * The sequence of the classes handle arguments to the functions
 */
template <class _CharT, class _Traits, class _Alloc>
class BasicFuncAdapterBase {
public: // Using
    using _FuncAdapterBase = typename algo::BasicFuncAdapterBase<_CharT, _Traits, _Alloc>;

private: // Using.
    using _Self = _FuncAdapterBase;

public: // functions
    // constructors/destructors.
    BasicFuncAdapterBase() {}
    virtual ~BasicFuncAdapterBase() {}

protected: // functions.

private: // data
};

template <class _CharT, class _Traits, class _Alloc, class _Tp, class _Rp>
class BasicFuncAdapterBase0 
    : public BasicFuncAdapterBase<_CharT, _Traits, _Alloc> {
public: // Using
    using _Base = typename algo::BasicFuncAdapterBase<_CharT, _Traits, _Alloc>;
    using _FuncAdapterBase0 = typename algo::BasicFuncAdapterBase0<_CharT, _Traits, _Alloc, _Tp, _Rp>;

private: // Using.
    using _Self = _FuncAdapterBase0;

public: // functions
    // constructors/destructors.
    BasicFuncAdapterBase0()
        : _Base(), m_type(0), m_rack(0) {}
    explicit BasicFuncAdapterBase0(_Rp& r)
        : _Base(), m_type(0), m_rack(&r) {}
    BasicFuncAdapterBase0(_Tp& _t, _Rp& r)
        : _Base(), m_type(&_t), m_rack(&r) {}
    virtual ~BasicFuncAdapterBase0() {}

    // Accessors.
    const _Tp* type() const { return m_type; }
    const _Rp* rack() const { return m_rack; }

protected: // functions.
    // Accessors.
    _Tp* type() { return m_type; }
    _Rp* rack() { return m_rack; }

private: // data
    _Tp* m_type;
    _Rp* m_rack;
};

template <class _CharT, class _Traits, class _Alloc, 
    class _Tp, class _A0, class _Rp>
class BasicFuncAdapterBase1
    : public BasicFuncAdapterBase0<_CharT, _Traits, _Alloc, _Tp, _Rp> {
public: // Using
    using _Base0 = typename algo::BasicFuncAdapterBase0<_CharT, _Traits, _Alloc, _Tp, _Rp>;
    using _FuncAdapterBase1 = typename algo::BasicFuncAdapterBase1<_CharT, _Traits, _Alloc, _Tp, _A0, _Rp>;

private: // Using.
    using _Self = _FuncAdapterBase1;

public: // functions
    // constructors/destructors.
    BasicFuncAdapterBase1()
        : _Base0(), m_arg0(0) {}
    explicit BasicFuncAdapterBase1(_Rp& r)
        : _Base0(r), m_arg0(0) {}
    BasicFuncAdapterBase1(_Tp& t, _A0 a0, _Rp& r)
        : _Base0(t, r), m_arg0(&a0) {}
    virtual ~BasicFuncAdapterBase1() {}

    // Accessors.
    const _A0* arg0() const { return m_arg0; }

protected: // functions.
    // Accessors.
    _A0* arg0() { return m_arg0; }

private: // data
    _A0* m_arg0;
};

template <class _CharT, class _Traits, class _Alloc, 
    class _Tp, class _A0, class _A1, class _Rp>
class BasicFuncAdapterBase2
    : public BasicFuncAdapterBase1<_CharT, _Traits, _Alloc, _Tp, _A0, _Rp> {
public: // Using
    using _Base1 = typename algo::BasicFuncAdapterBase1<_CharT, _Traits, _Alloc, _Tp, _A0, _Rp>;
    using _FuncAdapterBase2 = typename algo::BasicFuncAdapterBase2<_CharT, _Traits, _Alloc, _Tp, _A0, _A1, _Rp>;

private: // Using.
    using _Self = _FuncAdapterBase2;

public: // functions
    // constructors/destructors.
    BasicFuncAdapterBase2()
        : _Base1(), m_arg1(0) {}
    explicit BasicFuncAdapterBase2(_Rp& r)
        : _Base1(r), m_arg1(0) {}
    BasicFuncAdapterBase2(_Tp& t, _A0 a0, _A1 a1, _Rp& r)
        : _Base1(t, a0, r), m_arg1(&a1) {}
    virtual ~BasicFuncAdapterBase2() {}

    // Accessors.
    const _A1* arg1() const { return m_arg1; }

protected: // functions.
    // Accessors.
    _A1* arg1() { return m_arg1; }

private: // data
    _A1* m_arg1;
};

/*
 * The classes below are function wrappers.
 */
template <class _CharT, class _Traits, class _Alloc, 
    class _R0, class _Tp, class _Rp>
class BasicFuncAdapter0
    : public BasicFuncAdapterBase0<_CharT, _Traits, _Alloc, _Tp, _Rp> {
private: // Using.
    using _Base0 = typename algo::BasicFuncAdapterBase0<_CharT, _Traits, _Alloc, _Tp, _Rp>;

public: // functions
    // constructors/destructors.
    BasicFuncAdapter0()
        : _Base0(), m_func(0) {}
    BasicFuncAdapter0(_R0 (_Tp::*_func)(_Rp&))
        : _Base0(), m_func(_func) {}
    BasicFuncAdapter0(_R0 (_Tp::*_func)(_Rp&), _Rp& r)
        : _Base0(r), m_func(_func) {}
    BasicFuncAdapter0(_R0 (_Tp::*_func)(_Rp&), _Tp& t, _Rp& r)
        : _Base0(t, r), m_func(_func) {}
    virtual ~BasicFuncAdapter0() {}

public: // functions
    _R0 apply() { return _apply(); }
    _R0 apply(_Tp& t) { return _apply(t); }
    _R0 apply(_Tp& t, _Rp& r) { return _apply(t, r); }

public: // Operators
    _R0 operator()() { return _apply(); }
    _R0 operator()(_Tp& t) { return _apply(t); }
    _R0 operator()(_Tp& t, _Rp& r) { return _apply(t, r); }

private: // functions
    _R0 _apply() { return ((*_Base0::type()).*m_func)(*_Base0::rack()); }
    _R0 _apply(_Tp& t) { return (t.*m_func)(*_Base0::rack()); }
    _R0 _apply(_Tp& t, _Rp& r) { return (t.*m_func)(r); }

private: // data
    _R0 (_Tp::*m_func)(_Rp&);
};

template <class _CharT, class _Traits, class _Alloc, 
    class _R0, class _Tp, class _A0, class _Rp>
class BasicFuncAdapter1
    : public BasicFuncAdapterBase1<_CharT, _Traits, _Alloc, _Tp, _A0, _Rp> {
private: // Using.
    using _Base1 = typename algo::BasicFuncAdapterBase1<_CharT, _Traits, _Alloc, _Tp, _A0, _Rp>;

public: // functions
    // constructors/destructors.
    BasicFuncAdapter1()
        : _Base1(), m_func(0) {}
    BasicFuncAdapter1(_R0 (_Tp::*_func)(_A0, _Rp&))
        : _Base1(), m_func(_func) {}
    BasicFuncAdapter1(_R0 (_Tp::*_func)(_A0, _Rp&), _Rp& r)
        : _Base1(r), m_func(_func) {}
    BasicFuncAdapter1(_R0 (_Tp::*_func)(_A0, _Rp&), 
        _Tp& t, _A0 a0, _Rp& r)
        : _Base1(t, a0, r), m_func(_func) {}
    virtual ~BasicFuncAdapter1() {}

public: // functions
    _R0 apply() { return _apply(); }
    _R0 apply(_Tp& t, _A0 a0) { return _apply(t, a0); }
    _R0 apply(_Tp& t, _A0 a0, _Rp& r) { return _apply(t, a0, r); }

public: // Operators
    _R0 operator()() { return _apply(); }
    _R0 operator()(_Tp& t, _A0 a0) { return _apply(t, a0); }
    _R0 operator()(_Tp& t, _A0 a0, _Rp& r) { return _apply(t, a0, r); }

private: // functions
    _R0 _apply() { return ((*_Base1::type()).*m_func)(*_Base1::arg0(), *_Base1::rack()); }
    _R0 _apply(_Tp& t, _A0 a0) { return (t.*m_func)(a0, *_Base1::rack()); }
    _R0 _apply(_Tp& t, _A0 a0, _Rp& r) { return (t.*m_func)(a0, r); }

private: // data
    _R0 (_Tp::*m_func)(_A0, _Rp&);
};

template <class _CharT, class _Traits, class _Alloc, 
    class _R0, class _Tp, class _A0, class _A1, class _Rp>
class BasicFuncAdapter2
    : public BasicFuncAdapterBase2<_CharT, _Traits, _Alloc, _Tp, _A0, _A1, _Rp> {
private: // Using.
    using _Base2 = typename algo::BasicFuncAdapterBase2<_CharT, _Traits, _Alloc, _Tp, _A0, _A1, _Rp>;

public: // functions
    // constructors/destructors.
    BasicFuncAdapter2()
        : _Base2(), m_func(0) {}
    BasicFuncAdapter2(_R0 (_Tp::*_func)(_A0, _A1, _Rp&))
        : _Base2(), m_func(_func) {}
    BasicFuncAdapter2(_R0 (_Tp::*_func)(_A0, _A1, _Rp&), _Rp& r)
        : _Base2(r), m_func(_func) {}
    BasicFuncAdapter2(_R0 (_Tp::*_func)(_A0, _A1, _Rp&), 
        _Tp& t, _A0 a0, _A1 a1, _Rp& r)
        : _Base2(t, a0, a1, r), m_func(_func) {}
    virtual ~BasicFuncAdapter2() {}

public: // functions
    _R0 apply() { return _apply(); }
    _R0 apply(_Tp& t, _A0 a0, _A1 a1) { return _apply(t, a0, a1); }
    _R0 apply(_Tp& t, _A0 a0, _A1 a1, _Rp& r) { return _apply(t, a0, a1, r); }

public: // functions
    _R0 operator()() { return _apply(); }
    _R0 operator()(_Tp& t, _A0 a0, _A1 a1) { return _apply(t, a0, a1); }
    _R0 operator()(_Tp& t, _A0 a0, _A1 a1, _Rp& r) { return _apply(t, a0, a1, r); }

private: // Operators
    _R0 _apply() { return ((*_Base2::type()).*m_func)(*_Base2::arg0(), *_Base2::arg1(), *_Base2::rack()); }
    _R0 _apply(_Tp& t, _A0 a0, _A1 a1) { return (t.*m_func)(a0, a1, *_Base2::rack()); }
    _R0 _apply(_Tp& t, _A0 a0, _A1 a1, _Rp& r) { return (t.*m_func)(a0, a1, r); }

private: // data
    _R0 (_Tp::*m_func)(_A0, _A1, _Rp&);
};

}

#endif // __FUNC_ADAPTER_H__
