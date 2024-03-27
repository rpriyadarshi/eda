#ifndef __NAVIGATOR_BASE_H__
#define __NAVIGATOR_BASE_H__
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

template <class _CharT, class _Traits, class _Alloc, class _Nav>
class BasicNavigatorBase {
public: // Using.
    using _NavigatorBase = typename algo::BasicNavigatorBase<_CharT, _Traits, _Alloc, _Nav>;

private: // Using.
    using _Self = _NavigatorBase;

public: // constructors/destructors.
    explicit BasicNavigatorBase(_Nav& n) 
        : m_node(n) {}
    virtual ~BasicNavigatorBase() {}

public: // accessors.
    const _Nav& node() const { return m_node; }

public: // algorithms.
    bool dfs() {
        return false;
    }

protected: // functions.
    _Nav& node() { return m_node; }

private: // data
    _Nav&        m_node;
};

}

#endif // __NAVIGATOR_BASE_H__
