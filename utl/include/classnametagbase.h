#ifndef __CLASSNAMETAGBASE_H__
#define __CLASSNAMETAGBASE_H__
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
// Class to handle name tags
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicClassNameTagBase {
public: // Using
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;

public:
    explicit BasicClassNameTagBase() {}
    virtual ~BasicClassNameTagBase() {}

public: // data
    static _Str m_Undefined;
    static _Str m_Multistring;
    static _Str m_String;
    static _Str m_Dictionary;
};

////////////////////////////////////////////////////////////////////////////////
// Static members
#ifndef SWIG
template <typename _CharT, typename _Traits, typename _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTagBase<_CharT, _Traits, _Alloc>::m_Undefined("Undefined");
template <typename _CharT, typename _Traits, typename _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTagBase<_CharT, _Traits, _Alloc>::m_Multistring("Multistring");
template <typename _CharT, typename _Traits, typename _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTagBase<_CharT, _Traits, _Alloc>::m_String("String");
template <typename _CharT, typename _Traits, typename _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTagBase<_CharT, _Traits, _Alloc>::m_Dictionary("Dictionary");
#endif /* SWIG */
}

#ifdef SWIG
%define UTL_CLASSNAMETAG_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() utl::BasicClassNameTagBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

UTL_CLASSNAMETAG_TEMPLATE_CL_WRAP(cl, char,);
#endif /* SWIG */

#endif /* __CLASSNAMETAGBASE_H__ */
