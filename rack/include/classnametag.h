#ifndef __CLASSNAMETAG_H__
#define __CLASSNAMETAG_H__
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
class BasicClassNameTag 
    : public utl::BasicClassNameTagBase<_CharT, _Traits, _Alloc> {
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;

public:
    explicit BasicClassNameTag() {}
    virtual ~BasicClassNameTag() {}

public: // data
    static _Str __m_Blackbox;
    static _Str __m_Cell;
    static _Str __m_ComponentBase;
    static _Str __m_ConnectorBase;
    static _Str __m_Design;
    static _Str __m_DeviceBase;
    static _Str __m_Dictionary;
    static _Str __m_Diode;
    static _Str __m_DiodeBase;
    static _Str __m_EntityBase;
    static _Str __m_Exceptions;
    static _Str __m_Hdl;
    static _Str __m_Instance;
    static _Str __m_InstanceBase;
    static _Str __m_InterconnectBase;
    static _Str __m_Library;
    static _Str __m_LibraryBase;
    static _Str __m_Linkpin;
    static _Str __m_LinkpinBase;
    static _Str __m_Locator;
    static _Str __m_Module;
    static _Str __m_ModuleBase;
    static _Str __m_NamedEntityBase;
    static _Str __m_Net;
    static _Str __m_NetBase;
    static _Str __m_Netlist;
    static _Str __m_Nonroute;
    static _Str __m_Physical;
    static _Str __m_Pin;
    static _Str __m_PinBase;
    static _Str __m_Port;
    static _Str __m_PortBase;
    static _Str __m_Rack;
    static _Str __m_ReferenceBase;
    static _Str __m_RouteBase;
    static _Str __m_Route;
    static _Str __m_Segment;
    static _Str __m_SegmentBase;
    static _Str __m_Steiner;
    static _Str __m_SteinerBase;
    static _Str __m_Technology;
    static _Str __m_Tie;
    static _Str __m_TieBase;
    static _Str __m_Timing;
    static _Str __m_Transistor;
    static _Str __m_TransistorBase;
    static _Str __m_UnnamedEntityBase;
    static _Str __m_Via;
    static _Str __m_ViaBase;
    static _Str __m_ViewBase;
    static _Str __m_ViewGroup;
    static _Str __m_ViewManager;
};

#ifndef SWIG
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Blackbox("Blackbox");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Cell("Cell");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ComponentBase("ComponentBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ConnectorBase("ConnectorBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Design("Design");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_DeviceBase("DeviceBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Dictionary("Dictionary");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Diode("Diode");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_DiodeBase("DiodeBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_EntityBase("EntityBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Exceptions("Exceptions");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Hdl("Hdl");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Instance("Instance");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_InstanceBase("InstanceBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_InterconnectBase("InterconnectBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Library("Library");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_LibraryBase("LibraryBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Linkpin("Linkpin");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_LinkpinBase("LinkpinBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Locator("Locator");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Module("Module");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ModuleBase("ModuleBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_NamedEntityBase("NamedEntityBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Net("Net");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_NetBase("NetBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Netlist("Netlist");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Nonroute("Nonroute");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Physical("Physical");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Pin("Pin");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_PinBase("PinBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Port("Port");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_PortBase("PortBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Rack("Rack");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ReferenceBase("ReferenceBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_RouteBase("RouteBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Route("Route");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Segment("Segment");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_SegmentBase("SegmentBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Steiner("Steiner");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_SteinerBase("SteinerBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Technology("Technology");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Tie("Tie");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_TieBase("TieBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Timing("Timing");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Transistor("Transistor");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_TransistorBase("TransistorBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_UnnamedEntityBase("UnnamedEntityBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_Via("Via");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ViaBase("ViaBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ViewBase("ViewBase");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ViewGroup("ViewGroup");
template <class _CharT, class _Traits, class _Alloc>
std::basic_string<_CharT, _Traits, _Alloc> 
BasicClassNameTag<_CharT, _Traits, _Alloc>::__m_ViewManager("ViewManager");
#endif // SWIG

using NT = BasicClassNameTag<char, std::char_traits<char>,
    std::allocator<char> >;
using WNT = BasicClassNameTag<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

}

#ifdef SWIG
%define RACK_CLASSNAMETAG_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## RackClassNameTag) rack::BasicClassNameTag<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

RACK_CLASSNAMETAG_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif /* __CLASSNAMETAG_H__ */
