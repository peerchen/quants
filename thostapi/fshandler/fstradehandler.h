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
		int	   GetTradingDay();

	protected:
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected();

		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        0x1001 网络读失败
		///        0x1002 网络写失败
		///        0x2001 接收心跳超时
		///        0x2002 发送心跳失败
		virtual void OnFrontDisconnected(int nReason);

		///登录请求响应
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报单录入请求响应
		virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///报单操作请求响应
		virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约响应
		virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者结算结果响应
		virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询结算信息确认响应
		virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约保证金响应
		virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询合约手续费响应
		virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询资金账户响应
		virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者持仓响应
		virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者持仓明细响应
		virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者报单响应
		virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///请求查询投资者成交响应
		virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///合约状态通知
		virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

		///报单通知
		virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

		///成交通知
		virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	private:
		///请求连接前置机
		int ReqConnectFront();

		///请求登录
		int ReqUserLogin(char ReqType = THOST_RT_SYNC);

		///请求查询结算信息
		int ReqQrySettlementInfo(char ReqType = THOST_RT_SYNC);

		///请求查询结算确认
		int ReqQrySettlementInfoConfirm(char ReqType = THOST_RT_SYNC);

		///请求查询合约信息
		int ReqQryInstrument(char ReqType = THOST_RT_SYNC);

		///请求查询合约保证金
		int ReqQryInstrumentMarginRate(string InstrumentID = "", char HedgeFlag = THOST_HF_Speculation, char ReqType = THOST_RT_SYNC);

		///请求查询合约手续费
		int ReqQryInstrumentCommissionRate(string InstrumentID = "", char ReqType = THOST_RT_SYNC);

		///请求查询投资者资金账户
		int ReqQryTradingAccount(char ReqType = THOST_RT_SYNC);

		///请求查询投资者持仓
		int ReqQryInvestorPosition(string InstrumentID = "", char ReqType = THOST_RT_SYNC);

		///请求查询报单
		int ReqQryOrder(string InstrumentID, string ExchangeID, string OrderSysID, string InsertTimeStart, string InsertTimeEnd, char ReqType = THOST_RT_SYNC);

		///请求查询成交
		int ReqQryTrade(string InstrumentID, string ExchangeID, string TradeID, string TradeTimeStart, string TradeTimeEnd, char ReqType = THOST_RT_SYNC); 

		///生成报单录入的基础字段
		void MakeOrderInsertBase(CThostFtdcInputOrderField &reqInputOrderField, string InstrumentID, char Direction, char OffsetFlag, double LimitPrice, int Volume, char HedgeFlag = THOST_HF_Speculation);

		///生成thost报单引用关键字
		void MakeOrderRefKey(string &OrderRefKey, int FrontId, int SessionId, const char *pszOrderRef);

		///交易所合同编号关键字
		void MakeExchangeOrderKey(string &ExchangeOrderKey, const char *pszExchangeId, const char *pszOrderSysId);

		///根据报单录入生成报单回报 
		CThostOrderReportField* MakeOrderReport(CThostFtdcInputOrderField *pInputOrder);

		///根据交易所报单回报生成报单回报
		CThostOrderReportField* MakeOrderReport(CThostFtdcOrderField *pOrder);

		///报单录入请求
		string ReqOrderInsert(CThostFtdcInputOrderField &reqInputOrderField);

		///初始化报单
		void InitializeOrder();

	private:
		///同步请求的锁
		boost::mutex								m_SyncReqMutex;
		///同步请求的条件变量
		boost::condition_variable					m_SyncReqCondtion;
		///ctp trade api
		CThostFtdcTraderApi*						m_pTraderApi;
		///日志处理
		CLogHandler									*m_pLogHandler;
	};
}

#endif