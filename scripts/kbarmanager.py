#coding:utf-8
import thostdef
import kbarhandler
import os
import redis
import csv
import time
import MySQLdb

#���������ߵĲ�������
g_difflist = []

#���߶�ʧ������
g_onlinemissedlist = []

#���߶�ʧ������
g_offlinemissedlist = []

def GetCurrentTime() :
	return time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))

def CheckTwoStyleKBarList(PeriodValue, Symbol, InstrumentID, listOfflineKBar, listOnlineKBar) :
	'''
	Brief : 
		�Ƚ������������������ɵ�KBAR
	Arguments :
		listOfflineKBar				list<dict>			�������ɵ�KBAR
		listOnlineKBar				list<dict>			�������ɵ�KBAR
	Returns :
		ListKBar				list<dict>		1�������յ�kbar�б� 
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
			#�Ƚ����ߺ����ߵ�KBARֵ
			#��ʱȥ�����kbar�ıȽ�
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
	#�������߶�ʧ������
	for i in range(indexOffline, lenOffline) :
		g_onlinemissedlist.append(listOfflineKBar[i].copy())
	#�������߶�ʧ������
	for i in range(indexOnline, lenOnline) :
		g_offlinemissedlist.append(listOnlineKBar[i].copy())
	
def ReadInstrumentCSV(filepath) :
	'''
	Brief : 
		��ȡtick csv�ļ�
	Arguments :
		filepath			string			csv�ļ�·��
	Returns :
		listTick			list(dict)		tick�б�
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
		��ȡ��Լ��tick����(csv), ���ֱ�����1/5/15����kbar, ͬʱ��mysql��ȡ��Ӧkbar����
		Ȼ����жԱȣ��������Ĳ��챣�浽g_difflist��g_onlinemissedlist��
	Arguments :
		instrumentpathname			string			��Լcsv�ļ����ڵ�·��
		tradingday					string			������
		symbol						string			Ʒ��
		exchangeid					string			������
		instrumentid				string			��Լid
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
		����Ʒ��Ŀ¼���������еĺ�Լ���ٵ���HandlerInstrument����
	Arguments :
		symbolpath				exchangeid			Ʒ�����ڵ�Ŀ¼
		tradingday				string				������
		symbol					exchangeid			Ʒ��
		exchangeid				string				������
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
		�Ա�1������������(csv)������(mysql)���ɵ�kbar���������������浽g_difflist��g_onlinemissedlist
		����������Ŀ¼���������е�Ʒ�֣��ٵ���HandlerSymbol����
	Arguments :
		datadir					class			1��������tick���ݵ�Ŀ¼
		tradingday				string			������
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
		����mysql���ݿ�
	Arguments :
		host			string			��������ַ
		port			string			�������˿�
		user			string			�û���
		passwd			string			����
		db				string			���ݿ�
	Returns :
		mysql_con		class			mysql������
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
		��mysql�а��ս�����/��Լ/�������Ͷ�ȡkbar����
	Arguments :
		mysql_context			class			mysql�����Ļ���
		tradingday				string			������
		instrumentid			string			��Լ
		periodvalue				int				����kbar����
	Returns :
		listInstrumentKBar		list(dict)		1�������յĺ�Լ����kbar�б�
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
		���������������ɵ�kbar���챣�浽csv�ļ���
	Arguments :
		path					string			�����·��
		tradingday				string			������
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
