#ifndef INC_STRATEGYSAMPLE_H
#define INC_STRATEGYSAMPLE_H

#include "strategyapi.h"
#include <boost/thread.hpp>

class CStrategySample : public CStrategySpi {
public:
	CStrategySample(){};
	~CStrategySample(){};

	void Start()
	{
		m_pStrategyApi = CStrategyApi::CreateStrategyApi();
		m_pStrategyApi->RegisterStrategySpi(this);
		
		m_pStrategyApi->RegisterLogServer("", true);
		
		//m_pStrategyApi->RegisterCTPTraderServer("100103962", "042828", "66666","tcp://ctp1-front1.citicsf.com:41205", "", "");
		//m_pStrategyApi->RegisterCTPTraderServer("666856", "100911", "4050","tcp://shjy-front1-shctc-ctp.jyqh.com.cn:41205", "", "");
		//m_pStrategyApi->RegisterCTPTraderServer("100103962", "042828", "66666","tcp://27.115.78.196:41205", "", "");
		//m_pStrategyApi->RegisterCTPTraderServer("100103962", "042828", "66666","tcp://180.169.101.180:41205", "", "");
		m_pStrategyApi->RegisterCTPTraderServer("030985", "abc123", "9999","tcp://180.168.146.187:10001", "", "quant_ctp");
		CThostInstrumentField InstrumentInfo;
		m_pStrategyApi->GetInstrument(InstrumentInfo, "m1701");
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "CTP:%s %s %s %d", InstrumentInfo.InstrumentID.c_str(), InstrumentInfo.InstrumentName.c_str(), InstrumentInfo.ExchangeID.c_str(), InstrumentInfo.VolumeMultiple);
		CThostInstrumentMarginRateField InstrumentMarginRate;
		m_pStrategyApi->GetInstrumentMarginRate(InstrumentMarginRate, "ru1701");
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "CTP:%s %f %f", InstrumentMarginRate.InstrumentID.c_str(), InstrumentMarginRate.LongMarginRatioByMoney, InstrumentMarginRate.ShortMarginRatioByMoney);
		CThostInstrumentCommissionRateField InstrumentCommissionRate;
		m_pStrategyApi->GetInstrumentCommissionRate(InstrumentCommissionRate, "ru1701");
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "CTP:%s %f %f", InstrumentCommissionRate.InstrumentID.c_str(), InstrumentCommissionRate.OpenRatioByMoney, InstrumentCommissionRate.CloseRatioByMoney);
		
		
		
		CThostTradingAccountField tradingAccount;
		m_pStrategyApi->GetTradingAccount(tradingAccount, "030985");
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "CTP:%s %f %", tradingAccount.AccountID.c_str(), tradingAccount.PreBalance, tradingAccount.Balance);
		vector<CThostInvestorPositionField> vecInvestorPosition;
		m_pStrategyApi->GetInvestorPosition(vecInvestorPosition, "030985", "");
		for (vector<CThostInvestorPositionField>::iterator iter = vecInvestorPosition.begin();
			iter != vecInvestorPosition.end(); ++iter)
		{
			
			m_pStrategyApi->Log(LOG_LEVEL_INFO, "InstrumentID:%s PosiDirection:%s PositionDate:%s Position:%d TodayPosition:%d YdPosition:%d PositionProfit:%f PositionCost:%f UseMargin:%f",
				iter->InstrumentID.c_str(), iter->PosiDirection == THOST_PD_Long ? "多" : "空", 
				iter->PositionDate == THOST_PSD_History ? "昨" : "今",
				iter->Position, iter->TodayPosition, iter->YdPosition,
				iter->PositionProfit, iter->PositionCost, iter->UseMargin);
		}

 		m_pStrategyApi->RegisterCTPMdServer("100105138", "100911", "66666","tcp://ctp1-md1.citicsf.com:41213", "");
		m_pStrategyApi->RegisterFSMdServer("0600058", "888888", "0017", "tcp://140.206.81.6:47777", "");
// 		char *p[3] = {"ru1701", "rb1701", "Au(T+D)"};
// 		m_pStrategyApi->SubscribeQuote(p, 3);
		m_pStrategyApi->RegisterFSTraderServer("0600058", "888888", "0017", "tcp://140.206.81.6:47776", "", "quant_fs");
// 		m_pStrategyApi->LimitOrder("Au(T+D)", THOST_D_Short, THOST_OF_Open, 270, 2);
// 		OrderReportVec vecOrderReport;
// 		m_pStrategyApi->GetOrder("0600058", "", "", "", "", "", vecOrderReport);
// 		for (OrderReportVec::iterator iter = vecOrderReport.begin(); iter != vecOrderReport.end(); iter++)
// 		{
// 			m_pStrategyApi->Log(LOG_LEVEL_INFO, "OrderSysID:%s InstrumentID:%s Direction:%c OffsetFlag:%c Price:%f Volume:%d FilledVolume:%d OrderStatus:%c StatusMsg:%s",
// 				iter->OrderSysID.c_str(), iter->InstrumentID.c_str(), iter->Direction, iter->CombOffsetFlag[0], iter->LimitPrice, iter->VolumeTotalOriginal, iter->FilledVolume, iter->OrderStatus, iter->StatusMsg.c_str());
// 		}
// 
// 		TradeVec vecTrade;
// 		m_pStrategyApi->GetTrade("0600058", "", "", "", "", "", vecTrade);
// 		for (TradeVec::iterator iter = vecTrade.begin(); iter != vecTrade.end(); iter++)
// 		{
// 			m_pStrategyApi->Log(LOG_LEVEL_INFO, "TradeID:%s InstrumentID:%s	Direction:%c OffsetFlag:%c Price:%f Volume:%d TradeTime:%s", 
// 				iter->TradeID.c_str(), iter->InstrumentID.c_str(), iter->Direction, iter->OffsetFlag, iter->Price, iter->Volume, iter->TradeTime.c_str());
// 		}

		CThostTradingAccountField tradingAccountFS;
		m_pStrategyApi->GetTradingAccount(tradingAccountFS, "0600058");
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "FS:%s %f %", tradingAccountFS.AccountID.c_str(), tradingAccountFS.PreBalance, tradingAccountFS.Balance);
		vector<CThostInvestorPositionField> vecInvestorPositionFS;
		m_pStrategyApi->GetInvestorPosition(vecInvestorPositionFS, "0600058", "");
		for (vector<CThostInvestorPositionField>::iterator iter = vecInvestorPositionFS.begin();
			iter != vecInvestorPositionFS.end(); ++iter)
		{

			m_pStrategyApi->Log(LOG_LEVEL_INFO, "InstrumentID:%s PosiDirection:%s PositionDate:%s Position:%d TodayPosition:%d YdPosition:%d PositionProfit:%f PositionCost:%f UseMargin:%f",
				iter->InstrumentID.c_str(), iter->PosiDirection == THOST_PD_Long ? "多" : "空", 
				iter->PositionDate == THOST_PSD_History ? "昨" : "今",
				iter->Position, iter->TodayPosition, iter->YdPosition,
				iter->PositionProfit, iter->PositionCost, iter->UseMargin);
		}


// 		m_pStrategyApi->LimitOrder("rb1701", THOST_D_Long, THOST_OF_Open, 2500, 1);
// 
// 		m_pStrategyApi->MarketOrder("IF1609", THOST_D_Long, THOST_OF_Open, 1);
// 
// 		m_pStrategyApi->FokOrder("rb1701", THOST_D_Long, THOST_OF_Open, 2500, 1);
// 
// 		m_pStrategyApi->FakOrder("rb1701", THOST_D_Long, THOST_OF_Open, 2500, 1);
	}
	
	void Wait()
	{
		boost::mutex::scoped_lock lock(m_exitMutex);
		m_exitCond.wait(lock);
	}

public:
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData)
	{
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "%s %f", pDepthMarketData->InstrumentID.c_str(), pDepthMarketData->LastPrice);
	}

	virtual void OnRtnOrderReport(CThostOrderReportField *pOrderReport)
	{
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "~~~~~OnRtnOrderReport~~~~~~\nInvestorID:%s\nBrokerID:%s\nInstrumentID:%s\nExchangeID:%s\nOffsetFlag:%s\nDirection:%c\nLimitPrice:%f\nVolume:%d\nFrontID:%d\nSession:%d\nOrderRef:%s", 
			pOrderReport->InvestorID.c_str(), pOrderReport->BrokerID.c_str(),pOrderReport->InstrumentID.c_str(), pOrderReport->ExchangeID.c_str(), pOrderReport->CombOffsetFlag.c_str(),
			pOrderReport->Direction, pOrderReport->LimitPrice, pOrderReport->VolumeTotalOriginal, pOrderReport->FrontID, pOrderReport->SessionID, pOrderReport->OrderRef.c_str());
		
		m_pStrategyApi->Log(LOG_LEVEL_INFO, "OrderSysID:%s\nOrderStatus:%c\nFilledVolume:%d\nLastFilledVolume:%d\nLastFilledPrice:%f\nAveragePrice:%f\nTradingDay:%s\nStatusMsg:%s", pOrderReport->OrderSysID.c_str(),
			pOrderReport->OrderStatus, pOrderReport->FilledVolume, pOrderReport->LastFilledVolume,pOrderReport->LastFilledPrice, pOrderReport->AveragePrice, pOrderReport->TradingDay.c_str(),pOrderReport->StatusMsg.c_str());
// 		if (pOrderReport->OrderSysID.length() > 0 && pOrderReport->OrderStatus == THOST_OST_Submitted)
// 		{
// 			m_pStrategyApi->CancelOrder(pOrderReport->OrderRef, pOrderReport->OrderSysID, pOrderReport->ExchangeID);
// 		}
	}

private:
	CStrategyApi *m_pStrategyApi;
	boost::mutex m_exitMutex;
	boost::condition_variable m_exitCond;
};

#endif