#ifndef __LIBRARYBASE_H__
#define __LIBRARYBASE_H__
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
class BasicLibraryBase 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_LibraryBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDID();
    _ADDname();

public: // constructors/destructors.
    BasicLibraryBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name) 
        : _INITID(__id), _INITNAME(__name) {}
    virtual ~BasicLibraryBase() { purge(); }

public: // Copy constructors.
    BasicLibraryBase(const _Self& __c)
        : _Base(__c), _INITID(__c.__m_id), _INITNAME(__c.__m_name) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_ModuleSet& modules() const { return __m_modules; }

public: // creation.
    typename _Entity::_Module& createModule(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createModule(__name, __r);
    }

public: // Utility.
    _Pair pair() { return _Pair(name(), this); }
    void purge() {
        std::for_each(modules().begin(), modules().end(), DeleteContents<const typename _Entity::_Mstr, typename _Entity::_Module*>());
        std::for_each(modules().begin(), modules().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Module*>());
    }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), id());
        _Base::dump(__d);
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Module*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, modules());
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Module*, 
            std::less<typename _Entity::_Mstr>, _Alloc>(__d, modules());
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

public: // Search
    typename _Entity::_Module* findModule(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findModule(__name, __r);
    }
    typename _Entity::_Module* findModule(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findModule(__name, __r);
    }

private: // functions.
    // Accessors.
    typename _Entity::_ModuleSet& modules() { return __m_modules; }

    typename _Entity::_Module* _findModule(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__name);
        return _findModule(__ms, __r);
    }
    typename _Entity::_Module* _findModule(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_ModuleConstItr& __itr = modules().find(__name);
        return __itr == modules().end() ?
                0 : dynamic_cast<typename _Entity::_Module*>(__itr->second);
    }

    // creation.
    typename _Entity::_Module& _createModule(const typename _Entity::_Str& __name,
            typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Module* __p_v = new (__r.modAlloc().allocate(1))
            typename _Entity::_Module(__r.getid(), *__pm.first, this);
        __p_v->viewManager().parent(__p_v); /* This cannot be initialized within the constructor */
        modules().insert(__p_v->pair());
        return *__p_v;
    }

private:
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_LibraryBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        _ASSIGNID(__c.id());
        _ASSIGNNAME(__c.name());
        return *this;
    }
private: // Data
    typename _Entity::_ModuleSet      __m_modules;
};

using LibraryBase = BasicLibraryBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WLibraryBase = BasicLibraryBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicLibraryBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicLibraryBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicLibraryBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicLibraryBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define LIBRARYBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicLibraryBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

LIBRARYBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __LIBRARYBASE_H__
