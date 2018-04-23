#include "thostmdapi.h"


CThostMdApi::CThostMdApi()
{
	m_bConnected = false;
	m_nRequestId = 0;
	m_pThostMdHandlerSpi = NULL;
}

CThostMdApi::CThostMdApi(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath)
{
	m_bConnected = false;
	m_nRequestId = 0;
	m_sInvestorId = InvestorId;
	m_sPasswd = Password;
	m_sBrokerId = BrokerId;
	m_sFrontAddress = FrontAddress;
	m_sUserDataPath = FlowPath;
	m_pThostMdHandlerSpi = NULL;
}

void CThostMdApi::RegisterMdHandlerSpi(CThostMdHandlerSpi *pThostMdHandlerSpi)
{
	m_pThostMdHandlerSpi = pThostMdHandlerSpi;
}

void CThostMdApi::AddSubscribeList(vector<string> vecSubscribeList)
{
	int i;
	int j;
	for (i = 0; i < vecSubscribeList.size(); ++i)
	{
		for (j = 0; j < m_vecSubscribeList.size(); ++j)
		{
			if (strcmp(vecSubscribeList[i].c_str(), m_vecSubscribeList[j].c_str()) == 0)
			{
				break;
			}
		}
		if (j >= m_vecSubscribeList.size())
		{
			m_vecSubscribeList.push_back(vecSubscribeList[i]);
		}
	}
}

void CThostMdApi::SubscribeQuote(string InstrumentId)
{
	vector<string> vecInstrument;
	vecInstrument.push_back(InstrumentId);
	SubscribeQuoteList(vecInstrument);
}

void CThostMdApi::SubscribeQuoteList(vector<string> InstrumentList)
{
	if (m_bConnected == true && InstrumentList.size() > 0)
	{
		AddSubscribeList(InstrumentList);
		int nInstrumetCount = InstrumentList.size();
		char **ppInstrumentList = new char* [nInstrumetCount];
		int nIndex = 0;
		for (int i = 0; i < InstrumentList.size(); ++i)
		{
			string sInstrumentID = InstrumentList[i];
			char *pInstrument = new char[sInstrumentID.length()+1];
			memset(pInstrument, '\0', sInstrumentID.length()+1);
			strcpy(pInstrument, sInstrumentID.c_str());
			ppInstrumentList[nIndex] = pInstrument;
			nIndex++;
		}
		SubscribeQuote(ppInstrumentList, nInstrumetCount);
		for (int i = 0; i < nIndex; ++i) 
		{
			delete []ppInstrumentList[i];
		}
		delete []ppInstrumentList;
	}
}

void CThostMdApi::ReSubscribeQuoteBySnapshot()
{
	vector<string> vecSubscribeQuoteList;
	for (DepthMarketDataSnapshotMap::iterator iter = m_DepthMarketDataSnapshotMap.begin(); iter != m_DepthMarketDataSnapshotMap.end(); ++iter)
	{
		vecSubscribeQuoteList.push_back(iter->second.InstrumentID);
	}
	SubscribeQuoteList(vecSubscribeQuoteList);
}

int CThostMdApi::GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot)
{
	if (m_DepthMarketDataSnapshotMap.end() != m_DepthMarketDataSnapshotMap.find(InstrumentID))
	{
		DepthMarketDataSnapshot = m_DepthMarketDataSnapshotMap[InstrumentID];
		return 0;
	}
	return 1;
}