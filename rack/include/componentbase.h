#ifndef __COMPONENTBASE_H__
#define __COMPONENTBASE_H__
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
class BasicComponentBase 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_Component;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDID();
    _ADDname();

public: // constructors/destructors.
    BasicComponentBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name, typename _Entity::_LibraryBase* __p) 
        : _INITID(__id), _INITNAME(__name), __m_refCount(0), __m_parent(__p) {}
    virtual ~BasicComponentBase() {}

public: // Copy constructors.
    BasicComponentBase(const _Self& __c)
        : _Base(__c), _INITID(__c.__m_id), _INITNAME(__c.__m_name), 
        __m_refCount(0), __m_parent(__c.__m_parent) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const std::uint64_t refCount() const { return __m_refCount; }
    const typename _Entity::_LibraryBase* parent() const { return __m_parent; }
    virtual const typename _Entity::_ConnectorSet& ports() const = 0;
    virtual const typename _Entity::_ViewManager& viewManager() const = 0;
    virtual typename _Entity::_ViewManager& viewManager() = 0;

public: // Utility.
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(_nameTag(), id());
        __d.data("refcount", refCount());
        __d.data(parent()->nameTag(), parent()->name());
        _Base::dump(__d);
        __d.endTag(_nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }
    void incrRefCount() { refCount(refCount()+1); }
    void decrRefCount() { refCount(refCount()-1); }

    // creation.
    virtual void createPins(typename _Entity::_Reference& __inst, typename _Entity::_Rack& __r) = 0;
    virtual void erase(typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) = 0;
    virtual typename _Entity::_Entity* clone(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) const = 0;
    virtual void replace(typename _Entity::_Reference& _i, typename _Entity::_Rack& __r) = 0;

    // Search
    virtual typename _Entity::_Connector* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) = 0;
    virtual typename _Entity::_Connector* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) = 0;

private: //functions.
    std::uint64_t refCount() { return __m_refCount; }
    typename _Entity::_LibraryBase* parent() { return __m_parent; }
    void parent(typename _Entity::_LibraryBase* __p) { __m_parent = __p; }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_ComponentBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        _ASSIGNID(__c.id());
        _ASSIGNNAME(__c.name());
        refCount(0);
        auto& s = const_cast<_Self&>(__c);
        parent(s.parent());
        return *this;
    }
    void refCount(const std::uint64_t __r) { __m_refCount = __r; }

private: // data.
    std::uint64_t                            __m_refCount;
    typename _Entity::_LibraryBase*    __m_parent;
};

using ComponentBase = BasicComponentBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WComponentBase = BasicComponentBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicComponentBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicComponentBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicComponentBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicComponentBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define COMPONENTBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicComponentBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

COMPONENTBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __COMPONENTBASE_H__
