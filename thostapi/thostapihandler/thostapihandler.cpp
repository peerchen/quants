/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapihandlercpp
///@brief 实现客户端接口定义的功能
///@history 
///20160818@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
#define _MISC_AS_TEMPLATE_
#include "../../misc/loghandler.h"
#include "../../misc/mysqlmanager.h"
#undef _MISC_AS_TEMPLATE_

#include "thostapihandler.h"
#include "../ctphandler/thostmdhandler.h"
#include "../ctphandler/thosttradehandler.h"
#include "../fshandler/fsmdhandler.h"
#include "../fshandler/fstradehandler.h"

CThostApiHandler::CThostApiHandler()
{
	m_pThostSpi = NULL;
	m_pThostMdHandler = NULL;
	m_pThostTraderHandler = NULL;
	m_pFSMdHandler = NULL;
	m_pFSTraderHandler = NULL;
	m_pKBarHandler = NULL;
	m_pLogHandler = NULL;
}

void CThostApiHandler::Start(string JsonParam)
{
}

void CThostApiHandler::Stop()
{

}


void CThostApiHandler::RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath)
{
	if (m_pThostMdHandler == NULL)
	{
		m_pThostMdHandler = new CThostMdHandler(InvestorId, Password, BrokerId, FrontAddress, UserDataPath);
		m_pThostMdHandler->RegisterMdHandlerSpi(this);
		m_pThostMdHandler->Start();
	}
}

void CThostApiHandler::RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo)
{
	if (m_pThostTraderHandler == NULL)
	{
		m_pThostTraderHandler = new CThostTraderHandler(InvestorId, Password, BrokerId, FrontAddress, UserDataPath, UserProductInfo);
		m_pThostTraderHandler->RegisterTradeHandlerSpi(this);
		m_pThostTraderHandler->Start();
	}
}

void CThostApiHandler::RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath)
{
	if (m_pFSMdHandler == NULL)
	{
		m_pFSMdHandler = new fstech::CFSMdHandler(InvestorId, Password, BrokerId, FrontAddress, UserDataPath);
		m_pFSMdHandler->RegisterMdHandlerSpi(this);
		m_pFSMdHandler->Start();
	}
}

void CThostApiHandler::RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo)
{
	if (m_pFSTraderHandler == NULL)
	{
		m_pFSTraderHandler = new fstech::CFSTraderHandler(InvestorId, Password, BrokerId, FrontAddress, UserDataPath, UserProductInfo);
		m_pFSTraderHandler->RegisterTradeHandlerSpi(this);
		m_pFSTraderHandler->Start();		
	}
}

void CThostApiHandler::RegisterKBarServer()
{
	m_pKBarHandler = new CKBarHandler();
	m_pKBarHandler->RegisterKBarHandlerSpi(this);
}

int CThostApiHandler::RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db)
{
	///connect mysql
	if (m_mysqlManager.Connect(Host.c_str(), UserId.c_str(), Password.c_str(), db.c_str(), Port, NULL, 0))
	{
		m_pLogHandler->Error("THOSTAPI:%s", m_mysqlManager.GetErrMsg());
		return 1;
	} 
	else
	{
		m_pLogHandler->Info("THOSTAPI:connect database success.");
		return 0;
	}
}

void CThostApiHandler::RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole /* = false */)
{
	string sExePath = boost::filesystem::initial_path<boost::filesystem::path>().string(); 
	if (m_pLogHandler == NULL)
	{
		string sLogFilePath = sExePath + "/userdata/log/";
		string sLogFileName =  sLogFilePath + "thost.log";
		string sLoggerName = "thost"; 
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

int CThostApiHandler::RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset /* = 0 */)
{
	if (m_pKBarHandler != NULL)
	{
		return m_pKBarHandler->RegisterKBarEvent(InstrumentID, Symbol, ExchangeID, PeriodType, PeriodValue, Offset);
	}
	return 1;
}

int CThostApiHandler::UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset /* = 0 */)
{
	if (m_pKBarHandler != NULL)
	{
		return m_pKBarHandler->UnRegisterKBarEvent(InstrumentID, PeriodType, PeriodValue, Offset);
	}
	return 1;
}

int CThostApiHandler::GetThostApiMode()
{
	return THOST_API_MODE_TRADING;
}

void CThostApiHandler::SubscribeQuote(char **ppInstrument, int count)
{
	vector<string> subscribeList;
	for (int i = 0; i < count; ++i)
	{
		subscribeList.push_back(ppInstrument[i]);
	}
	SubscribeQuoteList(subscribeList);
}

void CThostApiHandler::SubscribeQuoteList(vector<string> InstrumentList)
{
	vector<string> fsList;
	vector<string> ctpList;
	for (vector<string>::iterator iter = InstrumentList.begin(); iter != InstrumentList.end(); ++iter)
	{
		string subcribeItem = *iter;
		if (BelongSGE(subcribeItem))
		{
			fsList.push_back(subcribeItem);
		}
		else
		{
			ctpList.push_back(subcribeItem);
		}
	}

	if (fsList.size() > 0 && m_pFSMdHandler != NULL)
	{
		m_pFSMdHandler->SubscribeQuoteList(fsList);
	}

	if (ctpList.size() > 0 && m_pThostMdHandler != NULL)
	{
		m_pThostMdHandler->SubscribeQuoteList(ctpList);
	}
}

int CThostApiHandler::GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSMdHandler != NULL)
		{
			return m_pFSMdHandler->GetMarketDataSnapshot(InstrumentID, DepthMarketDataSnapshot);
		}
	}
	else
	{
		if (m_pThostMdHandler != NULL)
		{
			return m_pThostMdHandler->GetMarketDataSnapshot(InstrumentID, DepthMarketDataSnapshot);
		}
	}
	return 1;
}

void CThostApiHandler::LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			m_pFSTraderHandler->LimitOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			m_pThostTraderHandler->LimitOrder(RequestID, InstrumentID, Direction,  OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
}

void CThostApiHandler::MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			m_pFSTraderHandler->MarketOrder(RequestID, InstrumentID, Direction, OffsetFlag, Volume, HedgeFlag);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			m_pThostTraderHandler->MarketOrder(RequestID, InstrumentID, Direction, OffsetFlag, Volume, HedgeFlag);
		}
	}
}

void CThostApiHandler::FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			m_pFSTraderHandler->FakOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			m_pThostTraderHandler->FakOrder(RequestID, InstrumentID, Direction,  OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
}

void CThostApiHandler::FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			m_pFSTraderHandler->FokOrder(RequestID, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			m_pThostTraderHandler->FokOrder(RequestID, InstrumentID, Direction,  OffsetFlag, LimitPrice, Volume, HedgeFlag);
		}
	}
}

int CThostApiHandler::CancelOrder(string OrderRef, string OrderSysID, string ExchangeID)
{
	if (strcmp(ExchangeID.c_str(), "SGE") == 0)
	{
		if (m_pFSTraderHandler != NULL)
		{
			return m_pFSTraderHandler->CancelOrder(OrderRef, OrderSysID, ExchangeID);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			return m_pThostTraderHandler->CancelOrder(OrderRef, OrderSysID, ExchangeID);
		}
	}
	return 1;
}

int	CThostApiHandler::GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo)
{
	if (m_pThostTraderHandler != NULL)
	{
		m_pThostTraderHandler->GetAllInstrument(vecInstrumentInfo);
	}
	return 0;
}

int CThostApiHandler::GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			return m_pFSTraderHandler->GetInstrument(InstrumentInfo, InstrumentID);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			return m_pThostTraderHandler->GetInstrument(InstrumentInfo, InstrumentID);
		}
	}
	return 1;
}

int CThostApiHandler::GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			return m_pFSTraderHandler->GetInstrumentMarginRate(InstrumentMarginRate, InstrumentID);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			return m_pThostTraderHandler->GetInstrumentMarginRate(InstrumentMarginRate, InstrumentID);
		}
	}
	return 1;
}

int CThostApiHandler::GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID)
{
	if (BelongSGE(InstrumentID))
	{
		if (m_pFSTraderHandler != NULL)
		{
			return m_pFSTraderHandler->GetInstrumentCommissionRate(InstrumentCommissionRate, InstrumentID);
		}
	}
	else
	{
		if (m_pThostTraderHandler != NULL)
		{
			return m_pThostTraderHandler->GetInstrumentCommissionRate(InstrumentCommissionRate, InstrumentID);
		}
	}
	return 1;
}

int CThostApiHandler::GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID)
{
	if (m_pThostTraderHandler != NULL && InvestorID == m_pThostTraderHandler->GetInvestorID())
	{
		return m_pThostTraderHandler->GetTradingAccount(TradingAccount);
	}
	if (m_pFSTraderHandler != NULL && InvestorID == m_pFSTraderHandler->GetInvestorID())
	{
		return m_pFSTraderHandler->GetTradingAccount(TradingAccount);
	}
	return 1;
}

int CThostApiHandler::GetInvestorPosition(vector<CThostInvestorPositionField> &vecInvestorPosition, string InvestorID, string InstrumentID)
{
	if (m_pThostTraderHandler != NULL && InvestorID == m_pThostTraderHandler->GetInvestorID())
	{
		return m_pThostTraderHandler->GetInvestorPosition(vecInvestorPosition, InstrumentID);
	}
	if (m_pFSTraderHandler != NULL && InvestorID == m_pFSTraderHandler->GetInvestorID())
	{
		return m_pFSTraderHandler->GetInvestorPosition(vecInvestorPosition, InstrumentID);
	}
	return 1;
}

int CThostApiHandler::GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd)
{
	if (m_pThostTraderHandler != NULL && InvestorID == m_pThostTraderHandler->GetInvestorID())
	{
		return m_pThostTraderHandler->GetOrder(vecOrderReport, InstrumentID, ExchangeID, OrderSysID, InsertTimeStart, InsertTimeEnd);
	}
	if (m_pFSTraderHandler != NULL && InvestorID == m_pFSTraderHandler->GetInvestorID())
	{
		return m_pFSTraderHandler->GetOrder(vecOrderReport, InstrumentID, ExchangeID, OrderSysID, InsertTimeStart, InsertTimeEnd);
	}
	return 1;
}

int CThostApiHandler::GetTrade(TradeVec  &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd)
{
	if (m_pThostTraderHandler != NULL && InvestorID == m_pThostTraderHandler->GetInvestorID())
	{
		return m_pThostTraderHandler->GetTrade(vecTrade, InstrumentID, ExchangeID, TradeID, TradeTimeStart, TradeTimeEnd);
	}
	if (m_pFSTraderHandler != NULL && InvestorID == m_pFSTraderHandler->GetInvestorID())
	{
		return m_pFSTraderHandler->GetTrade(vecTrade, InstrumentID, ExchangeID, TradeID, TradeTimeStart, TradeTimeEnd);
	}
	return 1;
}

int CThostApiHandler::ReqQryHistoryKBar(string InstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue)
{
	vector<string> vecInstrumentID;
	vecInstrumentID.push_back(InstrumentID);
	return ReqQryHistoryKBar(vecInstrumentID, StartTradinDate, EndTradingDate, PeriodType, PeriodValue);
}
int CThostApiHandler::ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue)
{
	if (m_mysqlManager.IsConnect())
	{
		///获取[start,end]中所有的交易日
		char strCommand[2000];
		memset(strCommand, '\0', sizeof(strCommand));
		sprintf(strCommand, "select date from futures_callendar where date >= %d and date <= %d and exchangeid = 'SHFE'", StartTradinDate, EndTradingDate);
		if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
		{
			m_pLogHandler->Error("THOSTAPI:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
			return 2;
		}
		vector<int> vecTradingDate;
		MYSQL_ROW row;
		MYSQL_RES *mysql_res = m_mysqlManager.GetResult();
		while ((row = mysql_fetch_row(mysql_res)) != NULL)
		{
			vecTradingDate.push_back(atoi(row[0]));
		}
		m_mysqlManager.ReleaseResult();
		int nTradingDateSize = vecTradingDate.size();
		///遍历所有交易日,每次获取单个交易日的kbar数据
		for (int i = 0; i < nTradingDateSize; ++i)
		{
			vector<CKBarField> vecKBarByTradingDate;
			for (int j = 0; j < vecInstrumentID.size(); ++j)
			{
				memset(strCommand, '\0', sizeof(strCommand));
				sprintf(strCommand, "select * from futures_%dmin where tradingdate = %d and contract = '%s'", PeriodValue, vecTradingDate[i], vecInstrumentID[j].c_str());
				if (m_mysqlManager.Execute(strCommand, strlen(strCommand)))
				{
					m_pLogHandler->Error("THOSTAPI:Errmsg=%s,Command=%s", m_mysqlManager.GetErrMsg(), strCommand);
					return 2;
				}
				mysql_res = m_mysqlManager.GetResult();
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
			sort(vecKBarByTradingDate.begin(), vecKBarByTradingDate.end());
			int nKBarByTradingDateSize = vecKBarByTradingDate.size();
			for (int k = 0; k < nKBarByTradingDateSize; ++k)
			{
				int nEndMask = THOST_ENDMASK_NOEND;
				if (k == nKBarByTradingDateSize - 1)
				{
					nEndMask = THOST_ENDMASK_TRADINGDAYEND;
					if (i == nTradingDateSize - 1)
					{
						nEndMask |= THOST_ENDMASK_ALLEND;	
					}
				}
				m_pThostSpi->OnRspQryHistoryKBar(vecTradingDate[i], vecKBarByTradingDate[k].InstrumentID, &vecKBarByTradingDate[k], nEndMask);				
			}
			vecKBarByTradingDate.clear();
		}
		return 0;
	}
	return 1;
}

void CThostApiHandler::Log(int level, const char *format, ...)
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

void CThostApiHandler::OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData)
{
	if (m_pThostSpi != NULL)
	{
		m_pThostSpi->OnRtnDepthMarketData(pDepthMarketData);
	}
}

void CThostApiHandler::OnRtnOrderReport(CThostOrderReportField *pOrder)
{
	if (pOrder != NULL && m_pThostSpi != NULL)
	{
		m_pThostSpi->OnRtnOrderReport(pOrder);
	}
}

bool CThostApiHandler::BelongSGE(string &InstrumentID)
{
	if (strcmp(InstrumentID.c_str(), "Au(T+D)") == 0 || strcmp(InstrumentID.c_str(), "Ag(T+D)") == 0)
	{
		return true;
	}
	return false;
}