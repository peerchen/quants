#define _MISC_AS_TEMPLATE_
#include "../misc/loghandler.h"
#include "../misc/servermodel.h"
#include "../misc/mysqlmanager.h"
#ifdef USE_REDIS
#include "../misc/redismanager.h"
#endif
#undef _MISC_AS_TEMPLATE_

#include "tickmanager.h"
#include "../thostapi/ctphandler/thostmdhandler.h"
#include "../thostapi/ctphandler/thosttradehandler.h"
#include "../thostapi/fshandler/fsmdhandler.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 
#include <boost/format.hpp>  
#include <boost/tokenizer.hpp>  
#include <boost/algorithm/string.hpp>  

CTickManager::CTickManager()
{
	m_nTradingDay = 0;
	m_pLogHandler = NULL;
	m_pThostMdHandler = NULL;
	m_pTraderHandler = NULL;
	#ifdef USE_REDIS
	m_pRedisManager = NULL;
	#endif
}

CTickManager::~CTickManager()
{
	Release();
}

int CTickManager::Start(int argc, char* argv[])
{
	CServerModel::Start(argc, argv);
	string sExePath = boost::filesystem::initial_path<boost::filesystem::path>().string(); 

	///read config file
	string sCfgFile = "";
	if (boost::filesystem::exists(sExePath + "/../config/tickcfg.ini"))
	{
		sCfgFile = sExePath + "/../config/tickcfg.ini";
	}
	else if (boost::filesystem::exists(sExePath + "/tickcfg.ini"))
	{
		sCfgFile = sExePath + "/tickcfg.ini";
	}
	else if (boost::filesystem::exists(sExePath + "/config/tickcfg.ini"))
	{
		sCfgFile = sExePath + "/config/tickcfg.ini";
	}

	if (sCfgFile == "")
	{
		m_pLogHandler->Error("TickManager:Please check tickcfg.ini!");
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

	int nEnableCTP = ptIni.get<int>("tick.enablectp", 1);
	int nEnableFS = ptIni.get<int>("tick.enablefs", 0);
	int nEnableSQL = ptIni.get<int>("tick.enablesql", 1);

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
			m_pLogHandler->Error("TICK:%s", m_mysqlManager.GetErrMsg());
		} 
		else
		{
			m_pLogHandler->Info("KBAR:connect database success.");
		}
	}

	#ifdef USE_REDIS
	int nEnableRedis = ptIni.get<int>("tick.enableredis", 0);
	if (nEnableRedis)
	{
		string redisHost = ptIni.get<string>("redis.host");
		int redisPort = ptIni.get<int>("redis.port", 6379);
		m_expireSeconds = ptIni.get<int>("redis.expireseconds", 86400);
		m_pRedisManager = new CRedisManager();
		m_pRedisManager->SetRedisServer(redisHost, redisPort);
		if (!m_pRedisManager->Connect())
		{
			m_pLogHandler->Info("REDIS:redis connected");
		}
		else
		{
			m_pLogHandler->Error("REDIS:redis connect failed, please check config.");
		}
	}
	#endif
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
		m_pLogHandler->Info("TICK:Enable FS.");
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
			m_mapSetKeyExpire[*iter] = 0;
		}
		
		m_pFSMdHandler->SubscribeQuoteList(vecSubscribeList);
		vecSubscribeList.clear();
	}
	return 0;
}

void CTickManager::Release()
{
}

void CTickManager::OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData)
{	
	boost::mutex::scoped_lock lock(m_tickMutex);
	if (FilterInvaildMarketData(pDepthMarketData))
	{
		return ;
	}
	Json::Value jsonDepthMarkeData;
	MakeJsonDepthMarketData(pDepthMarketData, jsonDepthMarkeData);
	Json::FastWriter writer;
	string strjsonDepthMarkeData = writer.write(jsonDepthMarkeData);
	#ifdef USE_REDIS
	char strLPUSHCommand[1500];
	memset(strLPUSHCommand, '\0', sizeof(strLPUSHCommand));
	sprintf (strLPUSHCommand, "LPUSH %d:ticks:%s %s", m_nTradingDay, pDepthMarketData->InstrumentID.c_str(), strjsonDepthMarkeData.c_str());
	redisReply *reply = m_pRedisManager->Command(strLPUSHCommand);
	if (reply != NULL)
	{
		freeReplyObject(reply);
	}

	//第一次LPUSH InstrumentID时, set EXPIRE KEY 86400
	if (m_mapSetKeyExpire.end() != m_mapSetKeyExpire.find(pDepthMarketData->InstrumentID))
	{
		int nSetKeyExpireFlag = m_mapSetKeyExpire[pDepthMarketData->InstrumentID];
		if (!nSetKeyExpireFlag)
		{
			char strEXPIRETicksCommand[100];
			memset(strEXPIRETicksCommand, '\0', sizeof(strEXPIRETicksCommand));
			sprintf (strEXPIRETicksCommand, "EXPIRE %d:ticks:%s %d", m_nTradingDay, pDepthMarketData->InstrumentID.c_str(), m_expireSeconds);
			reply = m_pRedisManager->Command(strEXPIRETicksCommand);
			if (reply != NULL)
			{
				freeReplyObject(reply);
			}
			m_mapSetKeyExpire[pDepthMarketData->InstrumentID] = 1;
		}
	}
	#endif
}

void CTickManager::OnRtnInitializeFinished(int ApiType)
{
	vector<string> vecSubscribeList;
	if (ApiType == THOST_API_TYPE_CTP)
	{
		m_nTradingDay = m_pTraderHandler->GetTradingDay();
		GetLastTradingDay();
		#ifdef USE_REDIS
		char strSetTradingDay[30];
		memset(strSetTradingDay, '\0', sizeof(strSetTradingDay));
		sprintf (strSetTradingDay, "SET TradingDay %d", m_nTradingDay);
		redisReply *reply = m_pRedisManager->Command(strSetTradingDay);
		if (reply != NULL)
		{	
			m_pLogHandler->Info("Redis:Set Tradingday Success.");
			freeReplyObject(reply);
		}
		#endif
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
			if (m_mapSetKeyExpire.end() == m_mapSetKeyExpire.find(vecInstrumentInfo[i].InstrumentID))
			{
				m_mapSetKeyExpire[vecInstrumentInfo[i].InstrumentID] = 0;
			}
			m_mapInstrumentInfo[vecInstrumentInfo[i].InstrumentID] = vecInstrumentInfo[i];
			vecSubscribeList.push_back(vecInstrumentInfo[i].InstrumentID);
			Json::Value jsonInstrumentInfo;
			MakeJsonInstrumentInfo(vecInstrumentInfo[i], jsonInstrumentInfo);
			Json::FastWriter writer;
			string strjsonInstrumentInfo = writer.write(jsonInstrumentInfo);
			#ifdef USE_REDIS
			char strRedisCommand[1500];
			memset(strRedisCommand, '\0', sizeof(strRedisCommand));
			sprintf (strRedisCommand, "LPUSH %d:instrumentinfo %s", m_nTradingDay, strjsonInstrumentInfo.c_str());
			redisReply *reply = m_pRedisManager->Command(strRedisCommand);
			if (reply != NULL)
			{
				freeReplyObject(reply);
			}
			
			char strEXPIRECommand[100];
			memset(strEXPIRECommand, '\0', sizeof(strEXPIRECommand));
			sprintf (strEXPIRECommand, "EXPIRE %d:instrumentinfo %d", m_nTradingDay, m_expireSeconds);
			reply = m_pRedisManager->Command(strEXPIRECommand);
			if (reply != NULL)
			{
				freeReplyObject(reply);
			}


			#endif
		}
		m_pThostMdHandler->SubscribeQuoteList(vecSubscribeList);
		vecSubscribeList.clear();
	}
}

bool CTickManager::FilterInvaildMarketData(CThostDepthMarketDataField *pDepthMarketData)
{
	string strSymbol;
	string strExchangeID;
	if (pDepthMarketData->InstrumentID == "Au(T+D)")
	{
		strSymbol = "autd";
		strExchangeID = "SGE";
	}
	else if (pDepthMarketData->InstrumentID == "Ag(T+D)")
	{
		strSymbol = "agtd";
		strExchangeID = "SGE";
	}
	else
	{
		if (m_mapInstrumentInfo.end() != m_mapInstrumentInfo.find(pDepthMarketData->InstrumentID))
		{
			strSymbol = m_mapInstrumentInfo[pDepthMarketData->InstrumentID].ProductID;
			strExchangeID = m_mapInstrumentInfo[pDepthMarketData->InstrumentID].ExchangeID;
		}
	}
	if (!THOST_IS_TRADINGTIME(strSymbol, strExchangeID, pDepthMarketData->UpdateTime))
	{
		return true;
	}

	if (m_nTradingDay != 0)
	{
		pDepthMarketData->TradingDay = m_nTradingDay;
	}

	if (m_nPrevTradingDay != 0)
	{
		if (pDepthMarketData->UpdateTime >= THOST_SECTIONTIME_200000 && pDepthMarketData->UpdateTime < THOST_SECTIONTIME_240000)
		{
			pDepthMarketData->ActionDay = m_nPrevTradingDay;
		}
		else
		{
			if (m_nTradingDay != 0)
			{
				pDepthMarketData->ActionDay = m_nTradingDay;
			}
			else
			{
				pDepthMarketData->ActionDay = pDepthMarketData->TradingDay;
			}
		}
	}
	
	///过滤掉过期的数据
	CThostDataTimeStamp curTimeStamp;
	curTimeStamp.date = pDepthMarketData->ActionDay;
	curTimeStamp.time = pDepthMarketData->UpdateTime;
	curTimeStamp.millisec = pDepthMarketData->UpdateMillisec;
	if (m_mapLastTimeStamp.end() != m_mapLastTimeStamp.find(pDepthMarketData->InstrumentID))
	{
		CThostDataTimeStamp lastTimeStamp = m_mapLastTimeStamp[pDepthMarketData->InstrumentID];
		if (lastTimeStamp > curTimeStamp)
		{
			m_pLogHandler->Error("TICK:Dirty MarketData, %s %d cur:%d:%d last:%d:%d", pDepthMarketData->InstrumentID.c_str(), pDepthMarketData->TradingDay, 
				curTimeStamp.date, curTimeStamp.time, lastTimeStamp.date, lastTimeStamp.time);
			return true;
		}
	}
	m_mapLastTimeStamp[pDepthMarketData->InstrumentID] = curTimeStamp;
	return false;
}

void CTickManager::MakeJsonDepthMarketData(CThostDepthMarketDataField *pDepthMarketData, Json::Value &jsonDepthMarketData)
{
	jsonDepthMarketData["InstrumentID"] = Json::Value(pDepthMarketData->InstrumentID);
	jsonDepthMarketData["ExchangeID"] = Json::Value(pDepthMarketData->ExchangeID);
	jsonDepthMarketData["LastPrice"] = Json::Value(pDepthMarketData->LastPrice);
	jsonDepthMarketData["PreSettlementPrice"] = Json::Value(pDepthMarketData->PreSettlementPrice);
	jsonDepthMarketData["PreClosePrice"] = Json::Value(pDepthMarketData->PreClosePrice);
	jsonDepthMarketData["PreOpenInterest"] = Json::Value(pDepthMarketData->PreOpenInterest);
	jsonDepthMarketData["OpenPrice"] = Json::Value(pDepthMarketData->OpenPrice);
	jsonDepthMarketData["HighestPrice"] = Json::Value(pDepthMarketData->HighestPrice);
	jsonDepthMarketData["LowestPrice"] = Json::Value(pDepthMarketData->LowestPrice);
	jsonDepthMarketData["Volume"] = Json::Value(pDepthMarketData->Volume);
	jsonDepthMarketData["Turnover"] = Json::Value(pDepthMarketData->Turnover);
	jsonDepthMarketData["OpenInterest"] = Json::Value(pDepthMarketData->OpenInterest);
	jsonDepthMarketData["ClosePrice"] = Json::Value(pDepthMarketData->ClosePrice);
	jsonDepthMarketData["SettlementPrice"] = Json::Value(pDepthMarketData->SettlementPrice);
	jsonDepthMarketData["UpperLimitPrice"] = Json::Value(pDepthMarketData->UpperLimitPrice);
	jsonDepthMarketData["LowerLimitPrice"] = Json::Value(pDepthMarketData->LowerLimitPrice);
	jsonDepthMarketData["PreDelta"] = Json::Value(pDepthMarketData->PreDelta);
	jsonDepthMarketData["CurrDelta"] = Json::Value(pDepthMarketData->CurrDelta);
	jsonDepthMarketData["UpdateTime"] = Json::Value(pDepthMarketData->UpdateTime);
	jsonDepthMarketData["UpdateMillisec"] = Json::Value(pDepthMarketData->UpdateMillisec);
	jsonDepthMarketData["BidPrice1"] = Json::Value(pDepthMarketData->BidPrice1);
	jsonDepthMarketData["BidVolume1"] = Json::Value(pDepthMarketData->BidVolume1);
	jsonDepthMarketData["AskPrice1"] = Json::Value(pDepthMarketData->AskPrice1);
	jsonDepthMarketData["AskVolume1"] = Json::Value(pDepthMarketData->AskVolume1);
	jsonDepthMarketData["BidPrice2"] = Json::Value(pDepthMarketData->BidPrice2);
	jsonDepthMarketData["BidVolume2"] = Json::Value(pDepthMarketData->BidVolume2);
	jsonDepthMarketData["AskPrice2"] = Json::Value(pDepthMarketData->AskPrice2);
	jsonDepthMarketData["AskVolume2"] = Json::Value(pDepthMarketData->AskVolume2);
	jsonDepthMarketData["BidPrice3"] = Json::Value(pDepthMarketData->BidPrice3);
	jsonDepthMarketData["BidVolume3"] = Json::Value(pDepthMarketData->BidVolume3);
	jsonDepthMarketData["AskPrice3"] = Json::Value(pDepthMarketData->AskPrice3);
	jsonDepthMarketData["AskVolume3"] = Json::Value(pDepthMarketData->AskVolume3);
	jsonDepthMarketData["BidPrice4"] = Json::Value(pDepthMarketData->BidPrice4);
	jsonDepthMarketData["BidVolume4"] = Json::Value(pDepthMarketData->BidVolume4);
	jsonDepthMarketData["AskPrice4"] = Json::Value(pDepthMarketData->AskPrice4);
	jsonDepthMarketData["AskVolume4"] = Json::Value(pDepthMarketData->AskVolume4);
	jsonDepthMarketData["BidPrice5"] = Json::Value(pDepthMarketData->BidPrice5);
	jsonDepthMarketData["BidVolume5"] = Json::Value(pDepthMarketData->BidVolume5);
	jsonDepthMarketData["AskPrice5"] = Json::Value(pDepthMarketData->AskPrice5);
	jsonDepthMarketData["AskVolume5"] = Json::Value(pDepthMarketData->AskVolume5);
	jsonDepthMarketData["AveragePrice"] = Json::Value(pDepthMarketData->AveragePrice);
	jsonDepthMarketData["ActionDay"] = Json::Value(pDepthMarketData->ActionDay);
	jsonDepthMarketData["TradingDay"] = Json::Value(pDepthMarketData->TradingDay);
}


void CTickManager::MakeJsonInstrumentInfo(CThostInstrumentField InstrumentInfo, Json::Value &jsonInstrumentInfo)
{
	jsonInstrumentInfo["InstrumentID"] = Json::Value(InstrumentInfo.InstrumentID);
	jsonInstrumentInfo["ExchangeID"] = Json::Value(InstrumentInfo.ExchangeID);
	jsonInstrumentInfo["InstrumentName"] = Json::Value(InstrumentInfo.InstrumentName);
	jsonInstrumentInfo["ProductID"] = Json::Value(InstrumentInfo.ProductID);
	jsonInstrumentInfo["ProductClass"] = Json::Value(InstrumentInfo.ProductClass);
	jsonInstrumentInfo["DeliveryYear"] = Json::Value(InstrumentInfo.DeliveryYear);
	jsonInstrumentInfo["DeliveryMonth"] = Json::Value(InstrumentInfo.DeliveryMonth);
	jsonInstrumentInfo["MaxMarketOrderVolume"] = Json::Value(InstrumentInfo.MaxMarketOrderVolume);
	jsonInstrumentInfo["MinMarketOrderVolume"] = Json::Value(InstrumentInfo.MinMarketOrderVolume);
	jsonInstrumentInfo["MaxLimitOrderVolume"] = Json::Value(InstrumentInfo.MaxLimitOrderVolume);
	jsonInstrumentInfo["MinLimitOrderVolume"] = Json::Value(InstrumentInfo.MinLimitOrderVolume);
	jsonInstrumentInfo["VolumeMultiple"] = Json::Value(InstrumentInfo.VolumeMultiple);
	jsonInstrumentInfo["PriceTick"] = Json::Value(InstrumentInfo.PriceTick);
	jsonInstrumentInfo["CreateDate"] = Json::Value(InstrumentInfo.CreateDate);
	jsonInstrumentInfo["OpenDate"] = Json::Value(InstrumentInfo.OpenDate);
	jsonInstrumentInfo["ExpireDate"] = Json::Value(InstrumentInfo.ExpireDate);
	jsonInstrumentInfo["StartDelivDate"] = Json::Value(InstrumentInfo.StartDelivDate);
	jsonInstrumentInfo["EndDelivDate"] = Json::Value(InstrumentInfo.EndDelivDate);
	jsonInstrumentInfo["InstLifePhase"] = Json::Value(InstrumentInfo.InstLifePhase);
	jsonInstrumentInfo["IsTrading"] = Json::Value(InstrumentInfo.IsTrading);
	jsonInstrumentInfo["LongMarginRatio"] = Json::Value(InstrumentInfo.LongMarginRatio);
	jsonInstrumentInfo["ShortMarginRatio"] = Json::Value(InstrumentInfo.ShortMarginRatio);
	jsonInstrumentInfo["MaxMarginSideAlgorithm"] = Json::Value(InstrumentInfo.MaxMarginSideAlgorithm);
}

void CTickManager::GetLastTradingDay()
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
}