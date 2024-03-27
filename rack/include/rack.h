#ifndef __RACK_H__
#define __RACK_H__
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
class BasicRack 
    : public BasicNamedEntityBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_NamedEntity;
    using _Self = typename _Entity::_Rack;

private: // Automated defs
    _ADDID();
    _ADDname();

public: // functions.
    // constructors/destructors.
    BasicRack(const utl::Id_t __id, const typename _Entity::_Mstr& __name) 
        : _INITID(__id), _INITNAME(__name), __m_idgen(0) {}
    explicit BasicRack(const typename _Entity::_Mstr& __name) 
        : _INITID(0), _INITNAME(__name), __m_idgen(0) {}
    virtual ~BasicRack() { purge(); }

private: // Copy constructors.
    BasicRack(const _Self& __c)
        : _Base(__c),
        __m_idgen(__c.__m_idgen),
        __m_dictionary(__c.__m_dictionary),
        __m_technologies(__c.__m_technologies),
        __m_libraries(__c.__m_libraries),
        __m_designs(__c.__m_designs),
        __m_hdls(__c.__m_hdls),
        __m_allocs(__c.__m_allocs),
        __m_exceptions(__c.__m_exceptions)/*,
        __m_threadManager(__c.__m_threadManager)*/ {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    const typename _Entity::_Dictionary& dictionary() const { return __m_dictionary; }
    const typename _Entity::_TechnologySet& technologies() const { return __m_technologies; }
    const typename _Entity::_LibrarySet& libraries() const { return __m_libraries; }
    const typename _Entity::_DesignSet& designs() const { return __m_designs; }
    const typename _Entity::_HdlSet& hdls() const { return __m_hdls; }
    const typename _Entity::_Exceptions& exceptions() const { return __m_exceptions; }
//    _ThreadGroup& threadManager() { return __m_threadManager; }
    // Non-const
    typename _Entity::_Dictionary& dictionary() { return __m_dictionary; }
    typename _Entity::_TechnologySet& technologies() { return __m_technologies; }
    typename _Entity::_LibrarySet& libraries() { return __m_libraries; }
    typename _Entity::_DesignSet& designs() { return __m_designs; }
    typename _Entity::_HdlSet& hdls() { return __m_hdls; }
    typename _Entity::_Exceptions& exceptions() { return __m_exceptions; }
//    _ThreadGroup& threadManager() const { return __m_threadManager; }

public: // creation.
    typename _Entity::_Technology& createTechnology(const typename _Entity::_Str& __name) {
        return _createTechnology(__name, techAlloc());
    }

    typename _Entity::_Library& createLibrary(const typename _Entity::_Str& __name) {
        return _createLibrary(__name, libAlloc());
    }

    typename _Entity::_Design& createDesign(const typename _Entity::_Str& __name) {
        return _createDesign(__name, dgnAlloc());
    }

    typename _Entity::_Hdl& createHdl(const typename _Entity::_Str& __name) {
        return _createHdl(__name, hdlAlloc());
    }

public: // Thread specific function.
//    _Thread& createThread(const boost::function0<void>& threadfunc) {
//        return _createThread(threadfunc);
//    }

public: // Utility.
    int getid() { return ++__m_idgen; };
    void purge() {
        std::for_each(technologies().begin(), technologies().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Technology*>());
        std::for_each(libraries().begin(), libraries().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Library*>());
        std::for_each(designs().begin(), designs().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Design*>());
        std::for_each(hdls().begin(), hdls().end(), DeletePair<const typename _Entity::_Mstr, typename _Entity::_Hdl*>());
    }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), id());
        _Base::dump(__d);

        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Technology*, 
            std::less<typename _Entity::_Mstr>, _Alloc> (__d, technologies());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Library*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, libraries());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Design*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, designs());
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Hdl*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, hdls());
        dictionary().dump(__d);
        exceptions().dump(__d);
        __d.dumpSizes();
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        utl::dump<_CharT, _Traits, typename _Entity::_Mstr, typename _Entity::_Design*, std::less<typename _Entity::_Mstr>, _Alloc>
            (__d, designs());
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    _Self& assign(const _Self& __c) { return _assign(__c); }
    const typename _Entity::_Technology* defaultTechnology() const {
        return technologies().empty() ?
            0 : dynamic_cast<const typename _Entity::_Technology*>(technologies().begin()->second);
    }
    const typename _Entity::_Library* defaultLibrary() const {
        return libraries().empty() ?
            0 : dynamic_cast<const typename _Entity::_Library*>(libraries().begin()->second);
    }
    const typename _Entity::_Design* defaultDesign() const {
        return designs().empty() ?
            0 : dynamic_cast<const typename _Entity::_Design*>(designs().begin()->second);
    }
    const typename _Entity::_Hdl* defaultHdl() const {
        return hdls().empty() ?
            0 : dynamic_cast<const typename _Entity::_Hdl*>(hdls().begin()->second);
    }

public: // Search
    typename _Entity::_Technology* findTechnology(const typename _Entity::_Str& __s) {
        const typename _Entity::_TechnologyItr& __itr = technologies().find(__s);
        return __itr == technologies().end() ?
                0 : dynamic_cast<typename _Entity::_Technology*>(__itr->second);
    }
    typename _Entity::_Library* findLibrary(const typename _Entity::_Str& __s) {
        const typename _Entity::_LibraryItr& __itr = libraries().find(__s);
        return __itr == libraries().end() ?
                0 : dynamic_cast<typename _Entity::_Library*>(__itr->second);
    }
    typename _Entity::_Design* findDesign(const typename _Entity::_Str& __s) {
        const typename _Entity::_DesignItr& __itr = designs().find(__s);
        return __itr == designs().end() ?
                0 : dynamic_cast<typename _Entity::_Design*>(__itr->second);
    }

    typename _Entity::_Hdl* findHdl(const typename _Entity::_Str& __s) {
        const typename _Entity::_HdlItr& __itr = hdls().find(__s);
        return __itr == hdls().end() ?
                0 : dynamic_cast<typename _Entity::_Hdl*>(__itr->second);
    }


public: // Thread specific functions.
//    void addThread(_Thread& __t) { _addThread(&__t); }
//    void removeThread(_Thread& __t) { _removeThread(&__t); }
//    void joinAllThreads() { _joinAllThreads(); }
//    const int sizeThread() const { return _sizeThread(); }

public: // allocators.
    typename _Entity::_TechAlloc& techAlloc() { return __m_allocs.__m_aTech; }
    typename _Entity::_LibAlloc& libAlloc() { return __m_allocs.__m_aLib; }
    typename _Entity::_DgnAlloc& dgnAlloc() { return __m_allocs.__m_aDgn; }
    typename _Entity::_HdlAlloc& hdlAlloc() { return __m_allocs.__m_aHdl; }
    typename _Entity::_BlackboxAlloc& blackboxAlloc() { return __m_allocs.__m_aBlackbox; }
    typename _Entity::_PhysicalAlloc& physicalAlloc() { return __m_allocs.__m_aPhysical; }
    typename _Entity::_NetlistAlloc& netlistAlloc() { return __m_allocs.__m_aNetlist; }
    typename _Entity::_TimingAlloc& timingAlloc() { return __m_allocs.__m_aTiming; }
    typename _Entity::_CellAlloc& cellAlloc() { return __m_allocs.__m_aCell; }
    typename _Entity::_ModAlloc& modAlloc() { return __m_allocs.__m_aMod; }
    typename _Entity::_PortAlloc& portAlloc() { return __m_allocs.__m_aPort; }
    typename _Entity::_NetAlloc& netAlloc() { return __m_allocs.__m_aNet; }
    typename _Entity::_InstAlloc& instAlloc() { return __m_allocs.__m_aInst; }
    typename _Entity::_PinAlloc& pinAlloc() { return __m_allocs.__m_aPin; }
    typename _Entity::_ViewGroupAlloc& viewGroupAlloc() { return __m_allocs.__m_aViewGroup; }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }

private: // functions.
    // raw creation.
    typename _Entity::_Technology& _createTechnology(const typename _Entity::_Str& __name,
        typename _Entity::_TechAlloc& __a) {
        std::pair<typename _Entity::_DictItr, bool> __ps = dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = dictionary().insert(*__ps.first); 
        typename _Entity::_Technology* __p_v = new (__a.allocate(1))
            typename _Entity::_Technology(getid(), *__pm.first);
        technologies().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Library& _createLibrary(const typename _Entity::_Str& __name,
        typename _Entity::_LibAlloc& __a) {
        std::pair<typename _Entity::_DictItr, bool> __ps = dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = dictionary().insert(*__ps.first); 
        typename _Entity::_Library* __p_v = new (__a.allocate(1))
            typename _Entity::_Library(getid(), *__pm.first);
        libraries().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Design& _createDesign(const typename _Entity::_Str& __name,
        typename _Entity::_DgnAlloc& __a) {
        std::pair<typename _Entity::_DictItr, bool> __ps = dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = dictionary().insert(*__ps.first); 
        typename _Entity::_Design* __p_v = new (__a.allocate(1))
            typename _Entity::_Design(getid(), *__pm.first);
        designs().insert(__p_v->pair());
        return *__p_v;
    }

    typename _Entity::_Hdl& _createHdl(const typename _Entity::_Str& __name,
        typename _Entity::_HdlAlloc& __a) {
        std::pair<typename _Entity::_DictItr, bool> __ps = dictionary().insert(__name);
        std::pair<typename _Entity::_DictItr, bool> __pm = dictionary().insert(*__ps.first); 
        typename _Entity::_Hdl* __p_v = new (__a.allocate(1))
            typename _Entity::_Hdl(getid(), *__pm.first);
        hdls().insert(__p_v->pair());
        return *__p_v;
    }

//    _Thread& _createThread(const boost::function0<void>& threadfunc) {
//        typename _Entity::_Thread* __t = threadManager().create_thread(threadfunc);
//        if (! __t) {
//            throw std::bad_alloc();
//        }
//        return *__t;
//    }

private: // Utility functions
    // Thread specific functions.
//    void _addThread(_Thread& __t) { threadManager().add_thread(&__t); }
//    void _removeThread(_Thread& __t) { threadManager().remove_thread(&__t); }
//    void _joinAllThreads() { threadManager().join_all(); }
//    const int _sizeThread() const { return threadManager().size(); }
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_Rack; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
//        idgen(__c.idgen());
        dictionary() = __c.dictionary();
        technologies() = __c.technologies();
        libraries() = __c.libraries();
        designs() = __c.designs();
        hdls() = __c.hdls();
//        allocs() = __c.allocs();
//        exceptions() = __c.exceptions();
//        threadManager() = __c.threadManager();
        return *this;
    }

private: // Data.
    utl::Id_t                    __m_idgen;
    typename _Entity::_Dictionary         __m_dictionary;
    typename _Entity::_TechnologySet      __m_technologies;
    typename _Entity::_LibrarySet         __m_libraries;
    typename _Entity::_DesignSet          __m_designs;
    typename _Entity::_HdlSet             __m_hdls;

    // Allocators. They consume space.
    // Size of each allocator is 1 byte.
    typedef struct {
        typename _Entity::_TechAlloc      __m_aTech;
        typename _Entity::_LibAlloc       __m_aLib;
        typename _Entity::_DgnAlloc       __m_aDgn;
        typename _Entity::_HdlAlloc       __m_aHdl;
        typename _Entity::_BlackboxAlloc  __m_aBlackbox;
        typename _Entity::_PhysicalAlloc  __m_aPhysical;
        typename _Entity::_NetlistAlloc   __m_aNetlist;
        typename _Entity::_TimingAlloc    __m_aTiming;
        typename _Entity::_CellAlloc      __m_aCell;
        typename _Entity::_ModAlloc       __m_aMod;
        typename _Entity::_PortAlloc      __m_aPort;
        typename _Entity::_NetAlloc       __m_aNet;
        typename _Entity::_InstAlloc      __m_aInst;
        typename _Entity::_PinAlloc       __m_aPin;
        typename _Entity::_ViewGroupAlloc __m_aViewGroup;
    } _Allocators;
    _Allocators         __m_allocs;

    // Error handling.
    typename _Entity::_Exceptions         __m_exceptions;

    // Multithreading support
//    _ThreadGroup        __m_threadManager;
};

using Rack = BasicRack<char, std::char_traits<char>,
    std::allocator<char> >;
using WRack = BasicRack<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicRack<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicRack<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicRack<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicRack<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define RACK_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## Rack) rack::BasicRack<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

RACK_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __RACK_H__
