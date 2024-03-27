#ifndef __NETLIST_H__
#define __NETLIST_H__
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
class BasicRack; 

template <class _CharT, class _Traits, class _Alloc>
class BasicNetlist 
    : public BasicViewBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_View;
    using _Self = typename _Entity::_Netlist;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: // functions
    // constructors/destructors.
    BasicNetlist(const utl::Id_t __id, const typename _Entity::_Mstr& __name, typename _Entity::_ViewManager& __parent) 
        : _Base(__id, __name, __parent) {}
    virtual ~BasicNetlist() { purge(); }

public: // Copy constructors.
    BasicNetlist(const _Self& __c)
        : _Base(__c), __m_nets(__c.__m_nets), 
        __m_instances(__c.__m_instances) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_NetSet& nets() const { return __m_nets; }
    const typename _Entity::_InstanceSet& instances() const { return __m_instances; }

public: // creation.
    typename _Entity::_Net& createNet(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _createNet(__name, __r);
    }
    typename _Entity::_Net& createNet(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _createNet(__name, __r);
    }
//    typename _Entity::_Instance& createInstance(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
//        return _createInstance(__name, __r);
//    }
    typename _Entity::_Instance& createInstance(const typename _Entity::_Str& __name,
        typename _Entity::_ViewGroup& __parent, typename _Entity::_ViewGroup& __viewOf, 
        typename _Entity::_Rack& __r) {
        return _createInstance(__name, __parent, __viewOf, __r);
    }
    typename _Entity::_Instance& createInstance(const typename _Entity::_Mstr& __name, 
        typename _Entity::_ViewGroup& __parent, typename _Entity::_ViewGroup& __viewOf, 
        typename _Entity::_Rack& __r) {
        return _createInstance(__name, __parent, __viewOf, __r);
    }

public: // Search
#ifdef SWIG
    // https://stackoverflow.com/questions/8847357/rename-and-inline-in-swig-error-checking
    %rename(findPortBase) findPort;
    %rename(findInstanceBase) findInstance;
    %rename(findNetBase) findNet;
#endif // SWIG
    typename _Entity::_Connector* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }
    typename _Entity::_Connector* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }
    typename _Entity::_InstanceBase* findInstance(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findInstance(__name, __r);
    }
    typename _Entity::_InstanceBase* findInstance(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findInstance(__name, __r);
    }
    typename _Entity::_NetBase* findNet(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findNet(__name, __r);
    }
    typename _Entity::_NetBase* findNet(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findNet(__name, __r);
    }
#ifdef SWIG
    %rename(findPort) swig_findPort;
    %rename(findInstance) swig_findInstance;
    %rename(findNet) swig_findNet;
    %extend {
        typename _Entity::_Port* swig_findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
            return self->CastToPort(self->findPort(__name, __r));
        }
        typename _Entity::_Port* swig_findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
            return self->CastToPort(self->findPort(__name, __r));
        }
        typename _Entity::_Instance* swig_findInstance(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
            return self->CastToInstance(self->findInstance(__name, __r));
        }
        typename _Entity::_Instance* swig_findInstance(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
            return self->CastToInstance(self->findInstance(__name, __r));
        }
        typename _Entity::_Net* swig_findNet(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
            return self->CastToNet(self->findNet(__name, __r));
        }
        typename _Entity::_Net* swig_findNet(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
            return self->CastToNet(self->findNet(__name, __r));
        }
    };
#endif // SWIG
public: // Utility.
    typename _Entity::_Entity* clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const { 
        return _clone(__c, __r);  
    }
    typename _Entity::_Entity* clone(const typename _Entity::_Mstr& __ms, typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const { 
        return _clone(__ms, __c, __r);  
    }
    _Pair pair() { return _Pair(_Base::name(), this); }
    void purge() {
    }
    void purgeContents() {
        std::for_each(nets().begin(), nets().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Net*>());
        std::for_each(instances().begin(), instances().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Instance*>());
    }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);

        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Net*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, nets());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Instance*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, instances());

        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Net*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, nets());
        __d.newline();
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Instance*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, instances());
        __d.newline();
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }
    void dissolve(const typename _Entity::_Str& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        _dissolve(__path, __p, __r);
    }
    void dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        _dissolve(__path, __p, __r);
    }
    void erase(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _erase(__i, __r);
    }

    typename _Entity::_Port* CastToPort(typename _Entity::_Connector* c) {
    	return static_cast<typename _Entity::_Port*>(c);
    }
    typename _Entity::_Instance* CastToInstance(typename _Entity::_InstanceBase* c) {
    	return static_cast<typename _Entity::_Instance*>(c);
    }
    typename _Entity::_Net* CastToNet(typename _Entity::_NetBase* c) {
    	return static_cast<typename _Entity::_Net*>(c);
    }

public: // Accessors.
    typename _Entity::_NetSet& nets() { return __m_nets; }
    typename _Entity::_InstanceSet& instances() { return __m_instances; }

private: // Search
    typename _Entity::_Connector* _findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ViewManager* __p_vm = _Base::parent();
        if (! __p_vm) {
            // TBD: Bad database
        }
        return __p_vm->findPort(__name, __r);
    }
    typename _Entity::_Connector* _findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ViewManager* __p_vm = _Base::parent();
        if (! __p_vm) {
            // TBD: Bad database
        }
        return __p_vm->findPort(__name, __r);
    }
    typename _Entity::_InstanceBase* _findInstance(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        return _findInstance(*__ps.first, __r);
    }
    typename _Entity::_InstanceBase* _findInstance(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
    	typename _Entity::_InstanceItr __itr = instances().find(__name);
        if (__itr == instances().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00005", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00005", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        return __itr->second;
    }
    typename _Entity::_NetBase* _findNet(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        return _findNet(*__ps.first, __r);
    }
    typename _Entity::_NetBase* _findNet(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_NetItr __itr = nets().find(__name);
        if (__itr == nets().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00004", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00004", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        return __itr->second;
    }

    typename _Entity::_Net& _createNet(const typename _Entity::_Str& __name,
        typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Net* __p_v = new (__r.netAlloc().allocate(1))
            typename _Entity::_Net(__r.getid(), *__pm.first);
        nets().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Net& _createNet(const typename _Entity::_Mstr& __name, 
        typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Net* __p_v = new (__r.netAlloc().allocate(1))
            typename _Entity::_Net(__r.getid(), *__pm.first);
        nets().insert(__p_v->pair());
        return *__p_v;
    }
//    typename _Entity::_Instance& _createInstance(const typename _Entity::_Str& __name,
//        typename _Entity::_Rack& __r) {
//        typename _Entity::_Instance* __p_v = __r.instAlloc().allocate(1);
//        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
//        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first);
//        construct(__p_v, typename _Entity::_Instance(__r.getid(), *__pm.first));
//        instances().insert(__p_v->pair());
//        return *__p_v;
//    }
    typename _Entity::_Instance& _createInstance(const typename _Entity::_Str& __name,
        typename _Entity::_ViewGroup& __parent, 
        typename _Entity::_ViewGroup& __viewOf, 
        typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Instance* __p_v = new (__r.instAlloc().allocate(1))
            typename _Entity::_Instance(__r.getid(), *__pm.first, __parent, __viewOf);
        instances().insert(__p_v->pair());
        __p_v->createPins(__r);
        return *__p_v;
    }
    typename _Entity::_Instance& _createInstance(const typename _Entity::_Mstr& __name, 
        typename _Entity::_ViewGroup& __parent, 
        typename _Entity::_ViewGroup& __viewOf, 
        typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Instance* __p_v = new (__r.instAlloc().allocate(1))
            typename _Entity::_Instance(__r.getid(), *__pm.first, __parent, __viewOf);
        instances().insert(__p_v->pair());
        __p_v->createPins(__r);
        return *__p_v;
    }

    typename _Entity::_Entity* _clone(typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const {
        return _clone(_Base::name(), __c, __r);
    }
    typename _Entity::_Entity* _clone(const typename _Entity::_Mstr& __ms, typename _Entity::_Component& __c, typename _Entity::_Rack& __r) const {
        typename _Entity::_ViewManager& __vm = __c.viewManager();
        typename _Entity::_Netlist& __nl = __vm.createNetlist(__ms, __r);
        for (typename _Entity::_InstanceConstItr __ii = instances().begin(); 
            __ii != instances().end(); __ii++) {
            typename _Entity::_Instance& __i = *((*__ii).second);
            __i.clone(__nl, __r);
        }
        for (typename _Entity::_NetConstItr __ni = nets().begin(); 
            __ni != nets().end(); __ni++) {
            typename _Entity::_Net& __n = *((*__ni).second);
            __n.clone(__nl, __r);
        }
        return &__nl;
    }
    void _dissolve(const typename _Entity::_Str& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__path);
        _dissolve(__ms, __p, __r);
    }
    void _dissolve(const typename _Entity::_Mstr& __path, typename _Entity::_ViewGroup& __p, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__path);
        const typename _Entity::_ViewGroup& __cp = __p;
        typename _Entity::_Netlist* __nl = const_cast<typename _Entity::_Netlist*>(__cp.netlist());
        for (typename _Entity::_NetConstItr __ni = nets().begin(); 
            __ni != nets().end(); __ni++) {
            typename _Entity::_Net& __n = *((*__ni).second);
            typename _Entity::_Mstr __newName = *__ps.first;
            __newName += __n.name();
            std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(__newName);
            __n.rename(*__pm.first);
            __nl->nets().insert(__n.pair());
        }
        for (typename _Entity::_InstanceConstItr __ii = instances().begin(); 
            __ii != instances().end(); __ii++) {
            typename _Entity::_Instance& __i = *((*__ii).second);
            typename _Entity::_Mstr __newName = *__ps.first;
            __newName += __i.name();
            std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(__newName);
            __i.rename(*__pm.first);
            __i.parent(&__p);
            __nl->instances().insert(__i.pair());
        }
        nets().clear();
        instances().clear();
    }
    void _erase(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_InstanceItr, typename _Entity::_InstanceItr> __p = 
            instances().equal_range(__i.name());
        typename _Entity::_InstanceItr& __first = __p.first;
        typename _Entity::_InstanceItr& __second = __p.second;
        while (__first != __second) {
            const typename _Entity::_Instance& __ir = *((*__first).second);
            if (&__ir == &__i) {
                instances().erase(__first);
                break; /* Stale iterator */
            }
            __first++; /* Untested increment */
        }
    }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_Netlist; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        nets() = __c.nets();
        instances() = __c.instances();
        return *this;
    }

private: // data
    typename _Entity::_NetSet         __m_nets;
    typename _Entity::_InstanceSet    __m_instances;
};

using Netlist = BasicNetlist<char, std::char_traits<char>,
    std::allocator<char> >;
using WNetlist = BasicNetlist<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicNetlist<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetlist<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetlist<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetlist<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define NETLIST_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## Netlist) rack::BasicNetlist<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

NETLIST_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __NETLIST_H__
