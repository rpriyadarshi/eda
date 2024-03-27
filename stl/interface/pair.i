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

%{
// #include <pair>
%}

%include <std_pair.i>

// %define PAIR_TEMPLATE_CL_WRAP1(prefix, _Tp1, _Tp2, type)
// %template(prefix ## type ## Pair) std::pair<
//     std::unordered_set<
//         utl::basic_multistring<_Tp1, std::char_traits<_Tp1>, 
//             std::allocator<_Tp1>>, 
//         std::hash<utl::basic_multistring<_Tp1, std::char_traits<_Tp1>, 
//             std::allocator<_Tp1>>>, 
//         std::equal_to<utl::basic_multistring<_Tp1, std::char_traits<_Tp1>, 
//             std::allocator<_Tp1>>>, s
//         td::allocator<_Tp1> >::const_iterator, 
//         _Tp2>;
// %enddef

// PAIR_TEMPLATE_CL_WRAP1(cl, char, bool, UnorderedSet);

// %define PAIR_TEMPLATE_CL_WRAP2(prefix, _Tp1, _Tp2, type)
// %template(prefix ## type ## Pair) std::pair<
//     utl::BasicDictionary<_Tp1, std::char_traits<_Tp1>, 
//         std::allocator<_Tp1> >::iterator, 
//     _Tp2>;
// %enddef

// PAIR_TEMPLATE_CL_WRAP2(cl, char, bool, Dictionary);

%template() std::pair<swig::SwigPtr_PysObject, swig::SwigPtr_PyObject>;
