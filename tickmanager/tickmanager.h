/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file tickmanager.h
///@brief ������tick����
///@history 
///20161123@�쿡		�������ļ�
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
	///����
	virtual int Start(int argc, char* argv[]);

	///�ͷ�
	void Release();

	///����ʱ��ص�
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///ctp��ʼ�����
	virtual void OnRtnInitializeFinished(int ApiType);

private:
	///���˲��Ϸ���tick
	bool FilterInvaildMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///��������json����
	void MakeJsonDepthMarketData(CThostDepthMarketDataField *pDepthMarketData, Json::Value &jsonDepthMarketData);

	///���ɺ�Լ��Ϣ��json����
	void MakeJsonInstrumentInfo(CThostInstrumentField InstrumentInfo, Json::Value &jsonInstrumentInfo);

	///��ȡ��һ��������
	void GetLastTradingDay();

private:
	///ctp����
	CThostMdApi							*m_pThostMdHandler;
	///ctp����
	CThostTraderApi						*m_pTraderHandler;
	///��������
	CThostMdApi							*m_pFSMdHandler;
	///��־����
	CLogHandler							*m_pLogHandler;
	///��������yyyyMMdd
	int									m_nTradingDay;
	///��һ��������
	int									m_nPrevTradingDay;
	///mysql����
	CMysqlManager						m_mysqlManager;
	///�Ƿ����ú�����
	int									m_nEnableBlackList;
	///�������б�
	vector<string>						m_vecBlackList;
	///�������б�
	int									m_nEnableWhiteList;
	///�������б�
	vector<string>						m_vecWhiteList;
	///kbar����
	boost::mutex						m_tickMutex;
	///��Լ��Ϣ��
	InstrumentInfoMap					m_mapInstrumentInfo;
	///����ʱ���ӳ��
	map<string, CThostDataTimeStamp>	m_mapLastTimeStamp;
	///�Ƿ��KEY����EXPIRE��־
	map<string, int>					m_mapSetKeyExpire;
	///����ʱ��
	int									m_expireSeconds;
	#ifdef USE_REDIS
	CRedisManager						*m_pRedisManager;
	#endif
};

#endif
