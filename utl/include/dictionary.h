#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__
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
// This class is designed to reduce memory consumption due to 
// hierarchical strings. This hierarchy refers to hierarchy
// in the EDA designs. Due to flattening of a hierarchical
// design, there may be too much string replication at the higher
// levels in the hierarchy. In the current implementation, 
// hierarchical strings are composed of multistrings. 
// Each string in a multistring refers to a real string, 
// without actually storing it. That is why there are 2 hash sets
// being used. String hash set stores actual strings.
// Multistring hash set stores multistrings that themselves
// refers to strings in string hash set.
//
// This should give considerable gains in a flattened design that
// had a lot of hierarchy and had replicated names. In worst case
// if each name is unique, this approach may end up taking more 
// memory. It may be better to use strings rather than 
// multistrings in that case.
template <typename _CharT, typename _Traits, typename _Alloc>
class BasicDictionary {
public: // Using
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _NT = utl::BasicClassNameTagBase<_CharT, _Traits, _Alloc>;
    using _Self = utl::BasicDictionary<_CharT, _Traits, _Alloc>;
    using _SSet = std::unordered_set<_Str, std::hash<_Str>, std::equal_to<_Str>, _Alloc>;
    using _MSet = std::unordered_set<_Mstr, std::hash<_Mstr>, std::equal_to<_Mstr>, _Alloc> ;
    using _StrAlloc = typename _Alloc::template rebind<_Str>::other;
    using _value_type = typename _SSet::value_type;
    using _const_iterator = typename _SSet::const_iterator;
    using _iterator = typename _SSet::iterator;

public: // Using.
    using value_type = typename _MSet::value_type;
    using const_iterator = typename _MSet::const_iterator;
    using iterator = typename _MSet::iterator;
    
public: // functions.
    // constructors/destructors.
    BasicDictionary() : m_index(0){}
    virtual ~BasicDictionary() {}

public: // Copy constructors.
    BasicDictionary(const _Self& c)
        : m_sSet(c.m_sSet), m_mSet(c.m_mSet), m_index(0) {
    }

public: // Operators.
    const _Self& operator=(const _Self& c) { return _assign(c); }

public: // utility functions.
    std::pair<iterator, bool> insert(const _Str& obj) {
        std::pair<_iterator, bool> ps = sSet().insert(obj);
        const _Str& s = (*ps.first);
        _Mstr m(s);
        return mSet().insert(m);
    }

    std::pair<iterator, bool> insert(const _Mstr& obj) {
        _Mstr m;
        for (size_t i = 0; i < obj.size(); i++) {
            std::pair<_iterator, bool> ps = 
                sSet().insert(obj[i]);
            // need to test for return status.
            const _Str& s = (*ps.first);
            m.push_back(s);
        }
        return mSet().insert(m);
    }

    // TBD: Need to implement more efficient conversion.
    std::pair<iterator, bool> unique(const _Str& obj) {
        std::stringstream ss;
        ss << ++m_index << obj << std::ends;
        return (insert(ss.str()));
    }
    
    // TBD: Need to implement more efficient conversion.
    std::pair<iterator, bool> unique(const _Mstr& obj) {
        std::stringstream ss;
        ss << ++m_index << obj << std::ends;
        return (insert(ss.str()));
    }

    const_iterator find(const _Str& key) const {
        _Mstr m(key);
        return mSet().find(m);
    }

    const_iterator find(const _Mstr& key) const {
        return mSet().find(key);
    }

    const_iterator begin() const {
        return mSet().begin();
    }

    const_iterator cbegin() const {
        return mSet().cbegin();
    }

    const_iterator end() const {
        return mSet().end();
    }

    const_iterator cend() const {
        return mSet().cend();
    }

    _Str str() {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    _Str strXml() {
        std::stringstream ss;
        BasicDumpXml<_CharT, _Traits, _Alloc> xml(ss);
        dump(xml);
        return ss.str();
    }

    _Str strXsd() {
        std::stringstream ss;
        BasicDumpXsd<_CharT, _Traits, _Alloc> xsd(ss);
        dump(xsd);
        return ss.str();
    }

    _Str strVerilog() {
        std::stringstream ss;
        BasicDumpVerilog<_CharT, _Traits, _Alloc> verilog(ss);
        dump(verilog);
        return ss.str();
    }

    _Str strJson() {
        std::stringstream ss;
        BasicDumpJson<_CharT, _Traits, _Alloc> json(ss);
        dump(json);
        return ss.str();
    }

    void clear() {
        sSet().clear();
        mSet().clear();
    }

    void dump(BasicDumpXml<_CharT, _Traits, _Alloc>& d) const {
        d.beginTag(_nameTag(), 0/*id()*/);
        d.beginTag(_NT::m_String);
        utl::dump<_CharT, _Traits, _Str, std::hash<_Str>, 
            std::equal_to<_Str>, _Alloc>(d, sSet());
        d.endTag(_NT::m_String);
        d.beginTag(_NT::m_Multistring);
        utl::dump<_CharT, _Traits, _Mstr, std::hash<_Mstr>, 
            std::equal_to<_Mstr>, _Alloc>(d, mSet());
        d.endTag(_NT::m_Multistring);
        d.endTag(_nameTag());
    }
    void dump(BasicDumpXsd<_CharT, _Traits, _Alloc>& d) const {
        d.beginTag(_nameTag());
        d.beginTag(_NT::m_String);
        utl::dump<_CharT, _Traits, _Str, std::hash<_Str>, 
            std::equal_to<_Str>, _Alloc>(d, sSet());
        d.endTag(_NT::m_String);
        d.beginTag(_NT::m_Multistring);
        utl::dump<_CharT, _Traits, _Mstr, std::hash<_Mstr>, 
            std::equal_to<_Mstr>, _Alloc>(d, mSet());
        d.endTag(_NT::m_Multistring);
        d.endTag(_nameTag());
    }
    void dump(BasicDumpVerilog<_CharT, _Traits, _Alloc>& d) const {
    }
    void dump(BasicDumpJson<_CharT, _Traits, _Alloc>& d) const {
        d.setId(_nameTag(), 0);
        utl::dump<_CharT, _Traits, _Str, std::hash<_Str>, 
            std::equal_to<_Str>, _Alloc>(d, sSet());
        utl::dump<_CharT, _Traits, _Mstr, std::hash<_Mstr>, 
            std::equal_to<_Mstr>, _Alloc>(d, mSet());
        d.dump();
    }
    const _Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& c) { return _assign(c); }
    _Str& createStr(const _CharT* name) {
        return _createStr(name, strAlloc());
    }
    _Str& createStr(const _Str& name) {
        return _createStr(name.c_str(), strAlloc());
    }

protected: // allocators.
    _StrAlloc& strAlloc() { return m_allocs.m_aStr; }
    // raw creation.
    _Str& _createStr(const _CharT* name,
        _StrAlloc& a) {
        _Str* p_s = a.allocate(1);
        a.construct(p_s, _Str(name));
        insert(*p_s);
        return *p_s;
    }

private: // functions.
    // Accessors
    const _SSet& sSet() const { return m_sSet; };
    _SSet& sSet() { return m_sSet; };
    const _MSet& mSet() const { return m_mSet; };
    _MSet& mSet() { return m_mSet; };
    const _Str& _nameTag() const { return _NT::m_Dictionary; }
    _Self& _assign(const _Self& c) {
        if (this == &c)
            return *this;
        sSet(c.sSet());
        mSet(c.mSet());
        m_index = 0;
        return *this;
    }
    void sSet(const _SSet& s) { m_sSet = s; }
    void mSet(const _MSet& m) { m_mSet = m; }

private: // data.
    _SSet   m_sSet;
    _MSet   m_mSet;
    std::uint64_t    m_index; // TBD: Temporary solution.
    // Allocators. They consume space.
    // Size of each allocator is 1 byte.
    typedef struct {
        _StrAlloc      m_aStr;
    } _Allocators;
    _Allocators         m_allocs;
};

////////////////////////////////////////////////////////////////////////////////
// Template specialization
using Dictionary = BasicDictionary<char, std::char_traits<char>, std::allocator<char> >;
using WDictionary = BasicDictionary<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >;

////////////////////////////////////////////////////////////////////////////////
// utl::dictionary dump operator
template <typename _CharT, typename _Traits, typename _Alloc>
inline std::ostream& operator<<(std::ostream& o, 
    const BasicDictionary<_CharT, _Traits, _Alloc>& d) 
{
    BasicDumpXml<_CharT, _Traits, _Alloc> xml(o);
    d.dump(xml);
    return o;
}

////////////////////////////////////////////////////////////////////////////////
// utl::dictionary dump handling
template <typename _CharT, typename _Traits, typename _Alloc>
inline BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(BasicDumpXml<_CharT, _Traits, _Alloc>& o, 
    const BasicDictionary<_CharT, _Traits, _Alloc>& v) 
{
    v.dump(o);
    return o;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(BasicDumpXsd<_CharT, _Traits, _Alloc>& o, 
    const BasicDictionary<_CharT, _Traits, _Alloc>& v) 
{
    v.dump(o);
    return o;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(BasicDumpVerilog<_CharT, _Traits, _Alloc>& o, 
    const BasicDictionary<_CharT, _Traits, _Alloc>& v) 
{
    v.dump(o);
    return o;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline BasicDumpJson<_CharT, _Traits, _Alloc>& 
operator<<(BasicDumpJson<_CharT, _Traits, _Alloc>& o, 
    const BasicDictionary<_CharT, _Traits, _Alloc>& v) 
{
    v.dump(o);
    return o;
}
}

#ifdef SWIG
namespace utl {
    %naturalvar Dictionary;
}

%define DICTIONARY_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## Dictionary) utl::BasicDictionary<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

DICTIONARY_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __DICTIONARY_H__
