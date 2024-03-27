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

%module utl_int
%{
#include <math.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <bitset>
#include <sstream>
#include <assert.h>

#include <json/json.h>
#include <gtest/gtest.h>

#include "common.h"
#include "classnametagbase.h"
#include "multistring.h"
#include "dump.h"
#include "message.h"
#include "except.h"
#include "dictionary.h"
%}

%feature("flatnested", "1");
#pragma SWIG nowarn=317

%warnfilter(509) std::basic_string;

%include bitset.i
%include string.i
%include vector.i
%include unordered_map.i
%include unordered_set.i
%include pair.i

%warnfilter(509) Json::Value::Value;
%include config.i
%include value.i
%include writer.i

%include <common.h>

%warnfilter(401);
%warnfilter(509) utl::BasicDictionary::createStr;
%ignore utl::basic_multistring::operator[];
%ignore utl::basic_multistring::operator=;
%ignore utl::BasicMessageBase::operator=;
%ignore utl::BasicMessage::operator=;
%ignore utl::BasicExceptionBase::operator=;
%ignore utl::BasicException::operator=;
%ignore utl::BasicDictionary::operator=;
%rename(_print) print;

%include <classnametagbase.h>
%include <multistring.h>
%include <dump.h>
%include <message.h>
%include <except.h>
%include <dictionary.h>
