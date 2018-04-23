/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapi.h
///@brief ctp����ʵ����
///@history 
///20160816@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTMDHANDLER_H
#define INC_THOSTMDHANDLER_H

#include "../thostmdapi.h"
#include "../../misc/loghandler.h"
#include <ThostFtdcUserApiDataType.h>
#include <ThostFtdcUserApiStruct.h>
#include <ThostFtdcMdApi.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

class CThostMdHandler : public CThostMdApi, CThostFtdcMdSpi {
public:
	CThostMdHandler();

	CThostMdHandler(string InvestorId, string Password, string BrokerId, string FrontAddress, string UserDataPath);

	~CThostMdHandler(){};


public:
	///������ʼ��
	int Start(string JsonParam = "");

	///�ȴ�
	int Wait();

	///ֹͣ�����ڴ�
	void Stop();

	///��������
	void SubscribeQuote(char **ppInstrument, int count);

protected:

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷���������
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	///����ǰ�û�����
	int ReqConnectFront();

private:
	///ͬ��������
	boost::mutex									m_connectedMutex;
	///ͬ��������������
	boost::condition_variable						m_connectedCond;
	///ctp����api
	CThostFtdcMdApi									*m_pMdApi;
};

#endif