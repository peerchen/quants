#coding:utf-8
import os
import redis
import csv
import time
import ujson

def GetCurrentTime() :
	return time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

#redis服务器ip
host = '127.0.0.1'
#redis服务器端口
port = 6379

#注意:不要用~代替home目录
#csv数据存放目录
aimdir = '/home/quants/tickdata'

r = redis.StrictRedis(host='localhost', port=6379, db=0)
print GetCurrentTime() + " redis:connected."

#获取交易日
TradingDay = r.get('TradingDay')
print GetCurrentTime() + " TradingDay:" + TradingDay

datadir = aimdir + "/" + TradingDay
#创建交易日目录
if (not os.path.exists(datadir)):
	os.system("mkdir -p " + datadir)

listInstrumentTickCSVHead = ["InstrumentID","ActionDay","UpdateTime","UpdateMillisec","ExchangeID","LastPrice","PreSettlementPrice","PreClosePrice","PreOpenInterest","OpenPrice","HighestPrice","LowestPrice","Volume","Turnover","OpenInterest","UpperLimitPrice","LowerLimitPrice","BidPrice1","BidVolume1","AskPrice1","AskVolume1","BidPrice2","BidVolume2","AskPrice2","AskVolume2","BidPrice3","BidVolume3","AskPrice3","AskVolume3","BidPrice4","BidVolume4","AskPrice4","AskVolume4","BidPrice5","BidVolume5","AskPrice5","AskVolume5","TradingDay"]

#获取合约列表长度
nInstrumentListCount = r.llen(TradingDay + ":instrumentinfo");
#获取合约列表信息
listInstrumentInfo = r.lrange(TradingDay + ":instrumentinfo", 1, nInstrumentListCount-1);


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
	symboldatadir = datadir + "/" + strSymbol
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
		#os.system("touch " + InstrumentFileName)
		InstrumentCSVFile = open(InstrumentFileName, 'wb')
		InstrumentCSVWriter = csv.writer(InstrumentCSVFile)
		InstrumentCSVWriter.writerow(listInstrumentTickCSVHead)

		print GetCurrentTime() + " processing:" + dictInstrumentInfo["InstrumentID"]
		nInstrumentTicksLen = r.llen(TradingDay + ":ticks:" + dictInstrumentInfo["InstrumentID"]);		
		listInstrumentTicks = r.lrange(TradingDay + ":ticks:" + dictInstrumentInfo["InstrumentID"], 0, nInstrumentTicksLen-1)
		listInstrumentTicks.reverse()
		listOneInstrumentTickTuple = []
		for strInstrumentTick in listInstrumentTicks:
			strInstrumentTick_utf8 = strInstrumentTick.decode('gbk').encode('utf8')	
			dictInstrumentTick = ujson.loads(strInstrumentTick_utf8)
			listOneInstrumentTick = []	
			listOneInstrumentTick.append(dictInstrumentTick["InstrumentID"])
			listOneInstrumentTick.append(dictInstrumentTick["ActionDay"])
			listOneInstrumentTick.append(dictInstrumentTick["UpdateTime"])
			listOneInstrumentTick.append(dictInstrumentTick["UpdateMillisec"])
			listOneInstrumentTick.append(dictInstrumentTick["ExchangeID"])
			listOneInstrumentTick.append(dictInstrumentTick["LastPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["PreSettlementPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["PreClosePrice"])
			listOneInstrumentTick.append(dictInstrumentTick["PreOpenInterest"])
			listOneInstrumentTick.append(dictInstrumentTick["OpenPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["HighestPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["LowestPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["Volume"])
			listOneInstrumentTick.append(dictInstrumentTick["Turnover"])
			listOneInstrumentTick.append(dictInstrumentTick["OpenInterest"])
			listOneInstrumentTick.append(dictInstrumentTick["UpperLimitPrice"])
			listOneInstrumentTick.append(dictInstrumentTick["LowerLimitPrice"])
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
			listOneInstrumentTick.append(dictInstrumentTick["TradingDay"])
			listOneInstrumentTickTuple.append(tuple(listOneInstrumentTick))	
		InstrumentCSVWriter.writerows(listOneInstrumentTickTuple)	
		InstrumentCSVFile.close()

print "Start packing tickdata."
targzfilename = TradingDay + ".tar.gz"
tar_ret = os.system("curdir=`pwd`;cd " + aimdir + ";tar -zcvf " + targzfilename + " " + TradingDay + "/;cd $curdir")

if tar_ret == 0:
        rm_ret = os.system("rm -rf " + datadir)
	print GetCurrentTime() + ' Done.'
else:
        print GetCurrentTime() + " Package failed."

