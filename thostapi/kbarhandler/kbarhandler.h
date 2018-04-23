/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file kbarhandler.h
///@brief 定义kbar生成的类
///@history 
///20160818@徐俊		创建该文件
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
	///清理内存
	void Release();

	///注册KBar事件回调函数
	void RegisterKBarHandlerSpi(CKBarHandlerSpi *pKBarHandlerSpi);

	///注册KBar事件
	int  RegisterKBarEvent(string InstrumentID, string Symbol, string ExchangeID, char PeriodType, int PeriodValue, int Offset = 0);

	///取消注册kbar事件
	int  UnRegisterKBarEvent(string InstrumentID, char PeriodType, int PeriodValue, int Offset = 0);

	///实时行情事件
	void  MarketDataEvent(CThostDepthMarketDataField *pDepthMarketData);

	///收盘事件
	void  TradingClosedEvent(string Symbol, string ExchangeID, int CloseTime);

	///设置交易日期与上一个交易日
	void  SetTradingDay(int nTradingDay, int nPreTradingDay);

	///设置合约表
	void  SetInstrumentMap(InstrumentInfoMap mapInstrument);

	///设置合约数组,转成map
	void  SetInstrumentVec(vector<CThostInstrumentField> vecInstrument);

	///设置日志处理对象
	void SetLogHandler(CLogHandler *pLogHandler)
	{
		m_pLogHandler = pLogHandler;
	}

	///设置存储Ticks
	void SetDumpTicks(int nDumpTicks)
	{
		m_nDumpTicks = nDumpTicks;
	}

private:
	///处理kbar事件
	void HandlerKBarEvent(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///首次更新kbar
	void UpdateNullKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///更新已经open的kbar
	void UpdateOpenKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///关闭并重置kbar
	void CloseResetKBar(CKBarField *pKBarField, CThostDepthMarketDataField *pDepthMarketData);

	///根据秒或分钟周期值将时间向下取整
	///例:5分钟 103305调整为103000; 1分钟103151调整为103100; 5秒 103003调整为103000
	int FloorTime(char PeriodType, int PeriodValue, int nTime);

private:
	///kbar回调对象
	CKBarHandlerSpi								*m_pKBarHandlerSpi;
	///<Instrument, vector<CKBarField*>>
	KBarMap										m_mapKBar;
	///合约表
	InstrumentInfoMap							m_mapInstrumentInfo;
	///当前交易日期
	int											m_nTradingDay;
	//前一个交易日期
	int											m_nPrevTradingDay;
	///最新时间戳映射
	map<string, CThostDataTimeStamp>			m_mapLastTimeStamp;
	///日志处理对象
	CLogHandler									*m_pLogHandler;
	///是否存储所有tick
	int											m_nDumpTicks;
};

#endif