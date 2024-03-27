#ifndef __DUMP_H__
#define __DUMP_H__
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

namespace utl {

////////////////////////////////////////////////////////////////////////////////
// Class to dump data
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDumpBase {
public: // Using
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Ostr = std::basic_ostream<_CharT, _Traits>;

public: // Constructors/descructors
    explicit BasicDumpBase(_Ostr& o) 
        : m_o(o), m_i(0), m_t(cDefaultTab), 
        m_d(std::numeric_limits<int>::max()), m_s(false) {}
    BasicDumpBase(_Ostr& o, 
        const _Str& style, const _Str& schem) 
        : m_o(o), m_i(0), m_t(cDefaultTab), 
        m_d(std::numeric_limits<int>::max()), m_s(true),
        m_style(style), m_schema(schem) {}
    BasicDumpBase(_Ostr& o, 
        const _Str& style, const _Str& schem, 
        const int i, const int t) 
        : m_o(o), m_i(i), m_t(t), 
        m_d(std::numeric_limits<int>::max()), m_s(true),
        m_style(style), m_schema(schem) {}
    virtual ~BasicDumpBase() {}

public: // Accessors.
    _Ostr& oStream() const { return m_o; }
    const int indent() const { return m_i; }
    void indent(const int i) { m_i = i; }
    const int tab() const { return m_t; }
    void tab(const int t) { m_t = t; }
    const int depth() const { return m_d; }
    void depth(const int d) { m_d = d; }
    const bool schemaref() const { return m_s; }
    void schemaref(const bool s) { m_s = s; }
    const _Str& style() const { return m_style; }
    _Str& style() { return m_style; }
    const _Str& schema() const { return m_schema; }
    _Str& schema() { return m_schema; }

public: // Utilities.
    _Str pad() { return Indent(indent()); }
    void tabUp() { indent(indent() + cDefaultTab); }
    void tabDn() { indent(indent() - cDefaultTab); }
    const bool backtrack() { 
        return depth() < indent()/cDefaultTab ? true : false; 
    }
    void newline() {
        oStream() << std::endl;
    }

public: // print

private: // data
    _Ostr&          m_o;
    int             m_i;
    int             m_t;
    int             m_d;
    bool            m_s;
    _Str            m_style;
    _Str            m_schema;
};

////////////////////////////////////////////////////////////////////////////////
// Class to dump Xml
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDumpXml
    : public BasicDumpBase<_CharT, _Traits, _Alloc> {
public: // Using
    using _Base = BasicDumpBase<_CharT, _Traits, _Alloc>;
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Ostr = std::basic_ostream<_CharT, _Traits>;

public: // Constructors/descructors
    explicit BasicDumpXml(_Ostr& o) 
        : _Base(o) {}
    BasicDumpXml(_Ostr& o, 
        const _Str& style, const _Str& schem) 
        : _Base(o, style, schem) {}
    BasicDumpXml(_Ostr& o, 
        const _Str& style, const _Str& schem, 
        const int i, const int t) 
        : _Base(o, i, t, style, schem) {}
    virtual ~BasicDumpXml() {}

public: // print.
    void dumpSizes() {
#if 0 // TBD: Will fix after all other errors are fixed
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<Sizes>" << std::endl;
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<Name>string</Name><Size>" << sizeof(string) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>multistring</Name><Size>" << sizeof(multistring) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Message</Name><Size>" << sizeof(Message) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Exception</Name><Size>" << sizeof(Exception) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Exceptions</Name><Size>" << sizeof(Exceptions) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>DumpXml</Name><Size>" << sizeof(DumpXml) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Dictionary</Name><Size>" << sizeof(Dictionary) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>EntityBase</Name><Size>" << sizeof(EntityBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>UnnamedEntityBase</Name><Size>" << sizeof(UnnamedEntityBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>NamedEntityBase</Name><Size>" << sizeof(NamedEntityBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ViewBase</Name><Size>" << sizeof(ViewBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ViewGroup</Name><Size>" << sizeof(ViewGroup) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ConnectorBase</Name><Size>" << sizeof(ConnectorBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>PortBase</Name><Size>" << sizeof(PortBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Port</Name><Size>" << sizeof(Port) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>InterconnectBase</Name><Size>" << sizeof(InterconnectBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ComponentBase</Name><Size>" << sizeof(ComponentBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ReferenceBase</Name><Size>" << sizeof(ReferenceBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>PinBase</Name><Size>" << sizeof(PinBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Pin</Name><Size>" << sizeof(Pin) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>NetBase</Name><Size>" << sizeof(NetBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Net</Name><Size>" << sizeof(Net) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>InstanceBase</Name><Size>" << sizeof(InstanceBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Instance</Name><Size>" << sizeof(Instance) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Blackbox</Name><Size>" << sizeof(Blackbox) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Physical</Name><Size>" << sizeof(Physical) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Netlist</Name><Size>" << sizeof(Netlist) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Timing</Name><Size>" << sizeof(Timing) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Cell</Name><Size>" << sizeof(Cell) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Hdl</Name><Size>" << sizeof(Hdl) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ViewManager</Name><Size>" << sizeof(ViewManager) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>ModuleBase</Name><Size>" << sizeof(ModuleBase) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Module</Name><Size>" << sizeof(Module) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Technology</Name><Size>" << sizeof(Technology) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Library</Name><Size>" << sizeof(Library) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Design</Name><Size>" << sizeof(Design) << "</Size>" << std::endl;
        _Base::oStream() << _Base::pad() << "<Name>Rack</Name><Size>" << sizeof(Rack) << "</Size>" << std::endl;
        _Base::tabDn();
        _Base::oStream() << _Base::pad() << "</Sizes>" << std::endl;
        _Base::tabDn();
#endif
    }
    void data(const int i) { 
        _Base::oStream() << i;
    }
    void data(const _Str& s) { 
        _Base::oStream() << "\"" << s << "\"";
    }
    void data(const _Mstr& ms) { 
        _Base::oStream()  << "\""<< ms << "\"";
    }
    void data(const _Str& tag, const int i) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << tag << ">" << i 
            << "</" << tag << ">" << std::endl;
        _Base::tabDn();
    }
    void data(const _Str& tag, const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << tag << ">\"" << s 
            << "\"</" << tag << ">" << std::endl;
        _Base::tabDn();
    }
    void data(const _Str& tag, const _Mstr& ms) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << tag << ">\"" << ms 
            << "\"</" << tag << ">" << std::endl;
        _Base::tabDn();
    }
    void header() { 
        _Base::oStream() << "<?xml version=\"1.0\"?>" << std::endl; 
    }
    void stylesheet() { 
        _Base::oStream() << "<?xml-stylesheet type=\"text/xsl\" " << std::endl
            << _Base::pad() << "href=\"" << _Base::style() << "\" ?>" << std::endl; 
    }
    void pointer(const _Str& s, const Id_t p) {
        _Base::oStream() << _Base::pad() << "<" << s << " xlink:href=\"#" 
            << s << "_" << p << "\"/>" << std::endl;
    }
    void beginTag(const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << s ;
        if (_Base::schemaref() == true) {
            _Base::oStream() << std::endl << _Base::pad() << "xmlns:xsi=" 
                << "\"http://www.w3.org/2001/XMLSchema-instance\" "
                << std::endl << _Base::pad() 
                << "xmlns:xlink=\"http://www.w3.org/1999/xlink\""
                << std::endl << _Base::pad() << "xsi:noNamespaceSchemaLocation=\""
                << _Base::schema() << "\"";
            _Base::schemaref(false);
        }
        _Base::oStream() << ">" << std::endl;
    }
    void beginTag(const _Str& s, const Id_t id) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << s;
        _Base::oStream() << " " << "ID=\"" << s << "_" << id << "\"";
        if (_Base::schemaref() == true) {
            _Base::oStream() << std::endl << _Base::pad() << "xmlns:xsi=" 
                << "\"http://www.w3.org/2001/XMLSchema-instance\" "
                << std::endl << _Base::pad() 
                << "xmlns:xlink=\"http://www.w3.org/1999/xlink\""
                << std::endl << _Base::pad() << "xsi:noNamespaceSchemaLocation=\""
                << _Base::schema() << "\"";
            _Base::schemaref(false);
        }
        _Base::oStream() << ">" << std::endl;
    }
    void endTag(const _Str& s) { 
        _Base::oStream() << _Base::pad() << "</" << s << ">" << std::endl;
        _Base::tabDn();
    }
    void beginData(const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << s << ">";
    }
    void beginData(const _Mstr& ms) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << ms << ">";
    }
    void endData(const _Str& s) { 
        _Base::oStream() << "</" << s << ">" << std::endl;
        _Base::tabDn();
    }
    void beginInputs() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<input>";
    }
    void endInputs() { 
        _Base::oStream() << "</input>" << std::endl;
        _Base::tabDn();
    }
    void beginOutputs() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<output>";
    }
    void endOutputs() { 
        _Base::oStream() << "</output>" << std::endl;
        _Base::tabDn();
    }
    void beginInouts() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<inout>";
    }
    void endInouts() { 
        _Base::oStream() << "</inout>" << std::endl;
        _Base::tabDn();
    }
};

////////////////////////////////////////////////////////////////////////////////
// Class to dump Xsd
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDumpXsd
    : public BasicDumpBase<_CharT, _Traits, _Alloc> {
public: // Using
    using _Base = BasicDumpBase<_CharT, _Traits, _Alloc>;
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Ostr = std::basic_ostream<_CharT, _Traits>;

public: // Constructors/descructors
    explicit BasicDumpXsd(_Ostr& o) 
        : _Base(o) {}
    BasicDumpXsd(_Ostr& o, 
        const _Str& style, const _Str& schem) 
        : _Base(o, style, schem) {}
    BasicDumpXsd(_Ostr& o, 
        const _Str& style, const _Str& schem, 
        const int i, const int t) 
        : _Base(o, i, t, style, schem) {}
    virtual ~BasicDumpXsd() {}

public: // print.
    void data(const int i) { 
        _Base::oStream() << i;
    }
    void data(const _Str& s) { 
        _Base::oStream() << s;
    }
    void data(const _Mstr& ms) { 
        _Base::oStream() << ms;
    }
    void header() { 
        _Base::oStream() << "<?xml version=\"1.0\" "
            << "encoding=\"UTF-8\"?>" << std::endl; 
        _Base::oStream() << "<xs:schema xmlns:xs="
            << "\"http://www.w3.org/2001/XMLSchema\" "
            << "elementstyleDefault=\"qualified\" "
            << "attributestyleDefault=\"unqualified\">" << std::endl;
    }
    void beginTag(const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() 
            << "<xs:element name=\"" << s << "\">" << std::endl;
    }
    void endTag(const _Str& s) { 
        _Base::oStream() << _Base::pad() << "</" << s << ">" << std::endl;
        _Base::tabDn();
    }
    void beginData(const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << s << ">";
    }
    void beginData(const _Mstr& ms) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "<" << ms << ">";
    }
    void endData(const _Str& s) { 
        _Base::oStream() << "</" << s << ">" << std::endl;
        _Base::tabDn();
    }
};

////////////////////////////////////////////////////////////////////////////////
// Class to dump Verilog
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDumpVerilog
    : public BasicDumpBase<_CharT, _Traits, _Alloc> {
public: // Using
    using _Base = BasicDumpBase<_CharT, _Traits, _Alloc>;
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Ostr = std::basic_ostream<_CharT, _Traits>;

public: // Constructors/descructors
    explicit BasicDumpVerilog(_Ostr& o) 
        : _Base(o) {}
    BasicDumpVerilog(_Ostr& o, 
        const _Str& style, const _Str& schem) 
        : _Base(o, style, schem) {}
    BasicDumpVerilog(_Ostr& o, 
        const _Str& style, const _Str& schem, 
        const int i, const int t) 
        : _Base(o, i, t, style, schem) {}
    virtual ~BasicDumpVerilog() {}

public: // print.
    void data(const int i) { 
        _Base::oStream() << i;
    }
    void data(const _Str& s) { 
        _Base::oStream() << s;
    }
    void data(const _Mstr& ms) { 
        _Base::oStream() << ms;
    }
    void beginModule(const _Str& s) { 
        _Base::oStream() << _Base::pad() << "module " << s;
    }
    void beginModule(const _Mstr& ms) { 
        _Base::oStream() << "module " << ms;
    }
    void endModule() { 
        _Base::oStream() << "endmodule" << std::endl << std::endl;
    }
    void beginPorts() { 
        _Base::oStream() << "(";
    }
    void endPorts() { 
        _Base::oStream() << ");" << std::endl;
    }
    void beginPort(const _Str& s) { 
        _Base::oStream() << s;
    }
    void beginPort(const _Mstr& ms) { 
        _Base::oStream() << ms;
    }
    void endPort() { 
    }
    void beginInputs() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "input ";
    }
    void endInputs() { 
        _Base::oStream() << ";" << std::endl;
        _Base::tabDn();
    }
    void beginOutputs() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "output ";
    }
    void endOutputs() { 
        _Base::oStream() << ";" << std::endl;
        _Base::tabDn();
    }
    void beginInouts() { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "inout ";
    }
    void endInouts() { 
        _Base::oStream() << ";" << std::endl;
        _Base::tabDn();
    }
    void beginInstance(const _Str& s1, const _Str& s2) { 
        _Base::oStream() << _Base::pad() << s1 << " "  << s2 << "(";
    }
    void beginInstance(const _Mstr& ms1, const _Mstr& ms2) { 
        _Base::oStream() << _Base::pad() << ms1 << " "  << ms2 << "(";
    }
    void endInstance() { 
        _Base::oStream() << ");" << std::endl;
    }
    void beginPin(const _Str& s) { 
        _Base::oStream() << "." << s << "(";
    }
    void beginPin(const _Mstr& ms) { 
        _Base::oStream() << "." << ms << "(";
    }
    void endPin() { 
        _Base::oStream() << ")";
    }
    void beginAssign(const _Str& s1, const _Str& s2) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "assign " << s1 << " = " << s2;
    }
    void beginAssign(const _Mstr& ms1, const _Mstr& ms2) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "assign " << ms1 << " = " << ms2;
    }
    void endAssign() { 
        _Base::oStream() << ";" << std::endl;
        _Base::tabDn();
    }
    void beginWire(const _Str& s) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "wire " << s;
    }
    void beginWire(const _Mstr& ms) { 
        _Base::tabUp();
        _Base::oStream() << _Base::pad() << "wire " << ms;
    }
    void endWire() { 
        _Base::oStream() << ";" << std::endl;
        _Base::tabDn();
    }
};

////////////////////////////////////////////////////////////////////////////////
// Class to dump Json
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDumpJson
    : public BasicDumpBase<_CharT, _Traits, _Alloc> {
public: // Using
    using _Base = BasicDumpBase<_CharT, _Traits, _Alloc>;
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _Ostr = std::basic_ostream<_CharT, _Traits>;

public: // Constructors/descructors
    explicit BasicDumpJson(_Ostr& o) 
        : _Base(o) {}
    BasicDumpJson(_Ostr& o, 
        const _Str& style, const _Str& schem) 
        : _Base(o, style, schem) {}
    BasicDumpJson(_Ostr& o, 
        const _Str& style, const _Str& schem, 
        const int i, const int t) 
        : _Base(o, i, t, style, schem) {}
    virtual ~BasicDumpJson() {}

public: // print.
    void data(const _Str& tag, const int i) { 
        root()[_nametag][tag].append(i);
    }
    void data(const _Str& tag, const _Str& s) { 
        root()[_nametag][tag].append(s);
    }
    void data(const _Str& tag, const _Mstr& ms) {
        std::stringstream ss;
        ss << ms;
        root()[_nametag][tag].append(ss.str());
    }
    void setId(const _Str& nametag, const Id_t id) {
        _nametag = nametag;
        std::stringstream ss;
        ss << id;

        root()[_nametag]["ID"] = _nametag + "_" + ss.str();
    }
    _Str str() {
        Json::StreamWriterBuilder builder;
        return Json::writeString(builder, root());
    }
    void dump() {
        _Base::oStream() << std::move(str());
    }
    bool equal(Json::Value& data) {
        return root() == data;
    }
    
public:
    Json::Value& root() { return _root; }

private:
    Json::Value _root;
    std::string _nametag;
};

////////////////////////////////////////////////////////////////////////////////
// Template specialization
using DumpBase = BasicDumpBase<char, std::char_traits<char>, std::allocator<char> >;
using WDumpBase = BasicDumpBase<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;
using DumpXml = BasicDumpXml<char, std::char_traits<char>, std::allocator<char> >;
using WDumpXML = BasicDumpXml<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;
using DumpXsd = BasicDumpXsd<char, std::char_traits<char>, std::allocator<char> >;
using WDumpXSD = BasicDumpXsd<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;
using DumpVerilog = BasicDumpVerilog<char, std::char_traits<char>, std::allocator<char> >;
using WDumpVerilog = BasicDumpVerilog<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;
using DumpJson = BasicDumpJson<char, std::char_traits<char>, std::allocator<char> >;
using WDumpJson = BasicDumpJson<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;

////////////////////////////////////////////////////////////////////////////////
// std::string handling
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXml<_CharT, _Traits, _Alloc>& d, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s) 
{
    d.data("String", s);
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXsd<_CharT, _Traits, _Alloc>& d, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s) 
{
    d.data(s);
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s) 
{
    d.oStream() << s;
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpJson<_CharT, _Traits, _Alloc>& d, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s) 
{
    d.data("String", s);
}

////////////////////////////////////////////////////////////////////////////////
// utl::multistring handling
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXml<_CharT, _Traits, _Alloc>& d, 
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    d.beginData("Multistring");
    d.oStream() << "\"";
    for (size_t i = 0; i < ms.size(); i++) {
        d.oStream() << ms[i];
    }
    d.oStream() << "\"";
    d.endData("Multistring");
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXsd<_CharT, _Traits, _Alloc>& d, 
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    d.beginData("Multistring");
    d.oStream() << "\"";
    for (size_t i = 0; i < ms.size(); i++) {
        d.oStream() << ms[i];
    }
    d.oStream() << "\"";
    d.endData("Multistring");
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    for (size_t i = 0; i < ms.size(); i++) {
        d.oStream() << ms[i];
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpJson<_CharT, _Traits, _Alloc>& d, 
    const basic_multistring<_CharT, _Traits, _Alloc>& ms) 
{
    d.data("Multistring", ms);
}

////////////////////////////////////////////////////////////////////////////////
// std::map handling
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXml<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    for (_Itr itr = map.begin(); itr != map.end(); itr++) {
        (*itr).second->dump(d);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXsd<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    for (_Itr itr = map.begin(); itr != map.end(); itr++) {
        (*itr).second->dump(d);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    for (_Itr itr = map.begin(); itr != map.end(); itr++) {
        (*itr).second->dump(d);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpJson<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    for (_Itr itr = map.begin(); itr != map.end(); itr++) {
        (*itr).second->dump(d);
    }
}

////////////////////////////////////////////////////////////////////////////////
// std::map with std::ostream operator
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline std::basic_ostream<_CharT, _Traits>& operator<<(
    std::basic_ostream<_CharT, _Traits>& o, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    for (_Itr itr = map.begin(); itr != map.end(); itr++) {
        o << *(*itr).second;
    }

    return o;
}

////////////////////////////////////////////////////////////////////////////////
// std::map with std::string handling for seperators
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dumpSep(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    _Itr itr = map.begin();
    (*itr).second->dump(d);
    if (itr != map.end()) itr++;
    for (; itr != map.end(); itr++) {
        d.data(s);
        (*itr).second->dump(d);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dumpSepFilter(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const std::map<_Key, _Tp, _Compare, _Alloc>& map, 
    const std::basic_string<_CharT, _Traits, _Alloc>& s,
    const Mask m) {
    using _Map = typename std::map<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Map::const_iterator;
    _Itr itr = map.begin();
    for (; itr != map.end(); itr++) {
        if ((*itr).second->test(m)) {
            (*itr).second->dump(d);
            break;
        }
    }
    if (itr != map.end()) itr++;
    for (; itr != map.end(); itr++) {
        if ((*itr).second->test(m)) {
            d.data(s);
            (*itr).second->dump(d);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// std::unordered_set handling
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXml<_CharT, _Traits, _Alloc>& d, 
    const std::unordered_set<_Key, _Tp, _Compare, _Alloc>& set) {
    using _Set = typename std::unordered_set<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Set::const_iterator;
    for (_Itr itr = set.begin(); itr != set.end(); itr++) {
        dump<_CharT, _Traits, _Key, _Tp, _Compare, _Alloc>(d, *itr);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpXsd<_CharT, _Traits, _Alloc>& d, 
    const std::unordered_set<_Key, _Tp, _Compare, _Alloc>& set) {
    using _Set = typename std::unordered_set<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Set::const_iterator;
    for (_Itr itr = set.begin(); itr != set.end(); itr++) {
        dump<_CharT, _Traits, _Key, _Tp, _Compare, _Alloc>(d, *itr);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d, 
    const std::unordered_set<_Key, _Tp, _Compare, _Alloc>& set) {
    using _Set = typename std::unordered_set<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Set::const_iterator;
    for (_Itr itr = set.begin(); itr != set.end(); itr++) {
        dump<_CharT, _Traits, _Key, _Tp, _Compare, _Alloc>(d, *itr);
    }
}

template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline void dump(BasicDumpJson<_CharT, _Traits, _Alloc>& d, 
    const std::unordered_set<_Key, _Tp, _Compare, _Alloc>& set) {
    using _Set = typename std::unordered_set<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Set::const_iterator;
    for (_Itr itr = set.begin(); itr != set.end(); itr++) {
        dump<_CharT, _Traits, _Key, _Tp, _Compare, _Alloc>(d, *itr);
    }
}

////////////////////////////////////////////////////////////////////////////////
// std::unordered_set with std::ostream operator
template <typename _CharT, typename _Traits, 
    typename _Key, typename _Tp, typename _Compare, typename _Alloc>
inline std::basic_ostream<_CharT, _Traits>& operator<<(
    std::basic_ostream<_CharT, _Traits>& o, 
    const std::unordered_set<_Key, _Tp, _Compare, _Alloc>& set) {
    using _Set = typename std::unordered_set<_Key, _Tp, _Compare, _Alloc>;
    using _Itr = typename _Set::const_iterator;
    for (_Itr itr = set.begin(); itr != set.end(); itr++) {
        o << *itr << std::endl;
    }

    return o;
}

}

#ifdef SWIG
namespace utl {
    %naturalvar DumpBase;
    %naturalvar DumpXml;
    %naturalvar DumpXsd;
    %naturalvar DumpVerilog;
    %naturalvar DumpJson;
}

%define DUMP_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(/*prefix ## type ## DumpBase*/) utl::BasicDumpBase<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(/*prefix ## type ## DumpXml*/) utl::BasicDumpXml<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(/*prefix ## type ## DumpXsd*/) utl::BasicDumpXsd<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(/*prefix ## type ## DumpVerilog*/) utl::BasicDumpVerilog<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%template(/*prefix ## type ## DumpJson*/) utl::BasicDumpJson<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

DUMP_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __DUMP_H__
