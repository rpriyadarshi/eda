#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__
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
class BasicNavigator 
    : public BasicNavigatorBase<_CharT, _Traits, _Alloc, _Nav> {
public: // Using.
    using _Base = typename algo::BasicNavigatorBase<_CharT, _Traits, _Alloc, _Nav>;
    using _Navigator = typename algo::BasicNavigator<_CharT, _Traits, _Alloc, _Nav>;

public: // Using.
    using _Self = _Navigator;

public: // constructors/destructors.
    explicit BasicNavigator(_Nav& c) 
        : _Base(c) {}
    virtual ~BasicNavigator() {}

private: // functions.

private: // data
};

}

#endif // __NAVIGATOR_H__
