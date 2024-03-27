#ifndef __VIEWMANAGER_H__
#define __VIEWMANAGER_H__
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
class BasicViewManager 
    : public BasicUnnamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_UnnamedEntity;
    using _Self = typename _Entity::_ViewManager;
    
private: // friends
    friend class rack::BasicLibraryBase<_CharT, _Traits, _Alloc>;
    friend class rack::BasicViewGroup<_CharT, _Traits, _Alloc>;

public: // constructors/destructors.
    BasicViewManager() : __m_parent (0) {}
    virtual ~BasicViewManager() { purge(); }

public: // Copy constructors.
    BasicViewManager(const _Self& __c)
        : _Base(__c),
        __m_blackbox(__c.__m_blackbox),
        __m_physical(__c.__m_physical),
        __m_netlist(__c.__m_netlist),
        __m_timing(__c.__m_timing),
        __m_cell(__c.__m_cell),
        __m_hdl(__c.__m_hdl),
        __m_viewGroup(__c.__m_viewGroup),
        __m_parent(__c.__m_parent) {}

public: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const utl::Id_t id() const { return 0; };
    const typename _Entity::_ViewSet& blackbox() const { return __m_blackbox; }
    const typename _Entity::_ViewSet& physical() const { return __m_physical; }
    const typename _Entity::_ViewSet& netlist() const { return __m_netlist; }
    const typename _Entity::_ViewSet& timing() const { return __m_timing; }
    const typename _Entity::_ViewSet& cell() const { return __m_cell; }
    const typename _Entity::_ViewSet& hdl() const { return __m_hdl; }
    const typename _Entity::_ViewGroupSet& viewGroup() const { return __m_viewGroup; }
    const typename _Entity::_Component* parent() const { return __m_parent; }
   
    // creation.
    typename _Entity::_Blackbox& createBlackbox(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createBlackbox(vBlackbox, __name, __r);
    }
    typename _Entity::_Blackbox& createBlackbox(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createBlackbox(__id, __name, __r);
    }
    typename _Entity::_Blackbox& createBlackbox(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createBlackbox(vBlackbox, __name, __r);
    }
    
    typename _Entity::_Physical& createPhysical(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createPhysical(vPhysical, __name, __r);
    }
    typename _Entity::_Physical& createPhysical(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createPhysical(__id, __name, __r);
    }
    typename _Entity::_Physical& createPhysical(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createPhysical(vPhysical, __name, __r);
    }

    typename _Entity::_Netlist& createNetlist(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createNetlist(vNetlist, __name, __r);
    }
    typename _Entity::_Netlist& createNetlist(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createNetlist(__id, __name, __r);
    }
    typename _Entity::_Netlist& createNetlist(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createNetlist(vNetlist, __name, __r);
    }

    typename _Entity::_Timing& createTiming(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createTiming(vTiming, __name, __r);
    }
    typename _Entity::_Timing& createTiming(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createTiming(__id, __name, __r);
    }
    typename _Entity::_Timing& createTiming(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createTiming(vTiming, __name, __r);
    }

    typename _Entity::_Cell& createCell(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createCell(vCell, __name, __r);
    }
    typename _Entity::_Cell& createCell(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createCell(__id, __name, __r);
    }
    typename _Entity::_Cell& createCell(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createCell(vCell, __name, __r);
    }

    typename _Entity::_Hdl& createHdl(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createHdl(vHdl, __name, __r);
    }
    typename _Entity::_Hdl& createHdl(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createHdl(__id, __name, __r);
    }
    typename _Entity::_Hdl& createHdl(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createHdl(vHdl, __name, __r);
    }

    typename _Entity::_ViewGroup& createViewGroup(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        return _createViewGroup(vHdl, __name, __r);
    }
    typename _Entity::_ViewGroup& createViewGroup(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createViewGroup(__id, __name, __r);
    }
    typename _Entity::_ViewGroup& createViewGroup(const typename _Entity::_Mstr& __name,
        typename _Entity::_Rack& __r) {
        return _createViewGroup(vHdl, __name, __r);
    }

public: // Utility.
    const std::uint64_t refCount() const { return parent()->refCount(); }
    //std::uint64_t refCount() { return parent()->refCount(); }
    void incrRefCount() { parent()->incrRefCount(); }
    void decrRefCount() { parent()->decrRefCount(); }
    typename _Entity::_Entity* clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const { 
        return _clone(__c, __r);  
    }
    void purge() {
        std::for_each(blackbox().begin(), blackbox().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(physical().begin(), physical().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(netlist().begin(), netlist().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(timing().begin(), timing().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(cell().begin(), cell().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(hdl().begin(), hdl().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_View*>());
        std::for_each(viewGroup().begin(), viewGroup().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_ViewGroup*>());
    }
    void purgeContents() {
        std::for_each(netlist().begin(), netlist().end(), DeleteContents<const typename _Entity::_Mstr, typename _Entity::_View*>());
    }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(_nameTag());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, blackbox());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, physical());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, netlist());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, timing());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, cell());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, hdl());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_ViewGroup*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, viewGroup());
        __d.endTag(_nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_View*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, netlist());
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Blackbox* defaultBlackbox() const {
        return blackbox().empty() ? 
            0: dynamic_cast<const typename _Entity::_Blackbox*>(blackbox().begin()->second);
    }
    const typename _Entity::_Physical* defaultPhysical() const {
        return physical().empty() ? 
            0: dynamic_cast<const typename _Entity::_Physical*>(physical().begin()->second);
    }
    const typename _Entity::_Netlist* defaultNetlist() const { 
        return netlist().empty() ? 
            0: dynamic_cast<const typename _Entity::_Netlist*>(netlist().begin()->second);
    }
    const typename _Entity::_Timing* defaultTiming() const { 
        return timing().empty() ? 
            0: dynamic_cast<const typename _Entity::_Timing*>(timing().begin()->second);
    }
    const typename _Entity::_Cell* defaultCell() const { 
        return cell().empty() ? 
            0: dynamic_cast<const typename _Entity::_Cell*>(cell().begin()->second);
    }
    const typename _Entity::_Hdl* defaultHdl() const { 
        return hdl().empty() ? 
            0: dynamic_cast<const typename _Entity::_Hdl*>(hdl().begin()->second);
    }
    const typename _Entity::_ViewGroup* defaultViewGroup() const { 
        return viewGroup().empty() ? 
            0: dynamic_cast<const typename _Entity::_ViewGroup*>(viewGroup().begin()->second);
    }

    // Search
    typename _Entity::_Connector* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_Component* __p_m = parent();
        if (!__p_m) {
            // TBD: Bad database
        }
        return __p_m->findPort(__name, __r);
    }
    typename _Entity::_Connector* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_Component* __p_m = parent();
        if (!__p_m) {
            // TBD: Bad database
        }
        return __p_m->findPort(__name, __r);
    }
    typename _Entity::_Blackbox* findBlackbox(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = blackbox().find(__s);
        return __itr == blackbox().end() ?
                0 : dynamic_cast<typename _Entity::_Blackbox*>(__itr->second);
    }
    typename _Entity::_Physical* findPhysical(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = physical().find(__s);
        return __itr == physical().end() ?
                0 : dynamic_cast<typename _Entity::_Physical*>(__itr->second);
    }
    typename _Entity::_Netlist* findNetlist(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = netlist().find(__s);
        return __itr == netlist().end() ?
                0 : dynamic_cast<typename _Entity::_Netlist*>(__itr->second);
    }
    typename _Entity::_Timing* findTiming(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = timing().find(__s);
        return __itr == timing().end() ?
                0 : dynamic_cast<typename _Entity::_Timing*>(__itr->second);
    }
    typename _Entity::_Cell* findCell(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = cell().find(__s);
        return __itr == cell().end() ?
                0 : dynamic_cast<typename _Entity::_Cell*>(__itr->second);
    }
    typename _Entity::_Hdl* findHdl(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewItr& __itr = hdl().find(__s);
        return __itr == hdl().end() ?
                0 : dynamic_cast<typename _Entity::_Hdl*>(__itr->second);
    }
    typename _Entity::_ViewGroup* findViewGroup(const typename _Entity::_Str& __s) {
        const typename _Entity::_ViewGroupItr& __itr = viewGroup().find(__s);
        return __itr == viewGroup().end() ?
                0 : dynamic_cast<typename _Entity::_ViewGroup*>(__itr->second);
    }
    typename _Entity::_ViewGroup* findViewGroup(const typename _Entity::_Mstr& __ms) {
        const typename _Entity::_ViewGroupItr& __itr = viewGroup().find(__ms);
        return __itr == viewGroup().end() ?
                0 : dynamic_cast<typename _Entity::_ViewGroup*>(__itr->second);
    }
    void erase(typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        //_ViewItr& __itr = views().find("netlist");
    }
    void replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _replace(__i, __r);
    }

    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

private: // functions
    // Accessors.
    void id(const utl::Id_t __id) {}
    typename _Entity::_ViewSet& blackbox() { return __m_blackbox; }
    typename _Entity::_ViewSet& physical() { return __m_physical; }
    typename _Entity::_ViewSet& netlist() { return __m_netlist; }
    typename _Entity::_ViewSet& timing() { return __m_timing; }
    typename _Entity::_ViewSet& cell() { return __m_cell; }
    typename _Entity::_ViewSet& hdl() { return __m_hdl; }
    typename _Entity::_ViewGroupSet& viewGroup() { return __m_viewGroup; }
    typename _Entity::_Component* parent() { return __m_parent; }
    void blackbox(const typename _Entity::_ViewSet& __s) { __m_blackbox = __s; }
    void physical(const typename _Entity::_ViewSet& __s) { __m_physical = __s; }
    void netlist(const typename _Entity::_ViewSet& __s) { __m_netlist = __s; }
    void timing(const typename _Entity::_ViewSet& __s) { __m_timing = __s; }
    void cell(const typename _Entity::_ViewSet& __s) { __m_cell = __s; }
    void hdl(const typename _Entity::_ViewSet& __s) { __m_hdl = __s; }
    void viewGroup(const typename _Entity::_ViewGroupSet& __s) { __m_viewGroup = __s; }
    void parent(typename _Entity::_Component* __p_c) { __m_parent = __p_c; }
    
    // raw creation.
    typename _Entity::_Blackbox& _createBlackbox(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Blackbox* __p_v = new (__r.blackboxAlloc().allocate(1))
            typename _Entity::_Blackbox(__id, *__pm.first, *this);
        blackbox().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Blackbox& _createBlackbox(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Blackbox* __p_v = new (__r.blackboxAlloc().allocate(1))
            typename _Entity::_Blackbox(__id, *__pm.first, *this);
        blackbox().insert(__p_v->pair());
        return *__p_v;
    }
    
    typename _Entity::_Physical& _createPhysical(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Physical* __p_v = new (__r.physicalAlloc().allocate(1))
            typename _Entity::_Physical(__id, *__pm.first, *this);
        physical().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Physical& _createPhysical(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Physical* __p_v = new (__r.physicalAlloc().allocate(1))
            typename _Entity::_Physical(__id, *__pm.first, *this);
        physical().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Netlist& _createNetlist(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Netlist* __p_v = new (__r.netlistAlloc().allocate(1))
            typename _Entity::_Netlist(__id, *__pm.first, *this);
        netlist().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Netlist& _createNetlist(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Netlist* __p_v = new (__r.netlistAlloc().allocate(1))
            typename _Entity::_Netlist(__id, *__pm.first, *this);
        netlist().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Timing& _createTiming(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Timing* __p_v = new (__r.timingAlloc().allocate(1))
            typename _Entity::_Timing(__id, *__pm.first, *this);
        timing().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Timing& _createTiming(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Timing* __p_v = new (__r.timingAlloc().allocate(1))
        typename _Entity::_Timing(__id, *__pm.first, *this);
        timing().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Cell& _createCell(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Cell* __p_v = new (__r.cellAlloc().allocate(1))
            typename _Entity::_Cell(__id, *__pm.first, *this);
        cell().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Cell& _createCell(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Cell* __p_v = new (__r.cellAlloc().allocate(1))
            typename _Entity::_Cell(__id, *__pm.first, *this);
        cell().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Hdl& _createHdl(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Hdl* __p_v = new (__r.hdlAlloc().allocate(1))
            typename _Entity::_Hdl(__id, *__pm.first, *this);
        hdl().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Hdl& _createHdl(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Hdl* __p_v = new (__r.hdlAlloc().allocate(1))
            typename _Entity::_Hdl(__id, *__pm.first, *this);
        hdl().insert(__p_v->pair());
        return *__p_v;
    }
    
    typename _Entity::_ViewGroup& _createViewGroup(const utl::Id_t __id, 
        const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_ViewGroup* __p_v = new (__r.viewGroupAlloc().allocate(1))
            typename _Entity::_ViewGroup(__id, *__pm.first, *this);
        viewGroup().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_ViewGroup& _createViewGroup(const utl::Id_t __id, 
        const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_ViewGroup* __p_v = new (__r.viewGroupAlloc().allocate(1))
            typename _Entity::_ViewGroup(__id, *__pm.first, *this);
        viewGroup().insert(__p_v->pair());
        return *__p_v;
    }
    
    typename _Entity::_Entity* _clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const {
        typename _Entity::_ViewManager& __vm = __c.viewManager();
        for (typename _Entity::_ViewConstItr __vi = blackbox().begin(); 
            __vi != blackbox().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewConstItr __vi = physical().begin(); 
            __vi != physical().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewConstItr __vi = netlist().begin(); 
            __vi != netlist().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewConstItr __vi = timing().begin(); 
            __vi != timing().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewConstItr __vi = cell().begin(); 
            __vi != cell().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewConstItr __vi = hdl().begin(); 
            __vi != hdl().end(); __vi++) {
            typename _Entity::_View& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        for (typename _Entity::_ViewGroupConstItr __vi = viewGroup().begin(); 
            __vi != viewGroup().end(); __vi++) {
            typename _Entity::_ViewGroup& __v = *((*__vi).second);
            __v.clone(__c, __r);
        }
        return &__vm;
    }
    typename _Entity::_Blackbox* defaultBlackbox() {
        return blackbox().empty() ? 
            0: dynamic_cast<typename _Entity::_Blackbox*>(blackbox().begin()->second); 
    }
    typename _Entity::_Physical* defaultPhysical() {
        return physical().empty() ? 
            0: dynamic_cast<typename _Entity::_Physical*>(physical().begin()->second); 
    }
    typename _Entity::_Netlist* defaultNetlist() { 
        return netlist().empty() ? 
            0: dynamic_cast<typename _Entity::_Netlist*>(netlist().begin()->second); 
    }
    typename _Entity::_Timing* defaultTiming() { 
        return timing().empty() ? 
            0: dynamic_cast<typename _Entity::_Timing*>(timing().begin()->second); 
    }
    typename _Entity::_Cell* defaultCell() { 
        return cell().empty() ? 
            0: dynamic_cast<typename _Entity::_Cell*>(cell().begin()->second); 
    }
    typename _Entity::_Hdl* defaultHdl() { 
        return hdl().empty() ? 
            0: dynamic_cast<typename _Entity::_Hdl*>(hdl().begin()->second); 
    }
    typename _Entity::_ViewGroup* defaultViewGroup() { 
        return viewGroup().empty() ? 
            0: dynamic_cast<typename _Entity::_ViewGroup*>(viewGroup().begin()->second); 
    }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_ViewManager; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        blackbox(__c.blackbox());
        physical(__c.physical());
        netlist(__c.netlist());
        timing(__c.timing());
        cell(__c.cell());
        hdl(__c.hdl());
        viewGroup(__c.viewGroup());
        parent(const_cast<typename _Entity::_Component*>(__c.parent()));
        return *this;
    }
    void _replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        const typename _Entity::_Reference& __ci = __i;
        const typename _Entity::_ViewGroup* __p_vg = __ci.viewOf();
        typename _Entity::_ViewGroup* __p_vg_new = findViewGroup(__p_vg->name());
        __p_vg_new->replace(__i, __r);
    }

private: // data
    typename _Entity::_ViewSet        __m_blackbox;
    typename _Entity::_ViewSet        __m_physical;
    typename _Entity::_ViewSet        __m_netlist;
    typename _Entity::_ViewSet        __m_timing;
    typename _Entity::_ViewSet        __m_cell;
    typename _Entity::_ViewSet        __m_hdl;
    typename _Entity::_ViewGroupSet   __m_viewGroup;
    typename _Entity::_Component*     __m_parent;
};

using ViewManager = BasicViewManager<char, std::char_traits<char>,
    std::allocator<char> >;
using WViewManager = BasicViewManager<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicViewManager<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewManager<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewManager<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicViewManager<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define VIEWMANAGER_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## ViewManager) rack::BasicViewManager<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

VIEWMANAGER_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __VIEWMANAGER_H__
