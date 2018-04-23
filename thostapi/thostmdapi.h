/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapi.h
///@brief 定义了行情接口的api
///@history 
///20160912@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTMDAPI_H
#define INC_THOSTMDAPI_H

#include "thostapistruct.h"
#include "../misc/loghandler.h"

class CThostMdApi{
public:
	CThostMdApi();

	CThostMdApi(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath);

	virtual ~CThostMdApi(){};

public:
	///启动初始化
	virtual int Start(string JsonParam = "") = 0;

	///等待
	virtual int Wait() = 0;

	///停止销毁内存
	virtual void Stop() = 0;

	///订阅行情
	virtual void SubscribeQuote(char **ppInstrument, int count) = 0;

	///注册行情处理的回调类
	void RegisterMdHandlerSpi(CThostMdHandlerSpi *pThostMdHandlerSpi);

	///增加订阅列表,用于断线重连
	void AddSubscribeList(vector<string> vecSubscribeList);

	///订阅单个合约
	void SubscribeQuote(string InstrumentId);

	///订阅合约列表
	void SubscribeQuoteList(vector<string> InstrumentList);

	///重新订阅根据快照中的合约
	void ReSubscribeQuoteBySnapshot();

	///获取行情快照
	int GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot);

	///设置日志处理对象
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

protected:
	///交易账户
	string											m_sInvestorId;
	///交易密码
	string											m_sPasswd;
	///经纪ID
	string											m_sBrokerId;
	///前置机地址
	string											m_sFrontAddress;
	///userdata路径
	string											m_sUserDataPath;
	///连接状态
	bool											m_bConnected;
	///请求ID
	int												m_nRequestId;
	///行情快照表 
	DepthMarketDataSnapshotMap						m_DepthMarketDataSnapshotMap;
	///订阅列表
	vector<string>									m_vecSubscribeList;
	///行情处理回调类
	CThostMdHandlerSpi								*m_pThostMdHandlerSpi;
	///日志对象
	CLogHandler										*m_pLogHandler;
};

#endif