#include "simulationtradehandler.h"
#include <json/json.h>

CSimulationTraderHandler::CSimulationTraderHandler()
{
	m_nOrderRef = 0;
	m_nOrderSysID = 0;
	m_nTradeID = 0;
	m_pLogHandler = NULL;
}

CSimulationTraderHandler::CSimulationTraderHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo) :
	CThostTraderApi(InvestorId, Password, BrokerId, FrontAddress, FlowPath, UserProductInfo)
{
	m_pLogHandler = NULL;
}	

/*
JsonParam结构如下
{
	"Account":{"InitialBalance":double}
	"InstrumentList":[{"InstrumentID":string, "ExchangeID:"string, "VolumeMultiple":int, "Slippage":double, 
					   "LongMarginRatioByMoney":double, "LongMarginRatioByVolume":double, "ShortMarginRatioByMoney":double, "ShortMarginRatioByVolume":double,},
					   "OpenRatioByMoney":double, "OpenRatioByVolume":double, "CloseRatioByMoney":double, "CloseRatioByVolume":double, "CloseTodayRatioByMoney":double, "CloseTodayRatioByVolume":double
					   {},......,{}
					 ]
}
*/
int CSimulationTraderHandler::Start(string JsonParam)
{
	string sExePath = boost::filesystem::initial_path<boost::filesystem::path>().string(); 
	if (m_pLogHandler == NULL)
	{
		string sLogFilePath = sExePath + "/../userdata/log/ctp/simulationtrade/";
		string sLogFileName = sLogFilePath + ".log";
		string sLoggerName = "simulationtrade"; 
		boost::filesystem::create_directories(sLogFilePath.c_str()); 
		m_pLogHandler = new CLogHandler();
		m_pLogHandler->InitLogger(sLoggerName.c_str());
		m_pLogHandler->AddFileLogger(sLogFileName.c_str());
		m_pLogHandler->AddConsoleLogger();
	}

	Json::Reader reader;
	Json::Value json_object;

	///校验参数
	if (!reader.parse(JsonParam.c_str(), JsonParam.c_str()+JsonParam.length(), json_object))
	{
		m_pLogHandler->Error("Error JsonParam!");
		return 1;
	}

	if (!json_object["Account"].isObject()
		|| !json_object["InstrumentList"].isArray())
	{
		m_pLogHandler->Error("Invaild Params!");
		return 1;
	}

	const Json::Value& json_account = json_object["Account"];
	if (!json_account["InitialBalance"].isNumeric())
	{
		m_pLogHandler->Error("Invaild InitialBalance!");
		return 1;
	}
	
	///初始化资金信息
	double InitialBalance = json_account["InitialBalance"].asDouble();
	SetInitializedBalance(InitialBalance);

	///初始化合约信息, 保证金, 手续费等
	const Json::Value& json_instrumentlist = json_object["InstrumentList"];
	for (int i = 0; i < json_instrumentlist.size(); ++i)
	{
		const Json::Value& json_instrument = json_instrumentlist[i];
		if (!json_instrument["InstrumentID"].isString()
			||!json_instrument["ExchangeID"].isString()
			||!json_instrument["VolumeMultiple"].isNumeric()
			||!json_instrument["Slippage"].isNumeric()
			||!json_instrument["LongMarginRatioByMoney"].isNumeric()
			||!json_instrument["LongMarginRatioByVolume"].isNumeric()
			||!json_instrument["ShortMarginRatioByMoney"].isNumeric()
			||!json_instrument["ShortMarginRatioByVolume"].isNumeric()
			||!json_instrument["OpenRatioByMoney"].isNumeric()
			||!json_instrument["OpenRatioByVolume"].isNumeric()
			||!json_instrument["CloseRatioByMoney"].isNumeric()
			||!json_instrument["CloseRatioByVolume"].isNumeric()
			||!json_instrument["CloseTodayRatioByMoney"].isNumeric()
			||!json_instrument["CloseTodayRatioByVolume"].isNumeric())
		{
			m_pLogHandler->Error("Invaild Instrumet Params!");
			return 1;
		}
		string InstrumentID = json_instrument["InstrumentID"].asString();
		string ExchangeID = json_instrument["ExchangeID"].asString();
		int VolumeMultiple = json_instrument["VolumeMultiple"].asInt();
		double Slippage = json_instrument["Slippage"].asDouble();
		double LongMarginRatioByMoney = json_instrument["LongMarginRatioByMoney"].asDouble();
		double LongMarginRatioByVolume = json_instrument["LongMarginRatioByVolume"].asDouble();
		double ShortMarginRatioByMoney = json_instrument["ShortMarginRatioByMoney"].asDouble();
		double ShortMarginRatioByVolume = json_instrument["ShortMarginRatioByVolume"].asDouble();
		double OpenRatioByMoney = json_instrument["OpenRatioByMoney"].asDouble();
		double OpenRatioByVolume = json_instrument["OpenRatioByVolume"].asDouble();
		double CloseRatioByMoney = json_instrument["CloseRatioByMoney"].asDouble();
		double CloseRatioByVolume = json_instrument["CloseRatioByVolume"].asDouble();
		double CloseTodayRatioByMoney = json_instrument["CloseTodayRatioByMoney"].asDouble();
		double CloseTodayRatioByVolume = json_instrument["CloseTodayRatioByVolume"].asDouble();

		CThostInstrumentField InstrumentInfo;
		InstrumentInfo.InstrumentID = InstrumentID;
		InstrumentInfo.ExchangeID = ExchangeID;
		InstrumentInfo.VolumeMultiple = VolumeMultiple;
		SetInstrumentInfo(InstrumentInfo);

		CThostInstrumentMarginRateField InstrumentMarginRate;
		InstrumentMarginRate.InstrumentID = InstrumentID;
		InstrumentMarginRate.LongMarginRatioByMoney = LongMarginRatioByMoney;
		InstrumentMarginRate.LongMarginRatioByVolume = LongMarginRatioByVolume;
		InstrumentMarginRate.ShortMarginRatioByMoney = ShortMarginRatioByMoney;
		InstrumentMarginRate.ShortMarginRatioByVolume = ShortMarginRatioByVolume;
		SetInstrumentMarginRate(InstrumentMarginRate);

		CThostInstrumentCommissionRateField InstrumetnCommissionRate;
		InstrumetnCommissionRate.InstrumentID = InstrumentID;
		InstrumetnCommissionRate.OpenRatioByMoney = OpenRatioByMoney;
		InstrumetnCommissionRate.OpenRatioByVolume = OpenRatioByVolume;
		InstrumetnCommissionRate.CloseRatioByMoney = CloseRatioByMoney;
		InstrumetnCommissionRate.CloseRatioByVolume = CloseRatioByVolume;
		InstrumetnCommissionRate.CloseTodayRatioByMoney = CloseTodayRatioByMoney;
		InstrumetnCommissionRate.CloseTodayRatioByVolume = CloseTodayRatioByVolume;
		SetInstrumentCommissionRate(InstrumetnCommissionRate);
	}
	return 0;
}

int CSimulationTraderHandler::Wait()
{
	return 0;
}

void CSimulationTraderHandler::Stop()
{
}

void CSimulationTraderHandler::LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
	///订单请求字段 
	CThostOrderReportField *pOrderReport = new CThostOrderReportField();
	pOrderReport->BrokerID = "99999";
	pOrderReport->InvestorID = "backtest";
	pOrderReport->UserID = "backtest";
	pOrderReport->InstrumentID = InstrumentID;
	pOrderReport->CombHedgeFlag = HedgeFlag;
	pOrderReport->Direction = Direction;
	pOrderReport->CombOffsetFlag = OffsetFlag;
	pOrderReport->VolumeTotalOriginal = Volume;
	pOrderReport->LimitPrice = LimitPrice;
	pOrderReport->OrderPriceType = THOST_OPT_LimitPrice;
	pOrderReport->VolumeCondition = THOST_VC_AV;
	pOrderReport->TimeCondition = THOST_TC_IOC;
	pOrderReport->ContingentCondition = THOST_CC_Immediately;
	pOrderReport->ForceCloseReason = THOST_FCC_NotForceClose;
	pOrderReport->IsSwapOrder = false;
	pOrderReport->IsAutoSuspend = false;
	pOrderReport->UserForceClose = false;
	pOrderReport->StopPrice = 0;
	pOrderReport->RequestID = RequestID;
	char pszOrderRef[13];
	memset(pszOrderRef, '\0', sizeof(pszOrderRef));
	sprintf(pszOrderRef, "%12d", ++m_nOrderRef);
	pOrderReport->OrderRef = pszOrderRef;
	pOrderReport->FilledVolume = 0;
	pOrderReport->AveragePrice = 0;
	pOrderReport->FrontID = 0;
	pOrderReport->SessionID = 0;

	///首先检查资金和持仓是否合规
	if (OffsetFlag == THOST_OF_Open)
	{
		/*
		0 资金足够
		1 资金不够
		-1 无法后去合约信息,保证金,手续费等
		*/
		int nRet = CheckBalanceToEntrust(InstrumentID, Direction, LimitPrice, Volume, HedgeFlag);
		if (nRet == 1)
		{
			///订单回报，资金不足
			pOrderReport->OrderStatus = THOST_OST_Error;
			pOrderReport->StatusMsg = "资金不足";
			if (m_pTraderHandlerSpi != NULL)
			{
				m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			}
		}
		else if (nRet == -1)
		{
			return ;
		}
	}
	else if (OffsetFlag == THOST_OF_Close || OffsetFlag == THOST_OF_CloseToday || OffsetFlag == THOST_OF_CloseYesterday)
	{
		char PositionDirection = THOST_PSD_ALL;
		if (OffsetFlag == THOST_OF_CloseToday)
		{
			PositionDirection = THOST_PSD_Today;
		}

		if (OffsetFlag == THOST_OF_CloseYesterday)
		{
			PositionDirection = THOST_PSD_History;
		}
		///检查持仓
		int positionAmount = GetPositionAmount(InstrumentID, Direction == THOST_D_Long ? THOST_D_Short : THOST_D_Long, PositionDirection);
		if (Volume > positionAmount)
		{
			///订单回报，可平量不足
			pOrderReport->OrderStatus = THOST_OST_Error;
			pOrderReport->StatusMsg = "可平量不足";
			if (m_pTraderHandlerSpi != NULL)
			{
				m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			}
			return ;
		}
	}

	CKBarField kbarField = m_mapKBarSnapshot[pOrderReport->InstrumentID];

	///OnRtnOrder 模拟委托成功委托
	pOrderReport->OrderStatus = THOST_OST_Submitted;
	pOrderReport->InsertDate = m_nTradingDay;
	pOrderReport->InsertTime = kbarField.EndTime;
	char pszOrderSysID[21];
	memset(pszOrderSysID, '\0', sizeof(pszOrderSysID));
	sprintf(pszOrderSysID, "%20d", ++m_nOrderSysID);
	pOrderReport->OrderSysID = pszOrderSysID;
	pOrderReport->StatusMsg = "委托成功";
	if (m_pTraderHandlerSpi != NULL)
	{
		m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
	}

	///更新用户权益
	UpdateTradingAccount(pOrderReport);
}



void CSimulationTraderHandler::MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
}

void CSimulationTraderHandler::FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
}

void CSimulationTraderHandler::FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag /* = THOST_HF_Speculation */)
{
}
int CSimulationTraderHandler::CancelOrder(string OrderRef, string OrderSysID, string ExchangeID)
{
	return 0;
}

int CSimulationTraderHandler::GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID)
{
	if (m_mapInstrumentInfo.end() == m_mapInstrumentInfo.find(InstrumentID))
	{
		return 1;
	}
	InstrumentInfo = m_mapInstrumentInfo[InstrumentID];
	return 0;
}

int CSimulationTraderHandler::GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo)
{
	for (InstrumentInfoMap::iterator iter = m_mapInstrumentInfo.begin(); iter != m_mapInstrumentInfo.end(); ++iter)
	{
		vecInstrumentInfo.push_back(iter->second);
	}
	return 0;
}

int CSimulationTraderHandler::GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID)
{
	if (m_mapInstrumentMarginRate.end() == m_mapInstrumentMarginRate.find(InstrumentID))
	{
		return 1;
	}
	InstrumentMarginRate = m_mapInstrumentMarginRate[InstrumentID];
	return 0;
}

int CSimulationTraderHandler::GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID)
{
	if (m_mapInstrumentComissionRate.end() == m_mapInstrumentComissionRate.find(InstrumentID))
	{
		return 1;
	}
	InstrumentCommissionRate = m_mapInstrumentComissionRate[InstrumentID];
	return 0;
}

int CSimulationTraderHandler::GetTradingAccount(CThostTradingAccountField &TradingAccount)
{
	UpdateAccountBalance();
	TradingAccount = m_TradingAccount;
	return 0;
}

int CSimulationTraderHandler::GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID)
{
	m_vecInvestorPosition.clear();
	vecInvestorPosition = m_vecInvestorPosition;
	return 0;
}

int CSimulationTraderHandler::GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd)
{
	m_vecOrderReport.clear();
	sort(m_vecOrderReport.begin(), m_vecOrderReport.end());
	vecOrderReport = m_vecOrderReport;
	return 0;
}

int CSimulationTraderHandler::GetTrade(TradeVec &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd)
{
	m_vecTrade.clear();
	sort(m_vecTrade.begin(), m_vecTrade.end());
	vecTrade = m_vecTrade;
	return 0;
}

string CSimulationTraderHandler::GetInvestorID()
{
	return m_sInvestorId;
}

int CSimulationTraderHandler::GetTradingDay()
{
	return m_nTradingDay;
}

void CSimulationTraderHandler::SetTradingDay(int TradingDay)
{
	m_nTradingDay = TradingDay;
}

void CSimulationTraderHandler::Clear()
{
	m_nOrderRef = 0;
	m_nOrderSysID = 0;
	m_nTradeID = 0;
	memset(&m_TradingAccount, 0, sizeof(CThostTradingAccountField));
	m_vecYdTrade.clear();
	m_vecTdTrade.clear();
	m_vecAllTrade.clear();
	m_mapKBarSnapshot.clear();
	m_vecBalanceList.clear();
	m_mapInstrumentInfo.clear();
	m_mapInstrumentMarginRate.clear();
	m_mapInstrumentComissionRate.clear();
}

int CSimulationTraderHandler::CheckBalanceToEntrust(string InstrumentID, char Direction, double LimitPrice, int Volume, char HedgeFlag)
{
	if (m_mapInstrumentInfo.end() == m_mapInstrumentInfo.find(InstrumentID))
	{
		m_pLogHandler->Error("Invaild InstrumentInfo:%s", InstrumentID.c_str());
		return -1;
	}
	CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[InstrumentID];

	if (m_mapInstrumentMarginRate.end() == m_mapInstrumentMarginRate.find(InstrumentID))
	{
		m_pLogHandler->Error("Invaild InstrumentMarginRate:%s", InstrumentID.c_str());
		return -1;
	}
	CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[InstrumentID];

	if (m_mapInstrumentComissionRate.end() == m_mapInstrumentComissionRate.find(InstrumentID))
	{
		m_pLogHandler->Error("Invaild InstrumentComissionRate:%s", InstrumentID.c_str());
		return -1;
	}
	CThostInstrumentCommissionRateField instrumentCommissionRate = m_mapInstrumentComissionRate[InstrumentID];
	
	double marginRateByMoney = Direction == THOST_D_Long ? instrumentMarginRate.LongMarginRatioByMoney : instrumentMarginRate.ShortMarginRatioByMoney;
	double marginRateByVolume = Direction == THOST_D_Long ? instrumentMarginRate.LongMarginRatioByVolume : instrumentMarginRate.ShortMarginRatioByVolume;
	double entrustMoney = LimitPrice * Volume * instrumentInfo.VolumeMultiple * marginRateByMoney + Volume * marginRateByVolume;
	double entrustComission = LimitPrice * Volume * instrumentInfo.VolumeMultiple * instrumentCommissionRate.OpenRatioByMoney + Volume * instrumentCommissionRate.OpenRatioByVolume;

	if (entrustMoney + entrustComission> m_TradingAccount.Balance)
	{
		return 1;
	}
	return 0;
}

int CSimulationTraderHandler::GetPositionAmount(string InstrumentID, char Direction, char PositionDirection)
{
	int positionAmount = 0;
	///获取昨日持仓
	if (PositionDirection == THOST_PSD_ALL || PositionDirection == THOST_PSD_History)
	{
		for (int i = 0; i < m_vecYdTrade.size(); ++i)
		{
			if (m_vecYdTrade[i].InstrumentID == InstrumentID && m_vecYdTrade[i].Direction == Direction)
			{	
				positionAmount += m_vecYdTrade[i].Volume;
			}
		}
	}
	///获取今日持仓
	if (PositionDirection == THOST_PSD_ALL || PositionDirection == THOST_PSD_Today)
	{
		for (int i = 0; i < m_vecTdTrade.size(); ++i)
		{
			if (m_vecTdTrade[i].InstrumentID == InstrumentID && m_vecTdTrade[i].Direction == Direction)
			{	
				positionAmount += m_vecTdTrade[i].Volume;
			}
		}
	}
	return positionAmount;
}

void CSimulationTraderHandler::UpdateAccountBalance()
{
	TradeVec::iterator iter;
	double positionprofit = 0;
	double usedmargin = 0;
	for (iter = m_vecYdTrade.begin(); iter != m_vecYdTrade.end(); ++iter)
	{
		CKBarField kbarField = m_mapKBarSnapshot[(*iter).InstrumentID];
		if (m_mapInstrumentInfo.end() != m_mapInstrumentInfo.find((*iter).InstrumentID))
		{
			CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[(*iter).InstrumentID];
			CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[(*iter).InstrumentID];
			double tempPositionProfit = (kbarField.ClosePrice - (*iter).Price) * (*iter).Volume * instrumentInfo.VolumeMultiple;
			if ((*iter).Direction == THOST_D_Short)
			{
				tempPositionProfit *= -1;
				usedmargin += (*iter).Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentMarginRate.ShortMarginRatioByMoney + (*iter).Volume * instrumentMarginRate.ShortMarginRatioByVolume;
			}
			else
			{
				usedmargin += (*iter).Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentMarginRate.LongMarginRatioByMoney + (*iter).Volume * instrumentMarginRate.LongMarginRatioByVolume;
			}
			positionprofit += tempPositionProfit;
		}
	}

	for (iter = m_vecTdTrade.begin(); iter != m_vecTdTrade.end(); ++iter)
	{
		CKBarField kbarField = m_mapKBarSnapshot[(*iter).InstrumentID];
		if (m_mapInstrumentInfo.end() != m_mapInstrumentInfo.find((*iter).InstrumentID))
		{
			CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[(*iter).InstrumentID];
			CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[(*iter).InstrumentID];
			double tempPositionProfit = (kbarField.ClosePrice - (*iter).Price) * (*iter).Volume * instrumentInfo.VolumeMultiple;
			if ((*iter).Direction == THOST_D_Short)
			{
				tempPositionProfit *= -1;
				usedmargin += (*iter).Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentMarginRate.ShortMarginRatioByMoney + (*iter).Volume * instrumentMarginRate.ShortMarginRatioByVolume;
			}
			else
			{
				usedmargin += (*iter).Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentMarginRate.LongMarginRatioByMoney + (*iter).Volume * instrumentMarginRate.LongMarginRatioByVolume;
			}
			positionprofit += tempPositionProfit;
		}
	}
	m_TradingAccount.CurrMargin = usedmargin;
	m_TradingAccount.PositionProfit = positionprofit;
	m_TradingAccount.Balance = m_TradingAccount.PreBalance + m_TradingAccount.PositionProfit + m_TradingAccount.CloseProfit - m_TradingAccount.Commission;
	m_TradingAccount.Available = m_TradingAccount.Balance - usedmargin;
}

void CSimulationTraderHandler::UpdateTradingAccount(CThostOrderReportField *pOrderReport)
{
	///生成成交明细
	CThostTradeField tradeField;
	MakeTradeFromOrderReport(tradeField, pOrderReport);
	m_vecAllTrade.push_back(tradeField);

	CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[pOrderReport->InstrumentID];
	CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[pOrderReport->InstrumentID];
	CThostInstrumentCommissionRateField instrumentCommissionRate = m_mapInstrumentComissionRate[pOrderReport->InstrumentID];

	///OnRtnOrder 模拟成交(通过参数增加滑点)
	pOrderReport->AveragePrice = 0;
	pOrderReport->FilledVolume = 0;
	
	if (pOrderReport->CombOffsetFlag[0] == THOST_OF_Open)
	{
		///开仓加入成交列表
		m_vecTdTrade.push_back(tradeField);
		double filledComission = tradeField.Price * tradeField.Volume * instrumentInfo.VolumeMultiple * instrumentCommissionRate.OpenRatioByMoney + tradeField.Volume * instrumentCommissionRate.OpenRatioByVolume;
		m_TradingAccount.Commission += filledComission;

		pOrderReport->AveragePrice = pOrderReport->LimitPrice;
		pOrderReport->FilledVolume += pOrderReport->VolumeTotalOriginal;
		pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
		pOrderReport->LastFilledVolume = pOrderReport->VolumeTotalOriginal;
		pOrderReport->OrderStatus = THOST_OST_Complete;
		pOrderReport->StatusMsg = "全部成交";
		if (m_pTraderHandlerSpi != NULL)
		{
			m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
		}
		return ;
	}

	int nCloseCount = pOrderReport->VolumeTotalOriginal;
	if (pOrderReport->CombOffsetFlag[0] == THOST_OF_CloseYesterday)
	{
		char Direction = tradeField.Direction == THOST_D_Long ? THOST_D_Short : THOST_D_Long;
		if (GetPositionAmount(tradeField.InstrumentID, Direction, THOST_PSD_History) >= nCloseCount)
		{
			CloseYdPosition(tradeField, nCloseCount);
			pOrderReport->CombOffsetFlag = THOST_OF_CloseYesterday;
			pOrderReport->AveragePrice = pOrderReport->LimitPrice;
			pOrderReport->FilledVolume += pOrderReport->VolumeTotalOriginal;
			pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
			pOrderReport->LastFilledVolume = pOrderReport->VolumeTotalOriginal;
			pOrderReport->OrderStatus = THOST_OST_Complete;
			pOrderReport->StatusMsg = "全部成交";
			if (m_pTraderHandlerSpi != NULL)
			{
				m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			}
			return;
		}
		else
		{
			m_pLogHandler->Error("Yd Position is't enough");
			return ;
		}
	}

	if (pOrderReport->CombOffsetFlag[0] == THOST_OF_CloseToday)
	{
		char Direction = tradeField.Direction == THOST_D_Long ? THOST_D_Short : THOST_D_Long;
		if (GetPositionAmount(tradeField.InstrumentID, Direction, THOST_PSD_Today) >= nCloseCount)
		{
			CloseTdPosition(tradeField, nCloseCount);
			pOrderReport->CombOffsetFlag = THOST_OF_CloseToday;
			pOrderReport->AveragePrice = pOrderReport->LimitPrice;
			pOrderReport->FilledVolume += pOrderReport->VolumeTotalOriginal;
			pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
			pOrderReport->LastFilledVolume = pOrderReport->VolumeTotalOriginal;
			pOrderReport->OrderStatus = THOST_OST_Complete;
			pOrderReport->StatusMsg = "全部成交";
			if (m_pTraderHandlerSpi != NULL)
			{
				m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
			}
		}
		else
		{
			m_pLogHandler->Error("Td Position is't enough!");
			return ;
		}
	}

	if (pOrderReport->CombOffsetFlag[0] == THOST_OF_Close)
	{
		char Direction = tradeField.Direction == THOST_D_Long ? THOST_D_Short : THOST_D_Long;
		if (GetPositionAmount(tradeField.InstrumentID, Direction, THOST_PSD_ALL) >= nCloseCount)
		{
			int nNeedCloseCount = nCloseCount;
			CloseYdPosition(tradeField, nCloseCount);
			if (nCloseCount > 0)
			{
				///有昨仓才推送
				if (nNeedCloseCount - nCloseCount > 0)
				{
					pOrderReport->CombOffsetFlag = THOST_OF_CloseYesterday;
					pOrderReport->AveragePrice = pOrderReport->LimitPrice;
					pOrderReport->FilledVolume += nNeedCloseCount - nCloseCount;
					pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
					pOrderReport->LastFilledVolume = nNeedCloseCount - nCloseCount;
					pOrderReport->OrderStatus = THOST_OST_Filled;
					pOrderReport->StatusMsg = "部分成交";
					if (m_pTraderHandlerSpi != NULL)
					{
						m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
					}
				}
				nNeedCloseCount = nCloseCount;
				CloseTdPosition(tradeField, nCloseCount);
				pOrderReport->CombOffsetFlag = THOST_OF_CloseToday;
				pOrderReport->AveragePrice = pOrderReport->LimitPrice;
				pOrderReport->FilledVolume += nNeedCloseCount;
				pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
				pOrderReport->LastFilledVolume = nNeedCloseCount;
				pOrderReport->OrderStatus = THOST_OST_Complete;
				pOrderReport->StatusMsg = "全部成交";
				if (m_pTraderHandlerSpi != NULL)
				{
					m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
				}
			}
			else
			{
				pOrderReport->CombOffsetFlag = THOST_OF_CloseYesterday;
				pOrderReport->AveragePrice = pOrderReport->LimitPrice;
				pOrderReport->FilledVolume += pOrderReport->VolumeTotalOriginal;
				pOrderReport->LastFilledPrice = pOrderReport->LimitPrice;
				pOrderReport->LastFilledVolume = pOrderReport->VolumeTotalOriginal;
				pOrderReport->OrderStatus = THOST_OST_Complete;
				pOrderReport->StatusMsg = "全部成交";
				if (m_pTraderHandlerSpi != NULL)
				{
					m_pTraderHandlerSpi->OnRtnOrderReport(pOrderReport);
				}
			}
		}
		else
		{
			m_pLogHandler->Error("Position is't enough");
			return ;
		}
		if (nCloseCount != 0)
		{
			m_pLogHandler->Error("Close Position Failed!");
		}
	}
}

void CSimulationTraderHandler::CloseYdPosition(CThostTradeField &tradeField, int &CloseCount)
{
	CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[tradeField.InstrumentID];
	CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[tradeField.InstrumentID];
	CThostInstrumentCommissionRateField instrumentCommissionRate = m_mapInstrumentComissionRate[tradeField.InstrumentID];
	for (TradeVec::iterator iter = m_vecYdTrade.begin(); iter != m_vecYdTrade.end();)
	{
		if ((*iter).InvestorID == tradeField.InvestorID && 
			(*iter).InstrumentID == tradeField.InstrumentID &&
			(*iter).Direction != tradeField.Direction)
		{
			if ((*iter).Volume > CloseCount)
			{
				m_TradingAccount.Commission += tradeField.Price * CloseCount * instrumentInfo.VolumeMultiple * instrumentCommissionRate.CloseRatioByMoney + CloseCount * instrumentCommissionRate.CloseRatioByVolume;
				double closeprofit = (tradeField.Price - (*iter).Price) * CloseCount * instrumentInfo.VolumeMultiple;
				if (tradeField.Direction == THOST_D_Long)
				{
					closeprofit *= -1;
				}
				m_TradingAccount.CloseProfit += closeprofit;

				(*iter).Volume -= CloseCount;
				CloseCount = 0;
				return ;
			}
			else
			{
				m_TradingAccount.Commission += tradeField.Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentCommissionRate.CloseRatioByMoney + (*iter).Volume * instrumentCommissionRate.CloseRatioByVolume;
				double closeprofit = (tradeField.Price - (*iter).Price) * (*iter).Volume * instrumentInfo.VolumeMultiple;
				if (tradeField.Direction == THOST_D_Long)
				{
					closeprofit *= -1;
				}
				m_TradingAccount.CloseProfit += closeprofit;

				CloseCount -= (*iter).Volume;
				iter = m_vecYdTrade.erase(iter);
				if (CloseCount == 0)
				{
					return ;
				}				
			}
		}
		else
		{
			iter++;
		}
	}
}

void CSimulationTraderHandler::CloseTdPosition(CThostTradeField &tradeField, int &CloseCount)
{
	CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[tradeField.InstrumentID];
	CThostInstrumentMarginRateField instrumentMarginRate = m_mapInstrumentMarginRate[tradeField.InstrumentID];
	CThostInstrumentCommissionRateField instrumentCommissionRate = m_mapInstrumentComissionRate[tradeField.InstrumentID];
	for (TradeVec::iterator iter = m_vecTdTrade.begin(); iter != m_vecTdTrade.end();)
	{
		if ((*iter).InvestorID == tradeField.InvestorID && 
			(*iter).InstrumentID == tradeField.InstrumentID &&
			(*iter).Direction != tradeField.Direction)
		{
			if ((*iter).Volume > CloseCount)
			{
				m_TradingAccount.Commission += tradeField.Price * CloseCount * instrumentInfo.VolumeMultiple * instrumentCommissionRate.CloseTodayRatioByMoney + CloseCount * instrumentCommissionRate.CloseTodayRatioByVolume;
				double closeprofit = (tradeField.Price - (*iter).Price) * CloseCount * instrumentInfo.VolumeMultiple;
				if (tradeField.Direction == THOST_D_Long)
				{
					closeprofit *= -1;
				}
				m_TradingAccount.CloseProfit += closeprofit;
				(*iter).Volume -= CloseCount;
				CloseCount = 0;
				break;
			}
			else
			{
				m_TradingAccount.Commission += tradeField.Price * (*iter).Volume * instrumentInfo.VolumeMultiple * instrumentCommissionRate.CloseTodayRatioByMoney + (*iter).Volume * instrumentCommissionRate.CloseTodayRatioByVolume;
				double closeprofit = (tradeField.Price - (*iter).Price) * (*iter).Volume * instrumentInfo.VolumeMultiple;
				if (tradeField.Direction == THOST_D_Long)
				{
					closeprofit *= -1;
				}
				m_TradingAccount.CloseProfit += closeprofit;
				CloseCount -= (*iter).Volume;
				iter = m_vecTdTrade.erase(iter);
				if (CloseCount == 0)
				{
					break;
				}
			}
		}
		else
		{
			iter++;
		}
	}
}

void CSimulationTraderHandler::MakeTradeFromOrderReport(CThostTradeField &tradeField, CThostOrderReportField *pOrderReport)
{
	tradeField.BrokerID = pOrderReport->BrokerID;
	tradeField.InvestorID = pOrderReport->InvestorID;
	tradeField.InstrumentID = pOrderReport->InstrumentID;
	tradeField.OrderRef = pOrderReport->OrderRef;
	tradeField.UserID = pOrderReport->UserID;
	tradeField.ExchangeID = pOrderReport->ExchangeID;
	char pszTradeID[13];
	memset(pszTradeID, '\0', sizeof(pszTradeID));
	sprintf(pszTradeID, "bt%10d", ++m_nTradeID);
	tradeField.TradeID = pszTradeID;
	tradeField.Direction = pOrderReport->Direction;
	tradeField.OrderSysID = pOrderReport->OrderSysID;
	tradeField.OffsetFlag = pOrderReport->CombOffsetFlag[0];
	tradeField.HedgeFlag = pOrderReport->CombHedgeFlag[0];
	tradeField.Price = pOrderReport->LimitPrice;
	tradeField.Volume = pOrderReport->VolumeTotalOriginal;
	tradeField.TradingDay = pOrderReport->InsertDate;
	tradeField.TradeDate = pOrderReport->InsertDate;
	tradeField.TradeTime = pOrderReport->InsertTime;
}

void CSimulationTraderHandler::BeginTradingDaySettle(int TradingDay)
{
	///更新资金
	UpdateAccountBalance();
	m_TradingAccount.TradingDay = TradingDay;
	m_vecBalanceList.push_back(m_TradingAccount.Balance);

	///资金结算
	m_TradingAccount.PreBalance = m_TradingAccount.Balance;
	m_TradingAccount.PositionProfit = 0;
	m_TradingAccount.CloseProfit = 0;
	m_TradingAccount.Commission = 0;

	///持仓结算
	///将昨日持仓中的价格替换成当日的结算价(收盘价)
	for (int i = 0; i < m_vecYdTrade.size(); ++i)
	{
		CKBarField kbarField = m_mapKBarSnapshot[m_vecYdTrade[i].InstrumentID];
		m_vecYdTrade[i].Price = kbarField.ClosePrice;
	}

	TradeVec vecNewPosition;
	int i, j;
	///按照收盘价将当日持仓合并到昨日持仓中
	for (i = 0; i < m_vecTdTrade.size(); ++i)
	{
		for (j = 0; j < m_vecYdTrade.size(); ++j)
		{
			if (m_vecTdTrade[i].InstrumentID == m_vecYdTrade[j].InstrumentID &&
				m_vecTdTrade[i].Direction == m_vecYdTrade[j].Direction)
			{
				break;
			}
		}
		if (j < m_vecYdTrade.size())
		{
			///合并昨日与今日持仓
			m_vecYdTrade[j].Volume += m_vecTdTrade[i].Volume;
		}
		else
		{
			///新建
			CKBarField kbarField = m_mapKBarSnapshot[m_vecTdTrade[i].InstrumentID];
			m_vecTdTrade[i].Price = kbarField.ClosePrice;
			vecNewPosition.push_back(m_vecTdTrade[i]);
		}	
	}
	for (int i = 0; i < vecNewPosition.size(); ++i)
	{
		m_vecYdTrade.push_back(vecNewPosition[i]);
	}
	m_vecTdTrade.clear();
	vecNewPosition.clear();
}

void CSimulationTraderHandler::UpdateKBarSnapshot(string InstrumentID, CKBarField &kbarField)
{
	m_mapKBarSnapshot[InstrumentID] = kbarField;
}

void CSimulationTraderHandler::SetInitializedBalance(double Balance)
{
	memset(&m_TradingAccount, 0, sizeof(CThostTradingAccountField));
	m_vecBalanceList.push_back(Balance);
	m_TradingAccount.PreBalance = Balance;
	m_TradingAccount.Balance = Balance;
	m_TradingAccount.Available = Balance;
}

void CSimulationTraderHandler::GetBalanceList(vector<double> &vecBalance)
{
	vecBalance = m_vecBalanceList;
}

void CSimulationTraderHandler::SetInstrumentInfo(CThostInstrumentField &InstrumentInfo)
{
	string InstrumentID = InstrumentInfo.InstrumentID;
	InstrumentInfoMap::iterator iter;
	if (m_mapInstrumentInfo.end() != (iter = m_mapInstrumentInfo.find(InstrumentID)))
	{
		m_mapInstrumentInfo.erase(iter);
	}
	CThostInstrumentField instrumentInfo;
	instrumentInfo = InstrumentInfo;
	m_mapInstrumentInfo[InstrumentID] = instrumentInfo;
}

void CSimulationTraderHandler::SetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate)
{
	string InstrumentID = InstrumentMarginRate.InstrumentID;
	InstrumentMarginRateMap::iterator iter;
	if (m_mapInstrumentMarginRate.end() != (iter = m_mapInstrumentMarginRate.find(InstrumentID)))
	{
		m_mapInstrumentMarginRate.erase(iter);
	}
	CThostInstrumentMarginRateField instrumentMarginRate;
	instrumentMarginRate = InstrumentMarginRate;
	m_mapInstrumentMarginRate[InstrumentID] = instrumentMarginRate;
}

void CSimulationTraderHandler::SetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate)
{
	string InstrumentID = InstrumentCommissionRate.InstrumentID;
	InstrumentComissionRateMap::iterator iter;
	if (m_mapInstrumentComissionRate.end() != (iter = m_mapInstrumentComissionRate.find(InstrumentID)))
	{
		m_mapInstrumentComissionRate.erase(iter);
	}
	CThostInstrumentCommissionRateField instrumentCommissionRate;
	instrumentCommissionRate = InstrumentCommissionRate;
	m_mapInstrumentComissionRate[InstrumentID] = instrumentCommissionRate;
}