/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapihandler.h
///@brief 定义客户端接口的实现类
///@history 
///20160818@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTAPIHANDLER_H
#define INC_THOSTAPIHANDLER_H

#include "../thostapi.h"
#include "../thostmdapi.h"
#include "../thosttradeapi.h"
#include "../kbarhandler/kbarhandler.h"
#include "../../misc/loghandler.h"
#include "../../misc/mysqlmanager.h"

class CThostApiHandler : public CThostApi, CThostMdHandlerSpi, CThostTraderHandlerSpi, CKBarHandlerSpi {
public:
	CThostApiHandler();

	~CThostApiHandler(){};

public: 
	///启动初始化
	void Start(string JsonParam = "");

	///停止销毁内存
	void Stop();

	///注册回调类
	void RegisterThostSpi(CThostSpi *pThostSpi)
	{
		m_pThostSpi = pThostSpi;
	}
 
	//注册CTP行情服务
	virtual void RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath);

	//注册CTP交易服务
	virtual void RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo);

	//注册飞鼠行情服务
	virtual void RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath);

	//注册飞鼠交易服务
	virtual void RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string FlowPath, string UserProductInfo);

	///注册kbar服务
	virtual void RegisterKBarServer();

	///注册mysql服务
	virtual int RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db);

	//注册日志服务
	virtual void RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole = false);

	///注册KBar事件
	int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0);

	///取消注册kbar事件
	int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0);

	///获取Api模式
	virtual	int	GetThostApiMode();

	//订阅行情
	virtual void SubscribeQuote(char **ppInstrument, int count);

	//订阅行情
	virtual void SubscribeQuoteList(vector<string> InstrumentList);

	//获取行情快照
	virtual int GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot);

	//限价单
	virtual void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//市价单
	virtual void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//部成并撤单
	virtual void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//全成或撤单
	virtual void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

	//撤单
	virtual int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID);

	///获取所有合约信息
	virtual int	GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo);

	//获取合约信息
	virtual int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID);

	//获取合约保证金
	virtual int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID);

	//获取合约手续费
	virtual int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID);

	//获取账户资金
	virtual int GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID);

	//获取账户持仓
	virtual int GetInvestorPosition(vector<CThostInvestorPositionField> &vecInvestorPosition, string InvestorID, string InstrumentID = "");

	//获取订单
	virtual int GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd);

	//获取成交
	virtual int GetTrade(TradeVec  &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd);

	///获取历史KBar数据
	virtual int ReqQryHistoryKBar(string InstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue);

	///获取历史KBar数据
	virtual int ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue);

	//记录日志
	virtual void Log(int level, const char *format, ...);

public:
	///实时行情
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///实时订单回报
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrder);

private:
	///是否属于上金所
	inline bool    BelongSGE(string &InstrumentID);

private:
	///回调类
	CThostSpi					*m_pThostSpi;
	///ctp行情
	CThostMdApi					*m_pThostMdHandler;
	///ctp交易
	CThostTraderApi				*m_pThostTraderHandler;
	///kbar处理
	CKBarHandler				*m_pKBarHandler;
	///飞鼠行情
	CThostMdApi					*m_pFSMdHandler;
	///飞鼠交易
	CThostTraderApi				*m_pFSTraderHandler;
	///mysql处理
	CMysqlManager				m_mysqlManager;
	///日志处理
	CLogHandler					*m_pLogHandler;
};

#endif