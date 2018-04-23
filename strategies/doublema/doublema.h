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
		///���ȶ�ȡ�����ò��Ե���ز���
		m_pThostApi = CThostApi::CreateThostApi(THOST_API_MODE_BACKTEST);
		m_pThostApi->RegisterThostSpi(this);
		m_pThostApi->RegisterLogServer("", true);

		///�ز������ʱ�̶�
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
		///�򵥸��³ֲ�
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

		///��һ��ShortMA
		if (m_dPreShortMAValue < 0 && ShortMA > 0)
		{
			m_dPreShortMAValue = ShortMA;
			return ;
		}

		///��һ��LongMA
		if (m_dPreLongMAValue < 0 && LongMA > 0)
		{
			m_dPreLongMAValue = LongMA;
			return ;
		}

		if (ShortMA > 0 && LongMA > 0 && m_dPreShortMAValue > 0 && m_dPreLongMAValue > 0)
		{
			///�̾����ϴ������ߣ���
			if (ShortMA > LongMA && m_dPreShortMAValue <= m_dPreLongMAValue)
			{
				///��ƽ��
				if (m_nShortPosition > 0)
				{
					m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Long, THOST_OF_Close, pKBar->AskPrice, m_nShortPosition);
				}
				///�ٿ���
				m_pThostApi->LimitOrder(++m_nRequestID, "ru1701", THOST_D_Long, THOST_OF_Open, pKBar->AskPrice, 1);
				m_pThostApi->Log(LOG_LEVEL_INFO, "LONG:%s %d %d %f", pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->EndTime, pKBar->ClosePrice);
			}

			///�̾����´������ߣ���
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
		///���ó�ʼ�ʽ�
		jsonParams["Account"]["InitialBalance"] = 1000000;
		///���ûز���Ϣ
		jsonParams["Data"]["StartDate"] = 20161020;
		jsonParams["Data"]["EndDate"] = 20161103;
		jsonParams["Data"]["PeriodType"] = THOST_KBAR_PERIOD_TYPE_MINUTE;
		jsonParams["Data"]["PeriodValue"] = 5;
		///�������ݿ���Ϣ
		jsonParams["Sql"]["Host"] = "172.31.11.215";
		jsonParams["Sql"]["Port"] = 3306;
		jsonParams["Sql"]["User"] = "kbar";
		jsonParams["Sql"]["Password"] = "123456";
		jsonParams["Sql"]["DB"] = "kbar_db";
		///���ú�Լ�б�
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
	///����id
	int						m_nRequestID;
	///�����߸���
	int						m_nShort;
	///����߸���
	int						m_nLong;
	///ǰһ��������ֵ
	double					m_dPreShortMAValue;
	///ǰһ�������ֵ
	double					m_dPreLongMAValue;
	///kbar��close����
	vector<double>			m_vecClosePrice;
	///��ͷ�ֲ�
	int						m_nLongPosition;
	///��ͷ�ֲ�	
	int						m_nShortPosition;
};

#endif
