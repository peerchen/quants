#coding:utf-8
import thostdef, unittest

class MakeKBarSectionSizeTestCase (unittest.TestCase) :
	def testRu1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ru", "SHFE", "m", 1)) == 346, 'Ru 1min MakeKBarSectionSize ERROR!')

	def testRu5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ru", "SHFE", "m", 5)) == 70, 'Ru 5min MakeKBarSectionSize ERROR!')

	def testRu15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ru", "SHFE", "m", 15)) == 24, 'Ru 15min MakeKBarSectionSize ERROR!')

	def testRb1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("rb", "SHFE", "m", 1)) == 346, 'Rb 1min MakeKBarSectionSize ERROR!')

	def testRb5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("rb", "SHFE", "m", 5)) == 70, 'Rb 5min MakeKBarSectionSize ERROR!')

	def testRb15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("rb", "SHFE", "m", 15)) == 24, 'Rb 15min MakeKBarSectionSize ERROR!')

	def testZn1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("zn", "SHFE", "m", 1)) == 466, 'Zn 1min MakeKBarSectionSize ERROR!')

	def testZn5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("zn", "SHFE", "m", 5)) == 94, 'Zn 5min MakeKBarSectionSize ERROR!')

	def testZn15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("zn", "SHFE", "m", 15)) == 32, 'Zn 15min MakeKBarSectionSize ERROR!')

	def testAu1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("au", "SHFE", "m", 1)) == 556, 'Au 1min MakeKBarSectionSize ERROR!')

	def testAu5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("au", "SHFE", "m", 5)) == 112, 'Au 5min MakeKBarSectionSize ERROR!')

	def testAu15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("au", "SHFE", "m", 15)) == 38, 'Au 15min MakeKBarSectionSize ERROR!')

	def testAg1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ag", "SHFE", "m", 1)) == 556, 'Ag 1min MakeKBarSectionSize ERROR!')

	def testAg5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ag", "SHFE", "m", 5)) == 112, 'Ag 5min MakeKBarSectionSize ERROR!')

	def testAg15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("ag", "SHFE", "m", 15)) == 38, 'Ag 15min MakeKBarSectionSize ERROR!')

	def testM1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("m", "DCE", "m", 1)) == 376, 'M 1min MakeKBarSectionSize ERROR!')

	def testM5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("m", "DCE", "m", 5)) == 76, 'M 5min MakeKBarSectionSize ERROR!')

	def testM15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("m", "DCE", "m", 15)) == 26, 'M 15min MakeKBarSectionSize ERROR!')

	def testRM1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("RM", "CZCE", "m", 1)) == 376, 'RM 1min MakeKBarSectionSize ERROR!')

	def testRM5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("RM", "CZCE", "m", 5)) == 76, 'RM 5min MakeKBarSectionSize ERROR!')

	def testRM15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("RM", "CZCE", "m", 15)) == 26, 'RM 15min MakeKBarSectionSize ERROR!')

	def testIF1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IF", "CFFEX", "m", 1)) == 241, 'IF 1min MakeKBarSectionSize ERROR!')

	def testIF5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IF", "CFFEX", "m", 5)) == 49, 'IF 5min MakeKBarSectionSize ERROR!')

	def testIF15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IF", "CFFEX", "m", 15)) == 17, 'IF 5min MakeKBarSectionSize ERROR!')

	def testIH1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IH", "CFFEX", "m", 1)) == 241, 'IH 1min MakeKBarSectionSize ERROR!')

	def testIH5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IH", "CFFEX", "m", 5)) == 49, 'IH 5min MakeKBarSectionSize ERROR!')

	def testIH15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("IH", "CFFEX", "m", 15)) == 17, 'IH 15min MakeKBarSectionSize ERROR!')

	def testTF1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("TF", "CFFEX", "m", 1)) == 271, 'TF 1min MakeKBarSectionSize ERROR!')

	def testTF5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("TF", "CFFEX", "m", 5)) == 55, 'TF 5min MakeKBarSectionSize ERROR!')

	def testTF15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("TF", "CFFEX", "m", 15)) == 19, 'TF 15min MakeKBarSectionSize ERROR!')

	def testT1min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("T", "CFFEX", "m", 1)) == 271, 'T 1min MakeKBarSectionSize ERROR!')

	def testT5min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("T", "CFFEX", "m", 5)) == 55, 'T 5min MakeKBarSectionSize ERROR!')

	def testT15min(self) :
		self.failUnless(len(thostdef.MakeKBarSection("T", "CFFEX", "m", 15)) == 19, 'T 15min MakeKBarSectionSize ERROR!')

if __name__ == '__main__' :
	unittest.main()