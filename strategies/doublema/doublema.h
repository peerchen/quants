#ifndef INC_DOUBLEMA_H
#define INC_DOUBLEMA_H

#include "thostapi.h"
#include <stdio.h>
#include <json/json.h>

class CDoubleMAStrategy : public CThostSpi 
{
public:
	CDoubleMAStrategy()
	{
		m_nShort = 5;
		m_nLong = 10;
		m_dPreShortMAValue = -1;
		m_dPreLongMAValue = -1;
		m_nRequestID = 0;
		m_nShortPosition = 0;
		m_nLongPosition = 0;
	}
	~CDoubleMAStrategy(){}

public:
	int Start()
	{
		///首先读取并设置策略的相关参数
		m_pThostApi = CThostApi::CreateThostApi(THOST_API_MODE_BACKTEST);
		m_pThostApi->RegisterThostSpi(this);
		m_pThostApi->RegisterLogServer("", true);

		///回测参数暂时固定
		string strJsonParams = GetBackTestParams();
		m_pThostApi->Start(strJsonParams);
		printf ("111111111111111111111111111111\n");
		return 0;
	}

public:
	virtual void OnRtnKBar(CKBarField *pKBar)
	{
		DoubleSMA(pKBar);
	}

	virtual void OnRtnOrderReport(CThostOrderReportField *pOrderReport)
	{
		///简单更新持仓
		if (pOrderReport->OrderStatus == THOST_OST_Filled || pOrderReport->OrderStatus == THOST_OST_Complete)
		{
			if (pOrderReport->CombOffsetFlag[0] == THOST_OF_Open)
			{
				if (pOrderReport->Direction == THOST_D_Long)
				{
					m_nLongPosition += pOrderReport->LastFilledVolume;
				}
				else
				{
					m_nShortPosition += pOrderReport->LastFilledVolume;
				}
			}
			else
			{
				if (pOrderReport->Direction == THOST_D_Long)
				{
					m_nShortPosition -= pOrderReport->LastFilledVolume;
				}
				else
				{
					m_nLongPosition -= pOrderReport->LastFilledVolume;
				}
			}
		}
	}

private:
	void DoubleSMA(CKBarField *pKBar)
	{
		m_vecClosePrice.push_back(pKBar->ClosePrice);

		double ShortMA = CalcSimpleMA(m_vecClosePrice, m_nShort);
		double LongMA = CalcSimpleMA(m_vecClosePrice, m_nLong);

		///第一个ShortMA
		if (m_dPreShortMAValue < 0 && ShortMA > 0)
		{
			m_dPreShortMAValue = ShortMA;
			return ;
		}

		///第一个LongMA
		if (m_dPreLongMAValue < 0 && LongMA > 0)
		{
			m_dPreLongMAValue = LongMA;
			return ;
		}

		if (ShortMA > 0 && LongMA > 0 && m_dPreShortMAValue > 0 && m_dPreLongMAValue > 0)
		{
			///短均线上穿长均线，多
			if (ShortMA > LongMA && m_dPreShortMAValue <= m_dPreLongMAValue)
			{
				///先平空
				if (m_nShortPosition > 0)
				{
					m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Long, THOST_OF_Close, pKBar->AskPrice, m_nShortPosition);
				}
				///再开多
				m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Long, THOST_OF_Open, pKBar->AskPrice, 1);
				m_pThostApi->Log(LOG_LEVEL_INFO, "LONG:%s %d %d %f", pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->EndTime, pKBar->ClosePrice);
			}

			///短均线下穿长均线，空
			if (ShortMA < LongMA && m_dPreShortMAValue >= m_dPreLongMAValue)
			{
				if (m_nLongPosition > 0)
				{
					m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Short, THOST_OF_Close, pKBar->BidPrice, m_nLongPosition);
				}
				m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Short, THOST_OF_Open, pKBar->BidPrice, 1);
				m_pThostApi->Log(LOG_LEVEL_INFO, "SHORT:%s %d %d %f", pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->EndTime, pKBar->ClosePrice);
			}
		}

		m_dPreShortMAValue = ShortMA;
		m_dPreLongMAValue = LongMA;

		if (m_vecClosePrice.size() > m_nLong)
		{
			m_vecClosePrice.erase(m_vecClosePrice.begin());	
		}
	}

	double CalcSimpleMA(vector<double> &vecClosePrice, int MAIndex)
	{
		if (vecClosePrice.size() < MAIndex)
		{
			return -1;
		}
		double sum = 0;
		for (vector<double>::iterator iter = vecClosePrice.end() - MAIndex; iter != vecClosePrice.end(); ++iter)
		{
			sum += *iter;
		}
		return sum / MAIndex;
	}	

	string GetBackTestParams()
	{
		Json::Value jsonParams;
		///设置初始资金
		jsonParams["Account"]["InitialBalance"] = 1000000;
		///设置回测信息
		jsonParams["Data"]["StartDate"] = 20161020;
		jsonParams["Data"]["EndDate"] = 20161103;
		jsonParams["Data"]["PeriodType"] = THOST_KBAR_PERIOD_TYPE_MINUTE;
		jsonParams["Data"]["PeriodValue"] = 5;
		///设置数据库信息
		jsonParams["Sql"]["Host"] = "172.31.11.215";
		jsonParams["Sql"]["Port"] = 3306;
		jsonParams["Sql"]["User"] = "kbar";
		jsonParams["Sql"]["Password"] = "123456";
		jsonParams["Sql"]["DB"] = "kbar_db";
		///设置合约列表
		Json::Value jsonInstrumentList;
		Json::Value jsonRu;
		jsonRu["InstrumentID"] = "ru1701";
		jsonRu["ExchangeID"] = "SHFE";
		jsonRu["VolumeMultiple"] = 10;
		jsonRu["Slippage"] = 5;
		jsonRu["LongMarginRatioByMoney"] = 0.19;
		jsonRu["LongMarginRatioByVolume"] = 0;
		jsonRu["ShortMarginRatioByMoney"] = 0.19;
		jsonRu["ShortMarginRatioByVolume"] = 0;
		jsonRu["OpenRatioByMoney"] = 0;
		jsonRu["OpenRatioByVolume"] = 6;
		jsonRu["CloseRatioByMoney"] = 0;
		jsonRu["CloseRatioByVolume"] = 6;
		jsonRu["CloseTodayRatioByMoney"] = 0;
		jsonRu["CloseTodayRatioByVolume"] = 6;
		jsonInstrumentList.append(jsonRu);
		jsonParams["InstrumentList"] = jsonInstrumentList;

		Json::FastWriter writer;
		string strJsonParams = writer.write(jsonParams);
		return strJsonParams;
	}
private:
	CThostApi				*m_pThostApi;
	///请求id
	int						m_nRequestID;
	///慢均线个数
	int						m_nShort;
	///快均线个数
	int						m_nLong;
	///前一个慢均线值
	double					m_dPreShortMAValue;
	///前一个快均线值
	double					m_dPreLongMAValue;
	///kbar的close队列
	vector<double>			m_vecClosePrice;
	///多头持仓
	int						m_nLongPosition;
	///空头持仓	
	int						m_nShortPosition;
};

#endif
