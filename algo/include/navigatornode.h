#ifndef __NAVIGATOR_NODE_H__
#define __NAVIGATOR_NODE_H__
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

template <class _CharT, class _Traits, class _Alloc, class _Tp>
class BasicNavigatorNodeBase {
public: // Using.
    using _NavigatorNodeBase = typename algo::BasicNavigatorNodeBase<_CharT, _Traits, _Alloc, _Tp>;

private: // Using.
    using _Self = _NavigatorNodeBase;

public: // constructors/destructors.
    BasicNavigatorNodeBase(_Tp& n, _Tp& c, _Tp& p, _Tp& pa) 
        : m_next(n), m_curr(c), m_prev(p), 
        m_parent(pa) {}
    virtual ~BasicNavigatorNodeBase() {}

public: // functions.
    const _Tp& next() const { return m_next; }
    const _Tp& curr() const { return m_curr; }
    const _Tp& prev() const { return m_prev; }
    const _Tp& parent() const { return m_parent; }
    _Tp& next() { return m_next; }
    _Tp& curr() { return m_curr; }
    _Tp& prev() { return m_prev; }
    _Tp& parent() { return m_parent; }

private: // data
    _Tp&        m_next;
    _Tp&        m_curr;
    _Tp&        m_prev;
    _Tp&        m_parent;
};

template <class _CharT, class _Traits, class _Alloc, 
    class _Tp, class _Fp, class _Bp, class _Sp>
class BasicNavigatorNode 
    : public BasicNavigatorNodeBase<_CharT, _Traits, _Alloc, _Tp> {
public: // Using.
    using _Base = typename algo::BasicNavigatorNodeBase<_CharT, _Traits, _Alloc, _Tp>;
    using _NavigatorNode = typename algo::BasicNavigatorNode<_CharT, _Traits, _Alloc, _Tp, _Fp, _Bp, _Sp>;

private: // Using.
    using _Self = _NavigatorNode;

public: // constructors/destructors.
    BasicNavigatorNode(_Tp& n, _Tp& c, _Tp& p, _Tp& pa, 
        _Fp& f, _Bp& b, _Sp& s) 
        : _Base(n, c, p, pa), 
        m_forward(f), m_backward(b), m_stopat(s) {}
    virtual ~BasicNavigatorNode() {}

public: // functions.
    const _Fp& forward() const { return m_forward; }
    const _Bp& backward() const { return m_backward; }
    const _Sp& stopat() const { return m_stopat; }
    _Fp& forward() { return m_forward; }
    _Bp& backward() { return m_backward; }
    _Sp& stopat() { return m_stopat; }

private: // data
    _Fp&        m_forward;
    _Bp&        m_backward;
    _Sp&        m_stopat;
};

}

#endif // __NAVIGATOR_NODE_H__
