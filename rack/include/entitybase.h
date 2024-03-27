#ifndef __ENTITYBASE_H__
#define __ENTITYBASE_H__
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

using Flags = utl::Mask;
using _Mask = utl::Mask;
using _Flags = Flags;

// View masks
const utl::Mask mNetlist		= 0x0001;
const utl::Mask mPhysical		= 0x0002;
const utl::Mask mTiming			= 0x0004;
const utl::Mask mHdl			= 0x0008;
const utl::Mask mBlackbox		= 0x000f;
const utl::Mask mCell			= 0x0010;
const utl::Mask mView 			= mNetlist|mPhysical|mTiming|mHdl|mBlackbox|mCell;

// Signal direction masks
const utl::Mask mOutput			= 0x0001;
const utl::Mask mInput			= 0x0002;
const utl::Mask mInout			= 0x0004;	/*(mOutput|mInput);*/
const utl::Mask mSigDir			= mOutput|mInput|mInout;

// Enumerations
enum eView { vBad=0, vNetlist, vPhysical, vTiming, vHdl, vBlackbox, vCell };

/* Don't noncopyable Mutex semantics mean that a class with a mutex 
 * member will be noncopyable as well? No, but what it does mean is 
 * that the compiler can't generate a copy constructor and assignment 
 * operator, so they will have to be coded explicitly. This is a good 
 * thing, however, since the compiler generated operations would not 
 * be thread-safe. The following is a simple example of a class with 
 * copyable semantics and internal synchronization through a mutex 
 * member. -- [Ref: Boost thread library docs]
 */
template <class _CharT, class _Traits, class _Alloc>
class BasicEntityBase {
public: // RACK using
    using _Str  = typename std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = typename utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _NT = typename rack::BasicClassNameTag<_CharT, _Traits, _Alloc>;
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _NamedEntity = typename rack::BasicNamedEntityBase<_CharT, _Traits, _Alloc>;
    using _UnnamedEntity = typename rack::BasicUnnamedEntityBase<_CharT, _Traits, _Alloc>;
    using _Interconnect = typename rack::BasicInterconnectBase<_CharT, _Traits, _Alloc>;
    using _Connector = typename rack::BasicConnectorBase<_CharT, _Traits, _Alloc>;
    using _View = typename rack::BasicViewBase<_CharT, _Traits, _Alloc>;
    using _ViewGroup = typename rack::BasicViewGroup<_CharT, _Traits, _Alloc>;
    using _Message = typename utl::BasicMessage<_CharT, _Traits, _Alloc>;
    using _Exception = typename utl::BasicExceptionBase<_CharT, _Traits, _Alloc>;
    using _Exceptions = typename utl::BasicExceptions<_CharT, _Traits, _Alloc>;
    using _Dictionary = typename utl::BasicDictionary<_CharT, _Traits, _Alloc>;
    using _Technology = typename rack::BasicTechnology<_CharT, _Traits, _Alloc>;
    using _Library = typename rack::BasicLibrary<_CharT, _Traits, _Alloc>;
    using _LibraryBase = typename rack::BasicLibraryBase<_CharT, _Traits, _Alloc>;
    using _Design = typename rack::BasicDesign<_CharT, _Traits, _Alloc>;
    using _Hdl = typename rack::BasicHdl<_CharT, _Traits, _Alloc>;
    using _Blackbox = typename rack::BasicBlackbox<_CharT, _Traits, _Alloc>;
    using _Physical = typename rack::BasicPhysical<_CharT, _Traits, _Alloc>;
    using _Netlist = typename rack::BasicNetlist<_CharT, _Traits, _Alloc>;
    using _Timing = typename rack::BasicTiming<_CharT, _Traits, _Alloc>;
    using _Cell = typename rack::BasicCell<_CharT, _Traits, _Alloc>;
    using _Rack = typename rack::BasicRack<_CharT, _Traits, _Alloc>;
    using _Reference = typename rack::BasicReferenceBase<_CharT, _Traits, _Alloc>;
    using _Component = typename rack::BasicComponentBase<_CharT, _Traits, _Alloc>;
    using _Device = typename rack::BasicDeviceBase<_CharT, _Traits, _Alloc>;
    using _DiodeBase = typename rack::BasicDiodeBase<_CharT, _Traits, _Alloc>;
    using _Diode = typename rack::BasicDiode<_CharT, _Traits, _Alloc>;
    using _InstanceBase = typename rack::BasicInstanceBase<_CharT, _Traits, _Alloc>;
    using _Instance = typename rack::BasicInstance<_CharT, _Traits, _Alloc>;
    using _LinkpinBase = typename rack::BasicLinkpinBase<_CharT, _Traits, _Alloc>;
    using _Linkpin = typename rack::BasicLinkpin<_CharT, _Traits, _Alloc>;
    using _Locator = typename rack::BasicLocator<_CharT, _Traits, _Alloc>;
    using _ViewManager = typename rack::BasicViewManager<_CharT, _Traits, _Alloc>;
    using _ModuleBase = typename rack::BasicModuleBase<_CharT, _Traits, _Alloc>;
    using _Module = typename rack::BasicModule<_CharT, _Traits, _Alloc>;
    using _NetBase = typename rack::BasicNetBase<_CharT, _Traits, _Alloc>;
    using _Net = typename rack::BasicNet<_CharT, _Traits, _Alloc>;
    using _Nonroute = typename rack::BasicNonroute<_CharT, _Traits, _Alloc>;
    using _PinBase = typename rack::BasicPinBase<_CharT, _Traits, _Alloc>;
    using _Pin = typename rack::BasicPin<_CharT, _Traits, _Alloc>;
    using _PortBase = typename rack::BasicPortBase<_CharT, _Traits, _Alloc>;
    using _Port = typename rack::BasicPort<_CharT, _Traits, _Alloc>;
    using _RouteBase = typename rack::BasicRouteBase<_CharT, _Traits, _Alloc>;
    using _Route = typename rack::BasicRoute<_CharT, _Traits, _Alloc>;
    using _SegmentBase = typename rack::BasicSegmentBase<_CharT, _Traits, _Alloc>;
    using _Segment = typename rack::BasicSegment<_CharT, _Traits, _Alloc>;
    using _SteinerBase = typename rack::BasicSteinerBase<_CharT, _Traits, _Alloc>;
    using _Steiner = typename rack::BasicSteiner<_CharT, _Traits, _Alloc>;
    using _TieBase = typename rack::BasicTieBase<_CharT, _Traits, _Alloc>;
    using _Tie = typename rack::BasicTie<_CharT, _Traits, _Alloc>;
    using _TransistorBase = typename rack::BasicTransistorBase<_CharT, _Traits, _Alloc>;
    using _Transistor = typename rack::BasicTransistor<_CharT, _Traits, _Alloc>;
    using _ViaBase = typename rack::BasicViaBase<_CharT, _Traits, _Alloc>;
    using _Via = typename rack::BasicVia<_CharT, _Traits, _Alloc>;

public: // MAP using
    using _TechnologyMap = typename std::map<_Mstr, _Technology*, std::less<_Mstr>, _Alloc>;
    using _LibraryMap = typename std::map<_Mstr, _Library*, std::less<_Mstr>, _Alloc>;
    using _DesignMap = typename std::map<_Mstr, _Design*, std::less<_Mstr>, _Alloc>;
    using _HdlMap = typename std::map<_Mstr, _Hdl*, std::less<_Mstr>, _Alloc>;
    using _TechnologyHashMap = typename std::unordered_map<_Mstr, _Technology,
        std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc>;
    using _LibraryHashMap = typename std::unordered_map<_Mstr, _Library,
        std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc>;
    using _DesignHashMap = typename std::unordered_map<_Mstr, _Design,
        std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc>;
    using _HdlHashMap = typename std::unordered_map<_Mstr, _Hdl,
        std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc>;
    using _ConnectorMap = typename std::map<_Mstr, _Connector*, std::less<_Mstr>, _Alloc>;
    using _ConnectorMultiMap = typename std::multimap<_Mstr, _Connector*, std::less<_Mstr>, _Alloc>;
    using _ModuleMap = typename std::map<_Mstr, _Module*, std::less<_Mstr>, _Alloc>;
    using _ModuleHashMap = typename std::unordered_map<_Mstr, _Module, std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc >;
    using _NetMap = typename std::map<_Mstr, _Net*, std::less<_Mstr>, _Alloc>;
    using _InstanceMap = typename std::map<_Mstr, _Instance*, std::less<_Mstr>, _Alloc>;
    using _ViewMap = typename std::map<_Mstr, _View*, std::less<_Mstr>, _Alloc>;
    using _ViewGroupMap = typename std::map<_Mstr, _ViewGroup*, std::less<_Mstr>, _Alloc>;

public: // SET using
    using _TechnologySet = _TechnologyMap;
    using _LibrarySet = _LibraryMap;
    using _DesignSet = _DesignMap;
    using _HdlSet = _HdlMap;
    using _ConnectorSet = _ConnectorMap;
    using _ConnectorMultiSet = _ConnectorMultiMap;
    using _ModuleSet = _ModuleMap;
    using _NetSet = _NetMap;
    using _InstanceSet = _InstanceMap;
    using _ViewSet = _ViewMap;
    using _ViewGroupSet = _ViewGroupMap;
    
public: // REBIND using
    // This using problem was solved using:
    // http://womble.decadentplace.org.uk/c++/template-faq.html
    using _DictAlloc = typename _Alloc::template rebind<_Dictionary>::other;
    using _TechAlloc = typename _Alloc::template rebind<_Technology>::other;
    using _LibAlloc = typename _Alloc::template rebind<_Library>::other;
    using _DgnAlloc = typename _Alloc::template rebind<_Design>::other;
    using _HdlAlloc = typename _Alloc::template rebind<_Hdl>::other;
    using _BlackboxAlloc = typename _Alloc::template rebind<_Blackbox>::other;
    using _PhysicalAlloc = typename _Alloc::template rebind<_Physical>::other;
    using _NetlistAlloc = typename _Alloc::template rebind<_Netlist>::other;
    using _TimingAlloc = typename _Alloc::template rebind<_Timing>::other;
    using _CellAlloc = typename _Alloc::template rebind<_Cell>::other;
    using _ModAlloc = typename _Alloc::template rebind<_Module>::other;
    using _PortAlloc = typename _Alloc::template rebind<_Port>::other;
    using _NetAlloc = typename _Alloc::template rebind<_Net>::other;
    using _InstAlloc = typename _Alloc::template rebind<_Instance>::other;
    using _PinAlloc = typename _Alloc::template rebind<_Pin>::other;
    using _ViewGroupAlloc = typename _Alloc::template rebind<_ViewGroup>::other;
    
public: // ITERATOR using
    using _DictItr = typename _Dictionary::iterator;
    using _DictConstItr = typename _Dictionary::const_iterator;
    using _TechnologyItr = typename _TechnologySet::iterator;
    using _TechnologyConstItr = typename _TechnologySet::const_iterator;
    using _LibraryItr = typename _LibrarySet::iterator;
    using _LibraryConstItr = typename _LibrarySet::const_iterator;
    using _DesignItr = typename _DesignSet::iterator;
    using _DesignConstItr = typename _DesignSet::const_iterator;
    using _HdlItr = typename _HdlSet::iterator;
    using _HdlConstItr = typename _HdlSet::const_iterator;
    using _ModuleItr = typename _ModuleSet::iterator;
    using _ModuleConstItr = typename _ModuleSet::const_iterator;
    using _ConnectorItr = typename _ConnectorSet::iterator;
    using _ConnectorConstItr = typename _ConnectorSet::const_iterator;
    using _ConnectorMultiItr = typename _ConnectorMultiSet::iterator;
    using _ConnectorMultiConstItr = typename _ConnectorMultiSet::const_iterator;
    using _ViewItr = typename _ViewSet::iterator;
    using _ViewConstItr = typename _ViewSet::const_iterator;
    using _ViewGroupItr = typename _ViewGroupSet::iterator;
    using _ViewGroupConstItr = typename _ViewGroupSet::const_iterator;
    using _InstanceItr = typename _InstanceSet::iterator;
    using _InstanceConstItr = typename _InstanceSet::const_iterator;
    using _NetItr = typename _NetSet::iterator;
    using _NetConstItr = typename _NetSet::const_iterator;
    
public: // Iterator pairs
    using _ConnectorMultiItrPair = typename std::pair<typename _Entity::_ConnectorMultiItr,
            typename _Entity::_ConnectorMultiItr>;
    using _ConnectorMultiConstItrPair = typename std::pair<typename _Entity::_ConnectorMultiConstItr,
            typename _Entity::_ConnectorMultiConstItr>;

public: // RACK using
    using _Self = _Entity;

public: // constructors/destructors.
    BasicEntityBase() : __m_flags(utl::mFalse) {}
    virtual ~BasicEntityBase() {}

public: // Copy constructors.
    BasicEntityBase(const _Self& __c) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
    virtual const utl::Id_t id() const = 0;
    const _Flags flags() const { return __m_flags; }
    const bool test(const _Mask __m) const { return (flags()&__m).any(); }

public: // Utility.
    // This accessor is commented out since is will not be used
    // outside this class hierarchy. 
    // const std::mutex& mutex() const { return __m_mutex; }
    virtual void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(_nameTag(), id());
        __d.beginData("Id");
        __d.data(id());
        __d.endData("Id");
        __d.endTag(_nameTag());
    }
    virtual void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    virtual void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
    }
    virtual void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    virtual const _Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

    // Navigation accessors.
    const _Self* navNext(_Rack&) const { return 0; }
    const _Self* navCurr(_Rack&) const { return this; }
    const _Self* navPrev(_Rack&) const { return 0; }
    const _Self* navLeft(_Rack&) const { return 0; }
    const _Self* navRight(_Rack&) const { return 0; }
    const _Self* navParent(_Rack&) const { return 0; }
    _Self* navNext(_Rack&) { return 0; }
    _Self* navCurr(_Rack&) { return this; }
    _Self* navPrev(_Rack&) { return 0; }
    _Self* navLeft(_Rack&) { return 0; }
    _Self* navRight(_Rack&) { return 0; }
    _Self* navParent(_Rack&) { return 0; }
    void navNext(_Self*, _Rack&) {}
    void navCurr(_Self*, _Rack&) {}
    void navPrev(_Self*, _Rack&) {}
    void navLeft(_Self*, _Rack&) {}
    void navRight(_Self*, _Rack&) {}
    void navParent(_Self*, _Rack&) {}
    // Test functions
    void forward(_Self*, _Rack&) { std::cout << "forward" << std::endl; }
    void backward(_Self*, _Rack&) { std::cout << "backward" << std::endl; }
    void stopat1(_Self*, _Rack&) { std::cout << "stopat1" << std::endl; }
    void stopat2(_Self*, int, _Rack&) { std::cout << "stopat2" << std::endl; }

protected: // functions
    // This accessor has non const return type, even though the 
    // function itself is a constant. This is because the of the
    // requirement of at the point of use.
    std::mutex& mutex() const { return __m_mutex; }
    _Flags flags() { return __m_flags; }
    //bool test(const _Mask __m) { return (flags()&__m).any(); }
    virtual void id(const utl::Id_t __id) = 0;
    void flags(const _Flags __v) { __m_flags = __v; }

protected: // functions
    const _Str& _nameTag() const { return _NT::__m_EntityBase; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        flags(__c.flags());
        return *this;
    }

protected: // data
    mutable std::mutex    __m_mutex;
    _Flags            __m_flags;
};

using EntityBase = BasicEntityBase<char, std::char_traits<char>,
    std::allocator<char> >;
using ConnectorMultiItr = BasicEntityBase<char, std::char_traits<char>,
    std::allocator<char> >::_ConnectorMultiItr;
using ConnectorMultiConstItr = BasicEntityBase<char, std::char_traits<char>,
    std::allocator<char> >::_ConnectorMultiConstItr;
using ConnectorMultiItrPair = BasicEntityBase<char, std::char_traits<char>,
    std::allocator<char> >::_ConnectorMultiItrPair;

using WEntityBase = BasicEntityBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;
using WConnectorMultiItr = BasicEntityBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >::_ConnectorMultiItr;
using WConnectorMultiConstItr = BasicEntityBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >::_ConnectorMultiConstItr;
using WConnectorMultiConstItrPair = BasicEntityBase<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >::_ConnectorMultiConstItrPair;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicEntityBase<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicEntityBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicEntityBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicEntityBase<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

//////////////////////////////////////////////////////////////////////
// Preconditions : None.
// Postconditions: Deleted pointer in collection.
//////////////////////////////////////////////////////////////////////
template<typename __Tp1, typename __Tp2>
struct DeletePair {
    void operator()( typename std::pair<__Tp1, __Tp2>& p ) const { delete p.second; }
};

template<typename __Tp1, typename __Tp2>
struct DeleteContents {
    void operator()( typename std::pair<__Tp1, __Tp2>& p ) const { p.second->purgeContents(); }
};

#define _INITNAME(__name) __m_name(__name)
#define _INITNAMEPTR(__name) __m_name(&__name)
#define _INITID(__id) __m_id(__id)

// Assignment of name needs to be worked out.
#define _ASSIGNNAME(__name) __m_name = __name
#define _ASSIGNNAMEPTR(__name) __m_name = &__name
#define _ASSIGNID(__id) __m_id = __id

#define _ADDname()  \
    public: virtual const typename _Entity::_Mstr& name() const { return __m_name; }  \
    private: typename _Entity::_Mstr __m_name

#define _ADDnamePtr()  \
    public: virtual const typename _Entity::_Mstr& name() const { return *__m_name; }  \
    private: const typename _Entity::_Mstr* __m_name; \
    public: void rename(const typename _Entity::_Mstr& __name) { _ASSIGNNAMEPTR(__name); }

#define _ADDID()  \
    public: virtual const utl::Id_t id() const { return __m_id; }         \
    protected: virtual void id(const utl::Id_t __id) { __m_id = __id; }    \
    private: utl::Id_t __m_id

}

#ifdef SWIG
%define ENTITY_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template() rack::BasicEntityBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

ENTITY_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __ENTITYBASE_H__
