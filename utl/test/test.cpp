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
#include <algorithm>

#include <json/json.h>
#include <gtest/gtest.h>

#include "common.h"
#include "classnametagbase.h"
#include "multistring.h"
#include "dump.h"
#include "message.h"
#include "except.h"
#include "dictionary.h"

struct SampleData {
public:
    SampleData() :
    _s0("Alpha"), _s1("Beta"), _s2("Gamma"), _s3("Zeta"),
    _s4(_s0), _s5(_s1), _s6(_s2), _s7(_s3) {
        buildMultiString();
        buildDictionary();
        _xml = std::move(buildXml());
        _json = std::move(buildJson());
    }

public:
    void buildMultiString() {
        _m0.push_back(_s0);
        _m0.push_back(_s1);
        _m0.push_back(_s2);
        _m0.push_back(_s3);

        _m1.push_back(_s4);
        _m1.push_back(_s5);
        _m1.push_back(_s6);
        _m1.push_back(_s7);

        _m2.push_back(_s5);
        _m2.push_back(_s4);
        _m2.push_back(_s6);
        _m2.push_back(_s7);

        _m3.push_back(_s4);
        _m3.push_back(_s6);
        _m3.push_back(_s5);
        _m3.push_back(_s7);

        _m4.push_back(_s4);
        _m4.push_back(_s5);
        _m4.push_back(_s7);
        _m4.push_back(_s6);
    }

    void buildDictionary() {
        _d.insert(_s4);
        _d.insert(_s5);
        _d.insert(_s6);
        _d.insert(_s7);

        // repeat
        _d.insert(_s4);
        _d.insert(_s5);
        _d.insert(_s6);
        _d.insert(_s7);

        _d.insert(_m1);
        _d.insert(_m2);
        _d.insert(_m3);
        _d.insert(_m4);

        // repeat
        _d.insert(_m1);
        _d.insert(_m2);
        _d.insert(_m3);
        _d.insert(_m4);
    }
    std::string buildXml() {
        return {
            "    <Dictionary ID=\"Dictionary_0\">\n"
            "        <String>\n"
            "            <String>\"Zeta\"</String>\n"
            "            <String>\"Beta\"</String>\n"
            "            <String>\"Gamma\"</String>\n"
            "            <String>\"Alpha\"</String>\n"
            "        </String>\n"
            "        <Multistring>\n"
            "            <Multistring>\"AlphaBetaZetaGamma\"</Multistring>\n"
            "            <Multistring>\"AlphaGammaBetaZeta\"</Multistring>\n"
            "            <Multistring>\"BetaAlphaGammaZeta\"</Multistring>\n"
            "            <Multistring>\"AlphaBetaGammaZeta\"</Multistring>\n"
            "            <Multistring>\"Zeta\"</Multistring>\n"
            "            <Multistring>\"Beta\"</Multistring>\n"
            "            <Multistring>\"Gamma\"</Multistring>\n"
            "            <Multistring>\"Alpha\"</Multistring>\n"
            "        </Multistring>\n"
            "    </Dictionary>\n"
        };
    }
    std::string buildJson() {
        int i = 0;
        _root["Dictionary"]["ID"] = "Dictionary_0";

        _root["Dictionary"]["String"].append("Zeta");
        _root["Dictionary"]["String"].append("Beta");
        _root["Dictionary"]["String"].append("Gamma");
        _root["Dictionary"]["String"].append("Alpha");
        i = 0;
        _root["Dictionary"]["Multistring"].append("AlphaBetaZetaGamma");
        _root["Dictionary"]["Multistring"].append("AlphaGammaBetaZeta");
        _root["Dictionary"]["Multistring"].append("BetaAlphaGammaZeta");
        _root["Dictionary"]["Multistring"].append("AlphaBetaGammaZeta");
        _root["Dictionary"]["Multistring"].append("Zeta");
        _root["Dictionary"]["Multistring"].append("Beta");
        _root["Dictionary"]["Multistring"].append("Gamma");
        _root["Dictionary"]["Multistring"].append("Alpha");

        // Transfer
        Json::StreamWriterBuilder builder;
        return Json::writeString(builder, _root);
    }

public:
    // Strings
    std::string _s0;
    std::string _s1;
    std::string _s2;
    std::string _s3;
    std::string _s4;
    std::string _s5;
    std::string _s6;
    std::string _s7;

    // Multi-strings
    utl::multistring _m0;
    utl::multistring _m1;
    utl::multistring _m2;
    utl::multistring _m3;
    utl::multistring _m4;

    utl::Dictionary _d;
    std::string _xml;
    std::string _json;
    Json::Value _root;
};

TEST(compileTest, multistring)
{
    const utl::multistring ms;

    EXPECT_EQ(0u, ms.size());
}

TEST(basicTest, multistring)
{
    utl::multistring ms;
    ms += std::string("Hello");
    ms += std::string(", ");
    ms += std::string("World!");
    std::stringstream ss;
    ss << ms << std::ends;
    EXPECT_STREQ("Hello, World!", ss.str().c_str());
}

TEST(vectorTest, multistringVector)
{
    using SV = std::vector<std::string*>;
    using MV = std::vector<utl::multistring>;

    SV sv;
    MV mv;

    sv.push_back(new std::string("one"));
    mv.push_back(*sv.back());
    sv.push_back(new std::string("two"));
    mv.push_back(*sv.back());
    sv.push_back(new std::string("three"));
    mv.push_back(*sv.back());
    sv.push_back(new std::string("four"));
    mv.push_back(*sv.back());
    sv.push_back(new std::string("five"));
    mv.push_back(*sv.back());

    EXPECT_TRUE(sv.size() == mv.size());
    for (size_t i = 0; i < sv.size(); ++i)
    {
        EXPECT_TRUE(sv[i] == &mv[i][0]);
        delete sv[i];
    }
}

TEST(joinTest, multistring)
{
    std::stringstream ss;
    const std::string s1("Hello");
    const std::string s2("World");
    const std::string s3("!");

    const std::string all(s1+s2+s3+s3+s2+s1+s2+s1+s3);

    utl::multistring ms1(s1);
    ms1 += s2;
    ms1 += s3;

    utl::multistring ms2(s3);
    ms2 += s2;
    ms2 += s1;

    utl::multistring ms3(s2);
    ms3 += s1;
    ms3 += s3;

    utl::multistring ms4 = ms1;
    ms4 += ms2;
    ms4 += ms3;

    EXPECT_STREQ(all.c_str(), ms4.str().c_str());
}

TEST(randomTest, multistring)
{
    std::stringstream ss;
    const std::string s1("Hello");
    const std::string s2("World");
    const std::string s3("!");

    const std::string all(s1+s2+s3);

    utl::multistring ms1(s1);
    ms1 += s2;
    ms1 += s3;
    ss << ms1 << std::ends;
    EXPECT_STREQ(all.c_str(), ss.str().c_str());

    utl::multistring ms2;
    ms2 += ms1;
    ss.str("");
    ss << ms2 << std::ends;
    EXPECT_STREQ(all.c_str(), ss.str().c_str());

    utl::multistring ms3;
    ms3 = ms2;
    ss.str("");
    ss << ms3 << std::ends;
    EXPECT_STREQ(all.c_str(), ss.str().c_str());

    EXPECT_TRUE(ms1 == ms1);
    EXPECT_TRUE(ms2 == ms3);
}

TEST(hashTest, multistringHash)
{
    std::unordered_set<utl::multistring> hSet;
    std::string s1("kiwi");
    std::string s2("plum");
    std::string s3("apple");
    std::string s4("mango");
    std::string s5("apricot");
    std::string s6("banana");
    std::string s7("durian");

    utl::multistring ms1(s1);
    utl::multistring ms2(s2);
    utl::multistring ms3(s3);
    utl::multistring ms4(s4);
    utl::multistring ms5(s5);
    utl::multistring ms6(s6);
    utl::multistring ms7(s7);

    //utl::multistring ms8("Must not compile");

    hSet.insert(ms1);
    hSet.insert(ms2);
    hSet.insert(ms3);
    hSet.insert(ms4);
    hSet.insert(ms5);
    hSet.insert(ms6);

    std::stringstream ss1;
    std::stringstream ss2;

    std::unordered_set<utl::multistring>::const_iterator it
        = hSet.find(ms4);
    EXPECT_TRUE(it != hSet.end());

    ss1 << ms4 << std::ends;
    ss2 << *it << std::ends;
    EXPECT_STREQ(ss1.str().c_str(), ss2.str().c_str());

    it = hSet.find(ms3);
    EXPECT_TRUE(it != hSet.end());

    ss1.str("");
    ss1 << ms3 << std::ends;
    ss2.str("");
    ss2 << *it << std::ends;
    EXPECT_STREQ(ss1.str().c_str(), ss2.str().c_str());

    it = hSet.find(ms7);
    EXPECT_TRUE(it == hSet.end());

}
void
addEntry1(utl::HashMap& hm, utl::Dictionary& d)
{
    utl::multistring ms;
    std::pair<utl::Dictionary::iterator, bool> pd;

    pd = d.insert("@ERROR: ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("Wrong options \"");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("\". RTFM man!");
    ms.push_back((*pd.first)[0]);

    pd = d.insert(ms);
    std::pair<std::string, utl::multistring> p("E_%s_1", *pd.first);
    hm.insert(p);
}

void
addEntry2(utl::HashMap& hm, utl::Dictionary& d)
{
    utl::multistring ms;
    std::pair<utl::Dictionary::iterator, bool> pd;

    pd = d.insert("@WARNING: ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("Why are you saying \"");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("\"? RTFM man! ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("");
    ms.push_back((*pd.first)[0]);

    pd = d.insert(ms);
    hm["W_%s%s_2"] = *pd.first;
}

void
addEntry3(utl::HashMap& hm, utl::Dictionary& d)
{
    utl::multistring ms;
    std::pair<utl::Dictionary::iterator, bool> pd;

    pd = d.insert("@INFO: ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("This is too bad!");
    ms.push_back((*pd.first)[0]);

    pd = d.insert(ms);
    hm["I__1"] = *pd.first;
}

void
addEntry4(utl::HashMap& hm, utl::Dictionary& d)
{
    utl::multistring ms;
    std::pair<utl::Dictionary::iterator, bool> pd;

    pd = d.insert("@WARNING: ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("Why are you saying \"");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("\"? RTFM man! ");
    ms.push_back((*pd.first)[0]);
    pd = d.insert("");
    ms.push_back((*pd.first)[0]);

    pd = d.insert(ms);
    hm["E_%n%m_3"] = *pd.first;
}
TEST(messageTest, basicMessage) {
    utl::Dictionary d;
    utl::HashMap hm;

    addEntry1(hm, d);
    addEntry2(hm, d);
    addEntry3(hm, d);
    addEntry4(hm, d);

    utl::Message msg(hm);
    std::stringstream ss;

    msg.init("E_%s_1", "Hello World!");
    ss << msg << std::ends;
    EXPECT_STREQ("@ERROR: Wrong options \"Hello World!\". RTFM man!", ss.str().c_str());

    // Wrong key
    msg.init("E_%s_2", "Oh! dear World!");
    ss.str("");
    ss << msg << std::ends;
    EXPECT_STREQ("", ss.str().c_str());

    // Key does not accept arguments
    msg.init("I__1", "Will not print!");
    ss.str("");
    ss << msg << std::ends;
    EXPECT_STREQ("@INFO: This is too bad!", ss.str().c_str());

    msg.init("W_%s%s_2", "Got it!", "Now!");
    ss.str("");
    ss << msg << std::ends;
    EXPECT_STREQ("@WARNING: Why are you saying \"Got it!\"? RTFM man! Now!", ss.str().c_str());

    std::string s1("This is std::string!");
    std::string s2("This is multistring!");
    utl::multistring ms(s2);
    msg.init("E_%n%m_3", &s1, &ms);
    ss.str("");
    ss << msg << std::ends;
    EXPECT_STREQ("@WARNING: Why are you saying \"This is std::string!\"? RTFM man! This is multistring!", ss.str().c_str());

    utl::Message msg0(hm);
    msg0 = msg;
    ss.str("");
    ss << msg0 << std::ends;
    EXPECT_STREQ("@WARNING: Why are you saying \"This is std::string!\"? RTFM man! This is multistring!", ss.str().c_str());
}

TEST(exceptionTest, exceptionMessage)
{
    utl::Dictionary d;
    utl::HashMap hm;

    addEntry1(hm, d);
    addEntry2(hm, d);
    addEntry3(hm, d);

    utl::Message msg(hm);
    utl::Exception e(msg);

    std::stringstream ss;
    msg.init("E_%s_1", "Hello World!");
    ss << e << std::ends;
    EXPECT_STREQ("@ERROR: Wrong options \"Hello World!\". RTFM man!", ss.str().c_str());

    // Wrong key
    msg.init("E_%s_2", "Oh! dear World!");
    ss.str("");
    ss << e << std::ends;
    EXPECT_STREQ("", ss.str().c_str());

    // Key does not accept arguments
    msg.init("I__1", "Will not print!");
    ss.str("");
    ss << e << std::ends;
    EXPECT_STREQ("@INFO: This is too bad!", ss.str().c_str());

    msg.init("W_%s%s_2", "Got it!", "Now!");
    ss.str("");
    ss << e << std::ends;
    EXPECT_STREQ("@WARNING: Why are you saying \"Got it!\"? RTFM man! Now!", ss.str().c_str());
}

TEST(MHashMapTest, multistringHashMap) {
    std::string s("TestInsert");
    utl::multistring m(s);

    utl::MHashMap hm;
    std::pair<utl::multistring, utl::multistring> p(m, m);
    hm.insert(p);

    utl::MHashMap::const_iterator it = hm.find(m);
    EXPECT_TRUE(it != hm.end());

    std::stringstream ss;
    ss << it->second << std::ends;
    EXPECT_STREQ("TestInsert", ss.str().c_str());
}

TEST(DictionaryXmlTest, sampleData) {
    SampleData sc;

    std::stringstream ss;
    ss << *sc._d.find(sc._m0) << std::ends;
    EXPECT_STREQ("AlphaBetaGammaZeta", ss.str().c_str());
    ss.str("");
    ss << *sc._d.find(sc._m1) << std::ends;
    EXPECT_STREQ("AlphaBetaGammaZeta", ss.str().c_str());
    ss.str("");
    ss << *sc._d.find(sc._m2) << std::ends;
    EXPECT_STREQ("BetaAlphaGammaZeta", ss.str().c_str());
    ss.str("");
    ss << *sc._d.find(sc._m3) << std::ends;
    EXPECT_STREQ("AlphaGammaBetaZeta", ss.str().c_str());
    ss.str("");
    ss << *sc._d.find(sc._m4) << std::ends;
    EXPECT_STREQ("AlphaBetaZetaGamma", ss.str().c_str());
}

TEST(DictionaryDumpOstreamTest, sampleData) {
    SampleData sc;

    std::stringstream ss;

    ss << sc._d << std::ends;
    EXPECT_STREQ(sc._xml.c_str(), ss.str().c_str());
}

TEST(DictionaryDumpXmlTest, sampleData) {
    SampleData sc;

    std::stringstream ss;

    utl::DumpXml out(ss);
    sc._d.dump(out);
    EXPECT_STREQ(sc._xml.c_str(), ss.str().c_str());
}

TEST(DictionaryDumpVerilogTest, sampleData) {
    SampleData sc;

    std::stringstream ss;

    utl::DumpVerilog out(ss);
    sc._d.dump(out);
    // Debug dump
    std::cout << ss.str() << std::endl;
}

TEST(DictionaryDumpJsonTest, sampleData) {
    SampleData sc;

    std::stringstream ss;

    utl::DumpJson out(ss);
    sc._d.dump(out);
    // Debug dump
//    std::cout << ss.str() << std::endl;
//    std::cout << sc._json << std::endl;
    EXPECT_EQ(out.equal(sc._root), 1);
}
