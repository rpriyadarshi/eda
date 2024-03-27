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
//#include <json/writer.h>
%}

namespace Json {

	class Value;

	class JSON_API StreamWriter {
	public:
		StreamWriter();
		virtual ~StreamWriter();
		virtual int write(Value const& root, OStream* sout) = 0;

		class JSON_API Factory {
		public:
			virtual ~Factory();
			virtual StreamWriter* newStreamWriter() const = 0;
		}; // Factory
	};   // StreamWriter

	class JSON_API StreamWriterBuilder : public StreamWriter::Factory {
	public:
		Json::Value settings_;

		StreamWriterBuilder();
		~StreamWriterBuilder() override;

		StreamWriter* newStreamWriter() const override;

		bool validate(Json::Value* invalid) const;
//		Value& operator[](const String& key);
        %extend {
        	Value& __setitem__(const String& key, const char* value) {
                return self->operator[](key) = value;
            }

        	Value& __getitem__(const String& key) {
                return self->operator[](key);
            }
        }
		static void setDefaults(Json::Value* settings);
	};

	String JSON_API writeString(StreamWriter::Factory const& factory,
	                            Value const& root);
	String JSON_API writeString(StreamWriterBuilder const& factory,
	                            Value const& root);
}


