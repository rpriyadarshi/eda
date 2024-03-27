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

// https://github.com/CTSRD-CHERI/cheritrace/blob/master/Python/std_bitset.i
%{
// #include <bitset>
%}

namespace std {
    template <size_t _Nb>
    class bitset {
    public:
        bitset();
        bitset(unsigned long __val);

          // 23.3.5.2 bitset operations:
        %newobject bitand;
        %newobject bitor;
        %newobject bitexp;
        %newobject bitlshift;
        %newobject bitrshift;
        %newobject biteq;
        %newobject bitneq;
        %extend {
            bitset<_Nb>& bitand(const bitset<_Nb>& __rhs) {
                return *self &= __rhs;
            }
            bitset<_Nb>& bitor(const bitset<_Nb>& __rhs) {
                return *self |= __rhs;
            }
            bitset<_Nb>& bitexp(const bitset<_Nb>& __rhs) {
                return *self ^= __rhs;
            }
            bitset<_Nb>& bitlshift(size_t __pos) {
                return *self <<= __pos;
            }
            bitset<_Nb>& bitrshift(size_t __pos) {
                return *self >>= __pos;
            }
              bool biteq(const bitset<_Nb>& __rhs) const {
                  return *self == __rhs;
              }
              bool bitneq(const bitset<_Nb>& __rhs) const {
                  return *self != __rhs;
              }
        }
        
          // Set, reset, and flip.        
        bitset<_Nb>& set();
        bitset<_Nb>& set(size_t __pos);
        bitset<_Nb>& set(size_t __pos, int __val);
        bitset<_Nb>& reset();
        bitset<_Nb>& reset(size_t __pos) ;
        bitset<_Nb>& flip();
        bitset<_Nb>& flip(size_t __pos);

          // element access:
          //for b[i];
          //%rename(index) operator[](size_t __pos);
          //reference operator[](size_t __pos);
          %rename(index) operator[](size_t __pos) const;
          bool operator[](size_t __pos) const;

          unsigned long to_ulong() const;

          template <class _CharT, class _Traits, class _Alloc>
          std::basic_string<_CharT, _Traits, _Alloc> to_string() const;
        %template(to_str) to_string<char, std::char_traits<char>, std::allocator<char> >;
        
          size_t count() const;
          size_t size() const;

          bool test(size_t __pos) const;
          bool any() const;
          bool none() const;
      };
}

%define BIT_TEMPLATE_CL_WRAP(prefix, _Tp)
%template(prefix ## Mask) std::bitset<_Tp>;
%enddef

BIT_TEMPLATE_CL_WRAP(cl, 32);
