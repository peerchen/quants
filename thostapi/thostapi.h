/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapi.h
///@brief 定义了客户端接口
///@history 
///20160818@徐俊		创建该文件
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
	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData){};
	
	///订单回报
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrderReport){};

	///kbar回报
	virtual void OnRtnKBar(CKBarField *pKBar){};

	///查询历史kbar回调
	virtual void OnRspQryHistoryKBar(int TradingDate, string InstrumentID, CKBarField *pKBar, int EndMask){};
};

class THOST_API_EXPORT CThostApi {
public:
	///创建Api实例
	static CThostApi *CreateThostApi(int nApiMode = THOST_API_MODE_TRADING);
	
	///策略启动
	virtual void Start(string JsonParam = "") = 0;

	///策略停止
	virtual void Stop() = 0;

	///注册Spi回调
	virtual void RegisterThostSpi(CThostSpi *pThostSpi) = 0;

	///注册CTP行情服务
	virtual void RegisterCTPMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath) = 0;

	///注册CTP交易服务
	virtual void RegisterCTPTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo) = 0;

	///注册飞鼠行情服务
	virtual void RegisterFSMdServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath) = 0;

	///注册飞鼠交易服务
	virtual void RegisterFSTraderServer(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath, string UserProductInfo) = 0;

	///注册kbar服务
	virtual void RegisterKBarServer() = 0;

	///注册mysql服务
	virtual int RegisterMySQLServer(string Host, int Port, string UserId, string Password, string db) = 0;

	///注册日志服务
	virtual void RegisterLogServer(const char *pszLogFileName, bool bIsEnableConsole = false) = 0;

	///注册KBar事件
	virtual int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0) = 0;

	///取消注册kbar事件
	virtual int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0) = 0;

	///获取Api模式
	virtual	int	GetThostApiMode() = 0;

	///订阅行情
	virtual void SubscribeQuote(char **ppInstrument, int count) = 0;

	///订阅行情
	virtual void SubscribeQuoteList(vector<string> InstrumentList) = 0;

	///获取行情快照
	virtual int  GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot) = 0;

	///限价单
	virtual void LimitOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///市价单
	virtual void MarketOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///部成并撤单
	virtual void FakOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///全成或撤单
	virtual void FokOrder(int RequstID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///撤单
	virtual int  CancelOrder(string OrderRef, string OrderSysID, string ExchangeID) = 0;

	///获取所有合约信息
	virtual int	 GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo) = 0;

	///获取合约信息
	virtual int  GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID) = 0;
	 
	///获取合约保证金
	virtual int  GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID) = 0;
		
	///获取合约手续费
	virtual int  GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID) = 0;

	///获取账户资金
	virtual int  GetTradingAccount(CThostTradingAccountField &TradingAccount, string InvestorID) = 0;

	///获取账户持仓
	virtual int  GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InvestorID, string InstrumentID = "") = 0;

	///获取订单
	virtual int  GetOrder(OrderReportVec &vecOrderReport, string InvestorID, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd) = 0;

	///获取成交
	virtual int  GetTrade(TradeVec &vecTrade, string InvestorID, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd) = 0;

	///获取历史KBar数据
	virtual int  ReqQryHistoryKBar(string InstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue) = 0;

	///获取历史KBar数据
	virtual int  ReqQryHistoryKBar(vector<string> vecInstrumentID, int StartTradinDate, int EndTradingDate, char PeriodType, int PeriodValue) = 0;

	///记录日志
	virtual void Log(int level, const char *format, ...) = 0;

protected:
	~CThostApi(){};
};

#endif