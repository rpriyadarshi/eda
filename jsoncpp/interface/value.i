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

namespace Json {
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


    class JSON_API StaticString {
    public:
        explicit StaticString(const char* czstring);

        %rename(__call__) operator const char*() const;
        operator const char*() const;

        const char* c_str() const;
    };

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
#if JSONCPP_USING_SECURE_MEMORY
        unsigned getCStringLength() const; // Allows you to understand the length of
                                           // the CString
#endif
        String asString() const; ///< Embedded zeroes are possible.
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
        template <typename T> T as() const = delete;
        template <typename T> bool is() const = delete;

        bool isConvertibleTo(ValueType other) const;
        ArrayIndex size() const;
        bool empty() const;
        JSONCPP_OP_EXPLICIT operator bool() const;
        void clear();
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


