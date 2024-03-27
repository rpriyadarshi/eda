#ifndef __MESSAGE_H__
#define __MESSAGE_H__
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
// Message handler base
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicMessageBase {
private:
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Self = utl::BasicMessageBase<_CharT, _Traits, _Alloc>;
    using _StrCache = std::vector<_Str*>;
    using _Custom = std::vector<_Mstr>;
    using _Map = std::unordered_map<_Str, _Mstr, std::hash<_Str>, std::equal_to<_Str>, _Alloc>;
    using _StrAlloc = typename _Alloc::template rebind<_Str>::other;

public: // Constructors.
    explicit BasicMessageBase(const _Map& m) 
        : m_map(m), m_key(), m_strCache(), m_custom() {}

    virtual ~BasicMessageBase() { purge(); }

public: // Accessors.
    const _Map& map() const {
        return m_map;
    }

    const _Str& key() const {
        return m_key;
    }

    const _StrCache& strCache() const {
        return m_strCache;
    }

    const _Custom& custom() const {
        return m_custom;
    }

public: // Utility functions.
    void purge() {
        for(size_t i = 0; i < strCache().size(); ++i) {
            delete strCache()[i];
        }
        strCache().clear();
        custom().clear();
    }
    // prints a filled message to stream.
    std::ostream& print(std::ostream& o) const {
        const _Str& k = key();
        const _Map& m = map();
        // Was fixed by the suggestion at: 
        // http://gcc.gnu.org/ml/gcc-help/2004-10/msg00179.html
        typename _Map::const_iterator it = m.find(k);
        if (it != m.end()) {
            const _Mstr& ref = (*it).second;
            size_t j = 0;
            const _Custom& c = custom();
            for (size_t i = 0; i < ref.size(); i++) {
                if (ref[i] == "") {
                    o << c[j++];
                }
                else {
                    o << ref[i];
                }
            }
        }

        return o;
    }

    _Str str() {
        std::stringstream ss;
        print(ss);
        return ss.str();
    }

    // initialize mesage.
    void init(const _CharT* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        init(fmt, args);
        va_end(args);
    }

    void init(const _CharT* fmt, va_list args) {
        purge();
        key() = fmt;

        while (fmt && *fmt) {
            init(*fmt++, args);
        }
    }

    // TBD: Operators.
    _Self& operator=(const _Self& s) {
        if (&s == this) {
            return *this;
        }
        // map is not copied.
        m_key = s.m_key;
        // Copy string cache
        for(size_t i = 0; i < s.strCache().size(); ++i)
        {
            createStr(s.strCache()[i]->c_str());
        }
        m_custom = s.m_custom;
        return *this;
    }

    _Str& createStr(const _CharT* name) {
        return _createStr(name, strAlloc());
    }
    _Str& createStr(const _Str& name) {
        return _createStr(name.c_str(), strAlloc());
    }

protected: // Utility functons
    void init(const _CharT fmt, va_list args) {
        if (fmt == '%') {
            return;
        }
        const _CharT* s = 0;
        const _Str* n = 0;
        const _Mstr* m = 0;

        switch (fmt) {
        case 'c': // This prints a number as an ASCII character.  
        // Thus, `printf "%c",65' outputs the letter `A'.  The 
        // output for a string value is the first character of 
        // the string.
            break;

        case 'd': // This prints a decimal integer.
        case 'i': // This also prints a decimal integer.
            break;

        case 'e': // This prints a number in scientific 
        // (exponential) notation.  For example, printf "%4.3e", 
        // 1950 prints `1.950e+03', with a total of four 
        // significant figures of which three follow the decimal 
        // point.  The `4.3' are "modifiers", discussed below.
            break;

        case 'f': // This prints a number in floating point 
        // notation.
            break;

        case 'g': // This prints a number in either scientific 
        // notation or floating point notation, whichever uses 
        // fewer characters.
            break;

        case 'o': // This prints an unsigned octal integer.
            break;

        case 's': // This prints a string.
            s = va_arg(args, _CharT*);
            createStr(s);
            custom().push_back(*strCache().back());
            break;

        case 'n': // This prints a string.
            n = va_arg(args, _Str*);
            createStr(*n);
            custom().push_back(*strCache().back());
            break;

        case 'm': // This prints a string.
            m = va_arg(args, _Mstr*);
            custom().push_back(*m);
            break;

        case 'x': // This prints an unsigned hexadecimal integer.
            break;

        case 'X': // This prints an unsigned hexadecimal integer. 
        // However, for the values 10 through 15, it uses the 
        // letters `A' through `F' instead of `a' through `f'.
            break;

        default:
            break;
        }
    }

protected: // allocators.
    _StrAlloc& strAlloc() { return m_allocs.m_aStr; }
    // raw creation.
    _Str& _createStr(const _CharT* name,
        _StrAlloc& a) {
        //_Str* p_s = a.allocate(1);
        //construct(p_s, _Str(name));
        _Str* p_s = new (a.allocate(1)) _Str(name);
        strCache().push_back(p_s);
        return *p_s;
    }

protected: // Accessors.
    _Str& key() {
        return m_key;
    }

    _StrCache& strCache() {
        return m_strCache;
    }

    _Custom& custom() {
        return m_custom;
    }

private: // data
    const _Map&         m_map;
    _Str                m_key;
    _StrCache           m_strCache;
    _Custom             m_custom;
    // Allocators. They consume space.
    // Size of each allocator is 1 byte.
    typedef struct {
        _StrAlloc      m_aStr;
    } _Allocators;
    _Allocators         m_allocs;
};

////////////////////////////////////////////////////////////////////////////////
// Message handler
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicMessage : 
    public BasicMessageBase<_CharT, _Traits, _Alloc> {
public:
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Base = utl::BasicMessageBase<_CharT, _Traits, _Alloc>;
    using _Self = utl::BasicMessage<_CharT, _Traits, _Alloc>;
    using _Map = std::unordered_map<_Str, _Mstr, std::hash<_Str>, std::equal_to<_Str>, _Alloc>;

public:
    explicit BasicMessage(const _Map& m)
        : _Base(m) {}

    virtual ~BasicMessage() {};

    // Operators.
    _Self& operator=(const _Self& s) {
        if (&s == this) {
            return *this;
        }
        _Base::operator=(s);
        return *this;
    }

#ifdef SWIG
public: // Base class Utility functions.
    _Str str();

    // initialize mesage.
    %varargs(10, const _CharT* fmt = nullptr) init;
    void init(const _CharT* fmt, ...);
	void init(const _CharT* fmt, std::basic_string<_CharT, _Traits, _Alloc>& s1, std::basic_string<_CharT, _Traits, _Alloc>& s2) {
		init(fmt, &s1, &s2);
	}
	void init(const _CharT* fmt, utl::basic_multistring<_CharT, _Traits, _Alloc>& s1, utl::basic_multistring<_CharT, _Traits, _Alloc>& s2) {
		init(fmt, &s1, &s2);
	}
	void init(const _CharT* fmt, std::basic_string<_CharT, _Traits, _Alloc>& s1, utl::basic_multistring<_CharT, _Traits, _Alloc>& s2) {
		init(fmt, &s1, &s2);
	}
	void init(const _CharT* fmt, utl::basic_multistring<_CharT, _Traits, _Alloc>& s1, std::basic_string<_CharT, _Traits, _Alloc>& s2) {
		init(fmt, &s1, &s2);
	}
#endif /* SWIG */

private:
};

////////////////////////////////////////////////////////////////////////////////
// Template specialization
using Message = BasicMessage<char, std::char_traits<char>, 
    std::allocator<char> >;
using WMessage = BasicMessage<wchar_t, std::char_traits<wchar_t>, 
    std::allocator<wchar_t> >;
using HashMap = std::unordered_map<
    std::basic_string<char, std::char_traits<char>, 
        std::allocator<char> >, 
    basic_multistring<char, std::char_traits<char>, 
        std::allocator<char> >, 
    std::hash<std::basic_string<char, std::char_traits<char>, 
        std::allocator<char> > >, 
    std::equal_to<std::basic_string<char, std::char_traits<char>, 
        std::allocator<char> > >, 
    std::allocator<char> >;
using WHashMap = std::unordered_map<
    std::basic_string<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> >, 
    basic_multistring<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> >, 
    std::hash<std::basic_string<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> > >, 
    std::equal_to<std::basic_string<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> > >, 
    std::allocator<wchar_t> >;
using MHashMap = std::unordered_map<
    basic_multistring<char, std::char_traits<char>, 
        std::allocator<char> >, 
    basic_multistring<char, std::char_traits<char>, 
        std::allocator<char> >, 
    std::hash<basic_multistring<char, std::char_traits<char>, 
        std::allocator<char> > >, 
    std::equal_to<basic_multistring<char, std::char_traits<char>, 
        std::allocator<char> > >, 
    std::allocator<char> >;
using WMHashMap = std::unordered_map<
    basic_multistring<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> >, 
    basic_multistring<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> >, 
    std::hash<basic_multistring<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> > >, 
    std::equal_to<basic_multistring<wchar_t, std::char_traits<wchar_t>, 
        std::allocator<wchar_t> > >, 
    std::allocator<wchar_t> >;

////////////////////////////////////////////////////////////////////////////////
// utl::message dump operator
template <typename _CharT, typename _Traits, typename _Alloc>
inline std::ostream& operator<<(std::ostream& o, 
    const BasicMessageBase<_CharT, _Traits, _Alloc>& bms) 
{
    return bms.print(o);
}

}

#ifdef SWIG
namespace utl {
    %naturalvar BasicMessage;
}

%define MESSAGE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() utl::BasicMessageBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(prefix ## type ## Message) utl::BasicMessage<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

MESSAGE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif /* __MESSAGE_H__ */
