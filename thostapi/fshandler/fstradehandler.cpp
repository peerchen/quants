#include "fstradehandler.h"

using namespace fstech;

CFSTraderHandler::CFSTraderHandler()
{
	m_pTraderApi = NULL;
	m_pLogHandler = NULL;
}

CFSTraderHandler::CFSTraderHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo) :
	CThostTraderApi(InvestorId, Password, BrokerId, FrontAddress, m_sUserDataPath, UserProductInfo)
{
	m_pTraderApi = NULL;
	m_pLogHandler = NULL;
}	

int CFSTraderHandler::Start(string JsonParam)
{
	ReqConnectFront();
	return 0;
}

int CFSTraderHandler::Wait()
{
	m_pTraderApi->Join();
	return 0;
}

void CFSTraderHandler::Stop()
{
	m_pTraderApi->Release();
}


int CFSTraderHandler::ReqConnectFront()
{
	string sflowpath;
	if (m_sUserDataPath != "")
	{
		sflowpath = m_sUserDataPath + "/flow/fs/trade/" + m_sInvestorId + "/";
	}
	else
	{
		string sflowpath = "./flow/fs/trade/" + m_sInvestorId + "/";
	}
	boost::filesystem::create_directories(sflowpath.c_str());
	m_pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(sflowpath.c_str());
	m_pTraderApi->RegisterSpi(this);
	m_pTraderApi->SubscribePrivateTopic(THOST_TERT_QUICK);
	m_pTraderApi->SubscribePublicTopic(THOST_TERT_QUICK);

	char *pszFrontAddress = (char*)malloc(m_sFrontAddress.length()+1);
	memset(pszFrontAddress, '\0', m_sFrontAddress.length() + 1);
	strcpy(pszFrontAddress, m_sFrontAddress.c_str());
	m_pTraderApi->RegisterFront(pszFrontAddress);
	free(pszFrontAddress);

	m_pTraderApi->Init();

	boost::mutex::scoped_lock lock(m_SyncReqMutex);		
	int nRet = m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	return !nRet;
}

int CFSTraderHandler::ReqUserLogin(char ReqType)
{
	CThostFtdcReqUserLoginField reqUserLoginField;
	memset(&reqUserLoginField, 0, sizeof(CThostFtdcReqUserLoginField));
	strcpy(reqUserLoginField.UserID, m_sInvestorId.c_str());
	strcpy(reqUserLoginField.Password, m_sPasswd.c_str());
	strcpy(reqUserLoginField.BrokerID, m_sBrokerId.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqUserLogin(&reqUserLoginField, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQrySettlementInfo(char ReqType)
{
	CThostFtdcQrySettlementInfoField qrySettlementInfo;
	memset(&qrySettlementInfo, 0, sizeof(CThostFtdcQrySettlementInfoField));
	strcpy(qrySettlementInfo.BrokerID, m_sBrokerId.c_str());
	strcpy(qrySettlementInfo.InvestorID, m_sInvestorId.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQrySettlementInfo(&qrySettlementInfo, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQrySettlementInfoConfirm(char ReqType)
{
	CThostFtdcQrySettlementInfoConfirmField qrySettlementInfoConfirm;
	memset(&qrySettlementInfoConfirm, 0, sizeof(CThostFtdcQrySettlementInfoConfirmField));
	strcpy(qrySettlementInfoConfirm.BrokerID, m_sBrokerId.c_str());
	strcpy(qrySettlementInfoConfirm.InvestorID, m_sInvestorId.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQrySettlementInfoConfirm(&qrySettlementInfoConfirm, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryInstrument(char ReqType)
{
	CThostFtdcQryInstrumentField qryInstrument;
	memset(&qryInstrument, 0, sizeof(CThostFtdcQryInstrumentField));
	strcpy(qryInstrument.ExchangeID, "");
	strcpy(qryInstrument.InstrumentID, "");
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryInstrument(&qryInstrument, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryInstrumentMarginRate(string InstrumentID, char HedgeFlag, char ReqType)
{
	CThostFtdcQryInstrumentMarginRateField qryInstrumentMarginRate;
	memset(&qryInstrumentMarginRate, 0, sizeof(CThostFtdcQryInstrumentMarginRateField));
	strcpy(qryInstrumentMarginRate.BrokerID, m_sBrokerId.c_str());
	strcpy(qryInstrumentMarginRate.InvestorID, m_sInvestorId.c_str());
	strcpy(qryInstrumentMarginRate.InstrumentID, InstrumentID.c_str());
	qryInstrumentMarginRate.HedgeFlag = HedgeFlag; 
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryInstrumentMarginRate(&qryInstrumentMarginRate, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryInstrumentCommissionRate(string InstrumentID, char ReqType)
{
	CThostFtdcQryInstrumentCommissionRateField qryInstrumentCommissionRate;
	memset(&qryInstrumentCommissionRate, 0, sizeof(CThostFtdcQryInstrumentCommissionRateField));
	strcpy(qryInstrumentCommissionRate.BrokerID, m_sBrokerId.c_str());
	strcpy(qryInstrumentCommissionRate.InvestorID, m_sInvestorId.c_str());
	strcpy(qryInstrumentCommissionRate.InstrumentID, InstrumentID.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryInstrumentCommissionRate(&qryInstrumentCommissionRate, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryTradingAccount(char ReqType)
{
	CThostFtdcQryTradingAccountField qryTradingAccount;
	memset(&qryTradingAccount, 0, sizeof(CThostFtdcQryTradingAccountField));
	strcpy(qryTradingAccount.BrokerID, m_sBrokerId.c_str());
	strcpy(qryTradingAccount.InvestorID, m_sInvestorId.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryTradingAccount(&qryTradingAccount, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryInvestorPosition(string InstrumentID, char ReqType)
{
	CThostFtdcQryInvestorPositionField qryInvestorPosition;

	memset(&qryInvestorPosition, 0, sizeof(CThostFtdcQryInvestorPositionField));
	strcpy(qryInvestorPosition.BrokerID, m_sBrokerId.c_str());
	strcpy(qryInvestorPosition.InvestorID, m_sInvestorId.c_str());
	strcpy(qryInvestorPosition.InstrumentID, InstrumentID.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryInvestorPosition(&qryInvestorPosition, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryOrder(string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd, char ReqType)
{
	CThostFtdcQryOrderField qryOrder;
	memset(&qryOrder, 0, sizeof(CThostFtdcQryOrderField));
	strcpy(qryOrder.BrokerID, m_sBrokerId.c_str());
	strcpy(qryOrder.InvestorID, m_sInvestorId.c_str());
	strcpy(qryOrder.InstrumentID, InstrumentID.c_str());
	strcpy(qryOrder.ExchangeID, ExchangeID.c_str());
	strcpy(qryOrder.OrderSysID, OrderSysID.c_str());
	strcpy(qryOrder.InsertTimeStart, InsertTimeStart.c_str());
	strcpy(qryOrder.InsertTimeEnd, InsertTimeEnd.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryOrder(&qryOrder, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

int CFSTraderHandler::ReqQryTrade(string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd, char ReqType)
{
	CThostFtdcQryTradeField qryTrade;
	memset(&qryTrade, 0, sizeof(CThostFtdcQryTradeField));
	strcpy(qryTrade.BrokerID, m_sBrokerId.c_str());
	strcpy(qryTrade.InvestorID, m_sInvestorId.c_str());
	strcpy(qryTrade.InstrumentID, InstrumentID.c_str());
	strcpy(qryTrade.ExchangeID, ExchangeID.c_str());
	strcpy(qryTrade.TradeTimeStart, TradeTimeStart.c_str());
	strcpy(qryTrade.TradeTimeEnd, TradeTimeEnd.c_str());
	int nRet = 0;
	while (true)
	{
		if (!m_pTraderApi->ReqQryTrade(&qryTrade, ++m_nRequestId))
		{
			m_mapReqType[m_nRequestId] = ReqType;
			break;
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(200));
		}
	}
	if (THOST_RT_SYNC == ReqType)
	{
		boost::mutex::scoped_lock lock(m_SyncReqMutex);		
		nRet = !m_SyncReqCondtion.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(THOST_SYNC_RQ_TIMEOUT_S));
	}
	return nRet;
}

void CFSTraderHandler::MakeOrderInsertBase(CThostFtdcInputOrderField &reqInputOrderField, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	memset(&reqInputOrderField, 0, sizeof(CThostFtdcInputOrderField));
	strcpy(reqInputOrderField.UserID, m_sInvestorId.c_str());
	strcpy(reqInputOrderField.InvestorID, m_sInvestorId.c_str());
	strcpy(reqInputOrderField.BrokerID, m_sBrokerId.c_str()); 
	strcpy(reqInputOrderField.InstrumentID, InstrumentID.c_str());
	reqInputOrderField.Direction = Direction;
	reqInputOrderField.CombOffsetFlag[0] = OffsetFlag;
	reqInputOrderField.VolumeTotalOriginal = Volume;
	reqInputOrderField.LimitPrice = LimitPrice;
	reqInputOrderField.CombHedgeFlag[0] = HedgeFlag;
	reqInputOrderField.MinVolume = 1;
	reqInputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
	reqInputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
	reqInputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	reqInputOrderField.IsSwapOrder = false;
	reqInputOrderField.IsAutoSuspend = false;
	reqInputOrderField.UserForceClose = false;
	reqInputOrderField.StopPrice = 0;
}


void CFSTraderHandler::LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	CThostFtdcInputOrderField reqInputOrderField;
	MakeOrderInsertBase(reqInputOrderField, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	reqInputOrderField.RequestID = RequestID;
	reqInputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	reqInputOrderField.TimeCondition = THOST_FTDC_TC_GFD;
	ReqOrderInsert(reqInputOrderField);
}

void CFSTraderHandler::MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	CThostFtdcInputOrderField reqInputOrderField;
	MakeOrderInsertBase(reqInputOrderField, InstrumentID, Direction, OffsetFlag, 0, Volume, HedgeFlag);
	reqInputOrderField.RequestID = RequestID;
	reqInputOrderField.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	reqInputOrderField.TimeCondition = THOST_FTDC_TC_IOC;
	ReqOrderInsert(reqInputOrderField);
}

void CFSTraderHandler::FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	CThostFtdcInputOrderField reqInputOrderField;
	MakeOrderInsertBase(reqInputOrderField, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	reqInputOrderField.RequestID = RequestID;
	reqInputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	reqInputOrderField.TimeCondition = THOST_FTDC_TC_IOC;
	reqInputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
	ReqOrderInsert(reqInputOrderField);
}

void CFSTraderHandler::FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	CThostFtdcInputOrderField reqInputOrderField;
	MakeOrderInsertBase(reqInputOrderField, InstrumentID, Direction, OffsetFlag, LimitPrice, Volume, HedgeFlag);
	reqInputOrderField.RequestID = RequestID;
	reqInputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	reqInputOrderField.TimeCondition = THOST_FTDC_TC_IOC;
	reqInputOrderField.VolumeCondition = THOST_FTDC_VC_CV;
	ReqOrderInsert(reqInputOrderField);
}


string CFSTraderHandler::ReqOrderInsert(CThostFtdcInputOrderField &reqInputOrderField)
{
	TThostFtdcOrderRefType OrderRef;
	sprintf(OrderRef, "%012d", ++m_nOrderRef);
	strcpy(reqInputOrderField.OrderRef, OrderRef);
	string OrderRefKey;
	MakeOrderRefKey(OrderRefKey, m_nFrontID, m_nSessionID, OrderRef);
	CThostOrderReportField *pOrderReport = MakeOrderReport(&reqInputOrderField);
	m_mapOrderRef[OrderRefKey] = pOrderReport;
	m_pTraderApi->ReqOrderInsert(&reqInputOrderField, ++m_nRequestId);
	return OrderRefKey; 
}

int CFSTraderHandler::CancelOrder(string OrderRef, string OrderSysID, string ExchangeID)
{
	///需要加一个判断，订单状态可撤的情况才报撤单动作
	CThostFtdcInputOrderActionField reqOrderActionField;
	memset(&reqOrderActionField, 0, sizeof(CThostFtdcInputOrderActionField));
	reqOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;
	strcpy(reqOrderActionField.BrokerID, m_sBrokerId.c_str());
	strcpy(reqOrderActionField.InvestorID, m_sInvestorId.c_str());
	strcpy(reqOrderActionField.UserID, m_sInvestorId.c_str());
	strcpy(reqOrderActionField.ExchangeID, ExchangeID.c_str());
	strcpy(reqOrderActionField.OrderSysID, OrderSysID.c_str());
	strcpy(reqOrderActionField.OrderRef,   OrderRef.c_str());
	string OrderRefKey;
	MakeOrderRefKey(OrderRefKey, 0, 0, OrderRef.c_str());
	if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
	{
		CThostOrderReportField *pOrderReport = m_mapOrderRef[OrderRefKey];
		strcpy(reqOrderActionField.InstrumentID, pOrderReport->InstrumentID.c_str());
		return m_pTraderApi->ReqOrderAction(&reqOrderActionField, m_nRequestId);
	}

	string ExchangeOrderKey;
	MakeExchangeOrderKey(ExchangeOrderKey, ExchangeID.c_str(), OrderSysID.c_str());
	if (m_mapExchangeOrder.end() != m_mapExchangeOrder.find(ExchangeOrderKey))
	{
		CThostOrderReportField *pOrderReport = m_mapExchangeOrder[ExchangeOrderKey];
		strcpy(reqOrderActionField.InstrumentID, pOrderReport->InstrumentID.c_str());
		return m_pTraderApi->ReqOrderAction(&reqOrderActionField, m_nRequestId);
	}
	return 1;
}

void CFSTraderHandler::OnFrontConnected()
{
	boost::mutex::scoped_lock lock(m_SyncReqMutex);
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Info("FSTD:OnFrontConnected");
	}
	
	ClearTradingData();
	ReqUserLogin(THOST_RT_ASYNC_INIT);
}

void CFSTraderHandler::OnFrontDisconnected(int nReason)
{
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Error("FSTD:OnFrontDisconnected:%d",nReason);
	}
	boost::this_thread::sleep(boost::posix_time::seconds(THOST_DISCONNECT_SLEEP_TIME_S));
}

void CFSTraderHandler::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
		}
		if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
		{
			m_nFrontID = pRspUserLogin->FrontID;
			m_nSessionID = pRspUserLogin->SessionID;
			m_nTradingDay = atoi(pRspUserLogin->TradingDay);
			m_nOrderRef = atoi(pRspUserLogin->MaxOrderRef);	
			if (m_pLogHandler != NULL)
			{
				m_pLogHandler->Info("FSTD:login success!");
			}
			if (THOST_RT_SYNC == cReqType)
			{
				boost::mutex::scoped_lock lock(m_SyncReqMutex);
				m_SyncReqCondtion.notify_one();
			}
			else if (THOST_RT_ASYNC_INIT == cReqType)
			{
				ReqQrySettlementInfo(THOST_RT_ASYNC_INIT);
			}
		}
		else
		{
			if (m_pLogHandler != NULL)
			{
				m_pLogHandler->Error("FSTD:login failed. Reason:%s!", pRspInfo->ErrorMsg);
			}
			///前置机不活跃, 睡眠10秒再重试
			if (pRspInfo->ErrorID == 8)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(THOST_SYNC_RQ_TIMEOUT_S*1000));
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:ready to relogin.");
				}
				ReqUserLogin(cReqType);
			}
		}
	}
}

void CFSTraderHandler::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query settlement success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query settlement failed.Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryInstrument(THOST_RT_ASYNC_INIT);
		}
	}
}

void CFSTraderHandler::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:confirm settlement sucess!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:confirm settlemenet failed.Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryInstrument(THOST_RT_ASYNC_INIT);
		}
	}
}

void CFSTraderHandler::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument != NULL && THOST_FTDC_PC_Futures == pInstrument->ProductClass)
	{
		CThostInstrumentField instrumentField;
		instrumentField.InstrumentID = pInstrument->InstrumentID;
		instrumentField.ExchangeID = pInstrument->ExchangeID;
		instrumentField.InstrumentName = pInstrument->InstrumentName;
		instrumentField.ProductID = pInstrument->ProductID;
		instrumentField.DeliveryYear = pInstrument->DeliveryYear;
		instrumentField.DeliveryMonth = pInstrument->DeliveryMonth;
		instrumentField.MaxMarketOrderVolume = pInstrument->MaxMarketOrderVolume;
		instrumentField.MinMarketOrderVolume = pInstrument->MinMarketOrderVolume;
		instrumentField.MaxLimitOrderVolume = pInstrument->MaxLimitOrderVolume;
		instrumentField.MinLimitOrderVolume = pInstrument->MinLimitOrderVolume;
		instrumentField.VolumeMultiple = pInstrument->VolumeMultiple;
		instrumentField.PriceTick = pInstrument->PriceTick;
		instrumentField.CreateDate = pInstrument->CreateDate;
		instrumentField.OpenDate = pInstrument->OpenDate;
		instrumentField.ExpireDate = pInstrument->ExpireDate;
		instrumentField.StartDelivDate = pInstrument->InstrumentID;
		instrumentField.EndDelivDate = pInstrument->EndDelivDate;
		instrumentField.InstLifePhase = pInstrument->InstLifePhase;
		instrumentField.IsTrading = pInstrument->IsTrading;
		instrumentField.LongMarginRatio = pInstrument->LongMarginRatio;
		instrumentField.ShortMarginRatio = pInstrument->ShortMarginRatio;
		instrumentField.MaxMarginSideAlgorithm = pInstrument->MaxMarginSideAlgorithm;

		m_mapInstrumentInfo[instrumentField.InstrumentID] = instrumentField;
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query instrument success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query instrument failed. Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryTradingAccount(THOST_RT_ASYNC_INIT);
		}
	}
}


void CFSTraderHandler::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrumentMarginRate != NULL)
	{
		CThostInstrumentMarginRateField instrumentMarginRateField;
		instrumentMarginRateField.InstrumentID = pInstrumentMarginRate->InstrumentID;
		instrumentMarginRateField.BrokerID = pInstrumentMarginRate->BrokerID;
		instrumentMarginRateField.InvestorID = pInstrumentMarginRate->InvestorID;
		instrumentMarginRateField.HedgeFlag = pInstrumentMarginRate->HedgeFlag;
		instrumentMarginRateField.LongMarginRatioByMoney = pInstrumentMarginRate->LongMarginRatioByMoney;
		instrumentMarginRateField.LongMarginRatioByVolume = pInstrumentMarginRate->LongMarginRatioByVolume;
		instrumentMarginRateField.ShortMarginRatioByMoney = pInstrumentMarginRate->ShortMarginRatioByMoney;
		instrumentMarginRateField.ShortMarginRatioByVolume = pInstrumentMarginRate->ShortMarginRatioByVolume;
		instrumentMarginRateField.IsRelative = pInstrumentMarginRate->IsRelative;
		m_mapInstrumentMarginRate[instrumentMarginRateField.InstrumentID] = instrumentMarginRateField;
	}
	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
	}
}

void CFSTraderHandler::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrumentCommissionRate != NULL)
	{
		CThostInstrumentCommissionRateField instrumentCommissionRateField;
		for (InstrumentInfoMap::iterator iter = m_mapInstrumentInfo.begin(); iter != m_mapInstrumentInfo.end(); ++iter)
		{
			if (strcmp(iter->second.ProductID.c_str(), pInstrumentCommissionRate->InstrumentID) == 0)
			{
				instrumentCommissionRateField.InstrumentID = iter->second.InstrumentID;
				instrumentCommissionRateField.BrokerID = pInstrumentCommissionRate->BrokerID;
				instrumentCommissionRateField.InvestorID = pInstrumentCommissionRate->InvestorID;
				instrumentCommissionRateField.OpenRatioByMoney = pInstrumentCommissionRate->OpenRatioByMoney;
				instrumentCommissionRateField.OpenRatioByVolume = pInstrumentCommissionRate->OpenRatioByVolume;
				instrumentCommissionRateField.CloseRatioByMoney = pInstrumentCommissionRate->CloseRatioByMoney;
				instrumentCommissionRateField.CloseRatioByVolume = pInstrumentCommissionRate->CloseRatioByVolume;
				instrumentCommissionRateField.CloseTodayRatioByMoney = pInstrumentCommissionRate->CloseTodayRatioByMoney;
				instrumentCommissionRateField.CloseTodayRatioByVolume = pInstrumentCommissionRate->CloseTodayRatioByVolume;
				m_mapInstrumentComissionRate[instrumentCommissionRateField.InstrumentID] = instrumentCommissionRateField;
			}
		}
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
	}
}

void CFSTraderHandler::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pTradingAccount != NULL)
	{
		m_TradingAccount.BrokerID = pTradingAccount->BrokerID;
		m_TradingAccount.AccountID = pTradingAccount->AccountID;
		m_TradingAccount.PreBalance = pTradingAccount->PreBalance;
		m_TradingAccount.PreMargin = pTradingAccount->PreMargin;
		m_TradingAccount.Deposit = pTradingAccount->Deposit;
		m_TradingAccount.Withdraw = pTradingAccount->Withdraw;
		m_TradingAccount.FrozenMargin = pTradingAccount->FrozenMargin;
		m_TradingAccount.FrozenCash = pTradingAccount->FrozenCash;
		m_TradingAccount.FrozenCommission = pTradingAccount->FrozenCommission;
		m_TradingAccount.CurrMargin = pTradingAccount->CurrMargin;
		m_TradingAccount.CashIn = pTradingAccount->CashIn;
		m_TradingAccount.Commission = pTradingAccount->Commission;
		m_TradingAccount.CloseProfit = pTradingAccount->CloseProfit;
		m_TradingAccount.PositionProfit = pTradingAccount->PositionProfit;
		m_TradingAccount.Balance = pTradingAccount->Balance;
		m_TradingAccount.Available = pTradingAccount->Available;
		m_TradingAccount.WithdrawQuota = pTradingAccount->WithdrawQuota;
		m_TradingAccount.TradingDay = atoi(pTradingAccount->TradingDay);
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query banlance success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query balance failed. Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryInvestorPosition("", THOST_RT_ASYNC_INIT);
		}
	}
}

//飞鼠持仓的TodayPosition和Position字段相反, 实盘待验证
void CFSTraderHandler::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInvestorPosition != NULL)
	{
		InvestorPositionVec::iterator iter = m_vecInvestorPosition.begin();
		for (; iter != m_vecInvestorPosition.end(); ++iter)
		{
			//update
			if (strcmp(pInvestorPosition->InstrumentID, iter->InstrumentID.c_str()) == 0 && pInvestorPosition->PosiDirection == iter->PosiDirection)
			{
				iter->Position += pInvestorPosition->Position;
				if (THOST_PSD_Today == pInvestorPosition->PositionDate)
				{
					iter->TodayPosition = pInvestorPosition->Position;
				}
				else
				{
					iter->YdPosition =	pInvestorPosition->Position;
				}
				iter->LongFrozen += pInvestorPosition->LongFrozen;
				iter->ShortFrozen += pInvestorPosition->ShortFrozen;
				iter->LongFrozenAmount += pInvestorPosition->LongFrozenAmount;
				iter->ShortFrozenAmount += pInvestorPosition->ShortFrozenAmount;
				iter->OpenVolume += pInvestorPosition->OpenVolume;
				iter->CloseVolume += pInvestorPosition->CloseVolume;
				iter->OpenAmount += pInvestorPosition->OpenAmount;
				iter->CloseAmount += pInvestorPosition->CloseAmount;
				iter->PositionCost += pInvestorPosition->PositionCost;
				iter->PreMargin += pInvestorPosition->PreMargin;
				iter->UseMargin += pInvestorPosition->UseMargin;
				iter->FrozenMargin += pInvestorPosition->FrozenMargin;
				iter->FrozenCash += pInvestorPosition->FrozenCash;
				iter->FrozenCommission += pInvestorPosition->FrozenCommission;
				iter->CashIn += pInvestorPosition->CashIn;
				iter->Commission += pInvestorPosition->Commission;
				iter->CloseProfit += pInvestorPosition->CloseProfit;
				iter->PositionProfit += pInvestorPosition->PositionProfit;
				iter->SettlementPrice = pInvestorPosition->SettlementPrice;
				iter->SettlementID = pInvestorPosition->SettlementID;
				iter->OpenCost += pInvestorPosition->OpenCost;
				iter->ExchangeMargin += pInvestorPosition->ExchangeMargin;
				iter->CloseProfitByDate += pInvestorPosition->CloseProfitByDate;
				iter->CloseProfitByTrade += pInvestorPosition->CloseProfitByTrade;
				break;
			}
		}

		//new insert
		if (iter == m_vecInvestorPosition.end())
		{
			CThostInvestorPositionField InvestorPosition;
			InvestorPosition.InstrumentID = pInvestorPosition->InstrumentID;
			InvestorPosition.BrokerID = pInvestorPosition->BrokerID;
			InvestorPosition.InvestorID = pInvestorPosition->InvestorID;
			InvestorPosition.PosiDirection = pInvestorPosition->PosiDirection;
			InvestorPosition.HedgeFlag = pInvestorPosition->HedgeFlag;
			InvestorPosition.PositionDate = pInvestorPosition->PositionDate;
			InvestorPosition.Position = pInvestorPosition->TodayPosition;
			InvestorPosition.YdPosition = pInvestorPosition->YdPosition;
			InvestorPosition.TodayPosition = pInvestorPosition->Position;
			InvestorPosition.LongFrozen = pInvestorPosition->LongFrozen;
			InvestorPosition.ShortFrozen = pInvestorPosition->ShortFrozen;
			InvestorPosition.LongFrozenAmount = pInvestorPosition->LongFrozenAmount;
			InvestorPosition.ShortFrozenAmount = pInvestorPosition->ShortFrozenAmount;
			InvestorPosition.OpenVolume = pInvestorPosition->OpenVolume;
			InvestorPosition.CloseVolume = pInvestorPosition->CloseVolume;
			InvestorPosition.OpenAmount = pInvestorPosition->OpenAmount;
			InvestorPosition.CloseAmount = pInvestorPosition->CloseAmount;
			InvestorPosition.PositionCost = pInvestorPosition->PositionCost;
			InvestorPosition.PreMargin = pInvestorPosition->PreMargin;
			InvestorPosition.UseMargin = pInvestorPosition->UseMargin;
			InvestorPosition.FrozenMargin = pInvestorPosition->FrozenMargin;
			InvestorPosition.FrozenCash = pInvestorPosition->FrozenCash;
			InvestorPosition.FrozenCommission = pInvestorPosition->FrozenCommission;
			InvestorPosition.CashIn = pInvestorPosition->CashIn;
			InvestorPosition.Commission = pInvestorPosition->Commission;
			InvestorPosition.CloseProfit = pInvestorPosition->CloseProfit;
			InvestorPosition.PositionProfit = pInvestorPosition->PositionProfit;
			InvestorPosition.PreSettlementPrice = pInvestorPosition->PreSettlementPrice;
			InvestorPosition.SettlementPrice = pInvestorPosition->SettlementPrice;
			InvestorPosition.TradingDay = pInvestorPosition->TradingDay;
			InvestorPosition.SettlementID = pInvestorPosition->SettlementID;
			InvestorPosition.OpenCost = pInvestorPosition->OpenCost;
			InvestorPosition.ExchangeMargin = pInvestorPosition->ExchangeMargin;
			InvestorPosition.CloseProfitByDate = pInvestorPosition->CloseProfitByDate;
			InvestorPosition.CloseProfitByTrade = pInvestorPosition->CloseProfitByTrade;
			m_vecInvestorPosition.push_back(InvestorPosition);
		}
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query position success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query position failed. Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryOrder("", "", "", "", "", THOST_RT_ASYNC_INIT);
		}
	}
}

void CFSTraderHandler::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
	}
}

void CFSTraderHandler::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pOrder != NULL && strlen(pOrder->OrderSysID) > 0)
	{	
		CThostOrderReportField orderReportField;
		orderReportField.BrokerID = pOrder->BrokerID;
		orderReportField.InvestorID = pOrder->InvestorID;
		orderReportField.InstrumentID = pOrder->InstrumentID;
		orderReportField.OrderRef = pOrder->OrderRef;
		orderReportField.UserID = pOrder->UserID;
		orderReportField.OrderPriceType = pOrder->OrderPriceType;
		orderReportField.Direction = pOrder->Direction;
		orderReportField.CombOffsetFlag = pOrder->CombOffsetFlag;
		orderReportField.CombHedgeFlag = pOrder->CombHedgeFlag;
		orderReportField.LimitPrice = pOrder->LimitPrice;
		orderReportField.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
		orderReportField.TimeCondition = pOrder->TimeCondition;
		orderReportField.GTDDate = pOrder->GTDDate;
		orderReportField.VolumeCondition = pOrder->VolumeCondition;
		orderReportField.MinVolume = pOrder->MinVolume;
		orderReportField.ContingentCondition = pOrder->ContingentCondition;
		orderReportField.ForceCloseReason = pOrder->ForceCloseReason;
		orderReportField.IsAutoSuspend = pOrder->IsAutoSuspend;
		orderReportField.BusinessUnit = pOrder->BusinessUnit;
		orderReportField.UserForceClose = pOrder->UserForceClose;
		orderReportField.IsSwapOrder = pOrder->IsSwapOrder;
		orderReportField.OrderSysID = pOrder->OrderSysID;
		orderReportField.ExchangeID = pOrder->ExchangeID;
		orderReportField.FilledVolume = pOrder->VolumeTraded;
		orderReportField.TradingDay = pOrder->TradingDay;
		orderReportField.InsertDate = pOrder->InsertDate;
		orderReportField.InsertTime = pOrder->InsertTime;
		orderReportField.FrontID = pOrder->FrontID;
		orderReportField.SessionID = pOrder->SessionID;
		orderReportField.UserProductInfo = pOrder->UserProductInfo;
		orderReportField.StatusMsg = pOrder->StatusMsg;
		orderReportField.LastFilledPrice = 0;
		orderReportField.LastFilledVolume = 0;

		switch(pOrder->OrderStatus)
		{
		case THOST_FTDC_OST_Unknown:
			orderReportField.OrderStatus = THOST_OST_Submitted;
		default:
			orderReportField.OrderStatus = pOrder->OrderStatus;
		}
		m_vecOrderReport.push_back(orderReportField);
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query order success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query order failed. Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			ReqQryTrade("", "", "", "", "", THOST_RT_ASYNC_INIT);
		}
	}
}

void CFSTraderHandler::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pTrade != NULL)
	{
		CThostTradeField tradeField;
		tradeField.BrokerID = pTrade->BrokerID;
		tradeField.InvestorID = pTrade->InvestorID;
		tradeField.InstrumentID = pTrade->InstrumentID;
		tradeField.OrderRef = pTrade->OrderRef;
		tradeField.UserID = pTrade->UserID;
		tradeField.ExchangeID = pTrade->ExchangeID;
		tradeField.TradeID = pTrade->TradeID;
		tradeField.Direction = pTrade->Direction;
		tradeField.OrderSysID = pTrade->OrderSysID;
		tradeField.OffsetFlag = pTrade->OffsetFlag;
		tradeField.HedgeFlag = pTrade->HedgeFlag;
		tradeField.Price = pTrade->Price;
		tradeField.Volume = pTrade->Volume;
		tradeField.TradeDate = pTrade->TradeDate;
		tradeField.TradeTime = pTrade->TradeTime;
		tradeField.TradingDay = pTrade->TradingDay;
		tradeField.SettlementID = pTrade->SettlementID;
		tradeField.BrokerOrderSeq = pTrade->BrokerOrderSeq;
		m_vecTrade.push_back(tradeField);
	}

	if (bIsLast)
	{
		char cReqType = THOST_RT_ASYNC;
		ReqTypeMap::iterator iter = m_mapReqType.find(nRequestID);
		if (m_mapReqType.end() != iter)
		{
			cReqType = iter->second;
			m_mapReqType.erase(iter);
		}
		if (THOST_RT_SYNC == cReqType)
		{
			boost::mutex::scoped_lock lock(m_SyncReqMutex);
			m_SyncReqCondtion.notify_one();
			if (pRspInfo == NULL || (pRspInfo != NULL && pRspInfo->ErrorID == 0))
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Info("FSTD:query trade success!");
				}
			}
			else
			{
				if (m_pLogHandler != NULL)
				{
					m_pLogHandler->Error("FSTD:query trade failed. Reason:%s", pRspInfo->ErrorMsg);
				}
			}
		}
		else if (THOST_RT_ASYNC_INIT == cReqType)
		{
			InitializeOrder();
		}
	}
}

void CFSTraderHandler::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	string OrderRefKey;
	MakeOrderRefKey(OrderRefKey, m_nFrontID, m_nSessionID, pInputOrder->OrderRef);
	if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
	{
		CThostOrderReportField *pOrderReport = m_mapOrderRef[OrderRefKey];
		pOrderReport->StatusMsg = pRspInfo->ErrorMsg;
		if (pRspInfo->ErrorID == 0)
		{
			pOrderReport->OrderStatus = THOST_OST_Recieved;
		}
		else
		{
			pOrderReport->OrderStatus = THOST_OST_Error;
		}
		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
		}
	}
	else
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Error("FSTD:OnRspOrderInsert");
		}
	}
}

void CFSTraderHandler::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//make OrderRefKey from FrontId|SessionId|OrderRef
	string OrderRefKey;
	MakeOrderRefKey(OrderRefKey, pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->OrderRef);
	if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
	{
		CThostOrderReportField *pOrderReport = m_mapOrderRef[OrderRefKey];
		pOrderReport->StatusMsg = pRspInfo->ErrorMsg;
		pOrderReport->OrderStatus = THOST_OST_Error;
		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
		}
	}
	else
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Error("FSTD:OnRspOrderAction");
		}
	}
}

void CFSTraderHandler::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	CThostInstrumentStatusField instrumentStatusField;
	instrumentStatusField.InstrumentID = pInstrumentStatus->InstrumentID;
	instrumentStatusField.ExchangeID = pInstrumentStatus->ExchangeID;
	instrumentStatusField.ExchangeInstID = pInstrumentStatus->ExchangeInstID;
	instrumentStatusField.InstrumentStatus = pInstrumentStatus->InstrumentStatus;
	instrumentStatusField.EnterTime = pInstrumentStatus->EnterTime;
	if (m_pTraderHandlerSpi != NULL)
	{
		m_pTraderHandlerSpi->OnRtnInstrumentStatus(&instrumentStatusField);
	}
}

void CFSTraderHandler::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	string OrderRefKey;
	MakeOrderRefKey(OrderRefKey, pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
	/// order receive by broker
	if (THOST_FTDC_OST_Unknown == pOrder->OrderStatus && 0 == strlen(pOrder->OrderSysID))
	{
		CThostOrderReportField *pOrderReport = NULL;
		if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
		{
			pOrderReport = m_mapOrderRef[OrderRefKey];
			//pOrderReport->ExchangeID = pOrder->ExchangeID;
			pOrderReport->TradingDay = pOrder->TradingDay;
			pOrderReport->OrderStatus = THOST_OST_Recieved;
			pOrderReport->InsertDate = pOrder->InsertDate;
			pOrderReport->InsertTime = pOrder->InsertTime;
			pOrderReport->UserProductInfo = pOrder->UserProductInfo;
			pOrderReport->StatusMsg = pOrder->StatusMsg;	
		}
		else
		{
			///add new order from other session
			pOrderReport = MakeOrderReport(pOrder);
			pOrderReport->OrderStatus = THOST_OST_Recieved;
			m_mapOrderRef[OrderRefKey] = pOrderReport;
		}

		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			return ;
		}
	}

	/// order submitted to exchange(insert or action)
	if (0 != strlen(pOrder->OrderSysID) && (THOST_FTDC_OST_Unknown == pOrder->OrderStatus || THOST_FTDC_OST_NoTradeQueueing == pOrder->OrderStatus))
	{
		CThostOrderReportField *pOrderReport = NULL;
		if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
		{
			pOrderReport = m_mapOrderRef[OrderRefKey];
		}
		else
		{
			pOrderReport = MakeOrderReport(pOrder);
		}
		string ExchangeOrderKey;
		MakeExchangeOrderKey(ExchangeOrderKey, pOrder->ExchangeID, pOrder->OrderSysID);
		if (m_mapExchangeOrder.end() == m_mapExchangeOrder.find(ExchangeOrderKey))
		{
			//insert submitted 
			m_mapExchangeOrder[ExchangeOrderKey] = pOrderReport;			
			pOrderReport->OrderSysID = pOrder->OrderSysID;
			pOrderReport->OrderStatus = THOST_OST_Submitted;
			pOrderReport->StatusMsg = pOrder->StatusMsg;
		}
		else 
		{
			//action submitted
			pOrderReport->OrderStatus = THOST_OST_Canceling;
			pOrderReport->StatusMsg = pOrder->StatusMsg;
		}
		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			return ;
		}
	}

	/// order canceled
	if (THOST_FTDC_OST_Canceled == pOrder->OrderStatus || THOST_FTDC_OST_NoTradeNotQueueing == pOrder->OrderStatus || THOST_FTDC_OST_PartTradedNotQueueing == pOrder->OrderStatus)
	{
		CThostOrderReportField *pOrderReport = NULL;
		if (m_mapOrderRef.end() != m_mapOrderRef.find(OrderRefKey))
		{
			pOrderReport = m_mapOrderRef[OrderRefKey];
		}
		else
		{
			string ExchangeOrderKey;
			MakeExchangeOrderKey(ExchangeOrderKey, pOrder->ExchangeID, pOrder->OrderSysID);
			if (m_mapExchangeOrder.end() != m_mapExchangeOrder.find(ExchangeOrderKey))
			{
				pOrderReport = m_mapExchangeOrder[ExchangeOrderKey];
			}
		}
		if (pOrderReport != NULL)
		{
			pOrderReport->CancelTime = pOrder->CancelTime;
			if (THOST_FTDC_OST_Canceled == pOrder->OrderStatus)
			{
				pOrderReport->OrderStatus = THOST_OST_Canceled;
			}
			else
			{
				pOrderReport->OrderStatus = THOST_OST_CancelFailed;
			}
			pOrderReport->StatusMsg = pOrder->StatusMsg;
			if (m_pTraderHandlerSpi != NULL)
			{
				m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
				return ;
			}
		}
		else
		{
			if (m_pLogHandler != NULL)
			{
				m_pLogHandler->Error("FSTD:OnRtnOrder-Cancel");
			}
		}
	}
}

void CFSTraderHandler::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	string ExchangeOrderKey;
	MakeExchangeOrderKey(ExchangeOrderKey, pTrade->ExchangeID, pTrade->OrderSysID);
	if (m_mapExchangeOrder.end() != m_mapExchangeOrder.find(ExchangeOrderKey))
	{
		CThostOrderReportField *pOrderReport = m_mapExchangeOrder[ExchangeOrderKey];
		pOrderReport->AveragePrice = (pOrderReport->AveragePrice * pOrderReport->FilledVolume + pTrade->Price * pTrade->Volume) / (pOrderReport->FilledVolume + pTrade->Volume);
		pOrderReport->FilledVolume += pTrade->Volume;
		pOrderReport->LastFilledVolume = pTrade->Volume;
		pOrderReport->LastFilledPrice = pTrade->Price;
		pOrderReport->LastFilledTime = pTrade->TradeTime;
		if (pOrderReport->FilledVolume == pOrderReport->VolumeTotalOriginal)
		{
			pOrderReport->OrderStatus = THOST_OST_Complete;
			pOrderReport->StatusMsg = "全部成交";
		}
		else
		{
			pOrderReport->OrderStatus = THOST_OST_Filled;
			pOrderReport->StatusMsg = "部分成交";
		}

		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
		}
	}
	else
	{
		if (m_pLogHandler != NULL)
		{
			m_pLogHandler->Error("FSTD:OnRtnTrade");
		}
	}
}

int CFSTraderHandler::GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo)
{
	return 0;
}

int CFSTraderHandler::GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID)
{
	if (m_mapInstrumentInfo.end() == m_mapInstrumentInfo.find(InstrumentID))
	{
		return 1;
	}
	InstrumentInfo = m_mapInstrumentInfo[InstrumentID];
	return 0;
}

int CFSTraderHandler::GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID)
{
	if (m_mapInstrumentMarginRate.end() == m_mapInstrumentMarginRate.find(InstrumentID))
	{
		if (ReqQryInstrumentMarginRate(InstrumentID))
		{
			return 1;
		}
	}
	InstrumentMarginRate = m_mapInstrumentMarginRate[InstrumentID];
	return 0;
}

int CFSTraderHandler::GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID)
{
	if (m_mapInstrumentComissionRate.end() == m_mapInstrumentComissionRate.find(InstrumentID))
	{
		if (ReqQryInstrumentCommissionRate(InstrumentID))
		{
			return 1;
		}
	}
	InstrumentCommissionRate = m_mapInstrumentComissionRate[InstrumentID];
	return 0;
}

int CFSTraderHandler::GetTradingAccount(CThostTradingAccountField &TradingAccount)
{
	if (ReqQryTradingAccount())
	{
		return 1;
	}
	TradingAccount = m_TradingAccount;
	return 0;
}

int CFSTraderHandler::GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID)
{
	m_vecInvestorPosition.clear();
	if (ReqQryInvestorPosition(InstrumentID))
	{
		return 1;
	}
	vecInvestorPosition = m_vecInvestorPosition;
	return 0;
}

int CFSTraderHandler::GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd)
{
	m_vecOrderReport.clear();
	if (ReqQryOrder(InstrumentID, ExchangeID, OrderSysID, InsertTimeStart, InsertTimeEnd))
	{
		return 1;
	}
	sort(m_vecOrderReport.begin(), m_vecOrderReport.end());
	vecOrderReport = m_vecOrderReport;
	return 0;
}

int CFSTraderHandler::GetTrade(TradeVec &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd)
{
	m_vecTrade.clear();
	if (ReqQryTrade(InstrumentID, ExchangeID, TradeID, TradeTimeStart, TradeTimeEnd))
	{
		return 1;
	}
	sort(m_vecTrade.begin(), m_vecTrade.end());
	vecTrade = m_vecTrade;
	return 0;
}

string CFSTraderHandler::GetInvestorID()
{
	return m_sInvestorId;
}

int CFSTraderHandler::GetTradingDay()
{
	return m_nTradingDay;
}

void CFSTraderHandler::MakeOrderRefKey(string &OrderRefKey, int FrontId, int SessionId, const char *pszOrderRef)
{
	//make OrderId from FrontId|SessionId|OrderRef
	char *pszOrderRefKey = new char[100];
	memset(pszOrderRefKey, '\0', sizeof(100));
	sprintf(pszOrderRefKey, "%d|%d|%s", m_nFrontID, m_nSessionID, pszOrderRef);
	OrderRefKey = pszOrderRefKey;
}

void CFSTraderHandler::MakeExchangeOrderKey(string &ExchangeOrderKey, const char *pszExchangeId, const char *pszOrderSysId)
{
	char *pszExchangeOrderKey = new char[100];
	memset(pszExchangeOrderKey, '\0', sizeof(100));
	sprintf(pszExchangeOrderKey, "%s|%s", pszExchangeId, pszOrderSysId);
	ExchangeOrderKey = pszExchangeOrderKey;
}

CThostOrderReportField* CFSTraderHandler::MakeOrderReport(CThostFtdcInputOrderField *pInputOrder)
{
	CThostOrderReportField *pOrderReport = new CThostOrderReportField();
	memset(pOrderReport, 0, sizeof(CThostOrderReportField));
	pOrderReport->BrokerID = pInputOrder->BrokerID;
	pOrderReport->InvestorID = pInputOrder->InvestorID;
	pOrderReport->UserID = pInputOrder->UserID;
	pOrderReport->InstrumentID = pInputOrder->InstrumentID;
	pOrderReport->ExchangeID = "SGE";
	pOrderReport->CombHedgeFlag = pInputOrder->CombHedgeFlag;
	pOrderReport->Direction = pInputOrder->Direction;
	pOrderReport->CombOffsetFlag = pInputOrder->CombOffsetFlag;
	pOrderReport->VolumeTotalOriginal = pInputOrder->VolumeTotalOriginal;
	pOrderReport->LimitPrice = pInputOrder->LimitPrice;
	pOrderReport->OrderPriceType = pInputOrder->OrderPriceType;
	pOrderReport->VolumeCondition = pInputOrder->VolumeCondition;
	pOrderReport->TimeCondition = pInputOrder->TimeCondition;
	pOrderReport->ContingentCondition = pInputOrder->ContingentCondition;
	pOrderReport->ForceCloseReason = pInputOrder->ForceCloseReason;
	pOrderReport->IsSwapOrder = pInputOrder->IsSwapOrder;
	pOrderReport->IsAutoSuspend = pInputOrder->IsAutoSuspend;
	pOrderReport->UserForceClose = pInputOrder->UserForceClose;
	pOrderReport->OrderRef = pInputOrder->OrderRef;

	pOrderReport->FilledVolume = 0;
	pOrderReport->AveragePrice = 0;
	pOrderReport->FrontID = m_nFrontID;
	pOrderReport->SessionID = m_nSessionID;
	return pOrderReport;
}

CThostOrderReportField* CFSTraderHandler::MakeOrderReport(CThostFtdcOrderField *pOrder)
{
	CThostOrderReportField *pOrderReport = new CThostOrderReportField();
	memset(pOrderReport, 0, sizeof(CThostOrderReportField));
	pOrderReport->BrokerID = pOrder->BrokerID;
	pOrderReport->InvestorID = pOrder->InvestorID;
	pOrderReport->InstrumentID = pOrder->InstrumentID;
	pOrderReport->OrderRef = pOrder->OrderRef;
	pOrderReport->UserID = pOrder->UserID;
	pOrderReport->OrderPriceType = pOrder->OrderPriceType;
	pOrderReport->Direction = pOrder->Direction;
	pOrderReport->CombOffsetFlag = pOrder->CombOffsetFlag;
	pOrderReport->CombHedgeFlag = pOrder->CombHedgeFlag;
	pOrderReport->LimitPrice = pOrder->LimitPrice;
	pOrderReport->VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
	pOrderReport->TimeCondition = pOrder->TimeCondition;
	pOrderReport->GTDDate = pOrder->GTDDate;
	pOrderReport->VolumeCondition = pOrder->VolumeCondition;
	pOrderReport->MinVolume = pOrder->MinVolume;
	pOrderReport->ContingentCondition = pOrder->ContingentCondition;
	pOrderReport->ForceCloseReason = pOrder->ForceCloseReason;
	pOrderReport->IsAutoSuspend = pOrder->IsAutoSuspend;
	pOrderReport->IsSwapOrder = pOrder->IsSwapOrder;
	pOrderReport->UserForceClose = pOrder->UserForceClose;

	pOrderReport->OrderSysID = pOrder->OrderSysID;
	pOrderReport->ExchangeID = pOrder->ExchangeID;
	pOrderReport->FilledVolume = 0;
	pOrderReport->AveragePrice = 0;
	pOrderReport->TradingDay = pOrder->TradingDay;
	pOrderReport->InsertDate = pOrder->InsertDate;
	pOrderReport->InsertTime = pOrder->InsertTime;
	pOrderReport->FrontID = pOrder->FrontID;
	pOrderReport->SessionID = pOrder->SessionID;
	pOrderReport->UserProductInfo = pOrder->UserProductInfo;
	pOrderReport->StatusMsg = pOrder->StatusMsg;
	return pOrderReport;
}

void CFSTraderHandler::InitializeOrder()
{
	for (OrderReportVec::iterator iterOrderReport = m_vecOrderReport.begin(); iterOrderReport != m_vecOrderReport.end(); iterOrderReport++)
	{
		CThostOrderReportField *pOrderReport = new CThostOrderReportField();
		memset(pOrderReport, 0, sizeof(CThostOrderReportField));
		memcpy(pOrderReport, &*iterOrderReport, sizeof(CThostOrderReportField));
		pOrderReport->FilledVolume = 0;
		pOrderReport->LastFilledPrice = 0;
		pOrderReport->LastFilledVolume = 0;
		pOrderReport->AveragePrice = 0;
		for (TradeVec::iterator iterTrade = m_vecTrade.begin(); iterTrade != m_vecTrade.end(); iterTrade++)
		{
			if (strcmp(iterOrderReport->InstrumentID.c_str(), iterTrade->InstrumentID.c_str()) == 0 &&
				iterOrderReport->Direction == iterTrade->Direction &&
				(iterOrderReport->CombOffsetFlag[0] == iterTrade->OffsetFlag || 
				(iterOrderReport->CombOffsetFlag[0] == THOST_OF_Close && (iterTrade->OffsetFlag == THOST_OF_CloseToday || iterTrade->OffsetFlag == THOST_OF_CloseYesterday))
				))
			{
				pOrderReport->LastFilledPrice = iterTrade->Price;
				pOrderReport->LastFilledVolume = iterTrade->Volume;
				pOrderReport->LastFilledTime = iterTrade->TradeTime;
				pOrderReport->AveragePrice = (pOrderReport->AveragePrice * pOrderReport->FilledVolume + iterTrade->Price * iterTrade->Volume) / (pOrderReport->FilledVolume + iterTrade->Volume);
				pOrderReport->FilledVolume += iterTrade->Volume;
			}
		}
		string ExchangeOrderKey;
		MakeExchangeOrderKey(ExchangeOrderKey, pOrderReport->ExchangeID.c_str(), pOrderReport->OrderSysID.c_str());
		if (m_mapExchangeOrder.end() == m_mapExchangeOrder.find(ExchangeOrderKey))
		{
			m_mapExchangeOrder[ExchangeOrderKey] = pOrderReport;
		}
	}
	if (m_pLogHandler != NULL)
	{
		m_pLogHandler->Info("FSTD:Init order success!");
	}
}