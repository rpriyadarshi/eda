#ifndef __MODULEBASE_H__
#define __MODULEBASE_H__
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
class BasicModuleBase 
    : public BasicComponentBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_Component;
    using _Self = typename _Entity::_ModuleBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: // constructors/destructors.
    // Warning C4355: If those other constructors access any members or 
    // call member functions on this, the result will be undefined. You 
    // should using the this pointer until all construction has completed.
    // -- The use here is an exception, and the pointer must never be 
    // accessed within the constructor.
    BasicModuleBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name, typename _Entity::_LibraryBase* __p) 
        : _Base(__id, __name, __p) {}
    virtual ~BasicModuleBase() { purge(); }

public: // Copy constructors.
    BasicModuleBase(const _Self& __c)
        : _Base(__c),
        __m_ports(__c.__m_ports), 
        __m_viewManager(__c.__m_viewManager) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_ConnectorSet& ports() const { return __m_ports; }
    const typename _Entity::_ViewManager& viewManager() const { return __m_viewManager; }

public: // creation.
    typename _Entity::_Port& createPort(const typename _Entity::_Str& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
        return _createPort(__name, __sd, __r);
    }
    typename _Entity::_Port& createPort(const typename _Entity::_Mstr& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
        return _createPort(__name, __sd, __r);
    }
    void createPins(typename _Entity::_Reference& __inst, typename _Entity::_Rack& __r) {
        for (typename _Entity::_ConnectorConstItr __itr = ports().begin(); __itr != ports().end(); __itr++) {
            __inst.createPin(*__itr->second, __r);
        }
    }

public: // Search
    typename _Entity::_Connector* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }
    typename _Entity::_Connector* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }

public: // Utility.
    const bool hasIOType(const utl::Mask __m) const {
        for (typename _Entity::_ConnectorConstItr __itr = ports().begin(); __itr != ports().end(); __itr++) {
            if (__itr->second->test(__m)) {
                return true;
            }
        }
        return false;
    }
    const bool hasInputs() const {
        return hasIOType(mInput);
    }
    const bool hasOutputs() const {
        return hasIOType(mOutput);
    }
    const bool hasInouts() const {
        return hasIOType(mInout);
    }
    typename _Entity::_Entity* clone(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) const {
        return _clone(__name, __r);  
    }

    _Pair pair() { return _Pair(_Base::name(), this); }
    void purge() {
        std::for_each(ports().begin(), ports().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Connector*>());
    }
    void purgeContents() {
        viewManager().purgeContents();
    }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        viewManager().dump(__d);

        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
            std::less<typename _Entity::_Mstr>, _Alloc>(__d, ports());

        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        __d.beginPorts();
        utl::dumpSepFilter<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
            std::less<typename _Entity::_Mstr>, _Alloc>(__d, ports(), ", ", utl::mTrue);
        __d.endPorts();
        if (hasInputs()) {
            __d.beginInputs();
            utl::dumpSepFilter<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
                std::less<typename _Entity::_Mstr>, _Alloc>(__d, ports(), ", ", mInput);
            __d.endInputs();
        }
        if (hasOutputs()) {
            __d.beginOutputs();
            utl::dumpSepFilter<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
                std::less<typename _Entity::_Mstr>, _Alloc>(__d, ports(), ", ", mOutput);
            __d.endOutputs();
        }
        if (hasInouts()) {
            __d.beginInouts();
            utl::dumpSepFilter<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Connector*, 
                std::less<typename _Entity::_Mstr>, _Alloc>(__d, ports(), ", ", mInout);
            __d.endInouts();
        }
        __d.newline();
        viewManager().dump(__d);
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    _Self& assign(const _Self& __c) { return _assign(__c); }

public: // Netlist modifications
    void connect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        _connect(__pname, __p_n, __r);
    }
    void disconnect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        _disconnect(__pname, __p_n, __r);
    }
    void erase(typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        _erase(__p_n, __r);
    }
    void replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        _replace(__i, __r);
    }

    // Accessors. Temporarily made public
    typename _Entity::_ViewManager& viewManager() { return __m_viewManager; }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }

private: // functions
    // Accessors.
    typename _Entity::_ConnectorSet& ports() { return __m_ports; }

    // Search
    typename _Entity::_Connector* _findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__name);
        return _findPort(__ms, __r);
    }
    typename _Entity::_Connector* _findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ConnectorItr __itr = ports().find(__name);
        if (__itr == ports().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00001", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00001", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        return __itr->second;
    }

    // creation.
    typename _Entity::_Port& _createPort(const typename _Entity::_Str& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Port* __p_v = new (__r.portAlloc().allocate(1))
            typename _Entity::_Port(__r.getid(), *__pm.first, __sd, *this);
        ports().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Port& _createPort(const typename _Entity::_Mstr& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
        std::pair<typename _Entity::_DictItr, bool> __ps = __r.dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = __r.dictionary().insert(*__ps.first); 
        typename _Entity::_Port* __p_v = new (__r.portAlloc().allocate(1))
            typename _Entity::_Port(__r.getid(), *__pm.first, __sd, *this);
        ports().insert(__p_v->pair());
        return *__p_v;
    }
    typename _Entity::_Entity* _clone(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) const {
        typename _Entity::_LibraryBase* __l = const_cast<typename _Entity::_LibraryBase*>(_Base::parent());
        typename _Entity::_Module& __m = __l->createModule(__name, __r); /* TBD: Generate new name */
        for (typename _Entity::_ConnectorConstItr __ci = ports().begin(); 
            __ci != ports().end(); __ci++) {
            typename _Entity::_Connector& __p = *((*__ci).second);
            __p.clone(__m, __r);
        }
        const typename _Entity::_ViewManager& __vm = viewManager();
        __vm.clone(__m, __r);

        return &__m;
    }

public: // Netlist modifications
    void _connect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        typename _Entity::_Connector& __p_v = *findPort(__pname, __r);
        __p_v.connect(__p_n, __r);
    }
    void _disconnect(const _CharT* __pname, typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        typename _Entity::_Connector& __p_v = *findPort(__pname, __r);
        __p_v.disconnect(__p_n, __r);
    }
    void _erase(typename _Entity::_Interconnect& __p_n, typename _Entity::_Rack& __r) {
        typename _Entity::_ViewManager& __vm = viewManager();
        __vm.erase(__p_n, __r);
    }
    void _replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
        typename _Entity::_ViewManager& __vm = viewManager();
        __vm.replace(__i, __r);
    }

    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_ModuleBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        ports() = __c.ports();
        viewManager() = __c.viewManager();
        return *this;
    }

private: // data
    typename _Entity::_ConnectorSet   __m_ports;
    typename _Entity::_ViewManager    __m_viewManager;
};

using ModuleBase = BasicModuleBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WModuleBase = BasicModuleBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicModuleBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicModuleBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicModuleBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicModuleBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define MODULEBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicModuleBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

MODULEBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __MODULEBASE_H__
