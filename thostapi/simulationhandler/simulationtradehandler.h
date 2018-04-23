/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapi.h
///@brief ���潻��ʵ����
///@history 
///20161025@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_SIMULATIONTRADERHANDLER_H
#define INC_SIMULATIONTRADERHANDLER_H

#include "../thosttradeapi.h"
#include "../../misc/loghandler.h"
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

class CSimulationTraderHandler : public CThostTraderApi {
public:
	CSimulationTraderHandler();

	CSimulationTraderHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo);

	~CSimulationTraderHandler(){};

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
	int GetTradingDay();

	///���ý�����
	void SetTradingDay(int TradingDay);

	///���������Ϣ
	void Clear();

	///����kbar�Ŀ���
	void UpdateKBarSnapshot(string InstrumentID, CKBarField &kbarField);

	///�����ս���
	void BeginTradingDaySettle(int TradingDay);

	///���ó�ʼ�ʽ�
	void SetInitializedBalance(double Balance);

	///��ȡ�ʽ��б�
	void GetBalanceList(vector<double> &vecBalance);

	///��ȡ�ֲ���
	int GetPositionAmount(string InstrumentID, char Direction, char PositionDirection = THOST_PSD_ALL);

	///���ú�Լ��Ϣ
	void SetInstrumentInfo(CThostInstrumentField &InstrumentInfo);

	///���ú�Լ��֤��
	void SetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate);

	///���ú�Լ������
	void SetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate);

private:
	///����ʽ��Ƿ��㹻
	int CheckBalanceToEntrust(string InstrumentID, char Direction, double LimitPrice, int Volume, char HedgeFlag);

	///�����ʽ��˻�-�ֲ�ӯ��,��̬Ȩ��
	void UpdateAccountBalance();

	///���ڳɽ��㱨�����ʽ��˻�
	void UpdateTradingAccount(CThostOrderReportField *pOrderReport);

	///ƽ���
	void CloseYdPosition(CThostTradeField &tradeField, int &CloseCount);

	///ƽ���
	void CloseTdPosition(CThostTradeField &tradeField, int &CloseCount);

	///���ݶ����ر����ɳɽ���ϸ
	void MakeTradeFromOrderReport(CThostTradeField &tradeField, CThostOrderReportField *pOrderReport);

private:
	///��־����
	CLogHandler									*m_pLogHandler;
	///ģ��OrderSysID
	int											m_nOrderSysID;
	///ģ��TradeID	
	int											m_nTradeID;
	///���ճɽ���ϸ
	TradeVec									m_vecYdTrade;
	///���ճɽ���ϸ
	TradeVec									m_vecTdTrade;
	///�ز��ڼ������еĳɽ���ϸ	
	TradeVec									m_vecAllTrade;
	///����kbar����
	KBarSnapshotMap								m_mapKBarSnapshot;
	///�ʽ��˻��б�
	vector<double>								m_vecBalanceList;
};

#endif

