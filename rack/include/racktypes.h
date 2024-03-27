#ifndef __RACKTYPES_H__
#define __RACKTYPES_H__
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

namespace rack {

// Adding the function object
// 0 args, apart from Rack.
using FuncAdapter0 = algo::BasicFuncAdapter0<char, std::char_traits<char>,
    std::allocator<char>, EntityBase*, EntityBase, Rack >;
using WFuncAdapter0 = algo::BasicFuncAdapter0<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WEntityBase*, WEntityBase, WRack >;

using VoidFuncAdapter0 = algo::BasicFuncAdapter0<char, std::char_traits<char>,
    std::allocator<char>, void, EntityBase, Rack >;
using WVoidFuncAdapter0 = algo::BasicFuncAdapter0<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, void, WEntityBase, WRack >;

// 1 arg, apart from Rack.
using FuncAdapter1 = algo::BasicFuncAdapter1<char, std::char_traits<char>,
    std::allocator<char>, EntityBase*, EntityBase, EntityBase*, Rack >;
using WFuncAdapter1 = algo::BasicFuncAdapter1<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WEntityBase*, WEntityBase, WEntityBase*, WRack >;

using VoidFuncAdapter1 = algo::BasicFuncAdapter1<char, std::char_traits<char>,
    std::allocator<char>, void, EntityBase, EntityBase*, Rack >;
using WVoidFuncAdapter1 = algo::BasicFuncAdapter1<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, void, WEntityBase, WEntityBase*, WRack >;

// 2 arg, apart from Rack.
using FuncAdapter2 = algo::BasicFuncAdapter2<char, std::char_traits<char>,
    std::allocator<char>, EntityBase*, EntityBase, EntityBase*, int, Rack >;
using WFuncAdapter2 = algo::BasicFuncAdapter2<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WEntityBase*, WEntityBase, WEntityBase*, int, WRack >;

using VoidFuncAdapter2 = algo::BasicFuncAdapter2<char, std::char_traits<char>,
    std::allocator<char>, void, EntityBase, EntityBase*, int, Rack >;
using WVoidFuncAdapter2 = algo::BasicFuncAdapter2<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, void, WEntityBase, WEntityBase*, int, WRack >;

// Adding the navigator object
// Navigator Nodes
using VoidNavigatorNode1 = algo::BasicNavigatorNode<char, std::char_traits<char>,
    std::allocator<char>, FuncAdapter0, VoidFuncAdapter1, VoidFuncAdapter1, VoidFuncAdapter1 >;
using WVoidNavigatorNode2 = algo::BasicNavigatorNode<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WFuncAdapter0, WVoidFuncAdapter1, WVoidFuncAdapter1, WVoidFuncAdapter2 >;
using VoidNavigatorNode2 = algo::BasicNavigatorNode<char, std::char_traits<char>,
    std::allocator<char>, FuncAdapter0, VoidFuncAdapter1, VoidFuncAdapter1, VoidFuncAdapter2 >;
using WVoidNavigatorNode2 = algo::BasicNavigatorNode<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WFuncAdapter0, WVoidFuncAdapter1, WVoidFuncAdapter1, WVoidFuncAdapter2 >;

// Navigators
using VoidNavigator2 = algo::BasicNavigator<char, std::char_traits<char>,
    std::allocator<char>, VoidNavigatorNode2 >;
using WVoidNavigator2 = algo::BasicNavigator<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WVoidNavigatorNode2 >;

}

#endif // __RACKTYPES_H__
