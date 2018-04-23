/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapi.h
///@brief �����˿ͻ��˽ӿ�
///@history 
///20160818@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTAPI_H
#define INC_THOSTAPI_H

#include "thostapistruct.h"

#if defined(ISLIB) && defined(WIN32)
#ifdef THOST_API_EXPORT
#define THOST_API_EXPORT __declspec(dllexport)
#else
#define THOST_API_EXPORT __declspec(dllimport)
#endif
#else
#define THOST_API_EXPORT 
#endif

class CThostSpi {
public:
	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData){};
	
	///�����ر�
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrderReport){};

	///kbar�ر�
	virtual void OnRtnKBar(CKBarField *pKBar){};

	///��ѯ��ʷkbar�ص�
	virtual void OnRspQryHistoryKBar(int TradingDate, string InstrumentID, CKBarField *pKBar, int EndMask){};
};

class THOST_API_EXPORT CThostApi {
public:
	///����Apiʵ��
	static CThostApi *CreateThostApi(int nApiMode = THOST_API_MODE_TRADING);
	
	///��������
	virtual void Start(string JsonParam = "") = 0;

	///����ֹͣ
	virtual void Stop() = 0;

	///ע��Spi�ص�
	virtual void RegisterThostSpi(CThostSpi *pThostSpi) = 0;

	///ע��CTP�������
	virtual void RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath) = 0;

	///ע��CTP���׷���
	virtual void RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo) = 0;

	///ע������������
	virtual void RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath) = 0;

	///ע������׷���
	virtual void RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo) = 0;

	///ע��kbar����
	virtual void RegisterKBarServer() = 0;

	///ע��mysql����
	virtual int RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db) = 0;

	///ע����־����
	virtual void RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole = false) = 0;

	///ע��KBar�¼�
	virtual int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0) = 0;

	///ȡ��ע��kbar�¼�
	virtual int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0) = 0;

	///��ȡApiģʽ
	virtual	int	GetThostApiMode() = 0;

	///��������
	virtual void SubscribeQuote(char **ppInstrument, int count) = 0;

	///��������
	virtual void SubscribeQuoteList(vector<string> InstrumentList) = 0;

	///��ȡ�������
	virtual int  GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot) = 0;

	///�޼۵�
	virtual void LimitOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///�м۵�
	virtual void MarketOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///���ɲ�����
	virtual void FakOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///ȫ�ɻ򳷵�
	virtual void FokOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///����
	virtual int  CancelOrder(string OrderRef, string OrderSysID, string ExchangeID) = 0;

	///��ȡ���к�Լ��Ϣ
	virtual int	 GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo) = 0;

	///��ȡ��Լ��Ϣ
	virtual int  GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID) = 0;
	 
	///��ȡ��Լ��֤��
	virtual int  GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID) = 0;
		
	///��ȡ��Լ������
	virtual int  GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID) = 0;

	///��ȡ�˻��ʽ�
	virtual int  GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID) = 0;

	///��ȡ�˻��ֲ�
	virtual int  GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InvestorID, string InstrumentID = "") = 0;

	///��ȡ����
	virtual int  GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd) = 0;

	///��ȡ�ɽ�
	virtual int  GetTrade(TradeVec &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd) = 0;

	///��ȡ��ʷKBar����
	virtual int  ReqQryHistoryKBar(string InstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue) = 0;

	///��ȡ��ʷKBar����
	virtual int  ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue) = 0;

	///��¼��־
	virtual void Log(int level, const char *format, ...) = 0;

protected:
	~CThostApi(){};
};

#endif