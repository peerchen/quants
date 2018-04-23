#coding:utf-8
import os
import redis
import csv
import time
import ujson

#redis服务器ip
host = '127.0.0.1'
#redis服务器端口
port = 6379

#注意:不要用~代替home目录
#csv数据存放目录
aimdir = '/home/quants/tickdata'

#交易日
TradingDay = ""

#连接Redis
def ConnectRedis() :
	redis_context = redis.StrictRedis(host='localhost', port=6379, db=0)
	print GetCurrentTime() + " redis:connected."
	return redis_context

#获取交易日
def GetTradingDay(redis_context) :
	global TradingDay
	TradingDay = redis_context.get('TradingDay')
	if (not os.path.exists(aimdir + "/" + TradingDay)):
		os.system("mkdir -p " + aimdir + "/" + TradingDay)
	print GetCurrentTime() + " TradingDay:" + TradingDay

#创建所有合约当日tick数据的csv
def Make_ticks_csv(redis_context) :
	#tick文件的表头
	listInstrumentTickCSVHead = ["InstrumentID","ActionDay","UpdateTime","UpdateMillisec","LastPrice","Volume","Turnover","OpenInterest","BidPrice1","BidVolume1","AskPrice1","AskVolume1","BidPrice2","BidVolume2","AskPrice2","AskVolume2","BidPrice3","BidVolume3","AskPrice3","AskVolume3","BidPrice4","BidVolume4","AskPrice4","AskVolume4","BidPrice5","BidVolume5","AskPrice5","AskVolume5"]
	#最新快照的表头
	listInstrumentSnapshotCSVHead = ["InstrumentID","ActionDay","UpdateTime","UpdateMillisec","ExchangeID","LastPrice","PreSettlementPrice","PreClosePrice","PreOpenInterest","OpenPrice","HighestPrice","LowestPrice","Volume","Turnover","OpenInterest","UpperLimitPrice","LowerLimitPrice","BidPrice1","BidVolume1","AskPrice1","AskVolume1","BidPrice2","BidVolume2","AskPrice2","AskVolume2","BidPrice3","BidVolume3","AskPrice3","AskVolume3","BidPrice4","BidVolume4","AskPrice4","AskVolume4","BidPrice5","BidVolume5","AskPrice5","AskVolume5","TradingDay"]
	#获取合约列表长度
	nInstrumentListCount = redis_context.llen(TradingDay + ":instrumentinfo");
	#获取合约列表信息
	listInstrumentInfo = redis_context.lrange(TradingDay + ":instrumentinfo", 1, nInstrumentListCount-1);

	#创建存储快照的csv文件
	InstrumentSnapshotFileName = aimdir + "/" + TradingDay + "/snapshot.csv"
	InstrumentSnapshotCSVFile = open(InstrumentSnapshotFileName, 'wb')
	InstrumentSnapshotCSVWriter = csv.writer(InstrumentSnapshotCSVFile)
	InstrumentSnapshotCSVWriter.writerow(listInstrumentSnapshotCSVHead)

	dictSymbol = {}
	#遍历合约列表
	for strInstrumentInfo in listInstrumentInfo:
		strInstrumentInfo_utf8 = strInstrumentInfo.decode('gbk').encode('utf8')
		jsonInstrumentInfo = ujson.loads(strInstrumentInfo_utf8)
		listSymbol = []
		if dictSymbol.has_key(jsonInstrumentInfo["ProductID"]) :
			listSymbol = dictSymbol[jsonInstrumentInfo["ProductID"]]
		listSymbol.append(jsonInstrumentInfo)
		dictSymbol[jsonInstrumentInfo["ProductID"]] = listSymbol;

	for strSymbol in dictSymbol.keys():
		listSymbol = dictSymbol[strSymbol]
		print GetCurrentTime() + " Starting get symbol:" + strSymbol
		#创建品种目录
		symboldatadir = aimdir + "/" + TradingDay + "/" + strSymbol
		if (not os.path.exists(symboldatadir)):
			os.system("mkdir -p " + symboldatadir)
		dictHandledInstrument = {}
		#开始遍历品种中的合约
		for dictInstrumentInfo in listSymbol :
			#handler unique
			if dictHandledInstrument.has_key(dictInstrumentInfo["InstrumentID"]) :
				continue
			dictHandledInstrument[dictInstrumentInfo["InstrumentID"]] = 1
			#创建合约CSV文件
			InstrumentFileName = symboldatadir + "/" + dictInstrumentInfo["InstrumentID"] + ".csv"
			InstrumentCSVFile = open(InstrumentFileName, 'wb')
			InstrumentCSVWriter = csv.writer(InstrumentCSVFile)
			InstrumentCSVWriter.writerow(listInstrumentTickCSVHead)

			#从redis中获取合约的ticks数据集合
			print GetCurrentTime() + " processing:" + dictInstrumentInfo["InstrumentID"]
			nInstrumentTicksLen = redis_context.llen(TradingDay + ":ticks:" + dictInstrumentInfo["InstrumentID"]);		
			listInstrumentTicks = redis_context.lrange(TradingDay + ":ticks:" + dictInstrumentInfo["InstrumentID"], 0, nInstrumentTicksLen-1)

			#range 0 0为快照, 写入snapshot.csv中
			if nInstrumentTicksLen > 0 :
				strInstrumentSnapshot = listInstrumentTicks[0]
				strInstrumentSnapshot_utf8 = strInstrumentSnapshot.decode('gbk').encode('utf8')	
				dictInstrumentSnapshot = ujson.loads(strInstrumentSnapshot_utf8)
				listInstrumentSnapshot = MakeSnapshot_Row(dictInstrumentSnapshot)
				InstrumentSnapshotCSVWriter.writerow(listInstrumentSnapshot)

			#开始写ticks到csv中
			listInstrumentTicks.reverse()
			listOneInstrumentTickTuple = []
			for strInstrumentTick in listInstrumentTicks:
				strInstrumentTick_utf8 = strInstrumentTick.decode('gbk').encode('utf8')	
				dictInstrumentTick = ujson.loads(strInstrumentTick_utf8)
				listOneInstrumentTick = MakeTick_Row(dictInstrumentTick)
				listOneInstrumentTickTuple.append(tuple(listOneInstrumentTick))	
			InstrumentCSVWriter.writerows(listOneInstrumentTickTuple)	
			InstrumentCSVFile.close()
	InstrumentSnapshotCSVFile.close()

def GetCurrentTime() :
	return time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

#快照行
def MakeSnapshot_Row(dictInstrumentTick) :
	listOneInstrumentSnapshot = []	
	listOneInstrumentSnapshot.append(dictInstrumentTick["InstrumentID"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["ActionDay"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["UpdateTime"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["UpdateMillisec"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["ExchangeID"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["LastPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["PreSettlementPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["PreClosePrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["PreOpenInterest"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["OpenPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["HighestPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["LowestPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["Volume"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["Turnover"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["OpenInterest"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["UpperLimitPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["LowerLimitPrice"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidPrice1"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidVolume1"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskPrice1"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskVolume1"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidPrice2"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidVolume2"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskPrice2"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskVolume2"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidPrice3"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidVolume3"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskPrice3"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskVolume3"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidPrice4"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidVolume4"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskPrice4"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskVolume4"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidPrice5"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["BidVolume5"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskPrice5"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["AskVolume5"])
	listOneInstrumentSnapshot.append(dictInstrumentTick["TradingDay"])
	return listOneInstrumentSnapshot

#tick行
def MakeTick_Row(dictInstrumentTick) :
	listOneInstrumentTick = []	
	listOneInstrumentTick.append(dictInstrumentTick["InstrumentID"])
	listOneInstrumentTick.append(dictInstrumentTick["ActionDay"])
	listOneInstrumentTick.append(dictInstrumentTick["UpdateTime"])
	listOneInstrumentTick.append(dictInstrumentTick["UpdateMillisec"])
	listOneInstrumentTick.append(dictInstrumentTick["LastPrice"])
	listOneInstrumentTick.append(dictInstrumentTick["Volume"])
	listOneInstrumentTick.append(dictInstrumentTick["Turnover"])
	listOneInstrumentTick.append(dictInstrumentTick["OpenInterest"])
	listOneInstrumentTick.append(dictInstrumentTick["BidPrice1"])	
	listOneInstrumentTick.append(dictInstrumentTick["BidVolume1"])
	listOneInstrumentTick.append(dictInstrumentTick["AskPrice1"])
	listOneInstrumentTick.append(dictInstrumentTick["AskVolume1"])
	listOneInstrumentTick.append(dictInstrumentTick["BidPrice2"])
	listOneInstrumentTick.append(dictInstrumentTick["BidVolume2"])
	listOneInstrumentTick.append(dictInstrumentTick["AskPrice2"])
	listOneInstrumentTick.append(dictInstrumentTick["AskVolume2"])
	listOneInstrumentTick.append(dictInstrumentTick["BidPrice3"])
	listOneInstrumentTick.append(dictInstrumentTick["BidVolume3"])
	listOneInstrumentTick.append(dictInstrumentTick["AskPrice3"])
	listOneInstrumentTick.append(dictInstrumentTick["AskVolume3"])
	listOneInstrumentTick.append(dictInstrumentTick["BidPrice4"])
	listOneInstrumentTick.append(dictInstrumentTick["BidVolume4"])
	listOneInstrumentTick.append(dictInstrumentTick["AskPrice4"])
	listOneInstrumentTick.append(dictInstrumentTick["AskVolume4"])
	listOneInstrumentTick.append(dictInstrumentTick["BidPrice5"])
	listOneInstrumentTick.append(dictInstrumentTick["BidVolume5"])
	listOneInstrumentTick.append(dictInstrumentTick["AskPrice5"])
	listOneInstrumentTick.append(dictInstrumentTick["AskVolume5"])
	return listOneInstrumentTick

def PackCSV() :
	print "Start packing tickdata."
	targzfilename = TradingDay + ".tar.gz"
	tar_ret = os.system("curdir=`pwd`;cd " + aimdir + ";tar -zcvf " + targzfilename + " " + TradingDay + "/;cd $curdir")

	if tar_ret == 0:
		rm_ret = os.system("rm -rf " + aimdir + "/" + TradingDay)
		print GetCurrentTime() + ' Done.'
	else:
		print GetCurrentTime() + " Package failed."

if __name__ == "__main__":  
	redis_context = ConnectRedis()
	GetTradingDay(redis_context)
	Make_ticks_csv(redis_context)
	PackCSV()
