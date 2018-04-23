/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapibthandler.h
///@brief ����ز⹦��ʵ����
///@history 
///20160926@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTAPIBTHANDLER_H
#define INC_THOSTAPIBTHANDLER_H

#include "../thostapi.h"
#include "../../misc/loghandler.h"
#include "../../misc/mysqlmanager.h"
#include "../../misc/strategyeval.h"
#include "../simulationhandler/simulationtradehandler.h"

class CThostApiBTHandler : public CThostApi, CThostTraderHandlerSpi{
public:
	CThostApiBTHandler();

	~CThostApiBTHandler(){};

public: 
	///������ʼ��
	void Start(string JsonParam = "");

	///ֹͣ�����ڴ�
	void Stop();

	///ע��ص���
	void RegisterThostSpi(CThostSpi *pThostSpi)
	{
		m_pThostSpi = pThostSpi;
	}

	//ע��CTP�������
	virtual void RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath);

	//ע��CTP���׷���
	virtual void RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo);

	//ע������������
	virtual void RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath);

	//ע������׷���
	virtual void RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo);

	///ע��kbar����
	virtual void RegisterKBarServer();

	///ע��mysql����
	virtual int RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db);

	//ע����־����
	virtual void RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole = false);

	///ע��KBar�¼�
	int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0);

	///ȡ��ע��kbar�¼�
	int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0);

	///��ȡApiģʽ
	virtual	int	GetThostApiMode();

	//��������
	virtual void SubscribeQuote(char **ppInstrument, int count);

	//��������
	virtual void SubscribeQuoteList(vector<string> InstrumentList);

	//��ȡ�������
	virtual int GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot);

	//�޼۵�
	virtual void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//�м۵�
	virtual void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//���ɲ�����
	virtual void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//ȫ�ɻ򳷵�
	virtual void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//����
	virtual int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID);

	///��ȡ���к�Լ��Ϣ
	virtual int	GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo);

	//��ȡ��Լ��Ϣ
	virtual int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID);

	//��ȡ��Լ��֤��
	virtual int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID);

	//��ȡ��Լ������
	virtual int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID);

	//��ȡ�˻��ʽ�
	virtual int GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID);

	//��ȡ�˻��ֲ�
	virtual int GetInvestorPosition(vector<CThostInvestorPositionField> &vecInvestorPosition, string InvestorID, string InstrumentID = "");

	//��ȡ����
	virtual int GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd);

	//��ȡ�ɽ�
	virtual int GetTrade(TradeVec  &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd);

	///��ȡ��ʷKBar����
	virtual int ReqQryHistoryKBar(string InstrumentID, int StartTradingDate, int EndTradingDate, char PeriodType, int PeriodValue);

	///��ȡ��ʷKBar����
	virtual int ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradingDate, int EndTradingDate, char PeriodType, int PeriodValue);

	//��¼��־
	virtual void Log(int level, const char *format, ...);

public:
	///ʵʱ����
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///ʵʱ�����ر�
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrder);

private:
	///�Ƿ������Ͻ���
	inline bool BelongSGE(string &InstrumentID);

	///��ȡ[start,end]�����еĽ�����
	void	GetTradingDayList(vector<int> &vecTradingDate, int StartTradinDate, int EndTradingDate, string ExchangeID = "SHFE");

	///��ȡ���������յ�kbar
	void	GetKBarListByTradingDay(vector<CKBarField> &vecKBarByTradingDate, vector<string> &vecInstrumentID, int PeriodType, int PeriodValue, int TradingDay);

	///����ز�ָ��
	void EvalStrategy();

private:
	///�ص���
	CThostSpi							*m_pThostSpi;
	///mysql����
	CMysqlManager						m_mysqlManager;
	///��־����
	CLogHandler							*m_pLogHandler;
	///�ο�ָ���Ƿ��б�
	vector<double>						m_vecBenchmarkIndex;
	///��������
	CThostStrategyEval					m_strategyeval;
	///ģ�⽻��
	CSimulationTraderHandler			*m_pSimulationTraderHandler;
};

#endif