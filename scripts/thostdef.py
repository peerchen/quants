#coding:utf-8

#THOST_EXCHANGEID_*
THOST_EXCHANGEID_CFFEX	= "CFFEX"
THOST_EXCHANGEID_SHFE   = "SHFE"
THOST_EXCHANGEID_DCE	= "DCE"
THOST_EXCHANGEID_CZCE	= "CZCE"
THOST_EXCHANGEID_SGE	= "SGE"

#品种与交易所ID映射表
dictSymbolExchangeID = {}
#设置SHFE
dictSymbolExchangeID["zn"] = "SHFE"
dictSymbolExchangeID["ag"] = "SHFE"
dictSymbolExchangeID["al"] = "SHFE"
dictSymbolExchangeID["au"] = "SHFE"
dictSymbolExchangeID["cu"] = "SHFE"
dictSymbolExchangeID["ru"] = "SHFE"
dictSymbolExchangeID["pb"] = "SHFE"
dictSymbolExchangeID["rb"] = "SHFE"
dictSymbolExchangeID["fu"] = "SHFE"
dictSymbolExchangeID["ni"] = "SHFE"
dictSymbolExchangeID["wr"] = "SHFE"
dictSymbolExchangeID["hc"] = "SHFE"
dictSymbolExchangeID["bu"] = "SHFE"
dictSymbolExchangeID["sn"] = "SHFE"
#设置CFFEX
dictSymbolExchangeID["TF"] = "CFFEX"
dictSymbolExchangeID["IC"] = "CFFEX"
dictSymbolExchangeID["IF"] = "CFFEX"
dictSymbolExchangeID["IH"] = "CFFEX"
dictSymbolExchangeID["T"] = "CFFEX"
#设置DCE
dictSymbolExchangeID["cs"] = "DCE"
dictSymbolExchangeID["pp"] = "DCE"
dictSymbolExchangeID["bb"] = "DCE"
dictSymbolExchangeID["jm"] = "DCE"
dictSymbolExchangeID["fb"] = "DCE"
dictSymbolExchangeID["jd"] = "DCE"
dictSymbolExchangeID["a"] = "DCE"
dictSymbolExchangeID["c"] = "DCE"
dictSymbolExchangeID["b"] = "DCE"
dictSymbolExchangeID["i"] = "DCE"
dictSymbolExchangeID["j"] = "DCE"
dictSymbolExchangeID["m"] = "DCE"
dictSymbolExchangeID["l"] = "DCE"
dictSymbolExchangeID["p"] = "DCE"
dictSymbolExchangeID["v"] = "DCE"
dictSymbolExchangeID["y"] = "DCE"
#设置CZCE
dictSymbolExchangeID["JR"] = "CZCE"
dictSymbolExchangeID["WH"] = "CZCE"
dictSymbolExchangeID["FG"] = "CZCE"
dictSymbolExchangeID["RS"] = "CZCE"
dictSymbolExchangeID["LR"] = "CZCE"
dictSymbolExchangeID["RM"] = "CZCE"
dictSymbolExchangeID["RI"] = "CZCE"
dictSymbolExchangeID["TA"] = "CZCE"
dictSymbolExchangeID["CF"] = "CZCE"
dictSymbolExchangeID["ZC"] = "CZCE"
dictSymbolExchangeID["MA"] = "CZCE"
dictSymbolExchangeID["OI"] = "CZCE"
dictSymbolExchangeID["SR"] = "CZCE"
dictSymbolExchangeID["SM"] = "CZCE"
dictSymbolExchangeID["PM"] = "CZCE"
dictSymbolExchangeID["SF"] = "CZCE"

#THOST_SECTIONTIME_*
THOST_SECTIONTIME_090000 = 90000
THOST_SECTIONTIME_091500 = 91500
THOST_SECTIONTIME_093000 = 93000
THOST_SECTIONTIME_101500 = 101500
THOST_SECTIONTIME_103000 = 103000
THOST_SECTIONTIME_113000 = 113000
THOST_SECTIONTIME_130000 = 130000
THOST_SECTIONTIME_133000 = 133000
THOST_SECTIONTIME_145900 = 145900
THOST_SECTIONTIME_150000 = 150000
THOST_SECTIONTIME_151500 = 151500
THOST_SECTIONTIME_153000 = 153000
THOST_SECTIONTIME_200000 = 200000
THOST_SECTIONTIME_210000 = 210000
THOST_SECTIONTIME_230000 = 230000
THOST_SECTIONTIME_233000 = 233000
THOST_SECTIONTIME_240000 = 240000
THOST_SECTIONTIME_000000 = 0
THOST_SECTIONTIME_010000 = 10000
THOST_SECTIONTIME_023000 = 23000

THOST_OFFSETTIME_030000 = 30000

#THOST_KBAR_PERIOD_TYPE_*
THOST_KBAR_PERIOD_TYPE_TICK			= 't'
THOST_KBAR_PERIOD_TYPE_SECOND		= 'S'
THOST_KBAR_PERIOD_TYPE_MINUTE		= 'M'
THOST_KBAR_PERIOD_TYPE_HOUR			= 'H'
THOST_KBAR_PERIOD_TYPE_DAY			= 'd'
THOST_KBAR_PERIOD_TYPE_WEEK			= 'w'
THOST_KBAR_PERIOD_TYPE_MONTH		= 'm'
THOST_KBAR_PERIOD_TYPE_YEAD			= 'y'

#THOST_KBAR_STATUS_*
THOST_KBAR_STATUS_MASK				= 15
THOST_KBAR_STATUS_NULL				= 1
THOST_KBAR_STATUS_OPEN				= 2
THOST_KBAR_STATUS_CLOSE				= 4
THOST_KBAR_STATUS_TRADINGCLOSE      = 8

def KBAR_ISNULL(X) :
	return X&THOST_KBAR_STATUS_NULL

def KBAR_ISOPEN(X) :
	return X&THOST_KBAR_STATUS_OPEN

def KBAR_ISCLOSE(X) :
	return X&THOST_KBAR_STATUS_CLOSE

def KBAR_ISTRADINGCLOSE(X) :
	X&THOST_KBAR_STATUS_TRADINGCLOSE

#获取收盘时间
def GetCloseTime(Symbol, ExchangeID) :
	if ExchangeID == THOST_EXCHANGEID_SGE :
		return 153000
	elif Symbol == "T" or Symbol == "TF" :
		return 151500
	else :
		return 150000

#获取KBar区间
def MakeKBarSection(Symbol, ExchangeID, PeridType, PeriodValue) :
	listKBarSection = []
	listKBarNightSection = MakeKBarNightSection(Symbol, ExchangeID, PeridType, PeriodValue)
	listKBarDaySection = MakeKBarDaySection(Symbol, ExchangeID, PeridType, PeriodValue)
	listKBarSection.extend(listKBarNightSection)
	listKBarSection.extend(listKBarDaySection)
	return listKBarSection

#获取夜盘KBar区间
def MakeKBarNightSection(Symbol, ExchangeID, PeridType, PeriodValue) :
	listKBarSection = []
	for h in range(20, 24) :
		for m in range(0, 60, PeriodValue) :
			Time = h * 10000 + m * 100
			#当前时间为交易时间且不是小节末尾
			if THOST_IS_NIGHTTRADINGTIME(Symbol, ExchangeID, Time) and THOST_IS_NIGHTSECTIONEND(Symbol, ExchangeID, Time) == False:
				listKBarSection.append(Time)

	for h in range(0, 3) :
		for m in range(0, 60, PeriodValue) :
			Time = h * 10000 + m * 100
			#当前时间为交易时间且不是小节末尾
			if THOST_IS_NIGHTTRADINGTIME(Symbol, ExchangeID, Time) and THOST_IS_NIGHTSECTIONEND(Symbol, ExchangeID, Time) == False:
				listKBarSection.append(Time)

	return listKBarSection
	
#获取日盘KBar区间
def MakeKBarDaySection(Symbol, ExchangeID, PeridType, PeriodValue) :
	listKBarSection = []
	for h in range(9, 16) :
		for m in range(0, 60, PeriodValue) :
			Time = h * 10000 + m * 100
			#当前时间为交易时间且不是小节末尾
			if THOST_IS_DAYTRADINGTIME(Symbol, ExchangeID, Time) and THOST_IS_DAYSECTIONEND(Symbol, ExchangeID, Time) == False:
				listKBarSection.append(Time)
	return listKBarSection

#日盘交易时间
def THOST_IS_DAYTRADINGTIME(symbol, exchangeid, time) :
	if exchangeid == THOST_EXCHANGEID_SGE :
		if (time >= THOST_SECTIONTIME_090000 and time <= THOST_SECTIONTIME_113000) or (time >= THOST_SECTIONTIME_133000 and time <= THOST_SECTIONTIME_153000) :
			return True
	elif exchangeid == THOST_EXCHANGEID_CFFEX :
		if symbol == "T" or symbol == "TF" :
			if (time >= THOST_SECTIONTIME_091500 and time <= THOST_SECTIONTIME_113000) or (time >= THOST_SECTIONTIME_130000 and time <= THOST_SECTIONTIME_151500) :
				return True
		else :
			if (time >= THOST_SECTIONTIME_093000 and time <= THOST_SECTIONTIME_113000) or (time >= THOST_SECTIONTIME_130000 and time <= THOST_SECTIONTIME_150000) :
				return True
	else :
		if (time >= THOST_SECTIONTIME_090000 and time <= THOST_SECTIONTIME_101500) or (time >= THOST_SECTIONTIME_103000 and time <= THOST_SECTIONTIME_113000) or (time >= THOST_SECTIONTIME_133000 and time <= THOST_SECTIONTIME_150000) :
			return True
	return False
	
#夜盘交易时间
def THOST_IS_NIGHTTRADINGTIME(symbol, exchangeid, time) :
	if exchangeid == THOST_EXCHANGEID_SGE :
		if (time >= THOST_SECTIONTIME_200000 and time <= THOST_SECTIONTIME_240000) or (time >= THOST_SECTIONTIME_000000 and time <= THOST_SECTIONTIME_023000) :
			return True
	elif exchangeid == THOST_EXCHANGEID_SHFE :
		if symbol == "ru" or symbol == "rb" :
			if time >= THOST_SECTIONTIME_210000 and time <= THOST_SECTIONTIME_230000 : 
				return True
		elif symbol == "au" or symbol == "ag" :
			if (time >= THOST_SECTIONTIME_210000 and time <= THOST_SECTIONTIME_240000) or (time >= THOST_SECTIONTIME_000000 and time <= THOST_SECTIONTIME_023000) :
				return True
		else :
			if (time >= THOST_SECTIONTIME_210000 and time <= THOST_SECTIONTIME_240000) or (time >= THOST_SECTIONTIME_000000 and time <= THOST_SECTIONTIME_010000) :
				return True
	elif exchangeid != THOST_EXCHANGEID_CFFEX :
		if time >= THOST_SECTIONTIME_210000 and time <= THOST_SECTIONTIME_233000 :
			return True
	return False

#交易日时间
def THOST_IS_TRADINGTIME(symbol, exchangeid, time) :
	if THOST_IS_DAYTRADINGTIME(symbol, exchangeid, time) or THOST_IS_NIGHTTRADINGTIME(symbol, exchangeid, time) :
		return True
	return False

#判断交易小节开始
def THOST_IS_SECTIONBEGIN(symbol, exchangeid, time) :
	if exchangeid == THOST_EXCHANGEID_SGE :
		if time == THOST_SECTIONTIME_090000 or time == THOST_SECTIONTIME_133000 or time == THOST_SECTIONTIME_200000 :
			return True
	elif exchangeid == THOST_EXCHANGEID_CFFEX :
		if time == THOST_SECTIONTIME_130000 :
			return True
		if symbol == "T" or symbol == "TF" :
			if time == THOST_SECTIONTIME_091500 :
				return True
		elif time == THOST_SECTIONTIME_093000 :
			return True
	else :
		if time == THOST_SECTIONTIME_090000 or time == THOST_SECTIONTIME_103000 or 	time == THOST_SECTIONTIME_133000 or time == THOST_SECTIONTIME_210000 :
			return True
	return False

#判断日盘交易小节结束
def THOST_IS_DAYSECTIONEND(symbol, exchangeid, time) :
	if exchangeid == THOST_EXCHANGEID_SGE :
		if time == THOST_SECTIONTIME_113000 :
			return True
	elif exchangeid == THOST_EXCHANGEID_CFFEX :
		if time == THOST_SECTIONTIME_113000 :
			return True
	else :
		if time == THOST_SECTIONTIME_101500 or time == THOST_SECTIONTIME_113000 :
			return True
	return False

#判断夜盘交易小节结束
def THOST_IS_NIGHTSECTIONEND(symbol, exchangeid, time) :
	if exchangeid == THOST_EXCHANGEID_SGE :
		if time == THOST_SECTIONTIME_023000 :
			return True
	elif exchangeid == THOST_EXCHANGEID_SHFE :
		if symbol == "ru" or symbol == "rb" :
			if time == THOST_SECTIONTIME_230000 :
				return True
		elif symbol == "au" or symbol == "ag" :
			if time == THOST_SECTIONTIME_023000 :
				return True
		else :
			if time == THOST_SECTIONTIME_010000 :
				return True
	else :
		if time == THOST_SECTIONTIME_233000 :
			return True
	return False 

#判断交易小节的起始
def THOST_IS_SECTIONEND(symbol, exchangeid, time) :
	if THOST_IS_DAYSECTIONEND(symbol, exchangeid, time) or THOST_IS_NIGHTSECTIONEND(symbol, exchangeid, time) :
		return True
	return False
