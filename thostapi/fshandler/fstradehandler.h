#ifndef INC_FSTRADEHANDLER_H
#define INC_FSTRADEHANDLER_H

#include "../thosttradeapi.h"
#include "../../misc/loghandler.h"
#include <SgitFtdcUserApiDataType.h>
#include <SgitFtdcUserApiStruct.h>
#include <SgitFtdcTraderApi.h>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

namespace fstech {
	class CFSTraderHandler : public CThostTraderApi, CThostFtdcTraderSpi {
	public:
		CFSTraderHandler();

		CFSTraderHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string m_sUserDataPath, string UserProductInfo);

		virtual ~CFSTraderHandler(){};		
	public:
		///������ʼ��
		int Start(string JsonParam = "");

		///�ȴ�
		int Wait();

		///ֹͣ�����ڴ�
		void Stop();

		///�޼۵�
		void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///�м۵�
		void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///Fill and Kill 
		void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///Fill or Kill
		void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///����
		int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID);

		///��ȡ��Լ��Ϣ
		int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID);

		///��ȡ���еĺ�Լ��Ϣ
		int GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo);

		///��ȡ��Լ��֤��
		int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID);

		///��ȡ��Լ������
		int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID);

		///��ȡ�˻��ʽ�
		int GetTradingAccount(CThostTradingAccountField &TradingAccount);

		///��ȡ�û��ֲ�
		int GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID);

		///��ȡ������Ϣ
		int GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd);

		///��ȡ�ɽ���Ϣ
		int GetTrade(TradeVec  &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd);

		///��ȡ�����˻�
		string GetInvestorID();

		///��ȡ������
		int	   GetTradingDay();

	protected:
		///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
		virtual void OnFrontConnected();

		///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
		///@param nReason ����ԭ��
		///        0x1001 �����ʧ��
		///        0x1002 ����дʧ��
		///        0x2001 ����������ʱ
		///        0x2002 ��������ʧ��
		virtual void OnFrontDisconnected(int nReason);

		///��¼������Ӧ
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///����¼��������Ӧ
		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///��������������Ӧ
		virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ��Լ��Ӧ
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���߽�������Ӧ
		virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ������Ϣȷ����Ӧ
		virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ��Լ��֤����Ӧ
		virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ��Լ��������Ӧ
		virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯ�ʽ��˻���Ӧ
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���ֲ߳���Ӧ
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���ֲ߳���ϸ��Ӧ
		virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���߱�����Ӧ
		virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///�����ѯͶ���߳ɽ���Ӧ
		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///��Լ״̬֪ͨ
		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

		///����֪ͨ
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

		///�ɽ�֪ͨ
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	private:
		///��������ǰ�û�
		int ReqConnectFront();

		///�����¼
		int ReqUserLogin(char ReqType = THOST_RT_SYNC);

		///�����ѯ������Ϣ
		int ReqQrySettlementInfo(char ReqType = THOST_RT_SYNC);

		///�����ѯ����ȷ��
		int ReqQrySettlementInfoConfirm(char ReqType = THOST_RT_SYNC);

		///�����ѯ��Լ��Ϣ
		int ReqQryInstrument(char ReqType = THOST_RT_SYNC);

		///�����ѯ��Լ��֤��
		int ReqQryInstrumentMarginRate(string InstrumentID = "", char HedgeFlag = THOST_HF_Speculation, char ReqType = THOST_RT_SYNC);

		///�����ѯ��Լ������
		int ReqQryInstrumentCommissionRate(string InstrumentID = "", char ReqType = THOST_RT_SYNC);

		///�����ѯͶ�����ʽ��˻�
		int ReqQryTradingAccount(char ReqType = THOST_RT_SYNC);

		///�����ѯͶ���ֲ߳�
		int ReqQryInvestorPosition(string InstrumentID = "", char ReqType = THOST_RT_SYNC);

		///�����ѯ����
		int ReqQryOrder(string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd, char ReqType = THOST_RT_SYNC);

		///�����ѯ�ɽ�
		int ReqQryTrade(string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd, char ReqType = THOST_RT_SYNC); 

		///���ɱ���¼��Ļ����ֶ�
		void MakeOrderInsertBase(CThostFtdcInputOrderField &reqInputOrderField, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///����thost�������ùؼ���
		void MakeOrderRefKey(string &OrderRefKey, int FrontId, int SessionId, const char *pszOrderRef);

		///��������ͬ��Źؼ���
		void MakeExchangeOrderKey(string &ExchangeOrderKey, const char *pszExchangeId, const char *pszOrderSysId);

		///���ݱ���¼�����ɱ����ر� 
		CThostOrderReportField* MakeOrderReport(CThostFtdcInputOrderField *pInputOrder);

		///���ݽ����������ر����ɱ����ر�
		CThostOrderReportField* MakeOrderReport(CThostFtdcOrderField *pOrder);

		///����¼������
		string ReqOrderInsert(CThostFtdcInputOrderField &reqInputOrderField);

		///��ʼ������
		void InitializeOrder();

	private:
		///ͬ���������
		boost::mutex								m_SyncReqMutex;
		///ͬ���������������
		boost::condition_variable					m_SyncReqCondtion;
		///ctp trade api
		CThostFtdcTraderApi*						m_pTraderApi;
		///��־����
		CLogHandler									*m_pLogHandler;
	};
}

#endif