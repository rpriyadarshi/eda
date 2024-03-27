#ifndef __PORTBASE_H__
#define __PORTBASE_H__
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
class BasicPortBase 
    : public BasicConnectorBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_Connector;
    using _Self = typename _Entity::_PortBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDname();

public: 
    // constructors/destructors.
    BasicPortBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name, const _Flags __sd, typename _Entity::_Entity& __o) 
        : _Base(__id), _INITNAME(__name), __m_interconnect(0), __m_owner(&__o) { _Entity::flags(utl::mFalse); sigDir(__sd); }
    BasicPortBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name, const _Flags __sd, typename _Entity::_Entity& __o, typename _Entity::_Interconnect& __i) 
        : _Base(__id), _INITNAME(__name), __m_interconnect(&__i), __m_owner(&__o) { _Base::flags(utl::mFalse); sigDir(__sd); }
    virtual ~BasicPortBase() {}

public: // Copy constructors.
    BasicPortBase(const _Self& __c)
        : _Base(__c.id()), _INITNAME(__c.name()), 
        __m_interconnect(const_cast<typename _Entity::_Interconnect*>(__c.interconnect())),  
        __m_owner(const_cast<typename _Entity::_Entity*>(__c.owner())) { _Entity::flags(__c.flags()); }

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_Connector* instanceOf() const { return 0; }
    const typename _Entity::_Interconnect* interconnect() const { return __m_interconnect; }
    const typename _Entity::_Entity* owner() const { return __m_owner; }
    const _Flags sigDir() const { return _Base::flags()&mSigDir; }

public: // Utility.
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        _dumpSigDir(__d);
        // pointer print
        if (interconnect()) {
            __d.pointer(interconnect()->nameTag(), interconnect()->id());
        }
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        __d.beginPort(name());
        __d.endPort();
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    void connect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) {
        __interconnect.connectPort(*this, __r);
    }
    void disconnect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) {
        __interconnect.disconnectPort(*this, __r);
    }
    void dissolve(typename _Entity::_Rack& __r) {
        typename _Entity::_Interconnect* __netint = interconnect();
        if (!__netint) { // No internal connecton.
            return;
        }

        disconnect(*__netint, __r);
        typename _Entity::_Component* __o = dynamic_cast<typename _Entity::_Component*>(owner());
        __o->erase(*__netint, __r);
    }
    // Delegates the pin movement to net, since net has the pin information.
    void movePins(typename _Entity::_Interconnect& __n, typename _Entity::_Rack& __r)  {
        typename _Entity::_Interconnect* __netint = interconnect();
        if (!__netint) { // No internal connecton.
            return;
        }

        __netint->movePins(__n, __r);
    }
    _Self& assign(const _Self& __c) { return _assign(__c); }

private: // functions
    // Accessors.
    typename _Entity::_Connector* instanceOf() { return 0; }
    typename _Entity::_Interconnect* interconnect() { return __m_interconnect; }
    typename _Entity::_Entity* owner() { return __m_owner; }
    _Flags sigDir() { return _Entity::__m_flags&mSigDir; }
    void sigDir(const _Flags __sd) { _Entity::__m_flags |= __sd&mSigDir; }
    void interconnect(typename _Entity::_Interconnect* __i) { __m_interconnect = __i; }
    void owner(typename _Entity::_Entity* __o) {__m_owner = __o; }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_PortBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        auto& s = const_cast<_Self&>(__c);
        owner(s.owner());
        interconnect(const_cast<typename _Entity::_Interconnect*>(__c.interconnect()));
        return *this;
    }
    void _dumpSigDir(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if((_Base::flags()&mOutput).any()) {
            __d.beginOutputs();
            __d.endOutputs();
        } else if((_Base::flags()&mInput).any()) {
            __d.beginInputs();
            __d.endInputs();
        } else if((_Base::flags()&mInout).any()) {
            __d.beginInouts();
            __d.endInouts();
        }
    }
    void _dumpSigDir(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        if((_Base::flags()&mOutput).any()) {
            __d.beginOutputs();
            __d.endOutputs();
        } else if((_Base::flags()&mInput).any()) {
            __d.beginInputs();
            __d.endInputs();
        } else if((_Base::flags()&mInout).any()) {
            __d.beginInouts();
            __d.endInouts();
        }
    }

private: // data
    typename _Entity::_Interconnect*  __m_interconnect;
    typename _Entity::_Entity*        __m_owner;
};

using PortBase = BasicPortBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WPortBase = BasicPortBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicPortBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicPortBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicPortBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicPortBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define PORTBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicPortBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

PORTBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __PORTBASE_H__
