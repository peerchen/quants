/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapi.h
///@brief 仿真交易实现类
///@history 
///20161025@徐俊		创建该文件
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
	///启动初始化
	int Start(string JsonParam = "");

	///等待
	int Wait();

	///停止销毁内存
	void Stop();

	///限价单
	void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	///市价单
	void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation);

	///Fill and Kill 
	void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	///Fill or Kill
	void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	///撤单
	int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID);

	///获取合约信息
	int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID);

	///获取所有的合约信息
	int GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo);

	///获取合约保证金
	int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID);

	///获取合约手续费
	int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID);

	///获取账户资金
	int GetTradingAccount(CThostTradingAccountField &TradingAccount);

	///获取用户持仓
	int GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID);

	///获取订单信息
	int GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd);

	///获取成交信息
	int GetTrade(TradeVec  &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd);
	
	///获取交易账户
	string GetInvestorID();

	///获取交易日
	int GetTradingDay();

	///设置交易日
	void SetTradingDay(int TradingDay);

	///清空所有信息
	void Clear();

	///更新kbar的快照
	void UpdateKBarSnapshot(string InstrumentID, CKBarField &kbarField);

	///交易日结算
	void BeginTradingDaySettle(int TradingDay);

	///设置初始资金
	void SetInitializedBalance(double Balance);

	///获取资金列表
	void GetBalanceList(vector<double> &vecBalance);

	///获取持仓量
	int GetPositionAmount(string InstrumentID, char Direction, char PositionDirection = THOST_PSD_ALL);

	///设置合约信息
	void SetInstrumentInfo(CThostInstrumentField &InstrumentInfo);

	///设置合约保证金
	void SetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate);

	///设置合约手续费
	void SetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate);

private:
	///检查资金是否足够
	int CheckBalanceToEntrust(string InstrumentID, char Direction, double LimitPrice, int Volume, char HedgeFlag);

	///更新资金账户-持仓盈亏,动态权益
	void UpdateAccountBalance();

	///基于成交汇报更新资金账户
	void UpdateTradingAccount(CThostOrderReportField *pOrderReport);

	///平昨仓
	void CloseYdPosition(CThostTradeField &tradeField, int &CloseCount);

	///平今仓
	void CloseTdPosition(CThostTradeField &tradeField, int &CloseCount);

	///根据订单回报生成成交明细
	void MakeTradeFromOrderReport(CThostTradeField &tradeField, CThostOrderReportField *pOrderReport);

private:
	///日志处理
	CLogHandler									*m_pLogHandler;
	///模拟OrderSysID
	int											m_nOrderSysID;
	///模拟TradeID	
	int											m_nTradeID;
	///昨日成交明细
	TradeVec									m_vecYdTrade;
	///今日成交明细
	TradeVec									m_vecTdTrade;
	///回测期间内所有的成交明细	
	TradeVec									m_vecAllTrade;
	///最新kbar快照
	KBarSnapshotMap								m_mapKBarSnapshot;
	///资金账户列表
	vector<double>								m_vecBalanceList;
};

#endif

