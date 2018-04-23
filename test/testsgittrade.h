#include <SgitFtdcUserApiDataType.h>
#include <SgitFtdcUserApiStruct.h>
#include <SgitFtdcTraderApi.h>

namespace fstech {
	class CTestSGITTrade : public CThostFtdcTraderSpi{
	public:
		CTestSGITTrade() {};	
		~CTestSGITTrade(){};

	public:
		void Start()
		{
			pThostTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./fsflow/ttt");
			pThostTraderApi->RegisterSpi(this);
			pThostTraderApi->SubscribePrivateTopic(THOST_TERT_QUICK);
			pThostTraderApi->SubscribePublicTopic(THOST_TERT_QUICK);
			pThostTraderApi->RegisterFront("tcp://172.31.90.41:7776");
			pThostTraderApi->Init();
		}

		virtual void OnFrontConnected()
		{
			printf ("FSTRADE:OnFrontConnected\n");
			CThostFtdcReqUserLoginField reqUserLogin;
			memset(&reqUserLogin, 0, sizeof(CThostFtdcReqUserLoginField));
			strcpy(reqUserLogin.BrokerID,"0017");
			strcpy(reqUserLogin.UserID, "0100156272");
			strcpy(reqUserLogin.Password, "888888");
			pThostTraderApi->ReqUserLogin(&reqUserLogin, 0);
		}

		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			if (pRspInfo->ErrorID == 0)
			{
				printf ("FSTRADE:OnRspUserLogin\n");
				CThostFtdcQrySettlementInfoField qrySettlementInfo;
				memset(&qrySettlementInfo, 0, sizeof(CThostFtdcQrySettlementInfoField));
				strcpy(qrySettlementInfo.BrokerID,"0017");
				strcpy(qrySettlementInfo.InvestorID, "0100156272");
				pThostTraderApi->ReqQrySettlementInfo(&qrySettlementInfo, 1);
			}
		}

		virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			if (pRspInfo->ErrorID == 0)
			{
				printf ("FSTRADE:OnRspQrySettlementInfo\n");
				CThostFtdcQryInstrumentField qryInstrument;
				memset(&qryInstrument, 0, sizeof(CThostFtdcQryInstrumentField));
				strcpy(qryInstrument.ExchangeID, "");
				strcpy(qryInstrument.InstrumentID, "");
				pThostTraderApi->ReqQryInstrument(&qryInstrument, 2);
			}
		}

		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf ("%s %d\n", pInstrument->InstrumentID, bIsLast);
			if (bIsLast == true)
			{
				CThostFtdcQryTradingAccountField qryTradingAccount;
				memset(&qryTradingAccount, 0, sizeof(CThostFtdcQryTradingAccountField));
				strcpy(qryTradingAccount.BrokerID, "0017");
				strcpy(qryTradingAccount.InvestorID, "0100156272");
				pThostTraderApi->ReqQryTradingAccount(&qryTradingAccount, 3);
			}
		}

		virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			printf ("OnRspQrySettlementInfoConfirm\n");
		}

		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
		{
			if(pTradingAccount != NULL)
			{
				printf ("%s %f %d\n", pTradingAccount->AccountID, pTradingAccount->Balance, bIsLast);
			}
		}

	private:
		CThostFtdcTraderApi *pThostTraderApi;
	};
}
