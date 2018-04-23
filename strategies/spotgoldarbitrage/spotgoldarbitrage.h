#ifndef INC_SPOTGOLDARBITRAGE_H
#define INC_SPOTGOLDARBITRAGE_H

#include "thostapi.h"
#include <math.h>
#include <json/json.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 

class CSpotGoldArbitrage : public CThostSpi{
public:
	CSpotGoldArbitrage() 
	{
		m_nSampleCount = 60;
		m_dStdCoeff = 1;
		m_nTDLongPosition = 0;
		m_nTDShortPosition = 0;
		m_nAuLongPosition = 0;
		m_nAuShortPosition = 0;
		m_nRequestID = 0;
		m_nTDSlippage = 1;
		m_nAuSlippage = 1;
		m_nEnableLogOrderDetail = 0;
		m_Balance = 1000000;
		m_nOrderVolume = 1;
	}

	~CSpotGoldArbitrage() {}

public:
	void Start()
	{
		///首先读取并设置策略的相关参数
		m_pThostApi = CThostApi::CreateThostApi(THOST_API_MODE_BACKTEST);
		m_pThostApi->RegisterThostSpi(this);
		m_pThostApi->RegisterLogServer("", true);
		
		boost::property_tree::ptree ptIni;	
		boost::property_tree::ini_parser::read_ini("./spotgoldarbitrage.ini", ptIni);
		
		m_nSampleCount = ptIni.get<int>("strategy.SampleCount", 60);
		m_dStdCoeff = ptIni.get<double>("strategy.StdCoeff", 1);
		
		m_nTDSlippage = ptIni.get<double>("strategy.Slippage", 1);
		m_nAuSlippage = ptIni.get<double>("strategy.Slippage", 1);

		m_nEnableLogOrderDetail = ptIni.get<int>("strategy.EnableLogOrderDetail", 0);
		m_Balance = ptIni.get<double>("strategy.Balance", 1000000);
		m_nOrderVolume = ptIni.get<int>("strategy.OrderVolume", 1);

		m_nBeginDate = ptIni.get<int>("strategy.begindate", 20160830);
		m_nEndDate = ptIni.get<int>("strategy.enddate", 20160830);
		///回测参数暂时固定
		string strJsonParams = GetBackTestParams();
		m_pThostApi->Start(strJsonParams);
	}

	void Wait()
	{
		boost::mutex						m_exitMutex;
		boost::condition_variable			m_exitCond;
		boost::mutex::scoped_lock			lock(m_exitMutex);
		m_exitCond.wait(lock);
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
					if (pOrderReport->InstrumentID == "Au(T+D)")
					{
						m_nTDLongPosition += pOrderReport->LastFilledVolume;
					}
					else
					{
						m_nAuLongPosition += pOrderReport->LastFilledVolume;
					}
				}
				else
				{
					if (pOrderReport->InstrumentID == "Au(T+D)")
					{
						m_nTDShortPosition += pOrderReport->LastFilledVolume;
					}
					else
					{
						m_nAuShortPosition += pOrderReport->LastFilledVolume;
					}
				}
			}
			else
			{
				if (pOrderReport->Direction == THOST_D_Long)
				{
					if (pOrderReport->InstrumentID == "Au(T+D)")
					{
						m_nTDShortPosition -= pOrderReport->LastFilledVolume;
					}
					else
					{
						m_nAuShortPosition -= pOrderReport->LastFilledVolume;
					}
				}
				else
				{
					if (pOrderReport->InstrumentID == "Au(T+D)")
					{
						m_nTDLongPosition -= pOrderReport->LastFilledVolume;
					}
					else
					{
						m_nAuLongPosition -= pOrderReport->LastFilledVolume;
					}
				}
			}
		}
	}

	virtual void OnRtnKBar(CKBarField *pKBar)
	{
		//m_pThostApi->Log (LOG_LEVEL_INFO, "%s %d %d %d %f", pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->BeginTime, pKBar->EndTime, pKBar->ClosePrice);
		SpotGoldArbitrage(pKBar);
	}	

private:
	void SpotGoldArbitrage(CKBarField *pKBar)
	{
		vector<CKBarField>::iterator iter;
		if (pKBar->InstrumentID == "Au(T+D)")
		{
			if ((pKBar->BeginTime >= 210000 && pKBar->BeginTime<= 240000) ||
				(pKBar->BeginTime >= 0 && pKBar->BeginTime <= 150000))
			{
				m_TDLastKBar = *pKBar;
				for (iter = m_vecAuKBar.begin(); iter != m_vecAuKBar.end(); iter++)
				{
					///对齐信号
					if ((*iter).Date == pKBar->Date && (*iter).BeginTime == pKBar->BeginTime)
					{
						double LastSpread = pKBar->ClosePrice - (*iter).ClosePrice;
						///判断信号，传入价差列表和最新价差
						///MakeSignal
						MakeSignal(pKBar->ClosePrice,(*iter).ClosePrice, pKBar);
						m_vecSpread.insert(m_vecSpread.begin(), pKBar->ClosePrice - (*iter).ClosePrice);
						if (m_vecSpread.size() > m_nSampleCount)
						{
							m_vecSpread.pop_back();
						}
						break;
					}

					///当前kbar时间大于另一半
					if (((*iter).Date == pKBar->Date && (*iter).BeginTime < pKBar->BeginTime) ||
						((*iter).Date < pKBar->Date))
					{
						break;
					}

					if (iter == m_vecAuKBar.begin())
					{
						break;
					}
				}
				m_vecTDKBar.insert(m_vecTDKBar.begin(), *pKBar);
				if (m_vecTDKBar.size() > 10)
				{
					m_vecTDKBar.pop_back();
				}
			}
		}

		if (pKBar->InstrumentID == "au1612")
		{
			m_AuLastKBar = *pKBar;
			for (iter = m_vecTDKBar.begin(); iter != m_vecTDKBar.end(); iter++)
			{
				///对齐信号
				if ((*iter).Date == pKBar->Date && (*iter).BeginTime == pKBar->BeginTime)
				{
					///判断信号，传入价差列表和最新价差
					///MakeSignal
					MakeSignal((*iter).ClosePrice, pKBar->ClosePrice, pKBar);
					m_vecSpread.insert(m_vecSpread.begin(), (*iter).ClosePrice - pKBar->ClosePrice);
					if (m_vecSpread.size() > m_nSampleCount)
					{
						m_vecSpread.pop_back();
					}
					break;
				}

				///当前kbar时间大于另一半
				if (((*iter).Date == pKBar->Date && (*iter).BeginTime < pKBar->BeginTime) ||
					((*iter).Date < pKBar->Date))
				{
					break;
				}

				if (iter == m_vecTDKBar.begin())
				{
					break;
				}
			}
			m_vecAuKBar.insert(m_vecAuKBar.begin(), *pKBar);
			if (m_vecAuKBar.size() > 10)
			{
				m_vecAuKBar.pop_back();
			}
		}	
	}

	void MakeSignal(double TDLastPrice, double AULastPrice, CKBarField *pKBar)
	{
		if (m_vecSpread.size() >= m_nSampleCount)
		{
			double sum = 0;
			double avg, stdev;
			for (int i = 0; i < m_nSampleCount; i++)
			{
				sum += m_vecSpread[i];
			}
			avg = sum / m_nSampleCount;
			sum = 0;
			for (int i = 0; i < m_nSampleCount; i++)
			{
				sum += (m_vecSpread[i] - avg) * (m_vecSpread[i] - avg);
			}
			stdev = sum / m_nSampleCount;
			stdev = sqrt(stdev);

			if ((TDLastPrice + m_nTDSlippage * 0.01) - (AULastPrice - m_nAuSlippage * 0.05) < avg)
			{
				///平多TD空AU
				if (m_nTDShortPosition)
				{
					if (m_nEnableLogOrderDetail)
					{
						m_pThostApi->Log(LOG_LEVEL_INFO, "平多TD,Au(T+D),%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_TDLastKBar.ClosePrice, avg, stdev, (TDLastPrice + m_nTDSlippage * 0.01) - (AULastPrice - m_nAuSlippage * 0.05), m_nTDShortPosition);
					}
					m_pThostApi->LimitOrder(++m_nRequestID, "Au(T+D)", THOST_D_Long, THOST_OF_Close, m_TDLastKBar.ClosePrice+0.01*m_nTDSlippage, m_nTDShortPosition);
				}

				if (m_nAuLongPosition)
				{
					if (m_nEnableLogOrderDetail)
					{
						m_pThostApi->Log(LOG_LEVEL_INFO, "平空AU,au1612,%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_AuLastKBar.ClosePrice, avg, stdev, (TDLastPrice + m_nTDSlippage * 0.01) - (AULastPrice - m_nAuSlippage * 0.05), m_nAuLongPosition);
					}
					m_pThostApi->LimitOrder(++m_nRequestID, "au1612", THOST_D_Short, THOST_OF_Close, m_AuLastKBar.ClosePrice-0.05*m_nAuSlippage, m_nAuLongPosition, m_nAuLongPosition);
				}
			}

			if ((TDLastPrice - m_nTDSlippage * 0.01) - (AULastPrice + m_nAuSlippage * 0.05) > avg)
			{
				///平空TD多AU
				if (m_nTDLongPosition)
				{
					if (m_nEnableLogOrderDetail)
					{
						m_pThostApi->Log(LOG_LEVEL_INFO, "平空TD,Au(T+D),%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_TDLastKBar.ClosePrice, avg, stdev, (TDLastPrice - m_nTDSlippage * 0.01) - (AULastPrice + m_nAuSlippage * 0.05),m_nTDLongPosition);
					}
					m_pThostApi->LimitOrder(++m_nRequestID, "Au(T+D)", THOST_D_Short, THOST_OF_Close, m_TDLastKBar.ClosePrice-0.01*m_nTDSlippage, m_nTDLongPosition);
				}

				if (m_nAuShortPosition)
				{
					if (m_nEnableLogOrderDetail)
					{
						m_pThostApi->Log(LOG_LEVEL_INFO, "平多AU,au1612,%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_AuLastKBar.ClosePrice, avg, stdev, (TDLastPrice - m_nTDSlippage * 0.01) - (AULastPrice + m_nAuSlippage * 0.05),m_nAuShortPosition);
					}
					m_pThostApi->LimitOrder(++m_nRequestID, "au1612", THOST_D_Long, THOST_OF_Close, m_AuLastKBar.ClosePrice+0.05*m_nAuSlippage, m_nAuShortPosition);
				}
			}

			if ((TDLastPrice - 0.01 * m_nTDSlippage) - (AULastPrice + m_nAuSlippage * 0.05) >= avg + m_dStdCoeff * stdev)
			{
				///开空TD多AU
				if (m_nEnableLogOrderDetail)
				{
					m_pThostApi->Log(LOG_LEVEL_INFO, "开空TD,Au(T+D),%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_TDLastKBar.ClosePrice, avg, stdev, (TDLastPrice - 0.01 * m_nTDSlippage) - (AULastPrice + m_nAuSlippage * 0.05),m_nOrderVolume);
					m_pThostApi->Log(LOG_LEVEL_INFO, "开多AU,au1612,%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_AuLastKBar.ClosePrice, avg, stdev, (TDLastPrice - 0.01 * m_nTDSlippage) - (AULastPrice + m_nAuSlippage * 0.05),m_nOrderVolume);
				}
				m_pThostApi->LimitOrder(++m_nRequestID, "Au(T+D)", THOST_D_Short, THOST_OF_Open, m_TDLastKBar.ClosePrice-0.01*m_nTDSlippage, m_nOrderVolume);
				m_pThostApi->LimitOrder(++m_nRequestID, "au1612", THOST_D_Long, THOST_OF_Open, m_AuLastKBar.ClosePrice+0.05*m_nAuSlippage, m_nOrderVolume);
			}

			if ((TDLastPrice + 0.01 * m_nTDSlippage) - (AULastPrice - 0.05 * m_nAuSlippage) <= avg - m_dStdCoeff * stdev)
			{
				///开多TD空AU
				if (m_nEnableLogOrderDetail)
				{
					m_pThostApi->Log(LOG_LEVEL_INFO, "开多TD,Au(T+D),%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_TDLastKBar.ClosePrice, avg, stdev, (TDLastPrice + 0.01 * m_nTDSlippage) - (AULastPrice - 0.05 * m_nAuSlippage),m_nOrderVolume);
					m_pThostApi->Log(LOG_LEVEL_INFO, "开空AU,au1612,%d,%d,%f,%f,%f,%f,%d", pKBar->Date, pKBar->EndTime, m_AuLastKBar.ClosePrice, avg, stdev, (TDLastPrice + 0.01 * m_nTDSlippage) - (AULastPrice - 0.05 * m_nAuSlippage),m_nOrderVolume);
				}
				m_pThostApi->LimitOrder(++m_nRequestID, "Au(T+D)", THOST_D_Long, THOST_OF_Open, m_TDLastKBar.ClosePrice+0.01*m_nTDSlippage, m_nOrderVolume);
				m_pThostApi->LimitOrder(++m_nRequestID, "au1612", THOST_D_Short, THOST_OF_Open, m_AuLastKBar.ClosePrice-0.05*m_nAuSlippage, m_nOrderVolume);
			}
		}
	}

	string GetBackTestParams()
	{
		Json::Value jsonParams;
		///设置初始资金
		jsonParams["Account"]["InitialBalance"] = m_Balance;
		///设置回测信息
		jsonParams["Data"]["StartDate"] = m_nBeginDate;
		jsonParams["Data"]["EndDate"] = m_nEndDate;
		jsonParams["Data"]["PeriodType"] = THOST_KBAR_PERIOD_TYPE_MINUTE;
		jsonParams["Data"]["PeriodValue"] = 1;
		///设置数据库信息
		jsonParams["Sql"]["Host"] = "127.0.0.1";
		jsonParams["Sql"]["Port"] = 3306;
		jsonParams["Sql"]["User"] = "kbar";
		jsonParams["Sql"]["Password"] = "123456";
		jsonParams["Sql"]["DB"] = "kbar_db";
		///设置合约列表
		Json::Value jsonInstrumentList;
		Json::Value jsonaAuSHFE;
		jsonaAuSHFE["InstrumentID"] = "au1612";
		jsonaAuSHFE["ExchangeID"] = "SHFE";
		jsonaAuSHFE["VolumeMultiple"] = 1000;
		jsonaAuSHFE["Slippage"] = 0.05;
		jsonaAuSHFE["LongMarginRatioByMoney"] = 0.06;
		jsonaAuSHFE["LongMarginRatioByVolume"] = 0;
		jsonaAuSHFE["ShortMarginRatioByMoney"] = 0.06;
		jsonaAuSHFE["ShortMarginRatioByVolume"] = 0;
		jsonaAuSHFE["OpenRatioByMoney"] = 0;
		jsonaAuSHFE["OpenRatioByVolume"] = 12;
		jsonaAuSHFE["CloseRatioByMoney"] = 0;
		jsonaAuSHFE["CloseRatioByVolume"] = 12;
		jsonaAuSHFE["CloseTodayRatioByMoney"] = 0;
		jsonaAuSHFE["CloseTodayRatioByVolume"] = 12;
		jsonInstrumentList.append(jsonaAuSHFE);
		
		Json::Value jsonAuSGE;
		jsonAuSGE["InstrumentID"] = "Au(T+D)";
		jsonAuSGE["ExchangeID"] = "SGE";
		jsonAuSGE["VolumeMultiple"] = 1000;
		jsonAuSGE["Slippage"] = 0.01;
		jsonAuSGE["LongMarginRatioByMoney"] = 0.10;
		jsonAuSGE["LongMarginRatioByVolume"] = 0;
		jsonAuSGE["ShortMarginRatioByMoney"] = 0.10;
		jsonAuSGE["ShortMarginRatioByVolume"] = 0;
		jsonAuSGE["OpenRatioByMoney"] = 0;
		jsonAuSGE["OpenRatioByVolume"] = 12;
		jsonAuSGE["CloseRatioByMoney"] = 0;
		jsonAuSGE["CloseRatioByVolume"] = 12;
		jsonAuSGE["CloseTodayRatioByMoney"] = 0;
		jsonAuSGE["CloseTodayRatioByVolume"] = 12;
		jsonInstrumentList.append(jsonAuSGE);

		jsonParams["InstrumentList"] = jsonInstrumentList;
		Json::FastWriter writer;
		string strJsonParams = writer.write(jsonParams);
		return strJsonParams;
	}

private:
	CThostApi						*m_pThostApi;
	int								m_nSampleCount;
	double							m_dStdCoeff;
	vector<CKBarField>				m_vecTDKBar;
	vector<CKBarField>				m_vecAuKBar;
	vector<double>					m_vecSpread;

	CKBarField						m_TDLastKBar;
	CKBarField						m_AuLastKBar;

	double								m_nTDSlippage;
	double								m_nAuSlippage;
	
	double							m_Balance;
	int								m_nOrderVolume;
	int								m_nEnableLogOrderDetail;
	
	int								m_nBeginDate;
	int								m_nEndDate;

	int								m_nRequestID;
	int								m_nTDLongPosition;
	int								m_nTDShortPosition;
	int								m_nAuLongPosition;
	int								m_nAuShortPosition;
};

#endif