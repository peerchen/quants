/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file kbarhandler.h
///@brief 实现kbarhandler类功能
///@history 
///20160818@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
#include "kbarhandler.h"

CKBarHandler::CKBarHandler()
{
	m_pKBarHandlerSpi = NULL;
	m_nTradingDay = 0;
	m_nPrevTradingDay = 0;
	m_pLogHandler = NULL;
	m_nDumpTicks = 0;
}

CKBarHandler::~CKBarHandler()
{
	Release();
}

void CKBarHandler::Release()
{
	for (KBarMap::iterator iter = m_mapKBar.begin(); iter != m_mapKBar.end(); ++iter)
	{
		vector<CKBarField*> &vecKBar = iter->second;
		for (int i = 0; i < vecKBar.size(); ++i)
		{
			if (vecKBar[i] != NULL)
			{
				delete vecKBar[i];
				vecKBar[i] = NULL;
			}
		}
		vecKBar.clear();
	}
	m_mapKBar.clear();

	m_mapInstrumentInfo.clear();
}

void CKBarHandler::RegisterKBarHandlerSpi(CKBarHandlerSpi *pKBarHandlerSpi)
{
	m_pKBarHandlerSpi = pKBarHandlerSpi;
}

int CKBarHandler::RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset)
{
	///60 % periodvalue must == 0
	if (60 % PeriodValue != 0)
	{
		return 1;
	}
	//if set mapinstrument, then check symbol and exchangeid
	if (m_mapInstrumentInfo.size() > 0)
	{
		if (m_mapInstrumentInfo.end() != m_mapInstrumentInfo.find(InstrumentID))
		{
			CThostInstrumentField instrumentInfo = m_mapInstrumentInfo[InstrumentID];
			if (instrumentInfo.ExchangeID != ExchangeID || instrumentInfo.ProductID != Symbol)
			{
				Symbol = instrumentInfo.ProductID;
				ExchangeID = instrumentInfo.ExchangeID;
			}
		}
	}

	///first kbar request index by instrumentid
	if (m_mapKBar.end() == m_mapKBar.find(InstrumentID))
	{
		vector<CKBarField*> vecKBar;
		CKBarField *pKBarField = new CKBarField(InstrumentID, Symbol, ExchangeID, PeriodType, PeriodValue, Offset);
		vecKBar.push_back(pKBarField);
		m_mapKBar[InstrumentID] = vecKBar;
		return 0;
	}

	vector<CKBarField*> &vecKBar = m_mapKBar[InstrumentID];
	int index;
	for (index = 0; index < vecKBar.size(); ++index)
	{
		if (vecKBar[index]->PeriodType == PeriodType && vecKBar[index]->PeriodValue == PeriodValue && vecKBar[index]->Offset == Offset)
		{
			break;
		}
	}
	///add new kbar request event
	if (index >= vecKBar.size())
	{
		CKBarField *pKBarField = new CKBarField(InstrumentID, Symbol, ExchangeID, PeriodType, PeriodValue, Offset);
		vecKBar.push_back(pKBarField);
	}
	return 0;
}

int CKBarHandler::UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset /* = 0 */)
{
	if (m_mapKBar.end() != m_mapKBar.find(InstrumentID))
	{
		vector<CKBarField*> &vecKBar = m_mapKBar[InstrumentID];
		
		for (vector<CKBarField*>::iterator iter = vecKBar.begin(); iter != vecKBar.end(); ++iter)
		{
			CKBarField *pKBarField = *iter;
			if (pKBarField->PeriodType == PeriodType && pKBarField->PeriodValue == PeriodValue && pKBarField->Offset == Offset)
			{
				if (pKBarField != NULL)
				{
					delete pKBarField;
					pKBarField = NULL;
				}
				vecKBar.erase(iter);
				return 0;
			}
		}
	}
	return 1;
}

void CKBarHandler::MarketDataEvent(CThostDepthMarketDataField *pDepthMarketData)
{
	if (m_mapKBar.end() != m_mapKBar.find(pDepthMarketData->InstrumentID))
	{
		vector<CKBarField*> &vecKBar = m_mapKBar[pDepthMarketData->InstrumentID];
		if (vecKBar.size() > 0) 
		{
			string strSymbol = vecKBar[0]->Symbol;
			string strExchangeID = vecKBar[0]->ExchangeID;
			///过滤掉非交易时间段的数据
			if (!THOST_IS_TRADINGTIME(strSymbol, strExchangeID, pDepthMarketData->UpdateTime))
			{
				return ;
			}
			
			///更新tradingday
			if (m_nTradingDay != 0)
			{
				pDepthMarketData->TradingDay = m_nTradingDay;
			}

			///更新actionday
			if (m_nPrevTradingDay != 0)
			{
				if (pDepthMarketData->UpdateTime >= THOST_SECTIONTIME_200000 && pDepthMarketData->UpdateTime < THOST_SECTIONTIME_240000)
				{
					pDepthMarketData->ActionDay = m_nPrevTradingDay;
				}
				else
				{
					if (m_nTradingDay != 0)
					{
						pDepthMarketData->ActionDay = m_nTradingDay;
					}
					else
					{
						pDepthMarketData->ActionDay = pDepthMarketData->TradingDay;
					}
				}
			}
			///过滤掉过期的数据
			CThostDataTimeStamp curTimeStamp;
			curTimeStamp.date = pDepthMarketData->ActionDay;
			curTimeStamp.time = pDepthMarketData->UpdateTime;
			curTimeStamp.millisec = pDepthMarketData->UpdateMillisec;
			if (m_mapLastTimeStamp.end() != m_mapLastTimeStamp.find(pDepthMarketData->InstrumentID))
			{
				CThostDataTimeStamp lastTimeStamp = m_mapLastTimeStamp[pDepthMarketData->InstrumentID];
				if (lastTimeStamp > curTimeStamp)
				{
					if (m_pLogHandler != NULL)
					{
						m_pLogHandler->Error("TICK:Dirty MarketData, %s %d cur:%d:%d last:%d:%d", pDepthMarketData->InstrumentID.c_str(), pDepthMarketData->TradingDay, 
							curTimeStamp.date, curTimeStamp.time, lastTimeStamp.date, lastTimeStamp.time);
					}
					return ;
				}
			}
			m_mapLastTimeStamp[pDepthMarketData->InstrumentID] = curTimeStamp;
			///存储tick
			if (m_pLogHandler != NULL && m_nDumpTicks)
			{
				m_pLogHandler->Debug("DumpTick:%d %s %d %d %f %d %f %f %d %f %d", pDepthMarketData->ActionDay, pDepthMarketData->InstrumentID.c_str(), pDepthMarketData->UpdateTime, pDepthMarketData->UpdateMillisec, 
					pDepthMarketData->LastPrice, pDepthMarketData->Volume, pDepthMarketData->OpenInterest, pDepthMarketData->BidPrice1, pDepthMarketData->BidVolume1, pDepthMarketData->AskPrice1, pDepthMarketData->AskVolume1);
			}
		}
		
		///遍历所有的kbar时间
		for (int i = 0; i < vecKBar.size(); ++i)
		{
			///handler second&minute kbar
			if (vecKBar[i]->PeriodType == THOST_KBAR_PERIOD_TYPE_SECOND || vecKBar[i]->PeriodType == THOST_KBAR_PERIOD_TYPE_MINUTE)
			{
				HandlerKBarEvent(vecKBar[i], pDepthMarketData);
			}
		}
	}
}

void CKBarHandler::TradingClosedEvent(string Symbol, string ExchangeID, int CloseTime)
{
	/// dce/czce last tick datestamp may be 1459XX
	if (CloseTime >= THOST_SECTIONTIME_145900 && CloseTime <= THOST_SECTIONTIME_150000)
	{
		CloseTime = THOST_SECTIONTIME_150000;
	}
	for (KBarMap::iterator iter = m_mapKBar.begin(); iter != m_mapKBar.end(); ++iter)
	{
		vector<CKBarField*> &vecKBar = iter->second;
		for (int i = 0; i < vecKBar.size(); ++i)
		{
			if (vecKBar[i]->Symbol == Symbol)
			{
				vecKBar[i]->BeginTime = FloorTime(vecKBar[i]->PeriodType, vecKBar[i]->PeriodValue, vecKBar[i]->BeginTime);
				vecKBar[i]->EndTime = CloseTime;
				vecKBar[i]->Status = THOST_KBAR_STATUS_CLOSE;
				if (m_pKBarHandlerSpi != NULL)
				{
					CKBarField *pNewKBarField = new CKBarField();
					*pNewKBarField = *vecKBar[i];
					if(pNewKBarField->Date == 0)
					{
						pNewKBarField->Date = m_nTradingDay;
					}
					pNewKBarField->Status = THOST_KBAR_STATUS_TRADINGCLOSE;
					m_pKBarHandlerSpi->OnRtnKBar(pNewKBarField);
					if (pNewKBarField != NULL)
					{
						delete pNewKBarField;
						pNewKBarField = NULL;
					}
					vecKBar[i]->BarIndex++;
				}
			}
		}
	}
}

void CKBarHandler::HandlerKBarEvent(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData)
{
	if (THOST_KBAR_STATUS_NULL == pKBarField->Status)
	{
		UpdateNullKBar(pKBarField, pDepthMarketData);
	}
	else if (THOST_KBAR_STATUS_OPEN == pKBarField->Status)
	{
		UpdateOpenKBar(pKBarField, pDepthMarketData);
	}

	int nFloorBeginTime = FloorTime(pKBarField->PeriodType, pKBarField->PeriodValue, pKBarField->BeginTime);
	int nFloorEndTime = FloorTime(pKBarField->PeriodType, pKBarField->PeriodValue, pKBarField->EndTime);
	if (nFloorBeginTime != nFloorEndTime)
	{
		//SECTIONEND NOT OVER
		if (THOST_IS_SECTIONEND(pKBarField->Symbol, pKBarField->ExchangeID, nFloorEndTime))
		{
			return;
		}

		pKBarField->BeginTime = nFloorBeginTime;
		pKBarField->EndTime = nFloorEndTime;
		pKBarField->Status = THOST_KBAR_STATUS_CLOSE;
		CloseResetKBar(pKBarField, pDepthMarketData);
	}
}

void CKBarHandler::UpdateNullKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData)
{
	//trans date to natuure day
	pKBarField->TradingDate = pDepthMarketData->TradingDay;
	pKBarField->Date = pDepthMarketData->ActionDay;
	pKBarField->BeginTime = pDepthMarketData->UpdateTime;
	pKBarField->EndTime = pDepthMarketData->UpdateTime;
	pKBarField->OpenPrice = pDepthMarketData->LastPrice;
	pKBarField->HighPrice = pDepthMarketData->LastPrice;
	pKBarField->LowPrice = pDepthMarketData->LastPrice;
	pKBarField->ClosePrice = pDepthMarketData->LastPrice;
	pKBarField->AskPrice = pDepthMarketData->AskPrice1;
	pKBarField->AskVolume = pDepthMarketData->AskVolume1;
	pKBarField->BidPrice = pDepthMarketData->BidPrice1;
	pKBarField->BidVolume = pDepthMarketData->BidVolume1;
	pKBarField->Volume = pDepthMarketData->Volume;
	pKBarField->OpenInterest = pDepthMarketData->OpenInterest;
	if (pKBarField->BarIndex == 0)
	{
		pKBarField->PreVolume = 0;
		pKBarField->PreClosePrice = pDepthMarketData->PreClosePrice;
		pKBarField->PreOpenInterest = pDepthMarketData->PreOpenInterest;
	}
	pKBarField->Ticks = 1;
	pKBarField->Status = THOST_KBAR_STATUS_OPEN;
}

void CKBarHandler::UpdateOpenKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData)
{
	pKBarField->TradingDate = pDepthMarketData->TradingDay;
	pKBarField->Date = pDepthMarketData->ActionDay;
	pKBarField->EndTime = pDepthMarketData->UpdateTime;
	if (pDepthMarketData->LastPrice > pKBarField->HighPrice)
	{
		pKBarField->HighPrice = pDepthMarketData->LastPrice;
	}
	if (pDepthMarketData->LastPrice < pKBarField->LowPrice)
	{
		pKBarField->LowPrice = pDepthMarketData->LastPrice;
	}
	pKBarField->ClosePrice = pDepthMarketData->LastPrice;
	pKBarField->AskPrice = pDepthMarketData->AskPrice1;
	pKBarField->AskVolume = pDepthMarketData->AskVolume1;
	pKBarField->BidPrice = pDepthMarketData->BidPrice1;
	pKBarField->BidVolume = pDepthMarketData->BidVolume1;
	pKBarField->Volume = pDepthMarketData->Volume - pKBarField->PreVolume;
	pKBarField->OpenInterest = pDepthMarketData->OpenInterest;
	pKBarField->Ticks++;
}

void CKBarHandler::CloseResetKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData)
{
	CKBarField *pNewKBarField = new CKBarField();
	*pNewKBarField = *pKBarField;
	m_pKBarHandlerSpi->OnRtnKBar(pNewKBarField);
	if (pNewKBarField != NULL)
	{
		delete pNewKBarField;
		pNewKBarField = NULL;
	}
	pKBarField->Status = THOST_KBAR_STATUS_NULL;
	pKBarField->Ticks = 0;
	pKBarField->BeginTime = pKBarField->EndTime;
	pKBarField->PreClosePrice = pKBarField->ClosePrice;
	pKBarField->PreVolume = pDepthMarketData->Volume;
	pKBarField->PreOpenInterest = pDepthMarketData->OpenInterest;
	pKBarField->BarIndex++;
}

int CKBarHandler::FloorTime(char PeriodType, int PeriodValue, int nTime)
{
	if (PeriodType == THOST_KBAR_PERIOD_TYPE_SECOND)
	{
		return (nTime / 100) * 100 + (nTime % 100 / PeriodValue) * PeriodValue;
	}
	else if (PeriodType == THOST_KBAR_PERIOD_TYPE_MINUTE)
	{
		return (nTime / 10000) * 10000 + ((nTime / 100) % 100 / PeriodValue) * PeriodValue * 100;
	}
	return -1;
}

void CKBarHandler::SetTradingDay(int nTradingDay, int nPreTradingDay)
{
	m_nTradingDay = nTradingDay;
	m_nPrevTradingDay = nPreTradingDay;
}

void CKBarHandler::SetInstrumentMap(InstrumentInfoMap mapInstrument)
{
	m_mapInstrumentInfo = mapInstrument;
}

void CKBarHandler::SetInstrumentVec(vector<CThostInstrumentField> vecInstrument)
{
	InstrumentInfoMap mapInstrument;
	for (int i = 0; i < vecInstrument.size(); ++i)
	{
		CThostInstrumentField instrumentInfo;
		instrumentInfo = vecInstrument[i];
		mapInstrument[vecInstrument[i].InstrumentID] = instrumentInfo;
	}
}