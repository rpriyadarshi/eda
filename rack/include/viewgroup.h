#ifndef __VIEWGROUP_H__
#define __VIEWGROUP_H__
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
class BasicViewGroup 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
	using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_ViewGroup;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

private: // Automated defs
    _ADDID();
    _ADDname();

public: // constructors/destructors.
    BasicViewGroup(const utl::Id_t __id, const typename _Entity::_Mstr& __name, typename _Entity::_ViewManager& __parent) 
        : _INITID(__id), _INITNAME(__name),
        __m_blackbox(__parent.defaultBlackbox()), 
        __m_physical(__parent.defaultPhysical()), 
        __m_netlist(__parent.defaultNetlist()), 
        __m_timing(__parent.defaultTiming()), 
        __m_cell(__parent.defaultCell()), 
        __m_hdl(__parent.defaultHdl()), 
        __m_parent(__parent) {}
    virtual ~BasicViewGroup() {}

public: // Copy constructors.
    BasicViewGroup(const _Self& __c)
        : _INITID(__c.id()), _INITNAME(__c.name()),
        __m_blackbox(__c.__m_blackbox), 
        __m_physical(__c.__m_physical), 
        __m_netlist(__c.__m_netlist), 
        __m_timing(__c.__m_timing), 
        __m_cell(__c.__m_cell), 
        __m_hdl(__c.__m_hdl), 
        __m_parent(__c.__m_parent) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_Blackbox* blackbox() const { return __m_blackbox; }
    const typename _Entity::_Physical* physical() const { return __m_physical; }
    const typename _Entity::_Netlist* netlist() const { return __m_netlist; }
    const typename _Entity::_Timing* timing() const { return __m_timing; }
    const typename _Entity::_Cell* cell() const { return __m_cell; }
    const typename _Entity::_Hdl* hdl() const { return __m_hdl; }
    const typename _Entity::_ViewManager& parent() const { return __m_parent; }

public: // Utility.
    const std::uint64_t refCount() const { return parent().refCount(); }
    std::uint64_t refCount() { return parent().refCount(); }
    void incrRefCount() { parent().incrRefCount(); }
    void decrRefCount() { parent().decrRefCount(); }
    typename _Entity::_Entity* clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const { 
        return _clone(__c, __r);  
    }
    typename _Entity::_Entity* clone(const typename _Entity::_Mstr& __ms, typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const { 
        return _clone(__ms, __c, __r);  
    }
    _Pair pair() { return _Pair(name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), id());
        _Base::dump(__d);
        if (blackbox()) blackbox()->dump(__d);
        if (physical()) physical()->dump(__d);
        if (netlist()) netlist()->dump(__d);
        if (timing()) timing()->dump(__d);
        if (cell()) cell()->dump(__d);
        if (hdl()) hdl()->dump(__d);
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    void replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _replace(__i, __r);
    }
    void replace(const typename _Entity::_Mstr& __ms, typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _replace(__ms, __i, __r);
    }
    void dissolve(const typename _Entity::_Str& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        _dissolve(__path, __p, __r);
    }
    void dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        _dissolve(__path, __p, __r);
    }
    void erase(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _erase(__i, __r);
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

private:
    // Accessors.
    typename _Entity::_Blackbox* blackbox() { return __m_blackbox; }
    typename _Entity::_Physical* physical() { return __m_physical; }
    typename _Entity::_Netlist* netlist() { return __m_netlist; }
    typename _Entity::_Timing* timing() { return __m_timing; }
    typename _Entity::_Cell* cell() { return __m_cell; }
    typename _Entity::_Hdl* hdl() { return __m_hdl; }
    typename _Entity::_ViewManager& parent() { return __m_parent; }

    // Set up data.
    void blackbox(typename _Entity::_Blackbox* __v) { __m_blackbox = __v; }
    void physical(typename _Entity::_Physical* __v) { __m_physical = __v; }
    void netlist(typename _Entity::_Netlist* __v) { __m_netlist = __v; }
    void timing(typename _Entity::_Timing* __v) { __m_timing = __v; }
    void cell(typename _Entity::_Cell* __v) { __m_cell = __v; }
    void hdl(typename _Entity::_Hdl* __v) { __m_hdl = __v; }

    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_ViewGroup; }
    typename _Entity::_Entity* _clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const {
        return _clone(name(), __c, __r);
    }
    typename _Entity::_Entity* _clone(const typename _Entity::_Mstr& __ms, typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const {
        typename _Entity::_ViewManager& __vm = __c.viewManager();
        typename _Entity::_ViewGroup& __vg = __vm.createViewGroup(__ms, __r);
        return &__vg;
    }
    void _replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _replace(name(), __i, __r);
    }
    void _replace(const typename _Entity::_Mstr& __ms, typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        typename _Entity::_ViewManager& __vm = parent();
        typename _Entity::_Component* __c = __vm.parent();
        typename _Entity::_Netlist* __nl = netlist();
        // TBD: Need to take care of the views
        if (__nl) {
            typename _Entity::_Netlist* __nl_new =     dynamic_cast<typename _Entity::_Netlist*>(__nl->clone(__ms, *__c, __r));
            typename _Entity::_ViewGroup* __vg_new = dynamic_cast<typename _Entity::_ViewGroup*>(clone(*__c, __r));
            __vg_new->netlist(__nl_new);
            __i.viewOf(__vg_new);
        }
    }
    void _dissolve(const typename _Entity::_Str& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__path);
        _dissolve(__ms, __p, __r);
    }
    void _dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        typename _Entity::_Netlist* __nl = netlist();
        // TBD: need to take care of other views
        if (__nl) {
            __nl->dissolve(__path, __p, __r);
        }
    }
    void _erase(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        typename _Entity::_Netlist* __nl = netlist();
        __nl->erase(__i, __r);
    }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        _ASSIGNID(__c.id());
        _ASSIGNNAME(__c.name());
        auto& s = const_cast<_Self&>(__c);
        blackbox(s.blackbox());
        physical(s.physical());
        netlist(s.netlist());
        timing(s.timing());
        cell(s.cell());
        hdl(s.hdl());
        parent() = s.parent();
        return *this;
    }

private: // data
    typename _Entity::_Blackbox*      __m_blackbox;
    typename _Entity::_Physical*      __m_physical;
    typename _Entity::_Netlist*       __m_netlist;
    typename _Entity::_Timing*        __m_timing;
    typename _Entity::_Cell*          __m_cell;
    typename _Entity::_Hdl*           __m_hdl;
    typename _Entity::_ViewManager&   __m_parent;
};

using ViewGroup = BasicViewGroup<char, std::char_traits<char>, 
    std::allocator<char> >;
using WViewGroup = BasicViewGroup<wchar_t, std::char_traits<wchar_t>, 
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicViewGroup<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewGroup<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewGroup<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewGroup<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define VIEWGROUP_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## ViewGroup) rack::BasicViewGroup<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

VIEWGROUP_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __VIEWGROUP_H__
