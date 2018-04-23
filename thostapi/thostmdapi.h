/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapi.h
///@brief ����������ӿڵ�api
///@history 
///20160912@�쿡		�������ļ�
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
	///������ʼ��
	virtual int Start(string JsonParam = "") = 0;

	///�ȴ�
	virtual int Wait() = 0;

	///ֹͣ�����ڴ�
	virtual void Stop() = 0;

	///��������
	virtual void SubscribeQuote(char **ppInstrument, int count) = 0;

	///ע�����鴦��Ļص���
	void RegisterMdHandlerSpi(CThostMdHandlerSpi *pThostMdHandlerSpi);

	///���Ӷ����б�,���ڶ�������
	void AddSubscribeList(vector<string> vecSubscribeList);

	///���ĵ�����Լ
	void SubscribeQuote(string InstrumentId);

	///���ĺ�Լ�б�
	void SubscribeQuoteList(vector<string> InstrumentList);

	///���¶��ĸ��ݿ����еĺ�Լ
	void ReSubscribeQuoteBySnapshot();

	///��ȡ�������
	int GetMarketDataSnapshot(string InstrumentID, CThostDepthMarketDataField &DepthMarketDataSnapshot);

	///������־�������
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

protected:
	///�����˻�
	string											m_sInvestorId;
	///��������
	string											m_sPasswd;
	///����ID
	string											m_sBrokerId;
	///ǰ�û���ַ
	string											m_sFrontAddress;
	///userdata·��
	string											m_sUserDataPath;
	///����״̬
	bool											m_bConnected;
	///����ID
	int												m_nRequestId;
	///������ձ� 
	DepthMarketDataSnapshotMap						m_DepthMarketDataSnapshotMap;
	///�����б�
	vector<string>									m_vecSubscribeList;
	///���鴦��ص���
	CThostMdHandlerSpi								*m_pThostMdHandlerSpi;
	///��־����
	CLogHandler										*m_pLogHandler;
};

#endif