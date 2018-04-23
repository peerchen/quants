#define _MISC_AS_TEMPLATE_
#include "../misc/loghandler.h"
#include "../misc/mysqlmanager.h"
#include "../misc/servermodel.h"
#undef _MISC_AS_TEMPLATE_

#include "kbarmanager.h"
#include "../thostapi/ctphandler/thostmdhandler.h"
#include "../thostapi/ctphandler/thosttradehandler.h"
#include "../thostapi/fshandler/fsmdhandler.h"
#include "../thostapi/fshandler/fstradehandler.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 
#include <boost/format.hpp>  
#include <boost/tokenizer.hpp>  
#include <boost/algorithm/string.hpp>  

CKBarManager::CKBarManager()
{
	m_nTradingDay = 0;
	m_nPrevTradingDay = 0;
	m_pLogHandler = NULL;
	m_pThostMdHandler = NULL;
	m_pTraderHandler = NULL;
	m_pKBarHandler = NULL;
}

CKBarManager::~CKBarManager()
{
	Release();
}

int CKBarManager::Start(int argc, char* argv[])
{
	CServerModel::Start(argc, argv);
	string sExePath = boost::filesystem::initial_path<boost::filesystem::path>().string(); 

	///read config file
	string sCfgFile = "";
	if (boost::filesystem::exists(sExePath + "/../config/kbarcfg.ini"))
	{
		sCfgFile = sExePath + "/../config/kbarcfg.ini";
	}
	else if (boost::filesystem::exists(sExePath + "/kbarcfg.ini"))
	{
		sCfgFile = sExePath + "/kbarcfg.ini";
	}
	else if (boost::filesystem::exists(sExePath + "/config/kbarcfg.ini"))
	{
		sCfgFile = sExePath + "/config/kbarcfg.ini";
	}

	if (sCfgFile == "")
	{
		m_pLogHandler->Error("KBarManager:Please check kbarcfg.ini!");
		return 1;
	}
	boost::property_tree::ptree ptIni;	
	boost::property_tree::ini_parser::read_ini(sCfgFile, ptIni);
	string strUserDataPath = ptIni.get<string>("userdata.path", "../userdata/");
	int nAddFileLogger = ptIni.get<int>("userdata.addfilelogger", 1);
	int nAddConsoleLogger = ptIni.get<int>("userdata.addconsolelogger", 1);
	int nAddprocessname = ptIni.get<int>("userdata.addprocessname", 1);
	int nAddprocessid = ptIni.get<int>("userdata.addprocessid", 0);
	if (nAddprocessname)
	{
		strUserDataPath += GetProcessName();
	}

	///init logger
	if (m_pLogHandler == NULL)
	{
		boost::filesystem::create_directories(strUserDataPath.c_str()); 
		string strLogPath = strUserDataPath + "/" + GetProcessName();
		string sLogFileName = strLogPath + ".log";		
		m_pLogHandler = new CLogHandler();
		m_pLogHandler->InitLogger(GetProcessName());
		if (nAddFileLogger)
		{
			m_pLogHandler->AddFileLogger(sLogFileName.c_str());
		}
		if (nAddConsoleLogger)
		{
			m_pLogHandler->AddConsoleLogger();
		}
	}

	///create kbar handler
	m_pKBarHandler = new CKBarHandler();
	m_pKBarHandler->RegisterKBarHandlerSpi(this);
	m_pKBarHandler->SetLogHandler(m_pLogHandler);
	int nDumpTicks = ptIni.get<int>("kbar.dumpticks", 0);
	m_pKBarHandler->SetDumpTicks(nDumpTicks);

	int nEnableCTP = ptIni.get<int>("kbar.enablectp", 1);
	int nEnableFS = ptIni.get<int>("kbar.enablefs", 0);
	int nEnableSQL = ptIni.get<int>("kbar.enablesql", 0);

	if (nEnableSQL)
	{
		string host = ptIni.get<string>("data.host", "localhost");
		int port = ptIni.get<int>("data.port", 3306);
		string user = ptIni.get<string>("data.user", "kbar");
		string password = ptIni.get<string>("data.password", "123456");
		string db = ptIni.get<string>("data.db", "kbar_db");

		///connect mysql
		if (m_mysqlManager.Connect(host.c_str(), user.c_str(), password.c_str(), db.c_str(), port, NULL, 0))
		{
			m_pLogHandler->Error("KBAR:%s", m_mysqlManager.GetErrMsg());
		} 
		else
		{
			m_pLogHandler->Info("KBAR:connect database success.");
		}
	}

	vector<string> vecSubscribeList;
	if (nEnableCTP)
	{
		m_nEnableBlackList = ptIni.get<int>("ctp.enableblacklist", 0);
		m_nEnableWhiteList = ptIni.get<int>("ctp.enablewhitelist", 0);
		///获取黑名单
		if (m_nEnableBlackList)
		{
			string strBlackList = ptIni.get<string>("ctp.blacklist","");
			boost::char_separator<char> sep(",");  
			boost::tokenizer<boost::char_separator<char> > tok(strBlackList, sep);
			for (boost::tokenizer<boost::char_separator<char> >::iterator iter = tok.begin();
				iter != tok.end(); ++iter)
			{
				m_vecBlackList.push_back(*iter);
			}
		}
		///获取白名单
		if (m_nEnableWhiteList)
		{
			string strWhiteList = ptIni.get<string>("ctp.whitelist", "");
			boost::char_separator<char> sep(",");  
			boost::tokenizer<boost::char_separator<char> > tok(strWhiteList, sep);
			for (boost::tokenizer<boost::char_separator<char> >::iterator iter = tok.begin();
				iter != tok.end(); ++iter)
			{
				m_vecWhiteList.push_back(*iter);
			}
		}

		///创建ctp md,trade实例
		string ctpUserIdField = ptIni.get<string>("ctp.userid");
		string ctpPasswordField = ptIni.get<string>("ctp.password");
		string ctpBrokerIdField = ptIni.get<string>("ctp.brokerid");
		string ctpMdFrontAddrField = ptIni.get<string>("ctp.mdfrontaddr");
		string ctpTradeFrontAddrField = ptIni.get<string>("ctp.tradefrontaddr");

		///create md handler
		m_pThostMdHandler = new CThostMdHandler(ctpUserIdField.c_str(), ctpPasswordField.c_str(), ctpBrokerIdField.c_str(), ctpMdFrontAddrField.c_str(), strUserDataPath);
		m_pThostMdHandler->SetLogHandler(m_pLogHandler);
		m_pThostMdHandler->RegisterMdHandlerSpi(this);
		m_pThostMdHandler->Start();

		///create trade handler
		m_pTraderHandler = new CThostTraderHandler(ctpUserIdField.c_str(), ctpPasswordField.c_str(), ctpBrokerIdField.c_str(), ctpTradeFrontAddrField.c_str(), strUserDataPath, "");
		m_pTraderHandler->SetLogHandler(m_pLogHandler);
		m_pTraderHandler->RegisterTradeHandlerSpi(this);
		m_pTraderHandler->Start();
	}

	if (nEnableFS)
	{
		m_pLogHandler->Info("KBAR:Enable FS.");
		string fsUserIdField = ptIni.get<string>("fs.userid");
		string fsPasswordField = ptIni.get<string>("fs.password");
		string fsBrokerIdField = ptIni.get<string>("fs.brokerid");
		string fsMdFrontAddrField = ptIni.get<string>("fs.mdfrontaddr");
		string fsTradeFrontAddrField = ptIni.get<string>("fs.tradefrontaddr");

		///create fsmd handler 
		m_pFSMdHandler = new fstech::CFSMdHandler(fsUserIdField.c_str(), fsPasswordField.c_str(), fsBrokerIdField.c_str(), fsMdFrontAddrField.c_str(), strUserDataPath);
		m_pFSMdHandler->SetLogHandler(m_pLogHandler);
		m_pFSMdHandler->RegisterMdHandlerSpi(this);
		m_pFSMdHandler->Start();

		string strSubList = ptIni.get<string>("fs.sublist","");
		boost::char_separator<char> sep(",");  
		boost::tokenizer<boost::char_separator<char> > tok(strSubList, sep);
		for (boost::tokenizer<boost::char_separator<char> >::iterator iter = tok.begin();
			iter != tok.end(); ++iter)
		{
			vecSubscribeList.push_back(*iter);
			m_pKBarHandler->RegisterKBarEvent(*iter, *iter, THOST_EXCHANGEID_SGE, THOST_KBAR_PERIOD_TYPE_MINUTE, 1);
			m_pKBarHandler->RegisterKBarEvent(*iter, *iter, THOST_EXCHANGEID_SGE, THOST_KBAR_PERIOD_TYPE_MINUTE, 5);
			m_pKBarHandler->RegisterKBarEvent(*iter, *iter, THOST_EXCHANGEID_SGE, THOST_KBAR_PERIOD_TYPE_MINUTE, 15);
		}
		
		m_pFSMdHandler->SubscribeQuoteList(vecSubscribeList);
		vecSubscribeList.clear();
	}
	return 0;
}

void CKBarManager::Release()
{
}

void CKBarManager::OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData)
{	
	//boost::mutex::scoped_lock lock(m_kbarMutex);
	HandlerKBarDaily(pDepthMarketData);
	m_pKBarHandler->MarketDataEvent(pDepthMarketData);
}

void CKBarManager::OnRtnInstrumentStatus(CThostInstrumentStatusField *pInstrumentStatus)
{
	boost::mutex::scoped_lock lock(m_kbarMutex);
// 	if (pInstrumentStatus->ExchangeID == "SGE")
// 	{
// 		return ;
// 	}
	m_pLogHandler->Info("KBAR:OnRtnInstrumentStatus:%s_%s_%c", pInstrumentStatus->InstrumentID.c_str(), pInstrumentStatus->EnterTime.c_str(), pInstrumentStatus->InstrumentStatus);
// 	if (pInstrumentStatus->InstrumentStatus == THOST_IS_Closed)
// 	{
// 		//trading closed then save kardaily to database
// 		for (KBarDailyMap::iterator iter = m_mapKBarDaily.begin(); iter != m_mapKBarDaily.end(); ++iter)
// 		{
// 			if (iter->second->Symbol == pInstrumentStatus->InstrumentID)
// 			{
// 				iter->second->Status = THOST_KBAR_STATUS_TRADINGCLOSE;
// 				SaveKBarDailyToSQL(iter->second);
// 			}
// 		}
// 		if (strlen(pInstrumentStatus->EnterTime.c_str()) >= 8)
// 		{
// 			int nCloseTime = (pInstrumentStatus->EnterTime[0]-'0')*100000 + (pInstrumentStatus->EnterTime[1]-'0')*10000 + (pInstrumentStatus->EnterTime[3]-'0')*1000 + 
// 				(pInstrumentStatus->EnterTime[4]-'0')*100 + (pInstrumentStatus->EnterTime[6]-'0')*10 + (pInstrumentStatus->EnterTime[7]-'0');
// 			m_pKBarHandler->TradingClosedEvent(pInstrumentStatus->InstrumentID, pInstrumentStatus->ExchangeID, nCloseTime);
// 		}
// 	}
}

void CKBarManager::OnRtnInitializeFinished(int ApiType)
{
	vector<string> vecSubscribeList;
	if (ApiType == THOST_API_TYPE_CTP)
	{
		m_nTradingDay = m_pTraderHandler->GetTradingDay();
		GetLastTradingDay();
		vector<CThostInstrumentField> vecInstrumentInfo;
		m_pTraderHandler->GetAllInstrument(vecInstrumentInfo);
		for (int i = 0; i < vecInstrumentInfo.size(); ++i)
		{
			///过滤掉黑名单
			if (m_nEnableBlackList)
			{
				int k;
				for (k = 0; k < m_vecBlackList.size(); ++k)
				{
					if (m_vecBlackList[k] == vecInstrumentInfo[i].ProductID)
					{
						break;
					}
				}
				if (k < m_vecBlackList.size())
				{
					continue;
				}
			}

			///选取白名单
			if (m_nEnableWhiteList)
			{	
				int k;
				for (k = 0; k < m_vecWhiteList.size(); ++k)
				{
					if (m_vecWhiteList[k] == vecInstrumentInfo[i].ProductID)
					{
						break;
					}
				}
				if (k >= m_vecWhiteList.size())
				{
					continue;
				}
			}
			///加入订阅列表并注册kbar事件
			vecSubscribeList.push_back(vecInstrumentInfo[i].InstrumentID);
			///register 1m, 5m, 15m
			m_pKBarHandler->RegisterKBarEvent(vecInstrumentInfo[i].InstrumentID, vecInstrumentInfo[i].ProductID, vecInstrumentInfo[i].ExchangeID, THOST_KBAR_PERIOD_TYPE_MINUTE, 1);
			m_pKBarHandler->RegisterKBarEvent(vecInstrumentInfo[i].InstrumentID, vecInstrumentInfo[i].ProductID, vecInstrumentInfo[i].ExchangeID, THOST_KBAR_PERIOD_TYPE_MINUTE, 5);
			m_pKBarHandler->RegisterKBarEvent(vecInstrumentInfo[i].InstrumentID, vecInstrumentInfo[i].ProductID, vecInstrumentInfo[i].ExchangeID, THOST_KBAR_PERIOD_TYPE_MINUTE, 15);

			///init daily kbar
			CKBarDailyField *pKBarDaily = new CKBarDailyField(vecInstrumentInfo[i].InstrumentID, vecInstrumentInfo[i].ProductID, vecInstrumentInfo[i].ExchangeID);
			m_mapKBarDaily[vecInstrumentInfo[i].InstrumentID] = pKBarDaily;

			if (m_mapSymbolCount.end() == m_mapSymbolCount.find(vecInstrumentInfo[i].ProductID))
			{
				m_mapSymbolCount[vecInstrumentInfo[i].ProductID] = 0;
			}
			else
			{
				m_mapSymbolCount[vecInstrumentInfo[i].ProductID]++;
			}
			m_mapSymbolCountUpdate[vecInstrumentInfo[i].ProductID] = 0;
		}
		m_pThostMdHandler->SubscribeQuoteList(vecSubscribeList);
		vecSubscribeList.clear();
	}
}

void CKBarManager::OnRtnKBar(CKBarField *pKBar)
{
	SaveKBarToSQL(pKBar);
	m_pLogHandler->Info("%c_%d%c %s %d %d %d %.2f %.2f %.2f %.2f %d", pKBar->Status == THOST_KBAR_STATUS_CLOSE ? 'N' : 'C', pKBar->PeriodValue, pKBar->PeriodType, pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->BeginTime, pKBar->EndTime, pKBar->OpenPrice, pKBar->HighPrice, pKBar->LowPrice, pKBar->ClosePrice, pKBar->BarIndex);
}

void CKBarManager::HandlerKBarDaily(CThostDepthMarketDataField *pDepthMarketData)
{
	if (m_mapKBarDaily.end() != m_mapKBarDaily.find(pDepthMarketData->InstrumentID))
	{
		CKBarDailyField *pKBarDaily = m_mapKBarDaily[pDepthMarketData->InstrumentID];

		pKBarDaily->Date = pDepthMarketData->TradingDay;
		pKBarDaily->OpenPrice = pDepthMarketData->OpenPrice;
		pKBarDaily->HighPrice = pDepthMarketData->HighestPrice;
		pKBarDaily->LowPrice = pDepthMarketData->LowestPrice;
		pKBarDaily->ClosePrice = pDepthMarketData->ClosePrice;
		pKBarDaily->AskPrice = pDepthMarketData->AskPrice1;
		pKBarDaily->AskVolume = pDepthMarketData->AskVolume1;
		pKBarDaily->BidPrice = pDepthMarketData->BidPrice1;
		pKBarDaily->BidVolume = pDepthMarketData->BidVolume1;
		pKBarDaily->Volume = pDepthMarketData->Volume;
		pKBarDaily->OpenInterest = pDepthMarketData->OpenInterest;
		pKBarDaily->Ticks = pKBarDaily->Ticks + 1;
		pKBarDaily->TWapPrice = 0;
		pKBarDaily->VWapPrice = 0;
		pKBarDaily->PreClosePrice = pDepthMarketData->PreClosePrice;
		pKBarDaily->PreOpenInterest = pDepthMarketData->PreOpenInterest;

		pKBarDaily->PreSettlePrice = pDepthMarketData->PreSettlementPrice;
		pKBarDaily->LimitUpPrice = pDepthMarketData->UpperLimitPrice;
		pKBarDaily->LimitDownPrice = pDepthMarketData->LowerLimitPrice;

		//first update daily kbar
		if (pKBarDaily->Status == THOST_KBAR_STATUS_NULL)
		{
			m_mapSymbolCountUpdate[pKBarDaily->Symbol]++;
			///collected all first marketdata index by symbol, then cacl contract rank.
			if (m_mapSymbolCountUpdate[pKBarDaily->Symbol] == m_mapSymbolCount[pKBarDaily->Symbol])
			{
				CalcContractRank(pKBarDaily->Symbol);
				m_pLogHandler->Info("KBAR:finish calc %s rank.", pKBarDaily->Symbol.c_str());
				///finish calc rank then save to database
				for (KBarDailyMap::iterator iter = m_mapKBarDaily.begin(); iter != m_mapKBarDaily.end(); ++iter)
				{
					if (iter->second->Symbol == pKBarDaily->Symbol)
					{
						SaveKBarDailyToSQL(iter->second);
					}
				}
			}
			pKBarDaily->Status = THOST_KBAR_STATUS_OPEN;
		}
	}
}

void CKBarManager::CalcContractRank(string Symbol)
{
	vector<CInstrumentRankItem> rankList;
	for (KBarDailyMap::iterator iter = m_mapKBarDaily.begin(); iter != m_mapKBarDaily.end(); ++iter)
	{
		CInstrumentRankItem rankItem;
		if (iter->second->Symbol == Symbol)
		{
			rankItem.InstrumentID = iter->second->InstrumentID;
			rankItem.RankKey  = (int)(iter->second->PreOpenInterest);
			rankList.push_back(rankItem);
		}
			
	}
	sort(rankList.begin(), rankList.end());
	for (int i = 0; i < rankList.size(); ++i)
	{
		string InstrumentID = rankList[i].InstrumentID;
		if (m_mapKBarDaily.find(InstrumentID) != m_mapKBarDaily.end())
		{
			CKBarDailyField *pKBarDaily = m_mapKBarDaily[InstrumentID];
			pKBarDaily->Rank = i+1;
		}
	}
	rankList.clear();
}

int CKBarManager::SaveKBarToSQL(CKBarField *pKBar)
{
	if (pKBar->PeriodType == THOST_KBAR_PERIOD_TYPE_MINUTE && 
		(pKBar->PeriodValue == 1 || pKBar->PeriodValue == 5 || pKBar->PeriodValue == 15))
	{
		FixExceptionPrice(pKBar->OpenPrice);
		FixExceptionPrice(pKBar->ClosePrice);
		FixExceptionPrice(pKBar->HighPrice);
		FixExceptionPrice(pKBar->LowPrice);
		FixExceptionPrice(pKBar->AskPrice);
		FixExceptionPrice(pKBar->BidPrice);
		FixExceptionPrice(pKBar->PreClosePrice);

		char strCommand[2000];
		memset(strCommand, '\0',sizeof(strCommand));
		sprintf(strCommand, "insert into futures_%dmin(period, symbol, contract, exchangeid, date, begintime, endtime, open, high, low, close, \
							askprice, askvolume, bidprice, bidvolume, volume, oi, ticks, twap, vwap, preclose, prevolume, preoi, offset, isfill, status, barindex, tradingdate)\
							values('%c', '%s', '%s', '%s', %d, %d, %d, %f, %f, %f, %f, %f, %d, %f, %d, %d, %d, %d, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d)",  \
							pKBar->PeriodValue, pKBar->PeriodType, pKBar->Symbol.c_str(), pKBar->InstrumentID.c_str(), pKBar->ExchangeID.c_str(), pKBar->Date, pKBar->BeginTime, pKBar->EndTime, 
							pKBar->OpenPrice, pKBar->HighPrice, pKBar->LowPrice, pKBar->ClosePrice, pKBar->AskPrice, pKBar->AskVolume, pKBar->BidPrice, pKBar->BidVolume, pKBar->Volume,
							(int)pKBar->OpenInterest, pKBar->Ticks, pKBar->TWapPrice, pKBar->VWapPrice, pKBar->PreClosePrice, pKBar->PreVolume, (int)pKBar->PreOpenInterest,
							pKBar->Offset, pKBar->IsFill, pKBar->Status, pKBar->BarIndex, pKBar->TradingDate);

		if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
		{
			m_pLogHandler->Error("KBarManager:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
			return 1;
		}
		return 0;
	}
	return 2;
}

int CKBarManager::SaveKBarDailyToSQL(CKBarDailyField *pKBarDaily)
{
	FixExceptionPrice(pKBarDaily->OpenPrice);
	FixExceptionPrice(pKBarDaily->ClosePrice);
	FixExceptionPrice(pKBarDaily->HighPrice);
	FixExceptionPrice(pKBarDaily->LowPrice);
	FixExceptionPrice(pKBarDaily->AskPrice);
	FixExceptionPrice(pKBarDaily->BidPrice);
	FixExceptionPrice(pKBarDaily->PreClosePrice);
	char strCommand[2000];
	memset(strCommand, '\0',sizeof(strCommand));
	sprintf(strCommand, "insert into futures_daily(symbol, contract, date, open, high, low, close, limitup, limitdown, \
						askprice, askvolume, bidprice, bidvolume, oi, ticks, twap, vwap, presettle, preclose, prevolume, preoi, isfill, status, rank)\
						values('%s', '%s', %d, %f, %f, %f, %f, %f, %f, %f, %d, %f, %d, %d, %d, %f, %f, %f, %f, %d, %d, %d, %d, %d) ON DUPLICATE KEY \
						update open = %f, high = %f, low = %f, close = %f, limitup = %f, limitdown = %f, askprice = %f, askvolume = %d, bidprice = %f, bidvolume = %d, \
						oi = %d, ticks = %d, twap = %f, vwap = %f, presettle = %f, preclose = %f, prevolume = %d, preoi = %d, isfill = %d, status = %d, rank = %d",  \
						pKBarDaily->Symbol.c_str(), pKBarDaily->InstrumentID.c_str(), pKBarDaily->Date, pKBarDaily->OpenPrice, pKBarDaily->HighPrice, pKBarDaily->LowPrice, pKBarDaily->ClosePrice, pKBarDaily->LimitUpPrice, pKBarDaily->LimitDownPrice,
						pKBarDaily->AskPrice, pKBarDaily->AskVolume, pKBarDaily->BidPrice, pKBarDaily->BidVolume, (int)pKBarDaily->OpenInterest, pKBarDaily->Ticks, pKBarDaily->TWapPrice, pKBarDaily->VWapPrice, pKBarDaily->PreSettlePrice, pKBarDaily->PreClosePrice, 
						pKBarDaily->PreVolume, (int)pKBarDaily->PreOpenInterest, pKBarDaily->IsFill, pKBarDaily->Status, pKBarDaily->Rank,

						pKBarDaily->OpenPrice, pKBarDaily->HighPrice, pKBarDaily->LowPrice, pKBarDaily->ClosePrice, pKBarDaily->LimitUpPrice, pKBarDaily->LimitDownPrice,
						pKBarDaily->AskPrice, pKBarDaily->AskVolume, pKBarDaily->BidPrice, pKBarDaily->BidVolume, (int)pKBarDaily->OpenInterest, pKBarDaily->Ticks, pKBarDaily->TWapPrice, pKBarDaily->VWapPrice, pKBarDaily->PreSettlePrice, pKBarDaily->PreClosePrice, 
						pKBarDaily->PreVolume, (int)pKBarDaily->PreOpenInterest, pKBarDaily->IsFill, pKBarDaily->Status, pKBarDaily->Rank);

	if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
	{
		m_pLogHandler->Error("KBarManager:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
		return 1;
	}
	return 0;
}

void CKBarManager::GetLastTradingDay()
{
	if (m_mysqlManager.IsConnect())
	{
		char szCommand[200];
		memset(szCommand, '\0', sizeof(szCommand));
		sprintf(szCommand, "select date from futures_callendar where date < %d and date > %d and exchangeid = 'DCE' ORDER BY date desc limit 1", m_nTradingDay, m_nTradingDay-2000);
		if (m_mysqlManager.Execute(szCommand, strlen(szCommand)))
		{
			m_pLogHandler->Error("KBAR:query fix date failed.");
		}
		else
		{
			MYSQL_ROW row; 
			MYSQL_RES *mysql_res = m_mysqlManager.GetResult();
			while ((row = mysql_fetch_row(mysql_res)) != NULL)
			{
				m_nPrevTradingDay = atoi(row[0]);
				//m_pLogHandler->Info("KBAR:prev trading day is %d", m_nPrevTradingDay);
			}
			m_mysqlManager.ReleaseResult();
		}
	}
	m_pLogHandler->Info("KBAR:current tradingday is %d, prev tradingday is %d", m_nTradingDay, m_nPrevTradingDay);
	m_pKBarHandler->SetTradingDay(m_nTradingDay, m_nPrevTradingDay);
}