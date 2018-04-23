/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapi.h
///@brief fs行情实现类
///@history 
///20160816@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef INC_FSMDHANDLER_H
#define INC_FSMDHANDLER_H

#include "../thostmdapi.h"
#include "../../misc/loghandler.h"
#include <SgitFtdcUserApiDataType.h>
#include <SgitFtdcUserApiStruct.h>
#include <SgitFtdcMdApi.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

namespace fstech {
	class CFSMdHandler : public CThostMdApi, CThostFtdcMdSpi
	{
	public:
		CFSMdHandler();

		CFSMdHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath);

		~CFSMdHandler(){};

	public:
		///启动初始化
		int Start(string JsonParam = "");

		///等待
		int Wait();

		///停止销毁内存
		void Stop();

		///订阅行情
		void SubscribeQuote(char **ppInstrument, int count);

	protected:
		///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected();

		///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
		///@param nReason 错误原因
		///        0x1001 网络读失败
		///        0x1002 网络写失败
		///        0x2001 接收心跳超时
		///        0x2002 发送心跳失败
		///        0x2003 收到错误报文
		virtual void OnFrontDisconnected(int nReason);

		///登录请求响应
		virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///订阅行情应答
		virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///取消订阅行情应答
		virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

		///深度行情通知
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	private:
		///连接前置机请求
		int ReqConnectFront();

	private:
		///同步连接锁
		boost::mutex									m_connectedMutex;
		///同步连接条件变量
		boost::condition_variable						m_connectedCond;
		///ctp行情api
		CThostFtdcMdApi									*m_pMdApi;
	};
}

#endif