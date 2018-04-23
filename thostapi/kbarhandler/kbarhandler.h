/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file kbarhandler.h
///@brief ����kbar���ɵ���
///@history 
///20160818@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_KBARHANDLER_H
#define INC_KBARHANDLER_H

#include "thostapistruct.h"
#include "../../misc/loghandler.h"

class CKBarHandler {
public:
	CKBarHandler();

	~CKBarHandler();

public:
	///�����ڴ�
	void Release();

	///ע��KBar�¼��ص�����
	void RegisterKBarHandlerSpi(CKBarHandlerSpi *pKBarHandlerSpi);

	///ע��KBar�¼�
	int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0);

	///ȡ��ע��kbar�¼�
	int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0);

	///ʵʱ�����¼�
	void  MarketDataEvent(CThostDepthMarketDataField *pDepthMarketData);

	///�����¼�
	void  TradingClosedEvent(string Symbol, string ExchangeID, int CloseTime);

	///���ý�����������һ��������
	void  SetTradingDay(int nTradingDay, int nPreTradingDay);

	///���ú�Լ��
	void  SetInstrumentMap(InstrumentInfoMap mapInstrument);

	///���ú�Լ����,ת��map
	void  SetInstrumentVec(vector<CThostInstrumentField> vecInstrument);

	///������־�������
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

	///���ô洢Ticks
	void SetDumpTicks(int nDumpTicks)
	{
		m_nDumpTicks = nDumpTicks;
	}

private:
	///����kbar�¼�
	void HandlerKBarEvent(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///�״θ���kbar
	void UpdateNullKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///�����Ѿ�open��kbar
	void UpdateOpenKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///�رղ�����kbar
	void CloseResetKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///��������������ֵ��ʱ������ȡ��
	///��:5���� 103305����Ϊ103000; 1����103151����Ϊ103100; 5�� 103003����Ϊ103000
	int FloorTime(char PeriodType, int PeriodValue, int nTime);

private:
	///kbar�ص�����
	CKBarHandlerSpi								*m_pKBarHandlerSpi;
	///<Instrument, vector<CKBarField*>>
	KBarMap										m_mapKBar;
	///��Լ��
	InstrumentInfoMap							m_mapInstrumentInfo;
	///��ǰ��������
	int											m_nTradingDay;
	//ǰһ����������
	int											m_nPrevTradingDay;
	///����ʱ���ӳ��
	map<string, CThostDataTimeStamp>			m_mapLastTimeStamp;
	///��־�������
	CLogHandler									*m_pLogHandler;
	///�Ƿ�洢����tick
	int											m_nDumpTicks;
};

#endif