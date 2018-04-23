/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file kbarmanager.h
///@brief ������kbar�������
///@history 
///20160830@�쿡		�������ļ�
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
	///����
	virtual int Start(int argc, char* argv[]);

	///�ͷ�
	void Release();

public:
	///KBar�¼��ص�
	virtual void OnRtnKBar(CKBarField *pKBar);

	///����ʱ��ص�
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData);

	///��Լ״̬�ص�
	virtual void OnRtnInstrumentStatus(CThostInstrumentStatusField *pInstrumentStatus);

	///ctp��ʼ�����
	virtual void OnRtnInitializeFinished(int ApiType);

private:
	///��mysql
	int SaveKBarToSQL(CKBarField *pKBar);

	///������
	int SaveKBarDailyToSQL(CKBarDailyField *pKBarDaily);

	///��������
	void HandlerKBarDaily(CThostDepthMarketDataField *pDepthMarketData);

	///�����Լ����
	void CalcContractRank(string Symbol);

	///��ȡ��һ��������
	void GetLastTradingDay();

private:
	///kbar����
	CKBarHandler						*m_pKBarHandler;
	///ctp����
	CThostMdApi							*m_pThostMdHandler;
	///ctp����
	CThostTraderApi						*m_pTraderHandler;
	///��������
	CThostMdApi							*m_pFSMdHandler;
	///��־����
	CLogHandler							*m_pLogHandler;
	///mysql����
	CMysqlManager						m_mysqlManager;
	///��������yyyyMMdd
	int									m_nTradingDay;
	///��һ����������yyyyMMdd
	int									m_nPrevTradingDay;
	///����map
	KBarDailyMap						m_mapKBarDaily;
	///Ʒ�ֶ�Ӧ�ĺ�Լ����
	map<string, int>					m_mapSymbolCount;
	///Ʒ�ֶ�Ӧ��Լ���ۼƸ���
	map<string, int>					m_mapSymbolCountUpdate;
	///�Ƿ����ú�����
	int									m_nEnableBlackList;
	///�������б�
	vector<string>						m_vecBlackList;
	///�������б�
	int									m_nEnableWhiteList;
	///�������б�
	vector<string>						m_vecWhiteList;
	///kbar����
	boost::mutex						m_kbarMutex;
};

#endif