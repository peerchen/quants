#coding:utf-8
import thostdef
import string

def MakeKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay, PeriodType, PeriodValue) :
	'''
	Brief : 
		按照交易日生成KBar
	Arguments :
		Symbol					string			品种
		ExchangeID				string			交易所
		InstrumentID			string			合约
		TicksByTradingDay		list<dict>		1个交易日的ticks数据列表
		PeridType				string			kbar周期类型, thostdef.THOST_KBAR_PERIOD_TYPE_*
		PeridValue				int				kbar周期值
	Returns :
		ListKBar				list<dict>		1个交易日的kbar列表 
	'''
	barindex = 0
	listKBar = []
	dictKBar = {}
	dictKBar['PeriodType'] = PeriodType
	dictKBar['PeriodValue'] = PeriodValue
	dictKBar['Symbol'] = Symbol
	dictKBar['InstrumentID'] = InstrumentID
	dictKBar["TradingDate"] = string.atoi(TradingDay, 10)
	dictKBar["PreClosePrice"] = 0;
	dictKBar["PreVolume"] = 0;
	dictKBar["PreOpenInterest"] = 0
	dictKBar["Status"] = thostdef.THOST_KBAR_STATUS_NULL
	for dictTick in TicksByTradingDay :
		if dictKBar["Status"] == thostdef.THOST_KBAR_STATUS_NULL :
			UpdateNullKBar(dictKBar, dictTick)
		elif dictKBar["Status"] == thostdef.THOST_KBAR_STATUS_OPEN :
			UpdateOpenKBar(dictKBar, dictTick)
			FloorBeginTime = FloorTime(PeriodType, PeriodValue, dictKBar["BeginTime"])
			FloorEndTime = FloorTime(PeriodType, PeriodValue, dictKBar["EndTime"])
			if (FloorBeginTime != FloorEndTime) :
				if thostdef.THOST_IS_SECTIONEND(Symbol, ExchangeID, FloorEndTime) :
					continue;
				dictKBar["BarIndex"] = barindex
				barindex = barindex + 1
				dictKBar["BeginTime"] = FloorBeginTime
				dictKBar["EndTime"] = FloorEndTime
				dictKBar["Status"] = thostdef.THOST_KBAR_STATUS_CLOSE				
				listKBar.append(dictKBar.copy())
				dictKBar["Status"] = thostdef.THOST_KBAR_STATUS_NULL;
				dictKBar["Ticks"] = 0;
				dictKBar["BeginTime"] = dictKBar["EndTime"];
				dictKBar["PreClosePrice"] = dictKBar["ClosePrice"];
				dictKBar["PreVolume"] = dictTick["Volume"];
				dictKBar["PreOpenInterest"] = dictTick["OpenInterest"]
	
	#收盘最后1个kbar
	if dictKBar["BeginTime"] < thostdef.GetCloseTime(Symbol, ExchangeID) :
		dictKBar["BarIndex"] = barindex
		dictKBar["EndTime"] = thostdef.GetCloseTime(Symbol, ExchangeID)
		dictKBar["Status"] = thostdef.THOST_KBAR_STATUS_CLOSE
		listKBar.append(dictKBar.copy())
	return listKBar
		
def Make1MinKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay) :
	'''
	Brief : 
		按照交易日生成1分钟kbar
	Arguments :
		Symbol					string			品种
		ExchangeID				string			交易所
		InstrumentID			string			合约
		TicksByTradingDay		list<dict>		1个交易日的ticks数据列表
	'''
	return MakeKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay, thostdef.THOST_KBAR_PERIOD_TYPE_MINUTE, 1)

def Make5MinKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay) :
	'''
	Brief : 
		按照交易日生成5分钟kbar
	Arguments :
		Symbol					string			品种
		ExchangeID				string			交易所
		InstrumentID			string			合约
		TicksByTradingDay		list<dict>		1个交易日的ticks数据列表
	'''
	return MakeKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay, thostdef.THOST_KBAR_PERIOD_TYPE_MINUTE, 5)

def Make15MinKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay) :
	'''
	Brief : 
		按照交易日生成15分钟kbar
	Arguments :
		Symbol					string			品种
		ExchangeID				string			交易所
		InstrumentID			string			合约
		TicksByTradingDay		list<dict>		1个交易日的ticks数据列表
	'''
	return MakeKBarByTradingDay(TradingDay, Symbol, ExchangeID, InstrumentID, TicksByTradingDay, thostdef.THOST_KBAR_PERIOD_TYPE_MINUTE, 15)


def FloorTime(PeriodType, PeriodValue, Time) :
	'''
	Brief : 
		根据PeriodType和PeriodValue向上取整时间Time
		例:s, 5, 135806 => 135805
		   m, 1, 135805 => 135800
		   m, 5, 135805 => 135500
		   m, 15,135805 => 134500
	Arguments :
		PeriodType				string			kbar周期类型, thostdef.THOST_KBAR_PERIOD_TYPE_*
		PeriodValue				int				kbar周期值
		Time					int				HHmmss
	'''
	if PeriodType == thostdef.THOST_KBAR_PERIOD_TYPE_SECOND :
		return (Time / 100) * 100 + (Time % 100 / PeriodValue) * PeriodValue
	elif PeriodType == thostdef.THOST_KBAR_PERIOD_TYPE_MINUTE :
		return (Time / 10000) * 10000 + ((Time / 100) % 100 / PeriodValue) * PeriodValue * 100
	return -1

def UpdateNullKBar(dictKBar, dictTick) :
	'''
	Brief : 
		第一次更新KBar
	Arguments :
		dictKBar				dict			kbar
		dictTick				dict			tick
	'''
	dictKBar["Date"] = dictTick["ActionDay"]
	dictKBar["BeginTime"] = dictTick["UpdateTime"]
	dictKBar["EndTime"] = dictTick["UpdateTime"]
	dictKBar["OpenPrice"] = dictTick["LastPrice"]
	dictKBar["HighPrice"] = dictTick["LastPrice"]
	dictKBar["LowPrice"] = dictTick["LastPrice"]
	dictKBar["ClosePrice"] = dictTick["LastPrice"]
	dictKBar["AskPrice"] = dictTick["AskPrice1"]
	dictKBar["AskVolume"] = dictTick["AskVolume1"]
	dictKBar["BidPrice"] = dictTick["BidPrice1"]
	dictKBar["BidVolume"] = dictTick["BidVolume1"]
	dictKBar["Volume"] = dictTick["Volume"]
	dictKBar["OpenInterest"] = dictTick["OpenInterest"]
	dictKBar["Ticks"] = 1
	dictKBar["Status"] = thostdef.THOST_KBAR_STATUS_OPEN

def UpdateOpenKBar(dictKBar, dictTick) :
	'''
	Brief : 
		更新Open的KBar
	Arguments :
		dictKBar				dict			kbar
		dictTick				dict			tick
	'''
	dictKBar["Date"] = dictTick["ActionDay"]
	dictKBar["EndTime"] = dictTick["UpdateTime"]
	if (dictTick["LastPrice"] > dictKBar["HighPrice"]) :
		dictKBar["HighPrice"] = dictTick["LastPrice"]
	if dictTick["LastPrice"] < dictKBar["LowPrice"] :
		dictKBar["LowPrice"] = dictTick["LastPrice"]
	dictKBar["ClosePrice"] = dictTick["LastPrice"]
	dictKBar["AskPrice"] = dictTick["AskPrice1"]
	dictKBar["AskVolume"] = dictTick["AskVolume1"]
	dictKBar["BidPrice"] = dictTick["BidPrice1"]
	dictKBar["BidVolume"] = dictTick["BidVolume1"]
	dictKBar["Volume"] = dictTick["Volume"]- dictKBar["PreVolume"]
	dictKBar["OpenInterest"] = dictTick["OpenInterest"]
	dictKBar["Ticks"] = dictKBar["Ticks"] + 1
