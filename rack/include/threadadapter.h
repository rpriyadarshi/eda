#ifndef __THREAD_ADAPTER_H__
#define __THREAD_ADAPTER_H__
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

/* 
 * The member function pointers are a very tricky subject. This is 
 * explained in some detail at the link shown below.
 * http://www.codeproject.com/cpp/FastDelegate.asp
 * Locally, the converted article is stored below:
 * file://../../../doc/technical/
 * In anycase, the syntax is strange, almost looking like a regular
 * expression.
 */
template <class _CharT, class _Traits, class _Alloc, class _T>
class BasicThreadAdapter {
public: // functions
    // constructors/destructors.
    BasicThreadAdapter(void (_T::*_func)(), _T& _t)
        : __m_func(_func), __m_type(_t) {}
    virtual ~BasicThreadAdapter() {}

    // Operators
    void operator()() { (type().*__m_func)(); }

    // Accessors.
    const _T& type() const { return __m_type; }

private: // functions.
    // Accessors.
    _T& type() { return __m_type; }

private: // data
    void (_T::*__m_func)();
    _T& __m_type;
};

template <class _CharT, class _Traits, class _Alloc, 
    class _T, class _R>
class BasicThreadAdapterRack {
public: // functions
    // constructors/destructors.
    BasicThreadAdapterRack(void (_T::*_func)(_R&), _T& _t, _R& _r)
        : __m_func(_func), __m_type(_t), __m_rack(_r) {}
    virtual ~BasicThreadAdapterRack() {}

    // Operators
    void operator()() { (type().*__m_func)(rack()); }

    // Accessors.
    const _T& type() const { return __m_type; }
    const _R& rack() const { return __m_rack; }

private: // functions.
    // Accessors.
    _T& type() { return __m_type; }
    _R& rack() { return __m_rack; }

private: // data
    void (_T::*__m_func)(_R&);
    _T& __m_type;
    _R& __m_rack;
};

// Instance based definitions.
using InstanceThreadAdapter = BasicThreadAdapter<char, std::char_traits<char>,
    std::allocator<char>, Instance >;
using WInstanceThreadAdapter = BasicThreadAdapter<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WInstance >;
using InstanceRackThreadAdapter = BasicThreadAdapterRack<char, std::char_traits<char>,
    std::allocator<char>, Instance, Rack >;
using WInstanceRackThreadAdapter = BasicThreadAdapterRack<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WInstance, WRack >;

// ConnectorBase based definitions.
using ConnectorThreadAdapter = BasicThreadAdapter<char, std::char_traits<char>,
    std::allocator<char>, Connector >;
using WConnectorThreadAdapter = BasicThreadAdapter<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WConnector >;
using ConnectorRackThreadAdapter = BasicThreadAdapterRack<char, std::char_traits<char>,
    std::allocator<char>, Connector, Rack >;
using WConnectorRackThreadAdapter = BasicThreadAdapterRack<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t>, WConnector, WRack >;

}

#ifdef SWIG
#endif // SWIG

#endif // __THREAD_ADAPTER_H__
