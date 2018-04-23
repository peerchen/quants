/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapibthandlercpp
///@brief 实现回测功能
///@history 
///20160926@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#define _MISC_AS_TEMPLATE_
#include "../../misc/strategyeval.h"
#undef _MISC_AS_TEMPLATE_

#include "thostapibthandler.h"
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <json/json.h>

CThostApiBTHandler::CThostApiBTHandler()
{
	m_pThostSpi = NULL;
	m_pLogHandler = NULL;
}

/*
JsonParam结构如下
{
	"Account":{"InitialBalance":double}
	"Data":{"StartDate":int, "EndDate":int, "PeriodType":int, "PeriodValue":int},
	"Sql":{"Host":string, "Port":int, "User":string, "Password":string, "DB":string},
	"InstrumentList":[{"InstrumentID":string, "ExchangeID:"string, "VolumeMultiple":int, "Slippage":double, 
					   "LongMarginRatioByMoney":double, "LongMarginRatioByVolume":double, "ShortMarginRatioByMoney":double, "ShortMarginRatioByVolume":double,},
					   "OpenRatioByMoney":double, "OpenRatioByVolume":double, "CloseRatioByMoney":double, "CloseRatioByVolume":double, "CloseTodayRatioByMoney":double, "CloseTodayRatioByVolume":double
					   {},......,{}
					 ]
}
*/
void CThostApiBTHandler::Start(string JsonParam)
{
	Json::Reader reader;
	Json::Value json_object, json_simulation_object;

	///校验参数
	if (!reader.parse(JsonParam.c_str(), JsonParam.c_str()+JsonParam.length(), json_object))
	{
		m_pLogHandler->Error("Error JsonParam!");
		return ;
	}

	if (!json_object["Account"].isObject()
		|| !json_object["Data"].isObject() 
		|| !json_object["Sql"].isObject()
		|| !json_object["InstrumentList"].isArray())
	{
		m_pLogHandler->Error("Invaild Params!");
		return ;
	}
	
	json_simulation_object = json_object;
	json_simulation_object.removeMember("Data");
	json_simulation_object.removeMember("Sql");
	Json::FastWriter fwt;
	string json_simulation_param = fwt.write(json_simulation_object);
	
	///初始化模拟交易
	m_pSimulationTraderHandler = new CSimulationTraderHandler();
	m_pSimulationTraderHandler->RegisterTradeHandlerSpi(this);
	m_pSimulationTraderHandler->Start(json_simulation_param);

	/*
	获取回测需要的参数
	1. 合约列表
	2. 数据时间范围
	3. 数据的周期类型
	前期开发先用固定参数
	*/

	const Json::Value& jsonSql = json_object["Sql"];
	if (!jsonSql["Host"].isString() 
		|| !jsonSql["Port"].isNumeric() 
		|| !jsonSql["User"].isString()
		|| !jsonSql["Password"].isString()
		|| !jsonSql["DB"].isString())
	{
		m_pLogHandler->Error("Invaild Sql Params!");
		return ;
	}
	string Host = jsonSql["Host"].asString();
	int Port = jsonSql["Port"].asInt();
	string User = jsonSql["User"].asString();
	string Password = jsonSql["Password"].asString();
	string DB = jsonSql["DB"].asString();
	
	if (!RegisterMySQLServer(Host.c_str(), Port, User.c_str(), Password.c_str(), DB.c_str()))
	{
		const Json::Value& json_data = json_object["Data"];
		if (!json_data["StartDate"].isNumeric()
			|| !json_data["EndDate"].isNumeric()
			|| !json_data["PeriodType"].isNumeric()
			|| !json_data["PeriodValue"].isNumeric())
		{
			m_pLogHandler->Error("Invaild Data Params!");
			return ;
		}

		int StartDate = json_data["StartDate"].asInt();
		int EndDate = json_data["EndDate"].asInt();
		char PeriodType = json_data["PeriodType"].asInt();
		int PeriodValue = json_data["PeriodValue"].asInt();

		vector<string> vecInstrumentID;
		const Json::Value& json_instrumentlist = json_object["InstrumentList"];
		for (int i = 0; i < json_instrumentlist.size(); ++i)
		{
			const Json::Value& json_instrument = json_instrumentlist[i];
			if (!json_instrument["InstrumentID"].isString())
			{
				m_pLogHandler->Error("Invaild Instrumet Params!");
				return ;
			}
			vecInstrumentID.push_back(json_instrument["InstrumentID"].asString());
		}
		m_pLogHandler->Info("Start backtest.");
		ReqQryHistoryKBar(vecInstrumentID, StartDate, EndDate, PeriodType, PeriodValue);
	}
}

void CThostApiBTHandler::Stop()
{
}


void CThostApiBTHandler::RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath)
{
}

void CThostApiBTHandler::RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo)
{
}

void CThostApiBTHandler::RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath)
{
}

void CThostApiBTHandler::RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo)
{
}

void CThostApiBTHandler::RegisterKBarServer()
{
}

int CThostApiBTHandler::RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db)
{
	///connect mysql
	if (m_mysqlManager.Connect(Host.c_str(), UserId.c_str(), Password.c_str(), db.c_str(), Port, NULL, 0))
	{
		m_pLogHandler->Error("THOSTBTAPI:%s", m_mysqlManager.GetErrMsg());
		return 1;
	} 
	else
	{
		m_pLogHandler->Info("THOSTBTAPI:connect database success.");
		return 0;
	}
}

void CThostApiBTHandler::RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole /* = false */)
{
	string sExePath = boost::filesystem::initial_path<boost::filesystem::path>().string(); 
	if (m_pLogHandler == NULL)
	{
		string sLogFilePath = sExePath + "/userdata/log/";
		string sLogFileName =  sLogFilePath + "thostbt.log";
		string sLoggerName = "thostbt"; 
		boost::filesystem::create_directories(sLogFilePath.c_str()); 
		m_pLogHandler = new CLogHandler();
		m_pLogHandler->InitLogger(sLoggerName.c_str());
		m_pLogHandler->AddFileLogger(sLogFileName.c_str());
		if (bIsEnableConsole == true)
		{
			m_pLogHandler->AddConsoleLogger();
		}
	}
}

int CThostApiBTHandler::RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset /* = 0 */)
{
	return 0;
}

int CThostApiBTHandler::UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset /* = 0 */)
{
	return 0;
}

int CThostApiBTHandler::GetThostApiMode()
{
	return THOST_API_MODE_BACKTEST;
}

void CThostApiBTHandler::SubscribeQuote(char **ppInstrument, int count)
{
	
}

void CThostApiBTHandler::SubscribeQuoteList(vector<string> InstrumentList)
{
}

int CThostApiBTHandler::GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot)
{
	return 0;
}

void CThostApiBTHandler::LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (m_pSimulationTraderHandler != NULL)
	{
		m_pSimulationTraderHandler->LimitOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	}
}

void CThostApiBTHandler::MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (m_pSimulationTraderHandler != NULL)
	{
		m_pSimulationTraderHandler->MarketOrder(RequestID, InstrumentID, Direction, OffsetFlag, Volume, HedgeFlag);
	}
}

void CThostApiBTHandler::FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (m_pSimulationTraderHandler != NULL)
	{
		m_pSimulationTraderHandler->FakOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	}
}

void CThostApiBTHandler::FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (m_pSimulationTraderHandler != NULL)
	{
		m_pSimulationTraderHandler->FokOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	}
}

int CThostApiBTHandler::CancelOrder(string OrderRef, string OrderSysID, string ExchangeID)
{
	if (m_pSimulationTraderHandler != NULL)
	{
		return m_pSimulationTraderHandler->CancelOrder(OrderRef, OrderSysID, ExchangeID);
	}
	return 0;
}

int	CThostApiBTHandler::GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo)
{
	return 0;
}

int CThostApiBTHandler::GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID)
{
	return 0;
}

int CThostApiBTHandler::GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID)
{
	return 1;
}

int CThostApiBTHandler::GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID)
{
	return 1;
}

int CThostApiBTHandler::GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID)
{
	return 1;
}

int CThostApiBTHandler::GetInvestorPosition(vector<CThostInvestorPositionField> &vecInvestorPosition, string InvestorID, string InstrumentID)
{
	return 1;
}

int CThostApiBTHandler::GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd)
{
	return 1;
}

int CThostApiBTHandler::GetTrade(TradeVec  &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd)
{
	return 1;
}

int CThostApiBTHandler::ReqQryHistoryKBar(string InstrumentID, int StartTradingDate, int EndTradingDate, char PeriodType, int PeriodValue)
{
	vector<string> vecInstrumentID;
	vecInstrumentID.push_back(InstrumentID);
	return ReqQryHistoryKBar(vecInstrumentID, StartTradingDate, EndTradingDate, PeriodType, PeriodValue);
}

int CThostApiBTHandler::ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradingDate, int EndTradingDate, char PeriodType, int PeriodValue)
{
	if (m_mysqlManager.IsConnect())
	{
		///获取[start,end]中所有的交易日
		vector<int> vecTradingDate;
		GetTradingDayList(vecTradingDate, StartTradingDate, EndTradingDate);

		int nTradingDateSize = vecTradingDate.size();
		///遍历所有交易日,每次获取单个交易日的kbar数据
		for (int i = 0; i < nTradingDateSize; ++i)
		{
			vector<CKBarField> vecKBarByTradingDate;
			GetKBarListByTradingDay(vecKBarByTradingDate, vecInstrumentID, PeriodType, PeriodValue, vecTradingDate[i]);

			///按照时间戳排序
			sort(vecKBarByTradingDate.begin(), vecKBarByTradingDate.end());
			int nKBarByTradingDateSize = vecKBarByTradingDate.size();
			m_pLogHandler->Info("Backtesting:%d", vecTradingDate[i]);
			for (int k  = 0; k < nKBarByTradingDateSize; ++k)
			{
				int nEndMask = THOST_ENDMASK_NOEND;
				///最后一个kbar
				if (k == nKBarByTradingDateSize - 1)
				{
					nEndMask = THOST_ENDMASK_TRADINGDAYEND;
					///单个交易日最后一个kbar
					if (i == nTradingDateSize - 1)
					{
						nEndMask |= THOST_ENDMASK_ALLEND;	
					}
				}
				///更新kbar的快照
				if (m_pSimulationTraderHandler != NULL)
				{
					m_pSimulationTraderHandler->UpdateKBarSnapshot(vecKBarByTradingDate[k].InstrumentID, vecKBarByTradingDate[k]);
				}
				if (m_pThostSpi != NULL)
				{
					m_pThostSpi->OnRtnKBar(&vecKBarByTradingDate[k]);	
				}
				if (nEndMask & THOST_ENDMASK_TRADINGDAYEND)
				{
					///当个交易日结束，发出清算信号
					if (m_pSimulationTraderHandler != NULL)
					{
						m_pSimulationTraderHandler->BeginTradingDaySettle(vecTradingDate[i]);
					}
				}
				if (nEndMask & THOST_ENDMASK_ALLEND)
				{
					///整个回测数据回溯结束，发出计算指标信号
					EvalStrategy();
				}
			}
			vecKBarByTradingDate.clear();
		}
		return 0;
	}
	return 1;
}

void CThostApiBTHandler::GetTradingDayList(vector<int> &vecTradingDate, int StartTradinDate, int EndTradingDate, string ExchangeID /* = "SHFE" */)
{
	vecTradingDate.clear();
	if (m_mysqlManager.IsConnect())
	{
		char strCommand[2000];
		memset(strCommand, '\0', sizeof(strCommand));
		sprintf(strCommand, "select date from futures_callendar where date >= %d and date <= %d and exchangeid = '%s'", StartTradinDate, EndTradingDate, ExchangeID.c_str());
		if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
		{
			m_pLogHandler->Error("THOSTAPI:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
			return ;
		}
		MYSQL_ROW row;
		MYSQL_RES *mysql_res = m_mysqlManager.GetResult();
		while ((row = mysql_fetch_row(mysql_res)) != NULL)
		{
			vecTradingDate.push_back(atoi(row[0]));
		}
		m_mysqlManager.ReleaseResult();
	}
}

void CThostApiBTHandler::GetKBarListByTradingDay(vector<CKBarField> &vecKBarByTradingDate, vector<string> &vecInstrumentID, int PeriodType, int PeriodValue, int TradingDay)
{
	vecKBarByTradingDate.clear();
	char strCommand[2000];
	for (int j = 0; j < vecInstrumentID.size(); ++j)
	{
		memset(strCommand, '\0', sizeof(strCommand));
		sprintf(strCommand, "select * from futures_%dmin where tradingdate = %d and contract = '%s'", PeriodValue, TradingDay, vecInstrumentID[j].c_str());
		if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
		{
			m_pLogHandler->Error("THOSTAPI:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
			return ;
		}
		MYSQL_ROW row;
		MYSQL_RES *mysql_res = m_mysqlManager.GetResult();
		while ((row = mysql_fetch_row(mysql_res)) != NULL)
		{
			CKBarField kbarField;
			///组装kbar
			kbarField.PeriodType = PeriodType;
			kbarField.PeriodValue = PeriodValue;
			kbarField.Symbol = row[1];
			kbarField.InstrumentID = row[2];
			kbarField.ExchangeID = row[3];
			kbarField.Date = atoi(row[4]);
			kbarField.BeginTime = atoi(row[5]);
			kbarField.EndTime = atoi(row[6]);
			kbarField.OpenPrice = atof(row[7]);
			kbarField.HighPrice = atof(row[8]);
			kbarField.LowPrice = atof(row[9]);
			kbarField.ClosePrice = atof(row[10]);
			kbarField.AskPrice = atof(row[11]);
			kbarField.AskVolume = atoi(row[12]);
			kbarField.BidPrice = atof(row[13]);
			kbarField.BidVolume = atoi(row[14]);
			kbarField.Volume = atoi(row[15]);
			kbarField.OpenInterest = atoi(row[16]);
			kbarField.Ticks = atoi(row[17]);
			kbarField.TWapPrice = atof(row[18]);   
			kbarField.VWapPrice = atof(row[19]);
			kbarField.PreClosePrice = atof(row[20]);
			kbarField.PreVolume = atoi(row[21]);
			kbarField.PreOpenInterest = atoi(row[22]);
			kbarField.Offset = atoi(row[23]);
			kbarField.IsFill = atoi(row[24]);
			kbarField.Status = atoi(row[25]);
			kbarField.BarIndex = atoi(row[26]);
			kbarField.TradingDate = atoi(row[27]);

			vecKBarByTradingDate.push_back(kbarField);
		}
		m_mysqlManager.ReleaseResult();
	}
}

void CThostApiBTHandler::Log(int level, const char *format, ...)
{
	if (m_pLogHandler == NULL)
	{
		return;
	}
	va_list args;
	char pszMessage[1024];
	memset(pszMessage, '\0', sizeof(pszMessage));
	va_start(args, format);
	vsnprintf(pszMessage, sizeof(pszMessage), format, args);
	va_end(args);
	switch(level)
	{
	case LOG_LEVEL_INFO:
		m_pLogHandler->Info(pszMessage);
		break;
	case LOG_LEVEL_ERROR:
		m_pLogHandler->Error(pszMessage);
		break;
	case LOG_LEVEL_DEBUG:
		m_pLogHandler->Debug(pszMessage);
	}
}

void CThostApiBTHandler::OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData)
{
	if (m_pThostSpi != NULL && pDepthMarketData != NULL)
	{
		m_pThostSpi->OnRtnDepthMarketData(pDepthMarketData);
	}
}

void CThostApiBTHandler::OnRtnOrderReport(CThostOrderReportField *pOrder)
{
	if (m_pThostSpi != NULL && pOrder != NULL)
	{
		m_pThostSpi->OnRtnOrderReport(pOrder);
	}
}

bool CThostApiBTHandler::BelongSGE(string &InstrumentID)
{
	if (strcmp(InstrumentID.c_str(), "Au(T+D)") == 0 || strcmp(InstrumentID.c_str(), "Ag(T+D)") == 0)
	{
		return true;
	}
	return false;
}

void CThostApiBTHandler::EvalStrategy()
{
	vector<double> vecBalance;
	m_pSimulationTraderHandler->GetBalanceList(vecBalance);
	m_strategyeval.SetEvalSample(vecBalance, m_vecBenchmarkIndex);
	m_strategyeval.StartEval();
	CThostStrategyEvalResultField strategyEvalResult;
	m_strategyeval.GetEvalResult(strategyEvalResult);
	m_pLogHandler->Info ("年化收益率:%.3f\n夏普比率:%.3f\n最大回撤:%.3f\n胜率:%.3f",
		strategyEvalResult.AnnualizedReturns,
		strategyEvalResult.SharpRatio,
		strategyEvalResult.MaxDrawDown,
		strategyEvalResult.WinRatio);
	
	char strBalanceList[10000];
	memset(strBalanceList, '\0', sizeof(strBalanceList));
	if (vecBalance.size() > 0)
	{
		sprintf(strBalanceList, "%.2f", vecBalance[0]);
		for (int i = 1; i < vecBalance.size(); ++i)
		{
			int nLen = strlen(strBalanceList);
			sprintf(strBalanceList+nLen, ",%.2f", vecBalance[i]);
		}
		m_pLogHandler->Info("BalanceList:%s", strBalanceList);
	}
}