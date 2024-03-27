#ifndef __PINBASE_H__
#define __PINBASE_H__
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

template <class _CharT, class _Traits, class _Alloc>
class BasicPinBase 
    : public BasicConnectorBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_Connector;
    using _Self = typename _Entity::_PinBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: 
    // constructors/destructors.
    BasicPinBase(const utl::Id_t __id, typename _Entity::_Connector& __instOf, typename _Entity::_Entity& __o) 
        : _Base(__id), __m_instOf(&__instOf), __m_interconnect(0), __m_owner(&__o) {}
    BasicPinBase(const utl::Id_t __id, typename _Entity::_Connector& __instOf, typename _Entity::_Entity& __o, typename _Entity::_Interconnect& __interconnect) 
        : _Base(__id), __m_instOf(&__instOf), __m_interconnect(__interconnect), __m_owner(&__o) {}
    virtual ~BasicPinBase() {}

public: // Copy constructors.
    BasicPinBase(const _Self& __c)
        : _Base(__c), 
        __m_instOf(__c.__m_instOf), 
        __m_interconnect(__c.__m_interconnect), 
        __m_owner(__c.__m_owner) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_Connector* instanceOf() const { return __m_instOf; }
    const typename _Entity::_Interconnect* interconnect() const { return __m_interconnect; }
    const typename _Entity::_Entity* owner() const { return __m_owner; }
    const _Flags sigDir() const { return instanceOf()->sigDir(); }
    virtual const typename _Entity::_Mstr& name() const {
        if (!__m_instOf) {
            // throw exception
            assert(0);
        }
        return __m_instOf->name(); 
    }

public: // Utility.
    _Pair pair() { return _Pair(/*_Base::*/name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        if (interconnect()) {
            __d.pointer(interconnect()->nameTag(), interconnect()->id());
        }
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        __d.beginPin(name());
        if (interconnect()) {
            __d.data(interconnect()->name());
        }
        __d.endPin();
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    void connect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) {
        __interconnect.connectPin(*this, __r);
    }
    void disconnect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) {
        __interconnect.disconnectPin(*this, __r);
    }
    // Disconnect the external net. The external net will acquire
    // all the internal connections acccessile from the pin's port
    // then the port needs to be removed from connnection. 
    void dissolve(typename _Entity::_Rack& __r) {
        typename _Entity::_Interconnect* __netext = interconnect();
        if (!__netext) { // No external connection.
            return;
        }
        disconnect(*__netext, __r);

        typename _Entity::_Connector* __port = instanceOf();
        if (!__port) {
            // throw exception: Corrupt database
            assert(0);
        }

        __port->movePins(*__netext, __r);
        __port->dissolve(__r);
    }
    void movePins(typename _Entity::_Interconnect& __n, typename _Entity::_Rack& __r)  {
    }
    _Self& assign(const _Self& __c) { return _assign(__c); }

private: // functions
    // Accessors.
    typename _Entity::_Connector* instanceOf() { return __m_instOf; }
    typename _Entity::_Interconnect* interconnect() { return __m_interconnect; }
    typename _Entity::_Entity* owner() { return __m_owner; }
    _Flags sigDir() { return instanceOf()->sigDir(); }
    void instanceOf(typename _Entity::_Connector* instOf) { __m_instOf = instOf; }
    void interconnect(typename _Entity::_Interconnect* __interconnect) { __m_interconnect = __interconnect; }
    void owner(typename _Entity::_Entity* __o) { __m_owner = __o; }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_PinBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        auto& s = const_cast<_Self&>(__c);
        instanceOf(s.instanceOf());
        owner(s.owner());
        interconnect(const_cast<typename _Entity::_Interconnect*>(__c.interconnect()));
        return *this;
    }

private: // data
    typename _Entity::_Connector*     __m_instOf;
    typename _Entity::_Interconnect*  __m_interconnect;
    typename _Entity::_Entity*        __m_owner;
};

using PinBase = BasicPinBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WPinBase = BasicPinBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicPinBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicPinBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicPinBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicPinBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define PINBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicPinBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

PINBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __PINBASE_H__
