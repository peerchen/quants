/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapi.h
///@brief �����˽��׽ӿڵ�api
///@history 
///20160912@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTTRADEAPI_H
#define INC_THOSTTRADEAPI_H

#include "thostapistruct.h"
#include "../misc/loghandler.h"

class CThostTraderApi {
public:
	CThostTraderApi()
	{
		m_pTraderHandlerSpi = NULL;
		m_bInitialize = true;
		m_nRequestId = 0;
	}

	CThostTraderApi(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo)
	{	
		m_pTraderHandlerSpi = NULL;
		m_bInitialize = true;
		m_nRequestId = 0;
		m_sInvestorId = InvestorId;
		m_sPasswd = Password;
		m_sBrokerId = BrokerId;
		m_sFrontAddress = FrontAddress;
		m_sUserProductInfo = UserProductInfo;
		m_sUserDataPath = UserDataPath;
	}

	virtual ~CThostTraderApi(){};

public:
	///������ʼ��
	virtual int Start(string JsonParam = "") = 0;

	///�ȴ�
	virtual int Wait() = 0;

	///ֹͣ�����ڴ�
	virtual void Stop() = 0;

	///ע�ύ�״���Ļص���
	virtual void RegisterTradeHandlerSpi(CThostTraderHandlerSpi *pTraderHandlerSpi)
	{
		m_pTraderHandlerSpi = pTraderHandlerSpi;
	}

	///�޼۵�
	virtual void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///�м۵�
	virtual void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///Fill and Kill 
	virtual void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///Fill or Kill
	virtual void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///����
	virtual int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID) = 0;

	///��ȡ��Լ��Ϣ
	virtual int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID) = 0;

	///��ȡ���еĺ�Լ��Ϣ
	virtual int GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo) = 0;

	///��ȡ��Լ��֤��
	virtual int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID) = 0;

	///��ȡ��Լ������
	virtual int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID) = 0;

	///��ȡ�˻��ʽ�
	virtual int GetTradingAccount(CThostTradingAccountField &TradingAccount) = 0;

	///��ȡ�û��ֲ�
	virtual int GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID) = 0;

	///��ȡ������Ϣ
	virtual int GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd) = 0;

	///��ȡ�ɽ���Ϣ
	virtual int GetTrade(TradeVec  &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd) = 0;

	///��ȡ�����˻�
	virtual string GetInvestorID() = 0;

	///��ȡ������
	virtual int    GetTradingDay() = 0;

	///�������
	void ClearTradingData();

	///�ж��Ƿ񳬳�����
	bool IsFlowControl(int nRet)
	{
		return nRet == -2 || nRet == -3;
	}

	///������־�������
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

protected:
	///�����˻�
	string										m_sInvestorId;
	///��������
	string										m_sPasswd;
	///����ID
	string										m_sBrokerId;
	///��Ʒ��Ϣ
	string										m_sUserProductInfo;
	///ǰ�û�ID
	int											m_nFrontID;
	///�ỰID
	int											m_nSessionID;
	///ǰ�û���ַ
	string										m_sFrontAddress;
	///userdata·��
	string										m_sUserDataPath;
	///������
	int											m_nTradingDay;
	///����ID
	int											m_nRequestId;
	///��������
	int											m_nOrderRef;
	///��ʼ��״̬
	bool										m_bInitialize;
	///�������ͱ�
	ReqTypeMap									m_mapReqType;

	///Thost�����ر���
	/// <FrontId|SessionId|OrderRef, CThostOrderReportField*>
	OrderReportMap								m_mapOrderRef;
	///�����������ر���
	/// <ExchangeId|OrderSysId, CThostOrderReportField*>
	OrderReportMap								m_mapExchangeOrder;
	///�˻��ʽ���Ϣ
	CThostTradingAccountField					m_TradingAccount;
	///�û��ֲ��б�
	InvestorPositionVec							m_vecInvestorPosition;	
	///�����ر��б�
	OrderReportVec								m_vecOrderReport;
	///�ɽ���Ϣ�б�
	TradeVec									m_vecTrade;
	///��Լ��Ϣ��
	InstrumentInfoMap							m_mapInstrumentInfo;
	///��Լ��֤���
	InstrumentMarginRateMap						m_mapInstrumentMarginRate;
	///��Լ�����ѱ�
	InstrumentComissionRateMap					m_mapInstrumentComissionRate;
	///���״���ص���
	CThostTraderHandlerSpi						*m_pTraderHandlerSpi;
	///��־����
	CLogHandler									*m_pLogHandler;
};

#endif