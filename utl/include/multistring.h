#ifndef __MULTISTRING_H__
#define __MULTISTRING_H__
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
// Multistring to hold hierarchical strings (tries)
template <typename _CharT, typename _Traits, typename _Alloc, 
    typename _StrAlloc = std::allocator<const std::basic_string<_CharT, _Traits, _Alloc>* > >
class basic_multistring {
public:
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Self = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    // CAUTION: Since the vector stores pointers to strings, make sure
    // that the strings do not change underneath. This is a problem
    // if you are storing strings by value somewhere, for example in
    // a vector. The string object will change as more elements are
    // inserted. Hence any pointer stored will become invalid.
    using _Strings = std::vector<const _Str*, _StrAlloc >;
    using value_type = _Str;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public: // constructors/destructors
    basic_multistring()
        : m_strings() { reserve(1); }

    basic_multistring(const _Str& s)
        : m_strings() {
        reserve(1);
        push_back(s);
    }

    basic_multistring(const _Self& s) {
        clear();
        reserve(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            push_back(s[i]);
        }
    }

    virtual ~basic_multistring() {}

    const_reference at(const size_t i) { return *strings()[i]; }
    const_reference at(const size_t i) const{
        const _Str* s = strings()[i];
        return *s;
    }

    _Self& add(const _Str& s) {
        push_back(s);
        return *this;
    }

    _Self& add(const _Self& s) {
        push_back(s);
        return *this;
    }

    // operators.
    const_reference operator[](const size_t i) { return at(i); }
    const_reference operator[](const size_t i) const { return at(i); }
    _Self& operator+=(const _Str& s) { return add(s); }
    _Self& operator+=(const _Self& s) { return add(s); }
    _Self& operator=(const _Self& s) { return assign(s); }

    _Self& assign(const _Self& c) { return _assign(c); }

    // utility functions.
    bool empty() const {
        size_t s = size();
        if (s) {
            return true;
        } else {
            for (size_t i = 0; i < s; i++) {
                if (!at(i).empty()) {
                    return false;
                }
            }
            return true;
        }
    }
    const size_t size() const {
        return strings().size();
    }

    void reserve(const size_t s) {
        strings().reserve(s);
    }

    void push_back(const _Str& s) {
        strings().push_back(&s);
    }
    
    void push_back(const _Self& s) {
        const size_t ms = size();
        const size_t ts = s.size();
        const size_t rs = ms+ts;
        reserve(rs);
        for (size_t i = 0; i < ts; i++) {
            push_back(s[i]);
        }
    }

    _Str str() {
        _Str res;
        for (size_t i = 0; i < size(); i++) {
            res += at(i);
        }
        return res;
    }

    _Str str() const {
        _Str res;
        for (size_t i = 0; i < size(); i++) {
            res += at(i);
        }
        return res;
    }

    void clear() { strings().clear(); }

    const int compare(const _Self& y) const {
        int c = static_cast<int>(size())-static_cast<int>(y.size());
        if (!c) { // equal sizes, can compare individual strings.
            for (size_t i = 0; i < size(); i++) {
                c = at(i).compare(y.at(i));
                if (c) {
                    return c;
                }
            }
        }

        return c;
    }

    const int compare(const _Str& y) const {
        int c = size()-1;
        if (!c) { // equal sizes, can compare individual strings.
            c = at(0).compare(y);
            if (c) {
                return c;
            }
        }

        return c;
    }
    
private: // functions.
    const _Strings& strings() const { return m_strings; }
    _Strings& strings() { return m_strings; }

    explicit basic_multistring(const _CharT* c)  : m_strings() {}

    _Self& _assign(const _Self& s) {
        if (&s == this) {
            return *this;
        }

        clear();
        reserve(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            push_back(s[i]);
        }
        return *this;
    }

#ifdef SWIG
public:
    %newobject __add__;
    %newobject __radd__;
    %extend {
        utl::basic_multistring<_CharT, _Traits, _Alloc >* __add__(const utl::basic_multistring<_CharT, _Traits, _Alloc >& v) {
        utl::basic_multistring<_CharT, _Traits, _Alloc >* res = new utl::basic_multistring<_CharT, _Traits, _Alloc >(*self);
            *res += v;
            return res;
        }

        utl::basic_multistring<_CharT, _Traits, _Alloc >* __radd__(const utl::basic_multistring<_CharT, _Traits, _Alloc >& v) {
        utl::basic_multistring<_CharT, _Traits, _Alloc >* res = new utl::basic_multistring<_CharT, _Traits, _Alloc >(v);
            *res += *self;
            return res;
        }

        utl::basic_multistring<_CharT, _Traits, _Alloc > __str__() {
            return *self;
        }

        std::basic_ostream<_CharT, std::char_traits<_CharT> >&
        __rlshift__(std::basic_ostream<_CharT, std::char_traits<_CharT> >& out) {
            out << *self;
            return out;
        }

        const_reference __setitem__(const size_t i) {
            return self->at(i);
        }

        const_reference __getitem__(const size_t i) const{
            return self->at(i);
        }

        bool __lt__(const utl::basic_multistring<_CharT, _Traits, _Alloc>& v) {
            return *self < v;
        }

        bool __gt__(const utl::basic_multistring<_CharT, _Traits, _Alloc>& v) {
            return *self > v;
        }

        bool __eq__(const utl::basic_multistring<_CharT, _Traits, _Alloc >& v) {
            return *self == v;
        }

        bool __eq__(const std::basic_string<_CharT, _Traits, _Alloc >& v) {
            return *self == v;
        }

        bool __ne__(const utl::basic_multistring<_CharT, _Traits, _Alloc >& v) {
            return *self != v;
        }

        bool __ne__(const std::basic_string<_CharT, _Traits, _Alloc >& v) {
            return *self != v;
        }
    }
#endif

private: // data
    _Strings m_strings;
};

////////////////////////////////////////////////////////////////////////////////
// Template specialization
using multistring = basic_multistring<char, std::char_traits<char>, std::allocator<char> >;
using wmultistring = basic_multistring<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;

////////////////////////////////////////////////////////////////////////////////
// utl::multistring dump operator
template <typename _CharT, typename _Traits, typename _Alloc>
inline std::ostream& operator<<(std::ostream& o, 
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    for (size_t i = 0; i < ms.size(); i++) {
        o << ms[i];
    }
    return o;
}

////////////////////////////////////////////////////////////////////////////////
// utl::multistring comparison operator
template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator<(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return x.compare(y) < 0;
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator>(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return x.compare(y) > 0;
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator==(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return x.compare(y) == 0;
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator==(
    const std::basic_string<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return (y.size() == 1 && x == y[0]);
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator==(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const std::basic_string<_CharT, _Traits, _Alloc>& y) 
{
    return (x.size() == 1 && x[0] == y);
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator!=(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return (!(y == x));
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator!=(
    const std::basic_string<_CharT, _Traits, _Alloc>& x,
    const basic_multistring<_CharT, _Traits, _Alloc>& y) 
{
    return (!(y == x));
}

template <typename _CharT, typename _Traits, typename _Alloc> 
inline bool operator!=(
    const basic_multistring<_CharT, _Traits, _Alloc>& x,
    const std::basic_string<_CharT, _Traits, _Alloc>& y) 
{
    return (!(y == x));
}

////////////////////////////////////////////////////////////////////////////////
// utl::multistring hash key generator
template <typename _CharT, typename _Traits, typename _Alloc>
size_t stl_multistring_hash(
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    unsigned long h = 0;
    using const_ptr = typename basic_multistring<_CharT, _Traits, _Alloc>::const_pointer;
    size_t len = ms.size();
    std::hash<std::basic_string<_CharT, _Traits, _Alloc> > hf;
    for (size_t i = 0; i < len; ++i)
        h = 5*h + hf(ms[i]);
    return size_t(h);
}

}

////////////////////////////////////////////////////////////////////////////////
// utl::multistring hash function
template <typename _CharT, typename _Traits, typename _Alloc>
struct std::hash<utl::basic_multistring<_CharT, _Traits, _Alloc> >
{
  size_t operator()(
      const utl::basic_multistring<_CharT, _Traits, _Alloc>& s) const
    { return utl::stl_multistring_hash(s); }
};

#ifdef SWIG
namespace utl {
	%naturalvar multistring;
}

%define MSTR_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## MultiString) utl::basic_multistring<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

MSTR_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __MULTISTRING_H__
