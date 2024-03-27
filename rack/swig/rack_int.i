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

%module rack_int
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
#include <mutex>
#include <algorithm>

#include <json/json.h>
#include <racksys.h>
%}

%feature("flatnested", "1");
#pragma SWIG nowarn=317

%warnfilter(509) std::basic_string;

%include bitset.i
%include string.i
%include vector.i
%include unordered_map.i
%include unordered_set.i
%include multimap.i
%include pair.i

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

%include <trm.h>

%ignore utl::BasicExceptions::operator=;
%ignore rack::BasicConnectorBase::operator=;
%ignore rack::BasicViewManager::operator=;
%ignore rack::BasicLibrary::operator=;
%ignore rack::BasicDesign::operator=;

%include <classnametag.h>
//%include funcadapter.i
//%include navigatornode.i
//%include navigatorbase.i
//%include navigator.i
%include <exceptions.h>
%include <entitybase.h>
%include <unnamedentitybase.h>
%include <namedentitybase.h>
%include <viewbase.h>
%include <viewgroup.h>
%include <connectorbase.h>
%include <portbase.h>
%include <port.h>
%include <interconnectbase.h>
%include <referencebase.h>
%include <componentbase.h>
%include <pinbase.h>
%include <pin.h>
%include <netbase.h>
%include <net.h>
%include <instancebase.h>
%include <instance.h>
%include <blackbox.h>
%include <physical.h>
%include <netlist.h>
%include <timing.h>
%include <cell.h>
%include <hdl.h>
%include <viewmanager.h>
%include <librarybase.h>
%include <modulebase.h>
%include <module.h>
%include <technology.h>
%include <library.h>
%include <design.h>
%include <rack.h>
%include <threadadapter.h>

//%include devicebase.h
//%include diodebase.h
//%include diode.h
//%include linkpinbase.h
//%include linkpin.h
//%include locator.h
//%include nonroute.h
//%include racksys.h
//%include routebase.h
//%include route.h
//%include segmentbase.h
//%include segment.h
//%include steinerbase.h
//%include steiner.h
//%include tiebase.h
//%include tie.h
//%include transistorbase.h
//%include transistor.h
//%include viabase.h
//%include via.h

