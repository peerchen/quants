#coding:utf-8
import thostdef
import kbarhandler
import os
import redis
import csv
import time
import MySQLdb

#离线与在线的差异数据
g_difflist = []

#在线丢失的数据
g_onlinemissedlist = []

#离线丢失的数据
g_offlinemissedlist = []

def GetCurrentTime() :
	return time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

def CheckTwoStyleKBarList(PeriodValue, Symbol, InstrumentID, listOfflineKBar, listOnlineKBar) :
	'''
	Brief : 
		比较离线生成与在线生成的KBAR
	Arguments :
		listOfflineKBar				list<dict>			离线生成的KBAR
		listOnlineKBar				list<dict>			离线生成的KBAR
	Returns :
		ListKBar				list<dict>		1个交易日的kbar列表 
	'''
	indexOffline = 0
	indexOnline = 0
	lenOffline = len(listOfflineKBar)
	lenOnline = len(listOnlineKBar)

	dictInstrumentInfo = {}
	dictInstrumentInfo['PeriodValue'] = PeriodValue
	dictInstrumentInfo['Symbol'] = Symbol
	dictInstrumentInfo['InstrumentID'] = InstrumentID
	listOnlineMissed = []
	while indexOffline < lenOffline and indexOnline < lenOnline :
		offlineKBar = listOfflineKBar[indexOffline];
		onlineKBar = listOnlineKBar[indexOnline]
		offline_timestamp = offlineKBar['Date'] * 1000000 +offlineKBar['BeginTime']
		online_timestamp = onlineKBar['Date'] * 1000000 + onlineKBar['BeginTime']
		if offline_timestamp < online_timestamp :
			g_onlinemissedlist.append(offlineKBar.copy())
			indexOffline = indexOffline + 1
		elif offline_timestamp == online_timestamp :
			#比较在线和离线的KBAR值
			#暂时去掉最后kbar的比较
			for key in onlineKBar.keys() :
				if indexOffline != 0 or (indexOffline == 0 and key != 'PreClosePrice' and key != 'PreOpenInterest') :
					if offlineKBar[key] != onlineKBar[key] :
						diffvaluelist = [Symbol, InstrumentID, PeriodValue, offline_timestamp, key, offlineKBar[key], onlineKBar[key]]
						g_difflist.append(diffvaluelist)
			indexOffline = indexOffline + 1
			indexOnline = indexOnline + 1
		else :
			g_offlinemissedlist.append(onlineKBar.copy())
			indexOnline = indexOnline + 1
	#增加离线丢失的数据
	for i in range(indexOffline, lenOffline) :
		g_onlinemissedlist.append(listOfflineKBar[i].copy())
	#增加在线丢失的数据
	for i in range(indexOnline, lenOnline) :
		g_offlinemissedlist.append(listOnlineKBar[i].copy())
	
def ReadInstrumentCSV(filepath) :
	'''
	Brief : 
		读取tick csv文件
	Arguments :
		filepath			string			csv文件路径
	Returns :
		listTick			list(dict)		tick列表
	'''
	listTick = []
	csvfile = file(filepath, 'rb')
	reader = csv.reader(csvfile)
	index = 0
	for line in reader:
		dictTick = {}
		if index == 0 :
			index = 1
			continue
		dictTick["ActionDay"] = int(line[1])
		dictTick["UpdateTime"] = int(line[2])
		dictTick["LastPrice"] = float(line[4])
		dictTick["Volume"] = int(line[5])
		dictTick["OpenInterest"] = float(line[7])
		dictTick["BidPrice1"] = float(line[8])
		dictTick["BidVolume1"] = int(line[9])
		dictTick["AskPrice1"] = float(line[10])
		dictTick["AskVolume1"] = int(line[11])
		listTick.append(dictTick)
	csvfile.close() 
	return listTick

def HandlerInstrument(instrumentpathname, tradingday, symbol, exchangeid, instrumentid) :
	'''
	Brief : 
		读取合约的tick数据(csv), 并分别生成1/5/15分钟kbar, 同时从mysql读取相应kbar数据
		然后进行对比，将产生的差异保存到g_difflist和g_onlinemissedlist中
	Arguments :
		instrumentpathname			string			合约csv文件所在的路径
		tradingday					string			交易日
		symbol						string			品种
		exchangeid					string			交易所
		instrumentid				string			合约id
	Returns :
	'''
	print GetCurrentTime() + ':handler:' + instrumentid
	listInstrumentTicks = ReadInstrumentCSV(instrumentpathname)
	if len(listInstrumentTicks) > 0 :
		list1minKBar = kbarhandler.Make1MinKBarByTradingDay(tradingday, symbol,  exchangeid, instrumentid, listInstrumentTicks)
		list5minKBar = kbarhandler.Make5MinKBarByTradingDay(tradingday, symbol,  exchangeid, instrumentid, listInstrumentTicks)
		list15minKBar = kbarhandler.Make15MinKBarByTradingDay(tradingday, symbol,  exchangeid, instrumentid, listInstrumentTicks)
		
		listInstrumentKBar1min = ReadInstrumentKBarFromMySQL(mysql_context, tradingday, instrumentid, 1)
		listInstrumentKBar5min = ReadInstrumentKBarFromMySQL(mysql_context, tradingday, instrumentid, 5)
		listInstrumentKBar15min = ReadInstrumentKBarFromMySQL(mysql_context, tradingday, instrumentid, 15)
		CheckTwoStyleKBarList(1, symbol, instrumentid, list1minKBar, listInstrumentKBar1min)
		CheckTwoStyleKBarList(5, symbol, instrumentid, list5minKBar, listInstrumentKBar5min)
		CheckTwoStyleKBarList(15, symbol, instrumentid, list15minKBar, listInstrumentKBar15min)
		#print instrumentid, len(list1minKBar), len(listInstrumentKBar1min), len(list5minKBar), len(listInstrumentKBar5min), len(list15minKBar), len(listInstrumentKBar15min)

def HandlerSymbol(symbolpath, tradingday, symbol, exchangeid) :
	'''
	Brief : 
		遍历品种目录，发现所有的合约，再调用HandlerInstrument函数
	Arguments :
		symbolpath				exchangeid			品种所在的目录
		tradingday				string				交易日
		symbol					exchangeid			品种
		exchangeid				string				交易所
	Returns :
	'''
	print GetCurrentTime() + ':handler symbol:' + symbol
	for symboldir, dirnames, instrumentfilenames in os.walk(symbolpath) :
		for instrumentfilename in instrumentfilenames :
			instrumentpathname = symboldir + instrumentfilename
			HandlerInstrument(instrumentpathname, tradingday, symbol, exchangeid, instrumentfilename.replace('.csv',''))
			
def HandlerTradingDay(datadir, tradingday) :
	'''
	Brief : 
		对比1个交易日离线(csv)和在线(mysql)生成的kbar，并将差异结果保存到g_difflist和g_onlinemissedlist
		遍历交易日目录，发现所有的品种，再调用HandlerSymbol函数
	Arguments :
		datadir					class			1个交易日tick数据的目录
		tradingday				string			交易日
	Returns :
	'''
	print GetCurrentTime() + ':handler tradingday:' + tradingday
	tradingdaypath = datadir + '/' + str(tradingday) + '/'
	for tradingdaydir, symbolnames, filenames in os.walk(tradingdaypath) :
		for symbol in symbolnames :
			symbolpath = tradingdaydir + symbol + '/'
			#get exchangeid by symbol
			if kbarhandler.thostdef.dictSymbolExchangeID.has_key(symbol) :
				HandlerSymbol(symbolpath, tradingday, symbol, kbarhandler.thostdef.dictSymbolExchangeID[symbol])

def ConnectMySQL(host, port, user, passwd, db) :
	'''
	Brief : 
		连接mysql数据库
	Arguments :
		host			string			服务器地址
		port			string			服务器端口
		user			string			用户名
		passwd			string			密码
		db				string			数据库
	Returns :
		mysql_con		class			mysql的连接
	'''
	conndb= MySQLdb.connect(
        host,
        user,
        passwd,
        db
        )
	return conndb


def ReadInstrumentKBarFromMySQL(mysql_context, tradingday, instrumentid, periodvalue) :
	'''
	Brief : 
		从mysql中按照交易日/合约/分钟类型读取kbar数据
	Arguments :
		mysql_context			class			mysql上下文环境
		tradingday				string			交易日
		instrumentid			string			合约
		periodvalue				int				分钟kbar类型
	Returns :
		listInstrumentKBar		list(dict)		1个交易日的合约分钟kbar列表
	'''
	cur = mysql_context.cursor()
	command = 'select * from futures_{0}min where contract = "{1}" and tradingdate = {2} ORDER BY barindex'.format(periodvalue, instrumentid, tradingday)
	cur.execute(command);
	data = cur.fetchall()
	listInstrumentKBar = []
	for row in data :
		dictInstrumentKBar = {}
		dictInstrumentKBar['PeriodType'] = row[0]
		dictInstrumentKBar['PeriodValue'] = periodvalue
		dictInstrumentKBar['Symbol'] = row[1]
		dictInstrumentKBar['InstrumentID'] = row[2]
		dictInstrumentKBar['Date'] = row[4]
		dictInstrumentKBar['BeginTime'] = row[5]
		dictInstrumentKBar['EndTime'] = row[6]
		dictInstrumentKBar['OpenPrice'] = row[7]
		dictInstrumentKBar['HighPrice'] = row[8]
		dictInstrumentKBar['LowPrice'] = row[9]
		dictInstrumentKBar['ClosePrice'] = row[10]
		dictInstrumentKBar['AskPrice'] = row[11]
		dictInstrumentKBar['AskVolume'] = row[12]
		dictInstrumentKBar['BidPrice'] = row[13]
		dictInstrumentKBar['BidVolume'] = row[14]
		dictInstrumentKBar['Volume'] = row[15]
		dictInstrumentKBar['OpenInterest'] = row[16]
		dictInstrumentKBar['Ticks'] = row[17]
		dictInstrumentKBar['PreClosePrice'] = row[20]
		dictInstrumentKBar['PreVolume'] = row[21]
		dictInstrumentKBar['PreOpenInterest'] = row[22]
		dictInstrumentKBar['TradingDate'] = row[27]
		listInstrumentKBar.append(dictInstrumentKBar)
	return listInstrumentKBar

mysql_context = ConnectMySQL('172.31.11.215', '3306', 'kbar', '123456', 'kbar_db')

def SaveDiffMissedToCSV(path, tradingday) :
	'''
	Brief : 
		将离线与在线生成的kbar差异保存到csv文件中
	Arguments :
		path					string			保存的路径
		tradingday				string			交易日
	Returns :
	'''
	print GetCurrentTime() + ':save diff and missed to csv.'
	listDiffCSVHead = ["Symbol", "InstrumentID", "PeriodValue", "BeginTime", "Field", "OfflineValue", "OnlineValue"]
	DiffCSVFile = open(path + '/' + tradingday + '_online_offline_diff.csv', 'wb')
	DiffCSVFileWriter = csv.writer(DiffCSVFile)
	DiffCSVFileWriter.writerow(listDiffCSVHead)
	DiffCSVFileWriter.writerows(g_difflist)

	listOnlineMissedCSVHead = ["Date",  "BeginTime"]
	OnlineMissedCSVFile = open(path + '/' + tradingday + '_online_missed.csv', 'wb')
	OnlineMissedCSVFileWriter = csv.writer(OnlineMissedCSVFile)
	OnlineMissedCSVFileWriter.writerow(listOnlineMissedCSVHead)
	listOnlineMissedCSVRows = []
	for onlineMissed in g_onlinemissedlist :
		listValue = [onlineMissed['Date'], onlineMissed['BeginTime']]
		listOnlineMissedCSVRows.append(listValue)
	OnlineMissedCSVFileWriter.writerows(listOnlineMissedCSVRows)
	OnlineMissedCSVFile.close()
	
	listOfflineMissedCSVHead = ["Date",  "BeginTime"]
	OfflineMissedCSVFile = open(path + '/' + tradingday + '_Offline_missed.csv', 'wb')
	OfflineMissedCSVFileWriter = csv.writer(OfflineMissedCSVFile)
	OfflineMissedCSVFileWriter.writerow(listOfflineMissedCSVHead)
	listOfflineMissedCSVRows = []
	for OfflineMissed in g_offlinemissedlist :
		listValue = [OfflineMissed['Date'], OfflineMissed['BeginTime']]
		listOfflineMissedCSVRows.append(listValue)
	OfflineMissedCSVFileWriter.writerows(listOfflineMissedCSVRows)
	OfflineMissedCSVFile.close()

if __name__ == "__main__" :
	#HandlerInstrument('/home/quants/tickdata/20161216/jm/jm1612.csv', '20161214', 'jm', 'CZCE', 'jm1709')
	#HandlerInstrument('/home/quants/tickdata/20161216/zn/zn1711.csv', '20161214', 'zn', 'SHFE', 'zn1711')
	#HandlerInstrument('/home/quants/tickdata/20161216/ru/ru1705.csv', '20161216', 'ru', 'SHFE', 'ru1705')
	#HandlerInstrument('/home/quants/tickdata/20161216/v/v1707.csv', '20161214', 'v', 'CZCE', 'v1707')
	#HandlerSymbol('/home/quants/tickdata/20161216/ru/', '20161214', 'ru', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/zn/', '20161214', 'zn', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/cu/', '20161214', 'cu', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/al/', '20161214', 'al', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/m/', '20161214', 'm', 'DCE')
	#HandlerSymbol('/home/quants/tickdata/20161216/RM/', '20161214', 'RM', 'CZCE')
	#HandlerSymbol('/home/quants/tickdata/20161216/IF/', '20161214', 'IF', 'CFFEX')
	#HandlerSymbol('/home/quants/tickdata/20161216/rb/', '20161214', 'rb', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/ru/', '20161214', 'ru', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/IF/', '20161214', 'IF', 'CFFEX')
	#HandlerSymbol('/home/quants/tickdata/20161216/au/', '20161214', 'au', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/ag/', '20161214', 'ag', 'SHFE')
	#HandlerSymbol('/home/quants/tickdata/20161216/jm/', '20161214', 'jm', 'CZCE')
	HandlerTradingDay('/home/quants/tickdata', '20161216')
	SaveDiffMissedToCSV('/home/quants/tickdata', '20161216');
	print 'Done.'
