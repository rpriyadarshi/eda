# Setup import from build location
# https://docs.python.org/3/tutorial/modules.html
# https://pyformat.info
# https://www.geeksforgeeks.org/unit-testing-python-unittest/
# https://docs.python.org/3/library/unittest.html
# https://stackoverflow.com/questions/22923696/how-to-wrap-a-c-function-which-takes-in-a-function-pointer-in-python-using-swi
# https://rszalski.github.io/magicmethods/
import unittest
import pyutl

class SampleData:
    def __init__(self):
        self.buildMultiString()
        self.buildDictionary()
        self._xml = self.buildXml()
        self._json = self.buildJson()

    def buildMultiString(self) :
        self._m0.push_back(self._s0)
        self._m0.push_back(self._s1)
        self._m0.push_back(self._s2)
        self._m0.push_back(self._s3)

        self._m1.push_back(self._s4)
        self._m1.push_back(self._s5)
        self._m1.push_back(self._s6)
        self._m1.push_back(self._s7)

        self._m2.push_back(self._s5)
        self._m2.push_back(self._s4)
        self._m2.push_back(self._s6)
        self._m2.push_back(self._s7)

        self._m3.push_back(self._s4)
        self._m3.push_back(self._s6)
        self._m3.push_back(self._s5)
        self._m3.push_back(self._s7)

        self._m4.push_back(self._s4)
        self._m4.push_back(self._s5)
        self._m4.push_back(self._s7)
        self._m4.push_back(self._s6)

    def buildDictionary(self) :
        self._d.insert(self._s4)
        self._d.insert(self._s5)
        self._d.insert(self._s6)
        self._d.insert(self._s7)

        # repeat
        self._d.insert(self._s4)
        self._d.insert(self._s5)
        self._d.insert(self._s6)
        self._d.insert(self._s7)

        self._d.insert(self._m1)
        self._d.insert(self._m2)
        self._d.insert(self._m3)
        self._d.insert(self._m4)

        # repeat
        self._d.insert(self._m1)
        self._d.insert(self._m2)
        self._d.insert(self._m3)
        self._d.insert(self._m4)

    def buildXml(self) :
        return (
            "    <Dictionary ID=\"Dictionary_0\">\n"
            "        <String>\n"
            "            <String>\"Zeta\"</String>\n"
            "            <String>\"Gamma\"</String>\n"
            "            <String>\"Alpha\"</String>\n"
            "            <String>\"Beta\"</String>\n"
            "        </String>\n"
            "        <Multistring>\n"
            "            <Multistring>\"AlphaGammaBetaZeta\"</Multistring>\n"
            "            <Multistring>\"BetaAlphaGammaZeta\"</Multistring>\n"
            "            <Multistring>\"AlphaBetaZetaGamma\"</Multistring>\n"
            "            <Multistring>\"Beta\"</Multistring>\n"
            "            <Multistring>\"Alpha\"</Multistring>\n"
            "            <Multistring>\"Gamma\"</Multistring>\n"
            "            <Multistring>\"Zeta\"</Multistring>\n"
            "            <Multistring>\"AlphaBetaGammaZeta\"</Multistring>\n"
            "        </Multistring>\n"
            "    </Dictionary>\n"
        )

    def buildJson(self) :
        i = 0;
                
        self._root["Dictionary"]["ID"] = "Dictionary_0";
        
        self._root["Dictionary"]["String"].append(pyutl.Value("Zeta"));
        self._root["Dictionary"]["String"].append(pyutl.Value("Gamma"));
        self._root["Dictionary"]["String"].append(pyutl.Value("Alpha"));
        self._root["Dictionary"]["String"].append(pyutl.Value("Beta"));
        i = 0;
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("AlphaGammaBetaZeta"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("BetaAlphaGammaZeta"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("AlphaBetaZetaGamma"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("Beta"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("Alpha"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("Gamma"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("Zeta"));
        self._root["Dictionary"]["Multistring"].append(pyutl.Value("AlphaBetaGammaZeta"));

        # # Transfer
        builder = pyutl.StreamWriterBuilder()
        json = pyutl.writeString(builder, self._root)
        return json

    _p0 = "Alpha"
    _p1 = "Beta"
    _p2 = "Gamma"
    _p3 = "Zeta"

    _s0 = pyutl.clString(_p0, len(_p0))
    _s1 = pyutl.clString(_p1, len(_p1))
    _s2 = pyutl.clString(_p2, len(_p2))
    _s3 = pyutl.clString(_p3, len(_p3))

    _s4 = pyutl.clString(_s0)
    _s5 = pyutl.clString(_s1)
    _s6 = pyutl.clString(_s2)
    _s7 = pyutl.clString(_s3)

    _m0 = pyutl.clMultiString()
    _m1 = pyutl.clMultiString()
    _m2 = pyutl.clMultiString()
    _m3 = pyutl.clMultiString()
    _m4 = pyutl.clMultiString()

    _d = pyutl.clDictionary()
    _xml = pyutl.clString()
    _json = pyutl.clString()
    _root = pyutl.Value()

class TestMultiString(unittest.TestCase): 
    _sc = SampleData()

    def setUp(self): 
        pass

    def test_compile(self) :
        ms = pyutl.clMultiString()
        sz = ms.size()
        self.assertEqual(0, sz) 

    def test_basic(self) :
        p0 = "Hello"
        p1 = ", "
        p2 = "World!"

        s0 = pyutl.clString(p0, len(p0))
        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))

        ms = pyutl.clMultiString()
        ms.push_back(s0)
        ms.push_back(s1)
        ms.push_back(s2)

        # ms = ms.add(s0)
        # ms = ms.add(s1)
        # ms = ms.add(s2)

        res = ""
        for i in range(ms.size()) :
            res += ms[i]

        self.assertEqual("Hello, World!", res)

    def test_vector(self) :
        sv = pyutl.clStringVector()
        mv = pyutl.clMultiString()

        p1 = "one"
        p2 = "two"
        p3 = "three"
        p4 = "four"
        p5 = "five"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        s4 = pyutl.clString(p4, len(p4))
        s5 = pyutl.clString(p5, len(p5))

        sv.push_back(s1)
        sv.push_back(s2)
        sv.push_back(s3)
        sv.push_back(s4)
        sv.push_back(s5)

        mv.push_back(s1)
        mv.push_back(s2)
        mv.push_back(s3)
        mv.push_back(s4)
        mv.push_back(s5)

        self.assertEqual(1, (sv.size() == mv.size()))
        for i in range(sv.size()) :
            self.assertEqual(1, (sv[i] == mv[i]))

    def test_join(self) :
        p1 = "Hello"
        p2 = "World"
        p3 = "!"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        sall = s1+s2+s3+s3+s2+s1+s2+s1+s3

        ms1 = pyutl.clMultiString(s1)
        ms1.push_back(s2)
        ms1.push_back(s3)

        ms2 = pyutl.clMultiString(s3)
        ms2.push_back(s2)
        ms2.push_back(s1)

        ms3 = pyutl.clMultiString(s2)
        ms3.push_back(s1)
        ms3.push_back(s3)

        ms4 = pyutl.clMultiString(ms1)
        ms4.push_back(ms2)
        ms4.push_back(ms3)

        self.assertEqual(sall.c_str(), ms4.str())

    def test_random(self) :
        p1 = "Hello"
        p2 = "World"
        p3 = "!"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        sall = s1+s2+s3

        ms1 = pyutl.clMultiString(s1)
        ms1.push_back(s2)
        ms1.push_back(s3)
        self.assertEqual(sall.c_str(), ms1.str())
        self.assertTrue(ms1.size(), 3)

        ms2 = pyutl.clMultiString()
        ms2.push_back(ms1)
        self.assertEqual(sall.c_str(), ms2.str())
        self.assertTrue(ms2.size(), 3)

        ms3 = pyutl.clMultiString()
        ms3 = ms2
        self.assertEqual(sall.c_str(), ms3.str())
        self.assertTrue(ms3.size(), 3)

        self.assertTrue(ms1 == ms1)
        self.assertTrue(ms1 == ms2)
        self.assertTrue(ms2 == ms3)

    def test_hash(self) :
        p1 = "kiwi"
        p2 = "plum"
        p3 = "apple"
        p4 = "mango"
        p5 = "apricot"
        p6 = "banana"
        p7 = "durian"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        s4 = pyutl.clString(p4, len(p4))
        s5 = pyutl.clString(p5, len(p5))
        s6 = pyutl.clString(p6, len(p6))
        s7 = pyutl.clString(p7, len(p7))

        ms1 = pyutl.clMultiString(s1)
        ms2 = pyutl.clMultiString(s2)
        ms3 = pyutl.clMultiString(s3)
        ms4 = pyutl.clMultiString(s4)
        ms5 = pyutl.clMultiString(s5)
        ms6 = pyutl.clMultiString(s6)
        ms7 = pyutl.clMultiString(s7)

        # sSet = pyutl.clStringUnorderedSet()
        # sSet.insert(s1)
        # sSet.insert(s2)
        # sSet.insert(s3)
        # sSet.insert(s4)
        # sSet.insert(s5)
        # sSet.insert(s6)
        
        hSet = pyutl.clMultiStringUnorderedSet()
        hSet.insert(ms1)
        hSet.insert(ms2)
        hSet.insert(ms3)
        hSet.insert(ms4)
        hSet.insert(ms5)
        hSet.insert(ms6)

        # sIter = sSet.begin()
        # while (sIter != sSet.end()) :
        #     print (sIter.next().c_str())

        # hIter = hSet.begin()
        # while (hIter != hSet.end()) :
        #     print (hIter.next().str().c_str())

        it = hSet.find(ms4)
        self.assertTrue(it != hSet.end())
        self.assertEqual(it.value(), ms4)

        it = hSet.find(ms3)
        self.assertTrue(it != hSet.end())
        self.assertEqual(it.value(), ms3)

        it = hSet.find(ms7)
        self.assertTrue(it == hSet.end())
        # self.assertEqual(it.value().str().c_str(), ms7.str().c_str())

    def addEntry1(self, hm, d) :
        p1 = "@ERROR: "
        p2 = "Wrong options \""
        p3 = ""
        p4 = "\". RTFM man!"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        s4 = pyutl.clString(p4, len(p4))

        ms = pyutl.clMultiString()
        pd = d.insert(s1)
        ms.push_back(pd[0].value())
        pd = d.insert(s2)
        ms.push_back(pd[0].value())
        pd = d.insert(s3)
        ms.push_back(pd[0].value())
        pd = d.insert(s4)
        ms.push_back(pd[0].value())

        pd = d.insert(ms)

        k = "E_%s_1"
        sk = pyutl.clString(k, len(k))

        hm[sk] = ms

    def addEntry2(self, hm, d) :
        p1 = "@WARNING: "
        p2 = "Why are you saying \""
        p3 = ""
        p4 = "\"? RTFM man! "
        p5 = ""

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        s4 = pyutl.clString(p4, len(p4))
        s5 = pyutl.clString(p5, len(p5))

        ms = pyutl.clMultiString()
        pd = d.insert(s1)
        ms.push_back(pd[0].value())
        pd = d.insert(s2)
        ms.push_back(pd[0].value())
        pd = d.insert(s3)
        ms.push_back(pd[0].value())
        pd = d.insert(s4)
        ms.push_back(pd[0].value())
        pd = d.insert(s5)
        ms.push_back(pd[0].value())

        pd = d.insert(ms)

        k = "W_%s%s_2"
        sk = pyutl.clString(k, len(k))

        hm[sk] = ms

    def addEntry3(self, hm, d) :
        p1 = "@INFO: "
        p2 = "This is too bad!"

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))

        ms = pyutl.clMultiString()
        pd = d.insert(s1)
        ms.push_back(pd[0].value())
        pd = d.insert(s2)
        ms.push_back(pd[0].value())

        pd = d.insert(ms)

        k = "I__1"
        sk = pyutl.clString(k, len(k))

        hm[sk] = ms

    def addEntry4(self, hm, d) :
        p1 = "@WARNING: "
        p2 = "Why are you saying \""
        p3 = ""
        p4 = "\"? RTFM man! "
        p5 = ""

        s1 = pyutl.clString(p1, len(p1))
        s2 = pyutl.clString(p2, len(p2))
        s3 = pyutl.clString(p3, len(p3))
        s4 = pyutl.clString(p4, len(p4))
        s5 = pyutl.clString(p5, len(p5))

        ms = pyutl.clMultiString()
        pd = d.insert(s1)
        ms.push_back(pd[0].value())
        pd = d.insert(s2)
        ms.push_back(pd[0].value())
        pd = d.insert(s3)
        ms.push_back(pd[0].value())
        pd = d.insert(s4)
        ms.push_back(pd[0].value())
        pd = d.insert(s5)
        ms.push_back(pd[0].value())

        pd = d.insert(ms)

        k = "E_%n%m_3"
        sk = pyutl.clString(k, len(k))

        hm[sk] = ms

    def test_message(self) :
        d = pyutl.clDictionary()
        hm = pyutl.clStringUnorderedMap()

        self.addEntry1(hm, d)
        self.addEntry2(hm, d)
        self.addEntry3(hm, d)
        self.addEntry4(hm, d)

        msg = pyutl.clMessage(hm)
        msg.init("E_%s_1", "Hello World!")
        s = "@ERROR: Wrong options \"Hello World!\". RTFM man!"
        self.assertEqual(s, msg.str())

        msg.init("E_%s_2", "Oh! dear World!")
        s = ""
        self.assertEqual(s, msg.str())

        msg.init("I__1", "Will not print!")
        s = "@INFO: This is too bad!"
        self.assertEqual(s, msg.str())

        msg.init("W_%s%s_2", "Got it!", "Now!")
        s = "@WARNING: Why are you saying \"Got it!\"? RTFM man! Now!"
        self.assertEqual(s, msg.str())

        p1 = "This is std::string!"
        p2 = "This is multistring!"
        s1 = pyutl.clString(p1, len(p1));
        s2 = pyutl.clString(p2, len(p2));
        ms = pyutl.clMultiString(s2);
        msg.init("E_%n%m_3", s1, ms);
        s = "@WARNING: Why are you saying \"This is std::string!\"? RTFM man! This is multistring!"
        self.assertEqual(s, msg.str())

        msg0 = pyutl.clMessage(hm)
        msg0 = msg;
        s = "@WARNING: Why are you saying \"This is std::string!\"? RTFM man! This is multistring!"
        self.assertEqual(s, msg.str())

    def test_exception(self) :
        d = pyutl.clDictionary()
        hm = pyutl.clStringUnorderedMap()

        self.addEntry1(hm, d)
        self.addEntry2(hm, d)
        self.addEntry3(hm, d)

        msg = pyutl.clMessage(hm)
        e = pyutl.clException(msg)
        msg.init("E_%s_1", "Hello World!")
        s = "@ERROR: Wrong options \"Hello World!\". RTFM man!"
        self.assertEqual(s, e.str())

        msg.init("E_%s_2", "Oh! dear World!")
        s = ""
        self.assertEqual(s, e.str())

        msg.init("I__1", "Will not print!")
        s = "@INFO: This is too bad!"
        self.assertEqual(s, e.str())

        msg.init("W_%s%s_2", "Got it!", "Now!")
        s = "@WARNING: Why are you saying \"Got it!\"? RTFM man! Now!"
        self.assertEqual(s, e.str())

    def test_MHashMap(self) :
        p0 = "TestInsert1"
        p1 = "TestInsert"

        s0 = pyutl.clString(p0, len(p0))
        s1 = pyutl.clString(p1, len(p1))

        ms0 = pyutl.clMultiString()
        ms0.push_back(s0)
        ms1 = pyutl.clMultiString()
        ms1.push_back(s1)

        hm = pyutl.clMultiStringUnorderedMap()
        hm[ms1] = ms1

        it0 = hm.find(ms0)
        it1 = hm.find(ms1)
        self.assertEqual(it0, hm.end())
        self.assertNotEqual(it1, hm.end())
        self.assertEqual(p1, it1.value()[0])

    def test_DictionaryXml(self) :
        it = self._sc._d.find(self._sc._m0)
        self.assertNotEqual(it, self._sc._d.end())
        self.assertEqual(self._sc._m0.str(), it.value().str())

        it = self._sc._d.find(self._sc._m1)
        self.assertNotEqual(it, self._sc._d.end())
        self.assertEqual(self._sc._m1.str(), it.value().str())

        it = self._sc._d.find(self._sc._m2)
        self.assertNotEqual(it, self._sc._d.end())
        self.assertEqual(self._sc._m2.str(), it.value().str())

        it = self._sc._d.find(self._sc._m3)
        self.assertNotEqual(it, self._sc._d.end())
        self.assertEqual(self._sc._m3.str(), it.value().str())

        it = self._sc._d.find(self._sc._m4)
        self.assertNotEqual(it, self._sc._d.end())
        self.assertEqual(self._sc._m4.str(), it.value().str())

    def test_DictionaryDumpOstream(self) :
        self.assertEqual(self._sc._xml, self._sc._d.str())

    def test_DictionaryDumpXml(self) :
        self.assertEqual(self._sc._xml, self._sc._d.strXml())

#     def test_DictionaryDumpVerilog(self) :
#         self.assertEqual(self._sc._xml, self._sc._d.strVerilog())
#  
    def test_DictionaryDumpJson(self) :
        self.assertEqual(pyutl.fnCast(self._sc._json), self._sc._d.strJson())

if __name__ == '__main__': 
    unittest.main() 
