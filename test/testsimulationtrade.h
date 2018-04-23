#ifndef INC_SIMULATIONTRADE_H
#define INC_SIMULATIONTRADE_H

#include "../thostapi/simulationhandler/simulationtradehandler.h"

class CTestSimulationTrade : public CThostTraderHandlerSpi{
public:
	CTestSimulationTrade(string testName)
	{
		m_testName = testName;
	}

	~CTestSimulationTrade(){}

public:
	void Start()
	{
		m_pSimulationTrader = new CSimulationTraderHandler();
		m_pSimulationTrader->RegisterTradeHandlerSpi(this);
		m_pSimulationTrader->Start();
		
		int ans1 = testOpen();
		int ans2 = testOpenClose();
		int ans3 = testOpenCloseSettlement();

		if (ans1 == 0 && ans2 == 0 && ans3 == 0)
		{
			printf ("%s:0\n", m_testName.c_str());
		}
	}

	int testOpen()
	{
		m_pSimulationTrader->SetInitializedBalance(1000000);
		SetInstrumentInfoMarginRateCommissionRate();
		CKBarField kbarField;
		kbarField.InstrumentID = "ru1701";
		kbarField.ExchangeID = "SHFE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 14100;
		kbarField.HighPrice = 14300;
		kbarField.LowPrice = 14000;
		kbarField.ClosePrice = 14200;
		kbarField.BidPrice = 141200;
		kbarField.BidVolume = 10;
		kbarField.AskPrice = 14205;
		kbarField.AskVolume = 10;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		kbarField.InstrumentID = "m1701";
		kbarField.ExchangeID = "DCE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 2968;
		kbarField.HighPrice = 2977;
		kbarField.LowPrice = 2965;
		kbarField.ClosePrice = 2970;
		kbarField.BidPrice = 2969;
		kbarField.BidVolume = 100;
		kbarField.AskPrice = 2971;
		kbarField.AskVolume = 100;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		m_pSimulationTrader->LimitOrder(1, "ru1701", THOST_D_Long, THOST_OF_Open, 14000, 1);
		string InstrumentID = "ru1701";

		m_pSimulationTrader->LimitOrder(2, "m1701", THOST_D_Short, THOST_OF_Open, 2970, 10);
	
		const double eps = 0.00001;
		CThostTradingAccountField exceptionTradingAccount;
		exceptionTradingAccount.PreBalance = 1000000;
		exceptionTradingAccount.Balance = 1001978.2981;
		exceptionTradingAccount.Available = 964388.2981;
		exceptionTradingAccount.PositionProfit = 2000;
		exceptionTradingAccount.CloseProfit = 0;
		exceptionTradingAccount.Commission = 21.7019;
		exceptionTradingAccount.CurrMargin = 37590;
	
		CThostTradingAccountField tradingAccount;
		m_pSimulationTrader->GetTradingAccount(tradingAccount);
		if (fabs(tradingAccount.PreBalance - exceptionTradingAccount.PreBalance) < eps &&
			fabs(tradingAccount.Balance - exceptionTradingAccount.Balance) < eps &&
			fabs(tradingAccount.Available - exceptionTradingAccount.Available) < eps &&
			fabs(tradingAccount.PositionProfit - exceptionTradingAccount.PositionProfit) < eps &&
			fabs(tradingAccount.CloseProfit - exceptionTradingAccount.CloseProfit) < eps &&
			fabs(tradingAccount.Commission - exceptionTradingAccount.Commission) < eps &&
			fabs(tradingAccount.CurrMargin - exceptionTradingAccount.CurrMargin) < eps &&
			1 == m_pSimulationTrader->GetPositionAmount("ru1701", THOST_D_Long, THOST_PSD_Today) &&
			10 == m_pSimulationTrader->GetPositionAmount("m1701", THOST_D_Short, THOST_PSD_Today)
			)
		{
			printf ("%s:testOpen:0\n", m_testName.c_str());
			return 0;
		}
// 		printf ("动态权益:%f\n可用:%f\n持仓盈亏:%f\n平仓盈亏:%f\n手续费:%f\n占用保证金:%f\n",
// 			tradingAccount.Balance, 
// 			tradingAccount.Available,
// 			tradingAccount.PositionProfit,
// 			tradingAccount.CloseProfit,
// 			tradingAccount.Commission,
// 			tradingAccount.CurrMargin);
		printf ("%s:testOpen:1\n", m_testName.c_str());
		return 1;
	}

	int testOpenClose()
	{
		m_pSimulationTrader->Clear();
		m_pSimulationTrader->SetInitializedBalance(1000000);
		SetInstrumentInfoMarginRateCommissionRate();
		CKBarField kbarField;
		kbarField.InstrumentID = "ru1701";
		kbarField.ExchangeID = "SHFE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 14100;
		kbarField.HighPrice = 14300;
		kbarField.LowPrice = 14000;
		kbarField.ClosePrice = 14200;
		kbarField.BidPrice = 141200;
		kbarField.BidVolume = 10;
		kbarField.AskPrice = 14205;
		kbarField.AskVolume = 10;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		kbarField.InstrumentID = "m1701";
		kbarField.ExchangeID = "DCE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 2968;
		kbarField.HighPrice = 2977;
		kbarField.LowPrice = 2965;
		kbarField.ClosePrice = 2970;
		kbarField.BidPrice = 2969;
		kbarField.BidVolume = 100;
		kbarField.AskPrice = 2971;
		kbarField.AskVolume = 100;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		m_pSimulationTrader->LimitOrder(1, "ru1701", THOST_D_Long, THOST_OF_Open, 14000, 1);
		m_pSimulationTrader->LimitOrder(2, "m1701", THOST_D_Short, THOST_OF_Open, 2970, 10);
		m_pSimulationTrader->LimitOrder(3, "m1701", THOST_D_Long, THOST_OF_Close, 2963, 7);
		m_pSimulationTrader->LimitOrder(4, "ru1701", THOST_D_Short, THOST_OF_Close, 13985, 1);
		const double eps = 0.00001;
		CThostTradingAccountField exceptionTradingAccount;
		exceptionTradingAccount.PreBalance = 1000000;
		exceptionTradingAccount.Balance = 1000301.205753;
		exceptionTradingAccount.Available = 994064.205753;
		exceptionTradingAccount.PositionProfit = 0;
		exceptionTradingAccount.CloseProfit = 340;
		exceptionTradingAccount.Commission = 38.794247;
		exceptionTradingAccount.CurrMargin = 6237;

		CThostTradingAccountField tradingAccount;
		m_pSimulationTrader->GetTradingAccount(tradingAccount);
		if (fabs(tradingAccount.PreBalance - exceptionTradingAccount.PreBalance) < eps &&
			fabs(tradingAccount.Balance - exceptionTradingAccount.Balance) < eps &&
			fabs(tradingAccount.Available - exceptionTradingAccount.Available) < eps &&
			fabs(tradingAccount.PositionProfit - exceptionTradingAccount.PositionProfit) < eps &&
			fabs(tradingAccount.CloseProfit - exceptionTradingAccount.CloseProfit) < eps &&
			fabs(tradingAccount.Commission - exceptionTradingAccount.Commission) < eps &&
			fabs(tradingAccount.CurrMargin - exceptionTradingAccount.CurrMargin) < eps &&
			0 == m_pSimulationTrader->GetPositionAmount("ru1701", THOST_D_Long, THOST_PSD_ALL) &&
			3 == m_pSimulationTrader->GetPositionAmount("m1701", THOST_D_Short, THOST_PSD_Today)
			)
		{
			printf ("%s:testOpenClose:0\n", m_testName.c_str());
			return 0;
		}
		 		printf ("动态权益:%f\n可用:%f\n持仓盈亏:%f\n平仓盈亏:%f\n手续费:%f\n占用保证金:%f\n",
					tradingAccount.Balance, 
					tradingAccount.Available,
					tradingAccount.PositionProfit,
					tradingAccount.CloseProfit,
					tradingAccount.Commission,
					tradingAccount.CurrMargin);
		printf ("%s:testOpenClose:1\n", m_testName.c_str());
		return 1;
	}

	int testOpenCloseSettlement()
	{
		m_pSimulationTrader->Clear();
		m_pSimulationTrader->SetInitializedBalance(1000000);
		SetInstrumentInfoMarginRateCommissionRate();
		CKBarField kbarField;
		kbarField.InstrumentID = "ru1701";
		kbarField.ExchangeID = "SHFE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 14100;
		kbarField.HighPrice = 14300;
		kbarField.LowPrice = 14000;
		kbarField.ClosePrice = 14200;
		kbarField.BidPrice = 14120;
		kbarField.BidVolume = 10;
		kbarField.AskPrice = 14205;
		kbarField.AskVolume = 10;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		kbarField.InstrumentID = "m1701";
		kbarField.ExchangeID = "DCE";
		kbarField.Date = 20161027;
		kbarField.BeginTime = 135800;
		kbarField.EndTime = 135900;
		kbarField.OpenPrice = 2968;
		kbarField.HighPrice = 2977;
		kbarField.LowPrice = 2965;
		kbarField.ClosePrice = 2970;
		kbarField.BidPrice = 2969;
		kbarField.BidVolume = 100;
		kbarField.AskPrice = 2971;
		kbarField.AskVolume = 100;
		kbarField.TradingDate = 20161027;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		m_pSimulationTrader->LimitOrder(1, "ru1701", THOST_D_Long, THOST_OF_Open, 14000, 1);

		m_pSimulationTrader->LimitOrder(2, "m1701", THOST_D_Short, THOST_OF_Open, 2970, 10);

		kbarField.InstrumentID = "ru1701";
		kbarField.Date = 20161027;
		kbarField.TradingDate = 20161027;
		kbarField.ClosePrice = 14100;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		kbarField.InstrumentID = "m1701";
		kbarField.Date = 20161027;
		kbarField.TradingDate = 20161027;
		kbarField.ClosePrice = 2965;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);
		
		m_pSimulationTrader->BeginTradingDaySettle(20161027);
		m_pSimulationTrader->SetTradingDay(20161028);

		m_pSimulationTrader->LimitOrder(3, "ru1701", THOST_D_Long, THOST_OF_Open, 14200, 2);
		m_pSimulationTrader->LimitOrder(4, "m1701", THOST_D_Long, THOST_OF_Close, 2980, 3);

		kbarField.InstrumentID = "ru1701";
		kbarField.Date = 201610278;
		kbarField.TradingDate = 201610278;
		kbarField.ClosePrice = 14300;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		kbarField.InstrumentID = "m1701";
		kbarField.Date = 201610278;
		kbarField.TradingDate = 201610278;
		kbarField.ClosePrice = 2995;
		m_pSimulationTrader->UpdateKBarSnapshot(kbarField.InstrumentID, kbarField);

		const double eps = 0.00001;
		CThostTradingAccountField exceptionTradingAccount;
		exceptionTradingAccount.PreBalance = 1001478.2981;
		exceptionTradingAccount.Balance = 1002910.73512;
		exceptionTradingAccount.Available = 937382.23512;
		exceptionTradingAccount.PositionProfit = 1900;
		exceptionTradingAccount.CloseProfit = -450;
		exceptionTradingAccount.Commission = 17.56298;
		exceptionTradingAccount.CurrMargin = 65528.5;

		CThostTradingAccountField tradingAccount;
		m_pSimulationTrader->GetTradingAccount(tradingAccount);
		if (fabs(tradingAccount.PreBalance - exceptionTradingAccount.PreBalance) < eps &&
			fabs(tradingAccount.Balance - exceptionTradingAccount.Balance) < eps &&
			fabs(tradingAccount.Available - exceptionTradingAccount.Available) < eps &&
			fabs(tradingAccount.PositionProfit - exceptionTradingAccount.PositionProfit) < eps &&
			fabs(tradingAccount.CloseProfit - exceptionTradingAccount.CloseProfit) < eps &&
			fabs(tradingAccount.Commission - exceptionTradingAccount.Commission) < eps &&
			fabs(tradingAccount.CurrMargin - exceptionTradingAccount.CurrMargin) < eps &&
			1 == m_pSimulationTrader->GetPositionAmount("ru1701", THOST_D_Long, THOST_PSD_History) &&
			2 == m_pSimulationTrader->GetPositionAmount("ru1701", THOST_D_Long, THOST_PSD_Today) &&
			7 == m_pSimulationTrader->GetPositionAmount("m1701", THOST_D_Short, THOST_PSD_History)
			)
		{
			printf ("%s:testOpenCloseSettlement:0\n", m_testName.c_str());
			return 0;
		}
		// 		printf ("动态权益:%f\n可用:%f\n持仓盈亏:%f\n平仓盈亏:%f\n手续费:%f\n占用保证金:%f\n",
		// 			tradingAccount.Balance, 
		// 			tradingAccount.Available,
		// 			tradingAccount.PositionProfit,
		// 			tradingAccount.CloseProfit,
		// 			tradingAccount.Commission,
		// 			tradingAccount.CurrMargin);
		printf ("%s:testOpenCloseSettlement:1\n", m_testName.c_str());
		return 1;
	}
	void OnRtnOrderReport(CThostOrderReportField *pOrderReport)
	{
	}

	void SetInstrumentInfoMarginRateCommissionRate()
	{
		CThostInstrumentField instrumentInfo;
		///设置ru的合约信息
		instrumentInfo.InstrumentID = "ru1701";
		instrumentInfo.ProductID = "ru";
		instrumentInfo.VolumeMultiple = 10;
		instrumentInfo.ExchangeID = "SHFE";
		instrumentInfo.PriceTick = 5;
		m_pSimulationTrader->SetInstrumentInfo(instrumentInfo);
		///设置m的合约信息
		instrumentInfo.InstrumentID = "m1701";
		instrumentInfo.ProductID = "m";
		instrumentInfo.VolumeMultiple = 10;
		instrumentInfo.PriceTick = 1;
		m_pSimulationTrader->SetInstrumentInfo(instrumentInfo);

		CThostInstrumentMarginRateField instrumentMarginRate;
		///设置ru的保证金信息
		instrumentMarginRate.InstrumentID = "ru1701";
		instrumentMarginRate.LongMarginRatioByMoney = 0.12;
		instrumentMarginRate.LongMarginRatioByVolume = 0;
		instrumentMarginRate.ShortMarginRatioByMoney = 0.12;
		instrumentMarginRate.ShortMarginRatioByVolume = 0;
		m_pSimulationTrader->SetInstrumentMarginRate(instrumentMarginRate);
		///设置m的保证金信息
		instrumentMarginRate.InstrumentID = "m1701";
		instrumentMarginRate.LongMarginRatioByMoney = 0.07;
		instrumentMarginRate.LongMarginRatioByVolume = 0;
		instrumentMarginRate.ShortMarginRatioByMoney = 0.07;
		instrumentMarginRate.ShortMarginRatioByVolume = 0;
		m_pSimulationTrader->SetInstrumentMarginRate(instrumentMarginRate);


		CThostInstrumentCommissionRateField instrumentCommissionRate;
		///设置ru的手续费信息
		instrumentCommissionRate.InstrumentID = "ru1701";
		instrumentCommissionRate.OpenRatioByMoney = 0.0000456;
		instrumentCommissionRate.OpenRatioByVolume = 0.01;
		instrumentCommissionRate.CloseRatioByMoney = 0.0000456;
		instrumentCommissionRate.CloseRatioByVolume = 0.01;
		instrumentCommissionRate.CloseTodayRatioByMoney = 0.0000456;
		instrumentCommissionRate.CloseTodayRatioByVolume = 0;
		m_pSimulationTrader->SetInstrumentCommissionRate(instrumentCommissionRate);
		///设置ru的手续费信息
		instrumentCommissionRate.InstrumentID = "m1701";
		instrumentCommissionRate.OpenRatioByMoney = 0.0000007;
		instrumentCommissionRate.OpenRatioByVolume = 1.51;
		instrumentCommissionRate.CloseRatioByMoney = 0.0000007;
		instrumentCommissionRate.CloseRatioByVolume = 1.51;
		instrumentCommissionRate.CloseTodayRatioByMoney = 0.0000007;
		instrumentCommissionRate.CloseTodayRatioByVolume = 1.51;
		m_pSimulationTrader->SetInstrumentCommissionRate(instrumentCommissionRate);
	}

private:
	string						m_testName;
	CSimulationTraderHandler	*m_pSimulationTrader;
};

#endif