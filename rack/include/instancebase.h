#ifndef __INSTANCEBASE_H__
#define __INSTANCEBASE_H__
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
 * Pins are an ordered collection. First pins are driver or source 
 * pins by all the sink pins. 
 */
template <class _CharT, class _Traits, class _Alloc>
class BasicInstanceBase 
    : public BasicReferenceBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
	using _Base = typename _Entity::_Reference;
    using _Self = typename _Entity::_InstanceBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: // constructors/destructors.
    BasicInstanceBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name, 
            typename _Entity::_ViewGroup& __parent, typename _Entity::_ViewGroup& __viewOf) 
        : _Base(__id, __name), __m_parent(&__parent), 
        __m_viewOf(&__viewOf) { incrRefCount(); }
    virtual ~BasicInstanceBase() { /*decrRefCount();*/ purge(); }
    void purge() {
        std::for_each(pins().begin(), pins().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Connector*>());
    }

public: // Copy constructors.
    BasicInstanceBase(const _Self& __c)
        : _Base(__c), __m_pins(__c.__m_pins), 
        __m_parent(__c.__m_parent), __m_viewOf(__c.__m_viewOf) { incrRefCount(); }

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_ConnectorSet& pins() const { return __m_pins; }
    const typename _Entity::_ViewGroup* parent() const { return __m_parent; }
    const typename _Entity::_ViewGroup* viewOf() const { return __m_viewOf; }
    const typename _Entity::_Component* instanceOf() const { return viewOf()->parent().parent(); }
    void parent(typename _Entity::_ViewGroup* __vg) { __m_parent = __vg; }
    void viewOf(typename _Entity::_ViewGroup* __vg) { __m_viewOf = __vg; }

public: // creation.
    virtual typename _Entity::_Pin& createPin(typename _Entity::_Connector& __instOf, typename _Entity::_Rack& __r) {
        return _createPin(__instOf, __r);
    }

    void createPins(typename _Entity::_Rack& __r) {
        _createPins(__r);
    }

public: // Search
    typename _Entity::_Connector* findPin(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findPin(__name, __r);
    }
    typename _Entity::_Connector* findPin(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findPin(__name, __r);
    }

public: // Utility.
    const std::uint64_t refCount() const { return viewOf()->refCount(); }
    std::uint64_t refCount() { return viewOf()->refCount(); }
    void incrRefCount() { viewOf()->incrRefCount(); }
    void decrRefCount() { viewOf()->decrRefCount(); }
    typename _Entity::_Entity* clone(typename _Entity::_View& __v, typename _Entity::_Rack& __r) const { return _clone(__v, __r); }
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        const typename _Entity::_Component* __instOf = instanceOf();
        const typename _Entity::_ViewGroup* __p = parent();
        if (__p) {
            // pointer print
            const typename _Entity::_ViewManager& __vm = 
                dynamic_cast<const typename _Entity::_ViewManager&>(__p->parent());
            const typename _Entity::_Component* __c = 
                dynamic_cast<const typename _Entity::_Component*>(__vm.parent());
            __d.data("Parent", __c->name());
        }
        if (__instOf) {
            // pointer print
            __d.pointer(__instOf->nameTag(), __instOf->id());
        }
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, pins());
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        const typename _Entity::_Component* __instOf = instanceOf();
        if (__instOf) {
            __d.beginInstance(__instOf->name(), _Base::name());
            utl::dumpSep<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
                std::less<typename _Entity::_Mstr>, _Alloc>(__d, pins(), ", ");
            __d.endInstance();
        }
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

public: // Netlist manipulations.
    void connect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r)  {
        _connect(__pname, __p_n, __r);
    }
    void disconnect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r)  {
        _disconnect(__pname, __p_n, __r);
    }
    void replace(typename _Entity::_Component* __p_c, typename _Entity::_Rack& __r) {
        _replace(__p_c, __r);
    }
    void replace(typename _Entity::_ViewGroup* __p_vg, typename _Entity::_Rack& __r) {
        _replace(__p_vg, __r);
    }
    // Disconnecting this instance means that all the pins must
    // disconnect the external net. The external net will acquire
    // all the internal connections accessible from the pin's port
    void dissolve(typename _Entity::_Rack& __r) {
        _dissolve(__r);
    }
    void dissolve(const typename _Entity::_Str& __path, typename _Entity::_Rack& __r) {
        _dissolve(__path, __r);
    }
    void dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_Rack& __r) {
        _dissolve(__path, __r);
    }

private: // functions
    // Accessors.
    typename _Entity::_ConnectorSet& pins() { return __m_pins; }
    typename _Entity::_ViewGroup* parent() { return __m_parent; }
    typename _Entity::_ViewGroup* viewOf() { return __m_viewOf; }
    typename _Entity::_Component* instanceOf() { /* NASTY!!! Find another solution */
        const typename _Entity::_ViewGroup* __vg = viewOf();
        const typename _Entity::_ViewManager& __vm = __vg->parent();
        return const_cast<typename _Entity::_Component*>(__vm.parent()); 
    }

    // creation.
    typename _Entity::_Pin& _createPin(typename _Entity::_Connector& __instOf, 
            typename _Entity::_Rack& __r) {
        typename _Entity::_Pin* __p_v = new (__r.pinAlloc().allocate(1))
            typename _Entity::_Pin(__r.getid(), __instOf, *this);
        std::pair<typename _Entity::_ConnectorItr, bool> __pr = pins().insert(__p_v->pair());
        if (__pr.second == false) {
            throw std::bad_alloc();
        }
        return *__p_v;
    }

    void _createPins(typename _Entity::_Rack& __r) {
        typename _Entity::_Component* __p_comp = instanceOf();
        if (!__p_comp) {
            // throw error
            assert(0);
        }
        __p_comp->createPins(*this, __r);
    }

    typename _Entity::_Entity* _clone(typename _Entity::_View& __v, typename _Entity::_Rack& __r) const {
        typename _Entity::_Netlist& __nl = *dynamic_cast<typename _Entity::_Netlist*>(&__v);
        typename _Entity::_Instance& __i = __nl.createInstance(_Base::name(), 
            *const_cast<typename _Entity::_ViewGroup*>(parent()), 
            *const_cast<typename _Entity::_ViewGroup*>(viewOf()), __r);
        return &__i;
    }

private: // Netlist manipulations
    void _connect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r)  {
        typename _Entity::_Connector& __p_v = *findPin(__pname, __r);
        __p_v.connect(__p_n, __r);
    }
    void _disconnect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r)  {
        typename _Entity::_Connector& __p_v = *findPin(__pname, __r);
        __p_v.disconnect(__p_n, __r);
    }
    void _replace(typename _Entity::_Component* __p_c, typename _Entity::_Rack& __r) {
        __p_c->replace(*this, __r);
    }
    void _replace(typename _Entity::_ViewGroup* __p_vg, typename _Entity::_Rack& __r) {
        __p_vg->replace(*this, __r);
    }
    // Disconnecting this instance means that all the pins must
    // disconnect the external net. The external net will acquire
    // all the internal connections accessible from the pin's port
    void _dissolve(typename _Entity::_Rack& __r) {
        _dissolveCore(_Base::name(), __r);
    }
    void _dissolve(const typename _Entity::_Str& __path, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__path);
        _dissolve(*__ps.first, __r);
    }
    void _dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__path);
        typename _Entity::_Mstr __newPath = *__ps.first;
        __newPath += _Base::name();
        _dissolveCore(__path, __r);
    }
    void _dissolveCore(const typename _Entity::_Mstr& __path, typename _Entity::_Rack& __r) {
        // The module corresponding to the instance needs to be 
        // cloned if needed ~ if the reference count is greater than 1.
        typename _Entity::_ViewGroup* __vg = viewOf();
        if (refCount() > 1) {
            std::pair<typename _Entity::_DictItr, bool> __pn = __r.dictionary().unique(_Base::name());
            __vg->replace(*__pn.first, *this, __r);
            __vg = viewOf(); /* Got replaced */
        }
        for(typename _Entity::_ConnectorConstItr __itr = pins().begin(); 
            __itr != pins().end(); __itr++) {
            typename _Entity::_Connector* __pinext = __itr->second;
            if (!__pinext) {
                // throw exception
                assert(0);
            }
            __pinext->dissolve(__r);
        }
        typename _Entity::_ViewGroup* __pvg = parent();
        __vg->dissolve(__path, *__pvg, __r);
        __pvg->erase(*this, __r);
        delete __vg;
    }

private: // Search
    typename _Entity::_Connector* _findPin(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__name);
        return _findPin(__ms, __r);
    }
    typename _Entity::_Connector* _findPin(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ConnectorItr __itr = pins().find(__name);
        if (__itr == pins().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00002", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00002", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        return __itr->second;
    }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_InstanceBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Self& s = const_cast<_Self&>(__c);
        _Base::assign(__c);
        pins() = s.pins();
        parent(s.parent());
        viewOf(s.viewOf());
        incrRefCount();
        return *this;
    }

private: // data
    typename _Entity::_ConnectorSet   __m_pins;
    typename _Entity::_ViewGroup*     __m_parent;
    typename _Entity::_ViewGroup*     __m_viewOf;
};

using InstanceBase = BasicInstanceBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WInstanceBase = BasicInstanceBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicInstanceBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicInstanceBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicInstanceBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicInstanceBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define INSTANCEBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicInstanceBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

INSTANCEBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __INSTANCEBASE_H__
