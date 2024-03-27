#ifndef __INTERCONNECTBASE_H__
#define __INTERCONNECTBASE_H__
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
class BasicInterconnectBase 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
	using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_Interconnect;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDID();
    _ADDnamePtr();

public: // constructors/destructors.
    BasicInterconnectBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name) 
        : _INITID(__id), _INITNAMEPTR(__name) {}
    virtual ~BasicInterconnectBase() {}

public: // Copy constructors.
    BasicInterconnectBase(const _Self& __c)
        : _Base(__c), _INITID(__c.__m_id), _INITNAME(__c.__m_name) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    virtual const typename _Entity::_ConnectorSet& ports() const = 0;
    virtual const typename _Entity::_ConnectorMultiSet& pins() const = 0;

public: // Utility.
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(_nameTag(), id());
        _Base::dump(__d);
        __d.endTag(_nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    virtual void connectPort(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  = 0;
    virtual void connectPin(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  = 0;
    virtual void disconnectPort(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  = 0;
    virtual void disconnectPin(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  = 0;
    virtual void movePins(typename _Entity::_Interconnect& __n, typename _Entity::_Rack& __r)  = 0;
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

private:
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_InterconnectBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        _ASSIGNID(__c.id());
        _ASSIGNNAMEPTR(__c.name());
        return *this;
    }
};

using InterconnectBase = BasicInterconnectBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WInterconnectBase = BasicInterconnectBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicInterconnectBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicInterconnectBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicInterconnectBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicInterconnectBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define INTERCONNECTBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicInterconnectBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

INTERCONNECTBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __INTERCONNECTBASE_H__
