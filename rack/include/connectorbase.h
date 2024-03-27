#ifndef __CONNECTORBASE_H__
#define __CONNECTORBASE_H__
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
class BasicConnectorBase 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_Connector;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDID();

public: // constructors/destructors.
    explicit BasicConnectorBase(const utl::Id_t __id) 
        : _INITID(__id) {}
    virtual ~BasicConnectorBase() {}

public: // Copy constructors.
    BasicConnectorBase(const _Self& __c)
        : _Base(__c), _INITID(__c.__m_id) {}

public: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    virtual const typename _Entity::_Mstr& name() const = 0;
    virtual const typename _Entity::_Connector* instanceOf() const = 0;
    virtual typename _Entity::_Connector* instanceOf() = 0;
    virtual const typename _Entity::_Interconnect* interconnect() const = 0;
    virtual typename _Entity::_Interconnect* interconnect() = 0;
    virtual void interconnect(typename _Entity::_Interconnect* __i) = 0;
    virtual const typename _Entity::_Entity* owner() const = 0;
    virtual typename _Entity::_Entity* owner() = 0;
    virtual const _Flags sigDir() const = 0;
    virtual _Flags sigDir() = 0;

public: // Utility.
    typename _Entity::_Entity* clone(typename _Entity::_Module& __m, typename _Entity::_Rack& __r) const { 
        return _clone(__m, __r);  
    }
    typename _Entity::_Entity* clone(typename _Entity::_Instance& __i, typename _Entity::_Rack& __r) const { 
        return _clone(__i, __r);  
    }
    _Pair pair() { return _Pair(/*_Base::*/name(), this); }
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
    virtual void connect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) = 0;
    virtual void disconnect(typename _Entity::_Interconnect& __interconnect, typename _Entity::_Rack& __r) = 0;
    virtual void dissolve(typename _Entity::_Rack& __r) = 0;
    virtual void movePins(typename _Entity::_Interconnect& __n, typename _Entity::_Rack& __r)  = 0;
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

    // Helpers for Python
    typename _Entity::_Pin* CastToPin() {
        return static_cast<typename _Entity::_Pin*>(this);
    }
    typename _Entity::_Port* CastToPort() {
        return static_cast<typename _Entity::_Port*>(this);
    }

private:
    typename _Entity::_Entity* _clone(typename _Entity::_Module& __m, typename _Entity::_Rack& __r) const {
        typename _Entity::_Port& __p = __m.createPort(/*_Base::*/name(), sigDir(), __r);
        return &__p;
    }
    typename _Entity::_Entity* _clone(typename _Entity::_Instance& __i, typename _Entity::_Rack& __r) const {
        typename _Entity::_Pin& __p = __i.createPin(*const_cast<typename _Entity::_Connector*>(instanceOf()), __r);
        return &__p;
    }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_ConnectorBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        _ASSIGNID(__c.id());
        return *this;
    }
};

using Connector = BasicConnectorBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WConnector = BasicConnectorBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicConnectorBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicConnectorBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicConnectorBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicConnectorBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
//https://stackoverflow.com/questions/38404806/error-c2039-type-name-is-not-a-member-of-of-swigtraitsbar
// error: ‘type_name’ is not a member of ‘swig::traits<rack::BasicConnectorBase<char, std::char_traits<char>, std::allocator<char> > >’

%{
    namespace swig {
    template <> struct traits<typename rack::BasicConnectorBase<char, std::char_traits<char>, std::allocator<char> >>
    {
        typedef pointer_category category;
        static const char* type_name()
        {
            return "rack::BasicConnectorBase";
        }
    };
    }
%}

%define CONNECTORBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicConnectorBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

CONNECTORBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#ifdef SWIG
namespace std {
%define RACK_MULTIMAP_TEMPLATE_CL_WRAP1(prefix, _Tp, type)
%template(prefix ## type ## MultiMap) std::multimap<
    typename utl::basic_multistring<_Tp, std::char_traits<_Tp>, std::allocator<_Tp>>,
    typename rack::BasicConnectorBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp>>*,
    std::less<typename utl::basic_multistring<_Tp, std::char_traits<_Tp>, std::allocator<_Tp>>>
>;
%enddef

RACK_MULTIMAP_TEMPLATE_CL_WRAP1(cl, char, Connector);
}
#endif // SWIG

#endif // __CONNECTORBASE_H__
