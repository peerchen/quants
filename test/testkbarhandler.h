#ifndef INC_TESTKBARHANDLER_H
#define INC_TESTKBARHANDLER_H
#include "../thostapi/kbarhandler/kbarhandler.h"
#include "../misc/csvhandler.h"

class CTestKBarHandler : public CKBarHandlerSpi{
public:
	CTestKBarHandler(){}
	~CTestKBarHandler(){}

public:
	void Start()
	{
		CKBarHandler *pKBarHandler = new CKBarHandler();
		pKBarHandler->RegisterKBarHandlerSpi(this);
		pKBarHandler->RegisterKBarEvent("v1707", "v", "CZCE", THOST_KBAR_PERIOD_TYPE_MINUTE, 1);
		
		CSVHandler csvhandler;
		csvhandler.ReadData("C:\\Users\\Jsky\\Desktop\\20161216\\20161216_v1707_tick.csv");
		typedef vector < vector<string> >::iterator csviter;
		for(csviter it1 = csvhandler.m_vecTable.begin(); it1 != csvhandler.m_vecTable.end(); ++it1)  
		{  
			vector<string> vecRow = *it1;
			CThostDepthMarketDataField marketDataField;
			memset(&marketDataField, 0, sizeof(marketDataField));
			marketDataField.ActionDay = atoi(vecRow[0].c_str());
			marketDataField.InstrumentID = vecRow[1].c_str();
			marketDataField.UpdateTime = atoi(vecRow[2].c_str());
			marketDataField.UpdateMillisec = atoi(vecRow[3].c_str());
			marketDataField.Volume = atoi(vecRow[5].c_str());
			marketDataField.OpenInterest = atoi(vecRow[6].c_str());
			marketDataField.TradingDay = 20161214;
			pKBarHandler->MarketDataEvent(&marketDataField);
		}
	}

	void OnRtnKBar(CKBarField *pKBar)
	{
		printf ("%s %d %d %d %d %f %f\n", pKBar->InstrumentID.c_str(), pKBar->Date, pKBar->BeginTime, pKBar->EndTime, pKBar->PreVolume, pKBar->OpenInterest, pKBar->PreOpenInterest);
	}
};

#endif