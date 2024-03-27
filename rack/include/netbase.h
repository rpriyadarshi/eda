#ifndef __NETBASE_H__
#define __NETBASE_H__
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
class BasicNetBase 
    : public BasicInterconnectBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_Interconnect;
    using _Self = typename _Entity::_NetBase;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: // constructors/destructors.
    BasicNetBase(const utl::Id_t __id, const typename _Entity::_Mstr& __name) 
        : _Base(__id, __name) {}
    virtual ~BasicNetBase() {}

public: // Copy constructors.
    BasicNetBase(const _Self& __c)
        : _Base(__c), __m_ports(__c.__m_ports), 
        __m_pins(__c.__m_pins) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_ConnectorSet& ports() const { return __m_ports; }
    const typename _Entity::_ConnectorMultiSet& pins() const { return __m_pins; }

public: // Search
    typename _Entity::_ConnectorMultiItrPair findPins(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findPins(__name, __r);
    }
    typename _Entity::_ConnectorMultiItrPair findPins(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findPins(__name, __r);
    }
    typename _Entity::_Connector* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }
    typename _Entity::_Connector* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        return _findPort(__name, __r);
    }

public: // Utility.
    typename _Entity::_Entity* clone(typename _Entity::_View& __v, typename _Entity::_Rack& __r) const { return _clone(__v, __r); }
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        for (typename _Entity::_ConnectorConstItr __itr = ports().begin(); __itr != ports().end(); __itr++) {
            // pointer print
            __d.pointer(__itr->second->nameTag(), __itr->second->id());
        }
        for (typename _Entity::_ConnectorMultiConstItr __itr = pins().begin(); __itr != pins().end(); __itr++) {
            __d.pointer(__itr->second->nameTag(), __itr->second->id());
        }
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        typename _Entity::_Connector* __p_c = 0;
        if (!ports().empty()) {
            for (typename _Entity::_ConnectorConstItr __itr = ports().begin(); __itr != ports().end(); __itr++) {
                if (__itr->second->test(mInput|mOutput|mInout) &&
                    __itr->second->name() != _Base::name()) {
                    __p_c = __itr->second;
                    break;
                }
            }
        }
        if (__p_c) {
            __d.beginWire(_Base::name());
            __d.endWire();
            if (__p_c->test(mInput|mInout)) {
                __d.beginAssign(_Base::name(), __p_c->name());
            }
            else { // swap for output.
                __d.beginAssign(__p_c->name(), _Base::name());
            }
            __d.endAssign();
        }
        else if (ports().empty()) {
            __d.beginWire(_Base::name());
            __d.endWire();
        }
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

    // Netlist Manipulations
    void connectPort(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  {
        ports().insert(__p_v.pair());
        __p_v.interconnect(this);
    }
    void connectPin(typename _Entity::_Connector& __p_v, typename _Entity::_Rack& __r)  {
        pins().insert(__p_v.pair());
        __p_v.interconnect(this);
    }
    void disconnectPort(typename _Entity::_Connector& __c, typename _Entity::_Rack& __r)  {
        std::pair<typename _Entity::_ConnectorItr, typename _Entity::_ConnectorItr> __p = 
            ports().equal_range(__c.name());
        typename _Entity::_ConnectorItr& __first = __p.first;
        typename _Entity::_ConnectorItr& __second = __p.second;
        while (__first != __second) {
            const typename _Entity::_Connector& __cr = *((*__first).second);
            if (&__cr == &__c) {
                ports().erase(__first);
                break; /* Stale iterator */
            }
            __first++; /* Untested increment */
        }
        //ports().erase(__c.name());
        __c.interconnect(0);
    }
    void disconnectPin(typename _Entity::_Connector& __c, typename _Entity::_Rack& __r)  {
        std::pair<typename _Entity::_ConnectorMultiItr, typename _Entity::_ConnectorMultiItr> __p =
            pins().equal_range(__c.name());
        typename _Entity::_ConnectorMultiItr& __first = __p.first;
        typename _Entity::_ConnectorMultiItr& __second = __p.second;
        while (__first != __second) {
            const typename _Entity::_Connector& __cr = *((*__first).second);
            if (&__cr == &__c) {
                pins().erase(__first);
                break; /* Stale iterator */
            }
            __first++; /* Untested increment */
        }
        //pins().erase(__c.name());
        __c.interconnect(0);
    }
    // Hands over the ownership of pins to the supplied net.
    void movePins(typename _Entity::_Interconnect& __n, typename _Entity::_Rack& __r)  {
        for(typename _Entity::_ConnectorMultiItr __itr = pins().begin();
            __itr != pins().end(); __itr++) {
            typename _Entity::_Connector* __p = __itr->second;
            if (!__p) {
                // throw exception
                assert(0);
            }
            __p->connect(__n, __r);
            //pins().erase(__itr);
            // Alternate is a clear()
        }
        // Need to check if the performance of the other method is better.
        pins().clear();
    }

private: // functions
    // Accessors.
    typename _Entity::_ConnectorSet& ports() { return __m_ports; }
    typename _Entity::_ConnectorMultiSet& pins() { return __m_pins; }

private: // Search
    typename _Entity::_ConnectorMultiItrPair _findPins(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__name);
        return _findPins(__ms, __r);
    }
    typename _Entity::_ConnectorMultiItrPair _findPins(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ConnectorMultiItrPair __pair = pins().equal_range(__name);
        if (__pair.first == __pair.second) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00004", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
#if 0
        if (__itr == pins().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00004", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00004", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
#endif
        return __pair;
    }
    typename _Entity::_Connector* _findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
        const typename _Entity::_Mstr& __ms = *__r.dictionary().find(__name);
        return _findPort(__ms, __r);
    }
    typename _Entity::_Connector* _findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
        typename _Entity::_ConnectorItr __itr = ports().find(__name);
        if (__itr == ports().end()) {
            // throw error
            __r.exceptions().message().init("E_%m%m_00003", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        if (!__itr->second) {
            // throw error
            __r.exceptions().message().init("F_%m%m_00003", &__name, &_Base::name());
            throw __r.exceptions().exception();
        }
        return __itr->second;
    }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_NetBase; }
    typename _Entity::_Entity* _clone(typename _Entity::_View& __v, typename _Entity::_Rack& __r) const {
        typename _Entity::_Netlist& __nl = *dynamic_cast<typename _Entity::_Netlist*>(&__v);
        typename _Entity::_Net& __n = __nl.createNet(_Base::name(), __r);
        for (typename _Entity::_ConnectorConstItr __ci = ports().begin(); 
            __ci != ports().end(); __ci++) {
            typename _Entity::_Connector& __c = *((*__ci).second);
            typename _Entity::_Connector* __p_c = __nl.findPort(__c.name(), __r);
            if (!__p_c) {
                // TBD: Bad database
            }
            __p_c->connect(__n, __r);
        }
        for (typename _Entity::_ConnectorMultiConstItr __ci = pins().begin();
            __ci != pins().end(); __ci++) {
            typename _Entity::_Connector& __c = *((*__ci).second);
            typename _Entity::_InstanceBase* __o = dynamic_cast<typename _Entity::_InstanceBase*>(__c.owner());
            typename _Entity::_InstanceBase* __p_o = __nl.findInstance(__o->name(), __r);
            if (!__p_o) {
                // TBD: Bad database: Could not find instance that should be there
            }
            typename _Entity::_Connector* __p_c = __p_o->findPin(__c.name(), __r);
            if (!__p_c) {
                // TBD: Bad database
            }
            __p_c->connect(__n, __r);
        }

        return &__n;
    }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        ports() = __c.ports();
        pins() = __c.pins();
        return *this;
    }

private: // data
    typename _Entity::_ConnectorSet           __m_ports;
    typename _Entity::_ConnectorMultiSet    __m_pins;
};

using NetBase = BasicNetBase<char, std::char_traits<char>,
    std::allocator<char> >;
using WNetBase = BasicNetBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicNetBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicNetBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define NETBASE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicNetBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

NETBASE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __NETBASE_H__
