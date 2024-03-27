#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__
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

template <class _CharT, class _Traits, class _Alloc>
class BasicExceptions {
public: // using
    using _Str = std::basic_string<_CharT, _Traits, _Alloc>;
    using _Mstr = utl::basic_multistring<_CharT, _Traits, _Alloc>;
    using _NT = typename rack::BasicClassNameTag<_CharT, _Traits, _Alloc>;
    using _Message = typename utl::BasicMessage<_CharT, _Traits, _Alloc>;
    using _Exception = typename utl::BasicException<_CharT, _Traits, _Alloc>;
    using _Base = typename utl::BasicExceptionBase<_CharT, _Traits, _Alloc>;
    using _Self = typename utl::BasicException<_CharT, _Traits, _Alloc>;
    using _MsgCache = typename std::unordered_map<_Str, _Mstr, std::hash<_Str>, std::equal_to<_Str>, _Alloc>;
    using _Dictionary = typename utl::BasicDictionary<_CharT, _Traits, _Alloc>;

public:
    using _MsgCacheItr = typename _MsgCache::iterator;
    using _MsgCacheConstItr = typename _MsgCache::const_iterator;

public: // Constructors.
    BasicExceptions() 
        : __mc_fatal("*** FATAL: "),
        __mc_error("*** ERROR: "),
        __mc_warning("*** WARNING: "),
        __mc_info("*** INFO: "),
        __mc_debug("*** DEBUG: "),
        __m_message(__m_msgCache), 
        __m_exception(__m_message) { 
            mapExceptions(); 
    }
    virtual ~BasicExceptions() {}

    // Operators.
    _Self& operator=(const _Self& __s) {
        if (&__s == this) {
            return *this;
        }
        _Base::operator=(__s);
        return *this;
    }

    // Accessors.
    const _Str& fatal() { return __mc_fatal; }
    const _Str& error() { return __mc_error; }
    const _Str& warning() { return __mc_warning; }
    const _Str& info() { return __mc_info; }
    const _Str& debug() { return __mc_debug; }
    const _MsgCache& msgCache() const { return __m_msgCache; }
    _MsgCache& msgCache() { return __m_msgCache; }
    const _Message& message() const { return __m_message; }
    _Message& message() { return __m_message; }
    const _Exception& exception() const { return __m_exception; }
    _Exception& exception() { return __m_exception; }
    const _Str& nameTag() const { return _nameTag(); }
    _Dictionary& dictionary() { return __m_dictionary; }
    const _Dictionary& dictionary() const { return __m_dictionary; }

    // Utility functions.
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(_nameTag(), 0);
        for (_MsgCacheConstItr __itr = msgCache().begin(); __itr != msgCache().end(); __itr++) {
            utl::dump<_CharT, _Traits, _Mstr, _Mstr*, std::less<_Mstr>, _Alloc>
                (__d, __itr->first);
            utl::dump<_CharT, _Traits, _Mstr, _Mstr*, std::less<_Mstr>, _Alloc>
                (__d, __itr->second);
        }
        dictionary().dump(__d);
        __d.endTag(_nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }

    void mapExceptions() {
    	// Fatal
        mapFatalPortModule();
        mapFatalPinInstance();
        mapFatalPortNet();
        mapFatalPinNet();
        mapFatalInstance();

        // Error
        mapErrorPortModule();
        mapErrorPinInstance();
        mapErrorPortNet();
        mapErrorPinNet();
        mapErrorInstance();
    }
    void mapFatalPortModule() {
        multistring ms;
        ms.push_back((*(dictionary().insert(fatal())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified port \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found in module \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Port mapping error!")).first)[0]);
        msgCache()["F_%m%m_00001"] = *(dictionary().insert(ms)).first;
    }
    void mapFatalPinInstance() {
        multistring ms;
        ms.push_back((*(dictionary().insert(fatal())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified pin \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on instance \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Pin mapping error!")).first)[0]);
        msgCache()["F_%m%m_00002"] = *(dictionary().insert(ms)).first;
    }
    void mapFatalPortNet() {
        multistring ms;
        ms.push_back((*(dictionary().insert(fatal())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified port \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on net \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Port mapping error!")).first)[0]);
        msgCache()["F_%m%m_00003"] = *(dictionary().insert(ms)).first;
    }
    void mapFatalPinNet() {
        multistring ms;
        ms.push_back((*(dictionary().insert(fatal())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified pin \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on net \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Pin mapping error!")).first)[0]);
        msgCache()["F_%m%m_00004"] = *(dictionary().insert(ms)).first;
    }
    void mapFatalInstance() {
        multistring ms;
        ms.push_back((*(dictionary().insert(fatal())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified instance \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found in netlist \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Instance mapping error!")).first)[0]);
        msgCache()["F_%m%m_00005"] = *(dictionary().insert(ms)).first;
    }

    void mapErrorPortModule() {
        multistring ms;
        ms.push_back((*(dictionary().insert(error())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified port \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found in module \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Port mapping error!")).first)[0]);
        msgCache()["E_%m%m_00001"] = *(dictionary().insert(ms)).first;
    }
    void mapErrorPinInstance() {
        multistring ms;
        ms.push_back((*(dictionary().insert(error())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified pin \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on instance \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Pin mapping error!")).first)[0]);
        msgCache()["E_%m%m_00002"] = *(dictionary().insert(ms)).first;
    }
    void mapErrorPortNet() {
        multistring ms;
        ms.push_back((*(dictionary().insert(error())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified port \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on net \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Port mapping error!")).first)[0]);
        msgCache()["E_%m%m_00003"] = *(dictionary().insert(ms)).first;
    }
    void mapErrorPinNet() {
        multistring ms;
        ms.push_back((*(dictionary().insert(error())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified pin \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on net \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Pin mapping error!")).first)[0]);
        msgCache()["E_%m%m_00004"] = *(dictionary().insert(ms)).first;
    }
    void mapErrorInstance() {
        multistring ms;
        ms.push_back((*(dictionary().insert(error())).first)[0]);
        ms.push_back((*(dictionary().insert("The specified instance \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\" could not be found on netlist \"")).first)[0]);
        ms.push_back((*(dictionary().insert("")).first)[0]);
        ms.push_back((*(dictionary().insert("\". Instance mapping error!")).first)[0]);
        msgCache()["E_%m%m_00005"] = *(dictionary().insert(ms)).first;
    }

private: // functions.
        const _Str& _nameTag() const { return _NT::__m_Exceptions; }

private: // constants
    const _Str          __mc_fatal;
    const _Str          __mc_error;
    const _Str          __mc_warning;
    const _Str          __mc_info;
    const _Str          __mc_debug;

private: // data.
    _MsgCache           __m_msgCache;
    _Message            __m_message;
    _Exception          __m_exception;
    _Dictionary            __m_dictionary;
};

using Exceptions = BasicExceptions<char, std::char_traits<char>,
    std::allocator<char> >;
using WExceptions = BasicExceptions<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

}

#ifdef SWIG
%define EXCEPTIONS_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## Exceptions) utl::BasicExceptions<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

EXCEPTIONS_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif /* __EXCEPTIONS_H__ */
