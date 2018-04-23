#include "fsmdhandler.h"

using namespace fstech;

CFSMdHandler::CFSMdHandler()
{
	m_pMdApi = NULL;
	m_pLogHandler = NULL;
}

CFSMdHandler::CFSMdHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath) :
	CThostMdApi(InvestorId, Password, BrokerId, FrontAddress, FlowPath)
{
	m_pMdApi = NULL;
	m_pLogHandler = NULL;
}

int CFSMdHandler::Start(string JsonParam)
{
	return ReqConnectFront();
}

int CFSMdHandler::Wait()
{
	m_pMdApi->Join();
	return 0;
}

void CFSMdHandler::Stop()
{
	m_pMdApi->Release();
}

int CFSMdHandler::ReqConnectFront()
{
	string sflowpath;
	if (m_sUserDataPath != "")
	{
		sflowpath = m_sUserDataPath + "/flow/fs/md/" + m_sInvestorId + "/";
	}
	else
	{
		string sflowpath = "./flow/fs/md/" + m_sInvestorId + "/";
	}
	boost::filesystem::create_directories(sflowpath.c_str());
	m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi(sflowpath.c_str());
	m_pMdApi->RegisterSpi(this);

	char *pszFrontAddress = (char*)malloc(m_sFrontAddress.length()+1);
	memset(pszFrontAddress, '\0', m_sFrontAddress.length()+1);
	strcpy(pszFrontAddress, m_sFrontAddress.c_str());
	m_pMdApi->RegisterFront(pszFrontAddress);
	m_pMdApi->Init();
	boost::mutex::scoped_lock lock(m_connectedMutex);
	int nRet = m_connectedCond.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	if (nRet)
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Info("FSMD:front connect success!");
		}
	}
	else
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Error("FSMD:front connect failed!");
		}
	}
	free(pszFrontAddress);
	return !nRet;
}

void CFSMdHandler::OnFrontConnected()
{
	m_bConnected = true;
	CThostFtdcReqUserLoginField reqUserLoginField;
	memset(&reqUserLoginField, 0, sizeof(CThostFtdcReqUserLoginField));
	strcpy(reqUserLoginField.UserID, m_sInvestorId.c_str());
	strcpy(reqUserLoginField.Password, m_sPasswd.c_str());
	strcpy(reqUserLoginField.BrokerID, m_sBrokerId.c_str());
	m_pMdApi->ReqUserLogin(&reqUserLoginField, ++m_nRequestId);
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Info("FSMD:OnFrontConnected");
	}
}	

void CFSMdHandler::OnFrontDisconnected(int nReason)
{
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Error("FSMD:OnFrontDisconnected:%d",nReason);
	}
	boost::this_thread::sleep(boost::posix_time::seconds(THOST_DISCONNECT_SLEEP_TIME_S));
}


void CFSMdHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo->ErrorID == 0) 
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Info("FSMD:OnRspUserLogin");
		}
		boost::mutex::scoped_lock lock(m_connectedMutex);
		m_connectedCond.notify_one();
		SubscribeQuoteList(m_vecSubscribeList);
	}
}

void CFSMdHandler::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Info("FSMD:%s subscribe success!", pSpecificInstrument->InstrumentID);
	}
	vector<string>::iterator iter = m_vecSubscribeList.begin();
	for (; iter != m_vecSubscribeList.end(); ++iter)
	{
		if (strcmp(pSpecificInstrument->InstrumentID, (*iter).c_str()) == 0)
		{
			break;
		}
	}
	if (iter == m_vecSubscribeList.end())
	{	
		m_vecSubscribeList.push_back(pSpecificInstrument->InstrumentID);
	}
}

void CFSMdHandler::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	vector<string>::iterator iter = m_vecSubscribeList.begin();
	for (; iter != m_vecSubscribeList.end(); ++iter)
	{
		if (strcmp(pSpecificInstrument->InstrumentID, (*iter).c_str()) == 0)
		{
			break;
		}
	}
	if (iter != m_vecSubscribeList.end())
	{	
		m_vecSubscribeList.erase(iter);
	}
}

void CFSMdHandler::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (m_pThostMdHandlerSpi != NULL)
	{
		CThostDepthMarketDataField depthMarketDataField;
		depthMarketDataField.InstrumentID = pDepthMarketData->InstrumentID;
		depthMarketDataField.ExchangeID = pDepthMarketData->ExchangeID;
		depthMarketDataField.LastPrice = pDepthMarketData->LastPrice;
		depthMarketDataField.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
		depthMarketDataField.PreClosePrice = pDepthMarketData->PreClosePrice;
		depthMarketDataField.PreOpenInterest = pDepthMarketData->PreOpenInterest;
		depthMarketDataField.OpenPrice = pDepthMarketData->OpenPrice;
		depthMarketDataField.HighestPrice = pDepthMarketData->HighestPrice;
		depthMarketDataField.LowestPrice = pDepthMarketData->LowestPrice;
		depthMarketDataField.Volume = pDepthMarketData->Volume;
		depthMarketDataField.Turnover = pDepthMarketData->Turnover;
		depthMarketDataField.OpenInterest = pDepthMarketData->OpenInterest;
		depthMarketDataField.ClosePrice = pDepthMarketData->ClosePrice;
		depthMarketDataField.SettlementPrice = pDepthMarketData->SettlementPrice;
		depthMarketDataField.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
		depthMarketDataField.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
		depthMarketDataField.PreDelta = pDepthMarketData->PreDelta;
		depthMarketDataField.CurrDelta = pDepthMarketData->CurrDelta;
		if (strlen(pDepthMarketData->UpdateTime) >= 8)
		{
			depthMarketDataField.UpdateTime = (pDepthMarketData->UpdateTime[0]-'0')*100000 + (pDepthMarketData->UpdateTime[1]-'0')*10000 + 
				(pDepthMarketData->UpdateTime[3]-'0')*1000 + (pDepthMarketData->UpdateTime[4]-'0')*100 + (pDepthMarketData->UpdateTime[6]-'0')*10 + (pDepthMarketData->UpdateTime[7]-'0');;
		}
		depthMarketDataField.UpdateMillisec = pDepthMarketData->UpdateMillisec;
		depthMarketDataField.BidPrice1 = pDepthMarketData->BidPrice1;
		depthMarketDataField.BidVolume1 = pDepthMarketData->BidVolume1;
		depthMarketDataField.AskPrice1 = pDepthMarketData->AskPrice1;
		depthMarketDataField.AskVolume1 = pDepthMarketData->AskVolume1;

		depthMarketDataField.BidPrice2 = pDepthMarketData->BidPrice2;
		depthMarketDataField.BidVolume2 = pDepthMarketData->BidVolume2;
		depthMarketDataField.AskPrice2 = pDepthMarketData->AskPrice2;
		depthMarketDataField.AskVolume2 = pDepthMarketData->AskVolume2;

		depthMarketDataField.BidPrice3 = pDepthMarketData->BidPrice3;
		depthMarketDataField.BidVolume3 = pDepthMarketData->BidVolume3;
		depthMarketDataField.AskPrice3 = pDepthMarketData->AskPrice3;
		depthMarketDataField.AskVolume3 = pDepthMarketData->AskVolume3;

		depthMarketDataField.BidPrice4 = pDepthMarketData->BidPrice4;
		depthMarketDataField.BidVolume4 = pDepthMarketData->BidVolume4;
		depthMarketDataField.AskPrice4 = pDepthMarketData->AskPrice4;
		depthMarketDataField.AskVolume4 = pDepthMarketData->AskVolume4;

		depthMarketDataField.BidPrice5 = pDepthMarketData->BidPrice5;
		depthMarketDataField.BidVolume5 = pDepthMarketData->BidVolume5;
		depthMarketDataField.AskPrice5 = pDepthMarketData->AskPrice5;
		depthMarketDataField.AskVolume5 = pDepthMarketData->AskVolume5;


		depthMarketDataField.AveragePrice = pDepthMarketData->AveragePrice;
		depthMarketDataField.ActionDay = atoi(pDepthMarketData->ActionDay);
		depthMarketDataField.TradingDay = atoi(pDepthMarketData->TradingDay);

		FixExceptionPrice(depthMarketDataField.OpenPrice);
		FixExceptionPrice(depthMarketDataField.ClosePrice);
		FixExceptionPrice(depthMarketDataField.HighestPrice);
		FixExceptionPrice(depthMarketDataField.LowestPrice);
		FixExceptionPrice(depthMarketDataField.LastPrice);
		FixExceptionPrice(depthMarketDataField.PreDelta);
		FixExceptionPrice(depthMarketDataField.CurrDelta);
		FixExceptionPrice(depthMarketDataField.SettlementPrice);

		FixExceptionPrice(depthMarketDataField.BidPrice1);
		FixExceptionPrice(depthMarketDataField.BidPrice2);
		FixExceptionPrice(depthMarketDataField.BidPrice3);
		FixExceptionPrice(depthMarketDataField.BidPrice4);
		FixExceptionPrice(depthMarketDataField.BidPrice5);
		FixExceptionPrice(depthMarketDataField.AskPrice1);
		FixExceptionPrice(depthMarketDataField.AskPrice2);
		FixExceptionPrice(depthMarketDataField.AskPrice3);
		FixExceptionPrice(depthMarketDataField.AskPrice4);
		FixExceptionPrice(depthMarketDataField.AskPrice5);
// 		DepthMarketDataSnapshotMap::iterator iter = m_DepthMarketDataSnapshotMap.find(depthMarketDataField.InstrumentID);
// 		if (iter != m_DepthMarketDataSnapshotMap.end())
// 		{
// 			/// remove dirty data 
// 			if (ISDIRTYDATA(iter->second.TradingDay, iter->second.UpdateTime, depthMarketDataField.TradingDay, depthMarketDataField.UpdateTime))
// 			{
// 				return ;
// 			}
// 		}
		m_DepthMarketDataSnapshotMap[depthMarketDataField.InstrumentID] = depthMarketDataField;
		m_pThostMdHandlerSpi->OnRtnDepthMarketData(&depthMarketDataField);	
	}
}

void CFSMdHandler::SubscribeQuote(char **ppInstrument, int count)
{
	m_pMdApi->SubscribeMarketData(ppInstrument, count);
}