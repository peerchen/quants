/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file tickmanager.h
///@brief 定义了tick的类
///@history 
///20161123@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef INC_TICKMANAGER_H
#define INC_TICKMANAGER_H

#include "thostapistruct.h"
#include "../thostapi/thostmdapi.h"
#include "../thostapi/thosttradeapi.h"
#include "../misc/loghandler.h"
#include "../misc/servermodel.h"
#include "../misc/mysqlmanager.h"
#include <boost/thread.hpp>
#include <json/json.h>
#ifdef USE_REDIS
#include "../misc/redismanager.h"
#endif

class CTickManager : public CServerModel, CThostMdHandlerSpi, CThostTraderHandlerSpi {
public:
	CTickManager();

	~CTickManager();

public:
	///启动
	virtual int Start(int argc, char* argv[]);

	///释放
	void Release();

	///行情时间回调
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///ctp初始化完成
	virtual void OnRtnInitializeFinished(int ApiType);

private:
	///过滤不合法的tick
	bool FilterInvaildMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///生成行情json对象
	void MakeJsonDepthMarketData(CThostDepthMarketDataField *pDepthMarketData, Json::Value &jsonDepthMarketData);

	///生成合约信息的json对象
	void MakeJsonInstrumentInfo(CThostInstrumentField InstrumentInfo, Json::Value &jsonInstrumentInfo);

	///获取上一个交易日
	void GetLastTradingDay();

private:
	///ctp行情
	CThostMdApi							*m_pThostMdHandler;
	///ctp交易
	CThostTraderApi						*m_pTraderHandler;
	///飞鼠行情
	CThostMdApi							*m_pFSMdHandler;
	///日志处理
	CLogHandler							*m_pLogHandler;
	///交易日期yyyyMMdd
	int									m_nTradingDay;
	///上一个交易日
	int									m_nPrevTradingDay;
	///mysql管理
	CMysqlManager						m_mysqlManager;
	///是否启用黑名单
	int									m_nEnableBlackList;
	///黑名单列表
	vector<string>						m_vecBlackList;
	///白名单列表
	int									m_nEnableWhiteList;
	///白名单列表
	vector<string>						m_vecWhiteList;
	///kbar的锁
	boost::mutex						m_tickMutex;
	///合约信息表
	InstrumentInfoMap					m_mapInstrumentInfo;
	///最新时间戳映射
	map<string, CThostDataTimeStamp>	m_mapLastTimeStamp;
	///是否对KEY设置EXPIRE标志
	map<string, int>					m_mapSetKeyExpire;
	///过期时间
	int									m_expireSeconds;
	#ifdef USE_REDIS
	CRedisManager						*m_pRedisManager;
	#endif
};

#endif
