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
//#include <json/value.h>
%}

// Conditional NORETURN attribute on the throw functions would:
// a) suppress false positives from static code analysis
// b) possibly improve optimization opportunities.
#if !defined(JSONCPP_NORETURN)
#if defined(_MSC_VER) && _MSC_VER == 1800
#define JSONCPP_NORETURN __declspec(noreturn)
#else
#define JSONCPP_NORETURN [[noreturn]]
#endif
#endif

// Support for '= delete' with template declarations was a late addition
// to the c++11 standard and is rejected by clang 3.8 and Apple clang 8.2
// even though these declare themselves to be c++11 compilers.
#if !defined(JSONCPP_TEMPLATE_DELETE)
#if defined(__clang__) && defined(__apple_build_version__)
#if __apple_build_version__ <= 8000042
#define JSONCPP_TEMPLATE_DELETE
#endif
#elif defined(__clang__)
#if __clang_major__ == 3 && __clang_minor__ <= 8
#define JSONCPP_TEMPLATE_DELETE
#endif
#endif
#if !defined(JSONCPP_TEMPLATE_DELETE)
#define JSONCPP_TEMPLATE_DELETE = delete
#endif
#endif

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
// be used by...
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251 4275)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#pragma pack(push)
#pragma pack()

/** \brief JSON (JavaScript Object Notation).
 */
namespace Json {

#if JSON_USE_EXCEPTION
/** Base class for all exceptions we throw.
 *
 * We use nothing but these internally. Of course, STL can throw others.
 */
class JSON_API Exception : public std::exception {
public:
  Exception(String msg);
  ~Exception() noexcept override;
  char const* what() const noexcept override;

protected:
  String msg_;
};

/** Exceptions which the user cannot easily avoid.
 *
 * E.g. out-of-memory (when we use malloc), stack-overflow, malicious input
 *
 * \remark derived from Json::Exception
 */
class JSON_API RuntimeError : public Exception {
public:
  RuntimeError(String const& msg);
};

/** Exceptions thrown by JSON_ASSERT/JSON_FAIL macros.
 *
 * These are precondition-violations (user bugs) and internal errors (our bugs).
 *
 * \remark derived from Json::Exception
 */
class JSON_API LogicError : public Exception {
public:
  LogicError(String const& msg);
};
#endif

/// used internally
JSONCPP_NORETURN void throwRuntimeError(String const& msg);
/// used internally
JSONCPP_NORETURN void throwLogicError(String const& msg);

    /** \brief Type of the value held by a Value object.
     */
    enum ValueType {
        nullValue = 0, ///< 'null' value
        intValue,      ///< signed integer value
        uintValue,     ///< unsigned integer value
        realValue,     ///< double value
        stringValue,   ///< UTF-8 string value
        booleanValue,  ///< bool value
        arrayValue,    ///< array value (ordered list)
        objectValue    ///< object value (collection of name/value pairs).
    };

    enum CommentPlacement {
        commentBefore = 0,      ///< a comment placed on the line before a value
        commentAfterOnSameLine, ///< a comment just after a value on the same line
        commentAfter, ///< a comment on the line after a value (only make sense for
        /// root value)
        numberOfCommentPlacement
    };

    /** \brief Type of precision for formatting of real values.
     */
    enum PrecisionType {
        significantDigits = 0, ///< we set max number of significant digits in string
        decimalPlaces          ///< we set max number of digits after "." in string
    };

/** \brief Lightweight wrapper to tag static string.
 *
 * Value constructor and objectValue member assignment takes advantage of the
 * StaticString and avoid the cost of string duplication when storing the
 * string or the member name.
 *
 * Example of usage:
 * \code
 * Json::Value aValue( StaticString("some text") );
 * Json::Value object;
 * static const StaticString code("code");
 * object[code] = 1234;
 * \endcode
 */
    class JSON_API StaticString {
    public:
        explicit StaticString(const char* czstring) : c_str_(czstring) {}

        %rename(__call__) operator const char*() const;
        operator const char*() const;

        const char* c_str() const;
    };

/** \brief Represents a <a HREF="http://www.json.org">JSON</a> value.
 *
 * This class is a discriminated union wrapper that can represents a:
 * - signed integer [range: Value::minInt - Value::maxInt]
 * - unsigned integer (range: 0 - Value::maxUInt)
 * - double
 * - UTF-8 string
 * - boolean
 * - 'null'
 * - an ordered list of Value
 * - collection of name/value pairs (javascript object)
 *
 * The type of the held value is represented by a #ValueType and
 * can be obtained using type().
 *
 * Values of an #objectValue or #arrayValue can be accessed using operator[]()
 * methods.
 * Non-const methods will automatically create the a #nullValue element
 * if it does not exist.
 * The sequence of an #arrayValue will be automatically resized and initialized
 * with #nullValue. resize() can be used to enlarge or truncate an #arrayValue.
 *
 * The get() methods can be used to obtain default value in the case the
 * required element does not exist.
 *
 * It is possible to iterate over the list of member keys of an object using
 * the getMemberNames() method.
 *
 * \note #Value string-length fit in size_t, but keys must be < 2^30.
 * (The reason is an implementation detail.) A #CharReader will raise an
 * exception if a bound is exceeded to avoid security holes in your app,
 * but the Value API does *not* check bounds. That is the responsibility
 * of the caller.
 */
    class JSON_API Value {
        friend class ValueIteratorBase;

    public:
        using Members = std::vector<String>;
        using iterator = ValueIterator;
        using const_iterator = ValueConstIterator;
        using UInt = Json::UInt;
        using Int = Json::Int;
#if defined(JSON_HAS_INT64)
        using UInt64 = Json::UInt64;
        using Int64 = Json::Int64;
#endif // defined(JSON_HAS_INT64)
        using LargestInt = Json::LargestInt;
        using LargestUInt = Json::LargestUInt;
        using ArrayIndex = Json::ArrayIndex;

        // Required for boost integration, e. g. BOOST_TEST
        using value_type = std::string;

        Value(ValueType type = nullValue);
        Value(Int value);
        Value(UInt value);
#if defined(JSON_HAS_INT64)
        Value(Int64 value);
        Value(UInt64 value);
#endif // if defined(JSON_HAS_INT64)
        Value(double value);
        Value(const char* value); ///< Copy til first 0. (NULL causes to seg-fault.)
        Value(const char* begin, const char* end); ///< Copy all, incl zeroes.
  /**
   * \brief Constructs a value from a static string.
   *
   * Like other value string constructor but do not duplicate the string for
   * internal storage. The given string must remain alive after the call to
   * this constructor.
   *
   * \note This works only for null-terminated strings. (We cannot change the
   * size of this class, so we have nowhere to store the length, which might be
   * computed later for various operations.)
   *
   * Example of usage:
   *   \code
   *   static StaticString foo("some text");
   *   Json::Value aValue(foo);
   *   \endcode
   */
        Value(const StaticString& value);
        Value(const String& value);
        Value(bool value);
        Value(const Value& other);
//        Value(Value&& other);
        ~Value();

//        Value& operator=(const Value& other);
//        Value& operator=(Value&& other);

        /// Swap everything.
        void swap(Value& other);
        /// Swap values but leave comments and source offsets in place.
        void swapPayload(Value& other);

        /// copy everything.
        void copy(const Value& other);
        /// copy values but leave comments and source offsets in place.
        void copyPayload(const Value& other);

        ValueType type() const;

        /// Compare payload only, not comments etc.
        bool operator<(const Value& other) const;
        bool operator<=(const Value& other) const;
        bool operator>=(const Value& other) const;
        bool operator>(const Value& other) const;
        bool operator==(const Value& other) const;
        bool operator!=(const Value& other) const;
        int compare(const Value& other) const;

        const char* asCString() const; ///< Embedded zeroes could cause you trouble!
#if JSONCPP_USE_SECURE_MEMORY
        unsigned getCStringLength() const; // Allows you to understand the length of
                                           // the CString
#endif
        String asString() const; ///< Embedded zeroes are possible.
  /** Get raw char* of string-value.
   *  \return false if !string. (Seg-fault if str or end are NULL.)
   */
        bool getString(char const** begin, char const** end) const;
        Int asInt() const;
        UInt asUInt() const;
#if defined(JSON_HAS_INT64)
        Int64 asInt64() const;
        UInt64 asUInt64() const;
#endif // if defined(JSON_HAS_INT64)
        LargestInt asLargestInt() const;
        LargestUInt asLargestUInt() const;
        float asFloat() const;
        double asDouble() const;
        bool asBool() const;

        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isInt64() const;
        bool isUInt() const;
        bool isUInt64() const;
        bool isIntegral() const;
        bool isDouble() const;
        bool isNumeric() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        /// The `as<T>` and `is<T>` member function templates and specializations.
        template <typename T> T as() const JSONCPP_TEMPLATE_DELETE;
        template <typename T> bool is() const JSONCPP_TEMPLATE_DELETE;

        bool isConvertibleTo(ValueType other) const;

  /// Number of values in array or object
        ArrayIndex size() const;

  /// \brief Return true if empty array, empty object, or null;
  /// otherwise, false.
        bool empty() const;

  /// Return !isNull()
        explicit operator bool() const;

  /// Remove all object members and array elements.
  /// \pre type() is arrayValue, objectValue, or nullValue
  /// \post type() is unchanged
        void clear();

  /// Resize the array to newSize elements.
  /// New elements are initialized to null.
  /// May only be called on nullValue or arrayValue.
  /// \pre type() is arrayValue or nullValue
  /// \post type() is arrayValue
        void resize(ArrayIndex newSize);

//        Value& operator[](ArrayIndex index);
//        Value& operator[](int index);
//        Value& operator[](const char* key);
//        Value& operator[](const String& key);
//        Value& operator[](const StaticString& key);
//
//        const Value& operator[](ArrayIndex index) const;
//        const Value& operator[](int index) const;
//        const Value& operator[](const char* key) const;
//        const Value& operator[](const String& key) const;

        %extend {
        	Value& __setitem__(const char* key, const char* value) {
                return self->operator[](key) = value;
            }

        	Value& __getitem__(ArrayIndex index) {
                return self->operator[](index);
            }
        	Value& __getitem__(int index) {
                return self->operator[](index);
            }
        	Value& __getitem__(const char* key) {
                return self->operator[](key);
            }
        	Value& __getitem__(const String& key) {
                return self->operator[](key);
            }
        	Value& __getitem__(const StaticString& key) {
                return self->operator[](key);
            }

        	const Value& __getitem__(ArrayIndex index) const {
                return self->operator[](index);
            }
        	const Value& __getitem__(int index) const {
                return self->operator[](index);
            }
        	const Value& __getitem__(const char* key) const {
                return self->operator[](key);
            }
        	const Value& __getitem__(const String& key) const {
                return self->operator[](key);
            }
        }

        Value get(ArrayIndex index, const Value& defaultValue) const;
        bool isValidIndex(ArrayIndex index) const;

        Value& append(const Value& value);
//        Value& append(Value&& value);

        bool insert(ArrayIndex index, const Value& newValue);
//        bool insert(ArrayIndex index, Value&& newValue);

        Value get(const char* key, const Value& defaultValue) const;
        Value get(const char* begin, const char* end,
                  const Value& defaultValue) const;
        Value get(const String& key, const Value& defaultValue) const;
        Value const* find(char const* begin, char const* end) const;

        Value* demand(char const* begin, char const* end);
        void removeMember(const char* key);
        void removeMember(const String& key);
        bool removeMember(const char* key, Value* removed);
        bool removeMember(String const& key, Value* removed);
        bool removeMember(const char* begin, const char* end, Value* removed);
        bool removeIndex(ArrayIndex index, Value* removed);

        bool isMember(const char* key) const;
        bool isMember(const String& key) const;
        bool isMember(const char* begin, const char* end) const;

        Members getMemberNames() const;

        void setComment(String comment, CommentPlacement placement);
        bool hasComment(CommentPlacement placement) const;
        String getComment(CommentPlacement placement) const;

        String toStyledString() const;

        const_iterator begin() const;
        const_iterator end() const;

        iterator begin();
        iterator end();

        void setOffsetStart(ptrdiff_t start);
        void setOffsetLimit(ptrdiff_t limit);
        ptrdiff_t getOffsetStart() const;
        ptrdiff_t getOffsetLimit() const;
    };
}

namespace Json {
    %naturalvar Value;
}


