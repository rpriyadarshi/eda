#ifndef __EXCEPT_H__
#define __EXCEPT_H__
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

////////////////////////////////////////////////////////////////////////////////
// Exception handler base
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicExceptionBase {
private: // Using
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Msg = utl::BasicMessage<_CharT, _Traits, _Alloc>;
    using _Self = utl::BasicExceptionBase<_CharT, _Traits, _Alloc>;

public:
    explicit BasicExceptionBase(const _Msg& msg) 
        : m_msg(msg) {}

    virtual ~BasicExceptionBase() {}

    const _Msg& msg() const {
        return m_msg;
    }

    _Str str() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    // prints a filled message to stream.
    std::ostream& print(std::ostream& o) const {
        o << msg();
        return o;
    }

    // Operators.
    _Self& operator=(const _Self& s) {
        if (&s == this) {
            return *this;
        }
        // map is not copied.
        m_msg = s.m_msg;
        return *this;
    }

protected:

private:
    const _Msg& m_msg;
};

////////////////////////////////////////////////////////////////////////////////
// Exception handler
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicException : 
    public BasicExceptionBase<_CharT, _Traits, _Alloc> {
public:
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Msg = utl::BasicMessage<_CharT, _Traits, _Alloc>;
    using _Base = utl::BasicExceptionBase<_CharT, _Traits, _Alloc>;
    using _Self = utl::BasicException<_CharT, _Traits, _Alloc>;

public:
    explicit BasicException(const _Msg& msg) 
        : _Base(msg) {}

    virtual ~BasicException() {}

    // Operators.
    _Self& operator=(const _Self& s) {
        if (&s == this) {
            return *this;
        }
        _Base::operator=(s);
        return *this;
    }

#ifdef SWIG
    _Str str();
#endif /* SWIG */

private:
};

////////////////////////////////////////////////////////////////////////////////
// Template specialization
using Exception = BasicException<char, std::char_traits<char>, std::allocator<char> >;
using WException = BasicException<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;

////////////////////////////////////////////////////////////////////////////////
// utl::exception dump handling
template <typename _CharT, typename _Traits, typename _Alloc>
inline std::ostream& operator<<(std::ostream& o, 
    const BasicExceptionBase<_CharT, _Traits, _Alloc>& e) 
{
    return e.print(o);
}

}

#ifdef SWIG
namespace utl {
    %naturalvar BasicException;
}

%define EXCEPTION_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() utl::BasicExceptionBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(prefix ## type ## Exception) utl::BasicException<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

EXCEPTION_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif /* __EXCEPT_H__ */
