#include <stdio.h>
#include <SgitFtdcUserApiDataType.h>
#include <SgitFtdcUserApiStruct.h>
#include <SgitFtdcMdApi.h>

namespace fstech {
	class CTestSGITMd : public CThostFtdcMdSpi{
	public:
		CTestSGITMd(){};
		~CTestSGITMd(){};

	public:
		void Start()
		{
			pThostMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
			pThostMdApi->RegisterSpi(this);
			pThostMdApi->RegisterFront("tcp://172.31.90.41:7777");
			pThostMdApi->Init();
		}

		virtual void OnFrontConnected()
		{
			printf ("FSMD:OnFrontConnected\n");
			CThostFtdcReqUserLoginField reqUserLoginField;
			memset(&reqUserLoginField, 0, sizeof(CThostFtdcReqUserLoginField));
			strcpy(reqUserLoginField.UserID, "0100156272");
			strcpy(reqUserLoginField.Password, "888888");
			strcpy(reqUserLoginField.BrokerID, "0017");
			pThostMdApi->ReqUserLogin(&reqUserLoginField, 0);
		}

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			if (pRspInfo->ErrorID == 0) 
			{
				printf ("FSMD:OnRspUserLogin\n");
				char *pSubscrbeList[2] = {"Au(T+D)","Ag(T+D)"};
				pThostMdApi->SubscribeMarketData(pSubscrbeList, 2);
			}
		}

		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			if (pRspInfo->ErrorID == 0)
			{
				printf ("FSMD:sub_%s\n", pSpecificInstrument->InstrumentID);
			}
		}

		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
		{
			//printf ("FSMD:%s %f\n", pDepthMarketData->InstrumentID, pDepthMarketData->LastPrice);
		}

	private:
		CThostFtdcMdApi			*pThostMdApi;
	};
}
