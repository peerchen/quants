#include "thosttradeapi.h"

void CThostTraderApi::ClearTradingData()
{
	for (OrderReportMap::iterator iter = m_mapOrderRef.begin(); iter != m_mapOrderRef.end(); ++iter)
	{
		if (iter->second != NULL)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_mapOrderRef.clear();

	for (OrderReportMap::iterator iter = m_mapExchangeOrder.begin(); iter != m_mapExchangeOrder.end(); ++iter)
	{
		if (iter->second != NULL)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	m_mapExchangeOrder.clear();

	m_mapInstrumentInfo.clear();
	m_mapInstrumentMarginRate.clear();
	m_mapInstrumentComissionRate.clear();

	m_vecOrderReport.clear();
	m_vecTrade.clear();
	m_vecInvestorPosition.clear();
}