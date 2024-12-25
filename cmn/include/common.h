#ifndef __COMMON_H__
#define __COMMON_H__

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

// https://en.cppreference.com/w/cpp/types/integer
#include <bitset>

namespace utl {

// Using.
using Id_t = std::uint64_t;
using Mask = std::bitset<64>;

// Constants.
const std::int64_t cByte = sizeof(char);
const double cOneByRootTwo = 1.0/sqrt(2.0);
const std::int64_t cDefaultTab = 4;

// Extreme masks
const Mask mFalse 		= 0x0000;
const Mask mTrue 		= 0xffff;

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Printed the individual bits of a type with in 
// a given size.
//////////////////////////////////////////////////////////////////////
template <typename _Tp> void 
PrintBits(std::ostream& o, _Tp f, int size)
{
    char* buf = new char[size+1];
    _Tp flag = 1;
    for (int i = 0; i < size; i++) {
        _Tp bit = f&flag;
        if (bit == 0) {
            buf[size-i-1] = '0';
        } else {
            buf[size-i-1] = '1';
        }
        flag = flag << 1;
    }
    buf[size] = '\0';
    o << buf;
    delete [] buf;
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Printed the individual bits of a type.
//////////////////////////////////////////////////////////////////////
template <typename _Tp> void 
PrintBits(std::ostream& o, _Tp f)
{
    int size = sizeof(_Tp)*cByte;
    printBits(o, f, size);
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Created an indent string.
//////////////////////////////////////////////////////////////////////
inline std::string
Indent(const int indent)
{
    std::string str;
    for (int i = 0; i < indent; i++) 
        str += " ";
    return str;
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: returned the max of two values.
//////////////////////////////////////////////////////////////////////
template <typename _Tp>
const _Tp 
Max(const _Tp v1, const _Tp v2)
{
    return v1 > v2 ? v1 : v2;
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: returned the min of two values.
//////////////////////////////////////////////////////////////////////
template <typename _Tp>
const _Tp 
Min(const _Tp v1, const _Tp v2)
{
    return v1 < v2 ? v1 : v2;
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: returned the min of two values.
//////////////////////////////////////////////////////////////////////
template <typename _Tp>
struct ptrless 
{
    bool operator()(const _Tp& x, const _Tp& y) const { return &x < &y; }
};

// Borrowed from:
// /usr/src/kernels/*/include/asm-i386/div64.h
// Linux Ref: http://www.ibm.com/developerworks/library/l-ia.html
// http://lkml.org/lkml/2007/10/20/57
// (long)X = ((long long)divs) / (long)div
// (long)rem = ((long long)divs) % (long)div
//
// Warning, this will do an exception if X overflows.
//
inline long
DivLongLongRem(long long divs, long div, long *rem)
{
#ifdef USE_ASM_DIV
    // Inline assembly version (for platforms supporting it)
    long dum2;
    __asm__("divq %2" : "=a"(dum2), "=d"(*rem)
                      : "rm"(div), "A"(divs));
    return dum2;
#else
    // Portable standard C++ version
    *rem = divs % div;
    return divs / div;
#endif
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: returned int converted to string.
//////////////////////////////////////////////////////////////////////
template <typename _CharT>
const _CharT* 
IntToStr(const std::int64_t n, _CharT* buf)
{
    std::int64_t d = 10;
    char* b = buf;
    char* s = buf;
    std::int64_t v = n;
    std::int64_t r;
    if (v < 0) {
        v *= -1;
        *b++ = '-';
        s++;
    }
    while (v) {
#if (!defined _USEASM)
        r = v%d;
        v = v/d;
#elif (defined __linux__)
        v = DivLongLongRem(v, d, (long*)&r);
#elif (defined __WINDOWS__) // This code avoids division twice.
        _asm {
            xor edx, edx;    // zero edx register
            mov eax, v;   // move v to eax register
            div d;        // divide eax by d
            mov v, eax;   // move quotient to v
            mov r, edx;   // move remainder to r
        }
#endif
        *b++ = '0'+(char)r;
    }
    *b-- = '\0';
    char t;
    while (s < b) {
        t = *b;
        *b-- = *s;
        *s++ = t;
    }
    return buf;
}

}

#if 0 /* Example uses */
const BasicXXX& operator=(const _Self& c) {
    if (this == &c)
        return *this;
    _ScopedLock _l1(&m_mutex < &c.mutex() ? mutex() : c.mutex());
    _ScopedLock _l2(&m_mutex > &c.mutex() ? mutex() : c.mutex());
    return *this;
}
#endif

#ifdef SWIG
%define CMN_TEMPLATE_FN_WRAP(prefix, _Tp, type)
%template(prefix ## Max ## type) utl::Max<_Tp>;
%template(prefix ## Min ## type) utl::Min<_Tp>;
%enddef

CMN_TEMPLATE_FN_WRAP(fn, int, Int);
CMN_TEMPLATE_FN_WRAP(fn, float, Float);
CMN_TEMPLATE_FN_WRAP(fn, double, Double);
#endif /* SWIG */

#endif /* __COMMON_H__ */
