/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapi.h
///@brief 定义了交易接口的api
///@history 
///20160912@徐俊		创建该文件
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
	///启动初始化
	virtual int Start(string JsonParam = "") = 0;

	///等待
	virtual int Wait() = 0;

	///停止销毁内存
	virtual void Stop() = 0;

	///注册交易处理的回调类
	virtual void RegisterTradeHandlerSpi(CThostTraderHandlerSpi *pTraderHandlerSpi)
	{
		m_pTraderHandlerSpi = pTraderHandlerSpi;
	}

	///限价单
	virtual void LimitOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///市价单
	virtual void MarketOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///Fill and Kill 
	virtual void FakOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///Fill or Kill
	virtual void FokOrder(int RequestID, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation) = 0;

	///撤单
	virtual int    CancelOrder(string OrderRef, string OrderSysID, string ExchangeID) = 0;

	///获取合约信息
	virtual int GetInstrument(CThostInstrumentField &InstrumentInfo, string InstrumentID) = 0;

	///获取所有的合约信息
	virtual int GetAllInstrument(vector<CThostInstrumentField> &vecInstrumentInfo) = 0;

	///获取合约保证金
	virtual int GetInstrumentMarginRate(CThostInstrumentMarginRateField &InstrumentMarginRate, string InstrumentID) = 0;

	///获取合约手续费
	virtual int GetInstrumentCommissionRate(CThostInstrumentCommissionRateField &InstrumentCommissionRate, string InstrumentID) = 0;

	///获取账户资金
	virtual int GetTradingAccount(CThostTradingAccountField &TradingAccount) = 0;

	///获取用户持仓
	virtual int GetInvestorPosition(InvestorPositionVec &vecInvestorPosition, string InstrumentID) = 0;

	///获取订单信息
	virtual int GetOrder(OrderReportVec &vecOrderReport, string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd) = 0;

	///获取成交信息
	virtual int GetTrade(TradeVec  &vecTrade, string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd) = 0;

	///获取交易账户
	virtual string GetInvestorID() = 0;

	///获取交易日
	virtual int    GetTradingDay() = 0;

	///清空数据
	void ClearTradingData();

	///判断是否超出流控
	bool IsFlowControl(int nRet)
	{
		return nRet == -2 || nRet == -3;
	}

	///设置日志处理对象
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

protected:
	///交易账户
	string										m_sInvestorId;
	///交易密码
	string										m_sPasswd;
	///经纪ID
	string										m_sBrokerId;
	///产品信息
	string										m_sUserProductInfo;
	///前置机ID
	int											m_nFrontID;
	///会话ID
	int											m_nSessionID;
	///前置机地址
	string										m_sFrontAddress;
	///userdata路径
	string										m_sUserDataPath;
	///交易日
	int											m_nTradingDay;
	///请求ID
	int											m_nRequestId;
	///报单引用
	int											m_nOrderRef;
	///初始化状态
	bool										m_bInitialize;
	///请求类型表
	ReqTypeMap									m_mapReqType;

	///Thost订单回报表
	/// <FrontId|SessionId|OrderRef, CThostOrderReportField*>
	OrderReportMap								m_mapOrderRef;
	///交易所订单回报表
	/// <ExchangeId|OrderSysId, CThostOrderReportField*>
	OrderReportMap								m_mapExchangeOrder;
	///账户资金信息
	CThostTradingAccountField					m_TradingAccount;
	///用户持仓列表
	InvestorPositionVec							m_vecInvestorPosition;	
	///订单回报列表
	OrderReportVec								m_vecOrderReport;
	///成交信息列表
	TradeVec									m_vecTrade;
	///合约信息表
	InstrumentInfoMap							m_mapInstrumentInfo;
	///合约保证金表
	InstrumentMarginRateMap						m_mapInstrumentMarginRate;
	///合约手续费表
	InstrumentComissionRateMap					m_mapInstrumentComissionRate;
	///交易处理回调类
	CThostTraderHandlerSpi						*m_pTraderHandlerSpi;
	///日志对象
	CLogHandler									*m_pLogHandler;
};

#endif