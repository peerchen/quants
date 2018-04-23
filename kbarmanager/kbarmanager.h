/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file kbarmanager.h
///@brief 定义了kbar管理的类
///@history 
///20160830@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef INC_KBARMANAGER_H
#define INC_KBARMANAGER_H

#include "thostapistruct.h"
#include "../thostapi/thostmdapi.h"
#include "../thostapi/thosttradeapi.h"
#include "../misc/loghandler.h"
#include "../misc/mysqlmanager.h"
#include "../misc/servermodel.h"
#include "../thostapi/kbarhandler/kbarhandler.h"
#include <boost/thread.hpp>

struct CInstrumentRankItem{
	int RankKey;
	string InstrumentID;
	bool operator<(const CInstrumentRankItem &item)const
	{  
		return RankKey > item.RankKey;
	}
};

class CKBarManager : public CServerModel, CKBarHandlerSpi, CThostMdHandlerSpi, CThostTraderHandlerSpi {
public:
	CKBarManager();

	~CKBarManager();

public:
	///启动
	virtual int Start(int argc, char* argv[]);

	///释放
	void Release();

public:
	///KBar事件回调
	virtual void OnRtnKBar(CKBarField *pKBar);

	///行情时间回调
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///合约状态回调
	virtual void OnRtnInstrumentStatus(CThostInstrumentStatusField *pInstrumentStatus);

	///ctp初始化完成
	virtual void OnRtnInitializeFinished(int ApiType);

private:
	///存mysql
	int SaveKBarToSQL(CKBarField *pKBar);

	///存日线
	int SaveKBarDailyToSQL(CKBarDailyField *pKBarDaily);

	///处理日线
	void HandlerKBarDaily(CThostDepthMarketDataField *pDepthMarketData);

	///计算合约排名
	void CalcContractRank(string Symbol);

	///获取上一个交易日
	void GetLastTradingDay();

private:
	///kbar处理
	CKBarHandler						*m_pKBarHandler;
	///ctp行情
	CThostMdApi							*m_pThostMdHandler;
	///ctp交易
	CThostTraderApi						*m_pTraderHandler;
	///飞鼠行情
	CThostMdApi							*m_pFSMdHandler;
	///日志处理
	CLogHandler							*m_pLogHandler;
	///mysql管理
	CMysqlManager						m_mysqlManager;
	///交易日期yyyyMMdd
	int									m_nTradingDay;
	///上一个交易日期yyyyMMdd
	int									m_nPrevTradingDay;
	///日线map
	KBarDailyMap						m_mapKBarDaily;
	///品种对应的合约个数
	map<string, int>					m_mapSymbolCount;
	///品种对应合约的累计个数
	map<string, int>					m_mapSymbolCountUpdate;
	///是否启用黑名单
	int									m_nEnableBlackList;
	///黑名单列表
	vector<string>						m_vecBlackList;
	///白名单列表
	int									m_nEnableWhiteList;
	///白名单列表
	vector<string>						m_vecWhiteList;
	///kbar的锁
	boost::mutex						m_kbarMutex;
};

#endif