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

%{
//#include <json/config.h>
%}


// If non-zero, the library uses exceptions to report bad input instead of C
// assertion macros. The default is to use exceptions.
#ifndef JSON_USE_EXCEPTION
#define JSON_USE_EXCEPTION 1
#endif

// Temporary, tracked for removal with issue #982.
#ifndef JSON_USE_NULLREF
#define JSON_USE_NULLREF 1
#endif

/// If defined, indicates that the source file is amalgamated
/// to prevent private header inclusion.
/// Remarks: it is automatically defined in the generated amalgamated header.
// #define JSON_IS_AMALGAMATION

// Export macros for DLL visibility
#if defined(JSON_DLL_BUILD)
#if defined(_MSC_VER) || defined(__MINGW32__)
#define JSON_API __declspec(dllexport)
#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
#elif defined(__GNUC__) || defined(__clang__)
#define JSON_API __attribute__((visibility("default")))
#endif // if defined(_MSC_VER)

#elif defined(JSON_DLL)
#if defined(_MSC_VER) || defined(__MINGW32__)
#define JSON_API __declspec(dllimport)
#define JSONCPP_DISABLE_DLL_INTERFACE_WARNING
#endif // if defined(_MSC_VER)
#endif // ifdef JSON_DLL_BUILD

#if !defined(JSON_API)
#define JSON_API
#endif

#if defined(_MSC_VER) && _MSC_VER < 1800
#error                                                                         \
    "ERROR:  Visual Studio 12 (2013) with _MSC_VER=1800 is the oldest supported compiler with sufficient C++11 capabilities"
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
// As recommended at
// https://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
extern JSON_API int msvc_pre1900_c99_snprintf(char* outBuf, size_t size,
                                              const char* format, ...);
#define jsoncpp_snprintf msvc_pre1900_c99_snprintf
#else
#define jsoncpp_snprintf std::snprintf
#endif

// If JSON_NO_INT64 is defined, then Json only support C++ "int" type for
// integer
// Storages, and 64 bits integer support is disabled.
// #define JSON_NO_INT64 1

// JSONCPP_OVERRIDE is maintained for backwards compatibility of external tools.
// C++11 should be used directly in JSONCPP.
#define JSONCPP_OVERRIDE override

#if __cplusplus >= 201103L
#define JSONCPP_NOEXCEPT noexcept
#define JSONCPP_OP_EXPLICIT explicit
#elif defined(_MSC_VER) && _MSC_VER < 1900
#define JSONCPP_NOEXCEPT throw()
#define JSONCPP_OP_EXPLICIT explicit
#elif defined(_MSC_VER) && _MSC_VER >= 1900
#define JSONCPP_NOEXCEPT noexcept
#define JSONCPP_OP_EXPLICIT explicit
#else
#define JSONCPP_NOEXCEPT throw()
#define JSONCPP_OP_EXPLICIT
#endif

#ifdef __clang__
#if __has_extension(attribute_deprecated_with_message)
#define JSONCPP_DEPRECATED(message) __attribute__((deprecated(message)))
#endif
#elif defined(__GNUC__) // not clang (gcc comes later since clang emulates gcc)
#if (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))
#define JSONCPP_DEPRECATED(message) __attribute__((deprecated(message)))
#elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#define JSONCPP_DEPRECATED(message) __attribute__((__deprecated__))
#endif                  // GNUC version
#elif defined(_MSC_VER) // MSVC (after clang because clang on Windows emulates
                        // MSVC)
#define JSONCPP_DEPRECATED(message) __declspec(deprecated(message))
#endif // __clang__ || __GNUC__ || _MSC_VER

#if !defined(JSONCPP_DEPRECATED)
#define JSONCPP_DEPRECATED(message)
#endif // if !defined(JSONCPP_DEPRECATED)

#if defined(__clang__) || (defined(__GNUC__) && (__GNUC__ >= 6))
#define JSON_USE_INT64_DOUBLE_CONVERSION 1
#endif

namespace Json {
	using Int = int;
	using UInt = unsigned int;
#if defined(JSON_NO_INT64)
	using LargestInt = int;
	using LargestUInt = unsigned int;
	#undef JSON_HAS_INT64
#else                 // if defined(JSON_NO_INT64)
	// For Microsoft Visual use specific types as long long is not supported
#if defined(_MSC_VER) // Microsoft Visual Studio
	using Int64 = __int64;
	using UInt64 = unsigned __int64;
#else                 // if defined(_MSC_VER) // Other platforms, use long long
	using Int64 = int64_t;
	using UInt64 = uint64_t;
#endif                // if defined(_MSC_VER)
	using LargestInt = Int64;
	using LargestUInt = UInt64;
#define JSON_HAS_INT64
#endif // if defined(JSON_NO_INT64)

template <typename T>
using Allocator =
    typename std::conditional<JSONCPP_USING_SECURE_MEMORY, SecureAllocator<T>,
                              std::allocator<T>>::type;
using String = std::basic_string<char, std::char_traits<char>, Allocator<char>>;
using IStringStream =
    std::basic_istringstream<String::value_type, String::traits_type,
                             String::allocator_type>;
using OStringStream =
    std::basic_ostringstream<String::value_type, String::traits_type,
                             String::allocator_type>;
using IStream = std::istream;
using OStream = std::ostream;
} // namespace Json

// Legacy names (formerly macros).
using JSONCPP_STRING = Json::String;
using JSONCPP_ISTRINGSTREAM = Json::IStringStream;
using JSONCPP_OSTRINGSTREAM = Json::OStringStream;
using JSONCPP_ISTREAM = Json::IStream;
using JSONCPP_OSTREAM = Json::OStream;

%inline %{
	namespace Json {
		template<typename _Tp1, typename _Tp2>
		_Tp1 Cast(_Tp2& data) {
			return static_cast<_Tp1>(data);
		}
	}
%}

%define JSON_TEMPLATE_FN_WRAP(prefix, _Tp, type)
%template(prefix ## Cast ## type) Json::Cast<std::basic_string<_Tp, std::char_traits<_Tp>, std::allocator<_Tp>>, std::basic_string<_Tp, std::char_traits<_Tp>, Json::Allocator<_Tp>>>;
%enddef

JSON_TEMPLATE_FN_WRAP(fn, char, );


