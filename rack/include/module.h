#ifndef __Module_H__
#define __Module_H__
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
class BasicModule 
    : public BasicModuleBase<_CharT, _Traits, _Alloc> {
public: //using
    using _Entity = typename rack::BasicEntityBase<_CharT, _Traits, _Alloc>;
    using _Base = typename _Entity::_ModuleBase;
    using _Self = typename _Entity::_Module;
    using _Pair = typename std::pair<typename _Entity::_Mstr, _Self*>;

public: // constructors/destructors.
    BasicModule(const utl::Id_t __id, const typename _Entity::_Mstr& __name, typename _Entity::_LibraryBase* __p) 
        : _Base(__id, __name, __p) {}
    virtual ~BasicModule() {}

public: // Copy constructors.
    BasicModule(const _Self& __c)
        : _Base(__c) {}

private: // Operators.
    const _Self& operator=(const _Self& __c) { return _assign(__c); }

public: // Accessors.
public: // Utility.
    _Pair pair() { return _Pair(_Base::name(), this); }
    void dump(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __d) const {
        if (__d.backtrack()) return;
        __d.beginTag(nameTag(), _Base::id());
        _Base::dump(__d);
        __d.endTag(nameTag());
    }
    void dump(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __d) const {
    }
    void dump(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __d) const {
        __d.beginModule(_Base::name());
        _Base::dump(__d);
        __d.endModule();
    }
    void dump(utl::BasicDumpJson<_CharT, _Traits, _Alloc>& __d) const {
    }
    const typename _Entity::_Str& nameTag() const { return _nameTag(); }
    _Self& assign(const _Self& __c) { return _assign(__c); }

    typename _Entity::_Port* CastToPort(typename _Entity::_Connector* c) {
    	return static_cast<typename _Entity::_Port*>(c);
    }
#ifdef SWIG
	%extend {
		const typename _Entity::_Mstr& name() const {
			return self->name();
		}
		const utl::Id_t id() const {
			return self->id();
		}

	    const typename _Entity::_ConnectorSet& ports() const {
	    	return self->ports();
	    }
	    const typename _Entity::_ViewManager& viewManager() const {
	    	return self->viewManager();
	    }

	    typename _Entity::_Port& createPort(const typename _Entity::_Str& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
	        return self->createPort(__name, __sd, __r);
	    }
	    typename _Entity::_Port& createPort(const typename _Entity::_Mstr& __name, const _Flags __sd, typename _Entity::_Rack& __r) {
	        return self->createPort(__name, __sd, __r);
	    }
	    void createPins(typename _Entity::_Reference& __inst, typename _Entity::_Rack& __r) {
	        return self->createPins(__inst, __r);
	    }

	    typename _Entity::_Port* findPort(const typename _Entity::_Str& __name, typename _Entity::_Rack& __r) {
	        return self->CastToPort(self->findPort(__name, __r));
	    }
	    typename _Entity::_Port* findPort(const typename _Entity::_Mstr& __name, typename _Entity::_Rack& __r) {
	        return self->CastToPort(self->findPort(__name, __r));
	    }
	    void connect(const _CharT* __pname, typename _Entity::_Net& __p_n, typename _Entity::_Rack& __r) {
	    	self->connect(__pname, __p_n, __r);
	    }
	    void disconnect(const _CharT* __pname, typename _Entity::_Net& __p_n, typename _Entity::_Rack& __r) {
	    	self->disconnect(__pname, __p_n, __r);
	    }
	    void erase(typename _Entity::_Net& __p_n, typename _Entity::_Rack& __r) {
	    	self->erase(__p_n, __r);
	    }
	    void replace(typename _Entity::_Reference& __i, typename _Entity::_Rack& __r) {
	    	self->replace(__i, __r);
	    }
	};
#endif // SWIG

private:
    const typename _Entity::_Str& _nameTag() const { return _Entity::_NT::__m_Module; }
    _Self& _assign(const _Self& __c) {
        if (this == &__c)
            return *this;
        _Base::assign(__c);
        return *this;
    }
};

using Module = BasicModule<char, std::char_traits<char>,
    std::allocator<char> >;
using WModule = BasicModule<wchar_t, std::char_traits<wchar_t>,
    std::allocator<wchar_t> >;

template <class _CharT, class _Traits, class _Alloc>
inline std::ostream& operator<<(std::ostream& __o, 
    const BasicModule<_CharT, _Traits, _Alloc>& __v) 
{
    utl::BasicDumpXml<_CharT, _Traits, _Alloc> __xml(__o);
    __v.dump(__xml);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXml<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXml<_CharT, _Traits, _Alloc>& __o, 
    const BasicModule<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpXsd<_CharT, _Traits, _Alloc>& __o, 
    const BasicModule<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

template <class _CharT, class _Traits, class _Alloc>
inline utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& 
operator<<(utl::BasicDumpVerilog<_CharT, _Traits, _Alloc>& __o, 
    const BasicModule<_CharT, _Traits, _Alloc>& __v) 
{
    __v.dump(__o);
    return __o;
}

}

#ifdef SWIG
%define MODULE_TEMPLATE_CL_WRAP(prefix, _Tp, type)
%template(prefix ## type ## Module) rack::BasicModule<_Tp, std::char_traits<_Tp>, std::allocator<_Tp> >;
%enddef

MODULE_TEMPLATE_CL_WRAP(cl, char,);
#endif // SWIG

#endif // __Module_H__
