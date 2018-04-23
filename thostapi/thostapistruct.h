/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file thostapistruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///20160818@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef INC_THOSTAPISTRUCT_H
#define INC_THOSTAPISTRUCT_H

#include <string>
#include <map>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>

using namespace std;

#define LOG_LEVEL_INFO	1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_DEBUG 4

///回测模式
#define THOST_API_MODE_BACKTEST		1
///实盘交易模式
#define THOST_API_MODE_TRADING		2

///ctp
#define THOST_API_TYPE_CTP			1
///fs
#define THOST_API_TYPE_FS			2

///Direction 买卖方向
///多
#define THOST_D_Long	'0'
///空
#define THOST_D_Short	'1'

///OF 开平仓
///开仓
#define THOST_OF_Open '0'
///平仓
#define THOST_OF_Close '1'
///强平
#define THOST_OF_ForceClose '2'
///平今
#define THOST_OF_CloseToday '3'
///平昨
#define THOST_OF_CloseYesterday '4'
///强减
#define THOST_OF_ForceOff '5'
///本地强平
#define THOST_OF_LocalForceClose '6'

///HedgeFlag是一个投机套保标志类型
///投机
#define THOST_HF_Speculation '1'
///套利
#define THOST_HF_Arbitrage '2'
///套保
#define THOST_HF_Hedge '3'

///VolumeCondition
///任何数量
#define THOST_VC_AV '1'
///最小数量
#define THOST_VC_MV '2'
///全部数量
#define THOST_VC_CV '3'

///TimeCondition
///立即完成，否则撤销
#define THOST_TC_IOC '1'
///本节有效
#define THOST_TC_GFS '2'
///当日有效
#define THOST_TC_GFD '3'
///指定日期前有效
#define THOST_TC_GTD '4'
///撤销前有效
#define THOST_TC_GTC '5'
///集合竞价有效
#define THOST_TC_GFA '6'

///OrderStatus
///broker接收订单
#define THOST_OST_Recieved 'a'
///交易所接收订单
#define THOST_OST_Submitted '3'
///部分成交
#define THOST_OST_Filled '1'
///完全成交
#define THOST_OST_Complete '0'
///撤单成功
#define THOST_OST_Canceled '5'
///正在撤单
#define THOST_OST_Canceling '6'
///撤单失败
#define THOST_OST_CancelFailed '7'
///错单
#define THOST_OST_Error '8'


///OrderPriceType
///任意价
#define THOST_OPT_AnyPrice '1'
///限价
#define THOST_OPT_LimitPrice '2'
///最优价
#define THOST_OPT_BestPrice '3'
///最新价
#define THOST_OPT_LastPrice '4'
///最新价浮动上浮1个ticks
#define THOST_OPT_LastPricePlusOneTicks '5'
///最新价浮动上浮2个ticks
#define THOST_OPT_LastPricePlusTwoTicks '6'
///最新价浮动上浮3个ticks
#define THOST_OPT_LastPricePlusThreeTicks '7'
///卖一价
#define THOST_OPT_AskPrice1 '8'
///卖一价浮动上浮1个ticks
#define THOST_OPT_AskPrice1PlusOneTicks '9'
///卖一价浮动上浮2个ticks
#define THOST_OPT_AskPrice1PlusTwoTicks 'A'
///卖一价浮动上浮3个ticks
#define THOST_OPT_AskPrice1PlusThreeTicks 'B'
///买一价
#define THOST_OPT_BidPrice1 'C'
///买一价浮动上浮1个ticks
#define THOST_OPT_BidPrice1PlusOneTicks 'D'
///买一价浮动上浮2个ticks
#define THOST_OPT_BidPrice1PlusTwoTicks 'E'
///买一价浮动上浮3个ticks
#define THOST_OPT_BidPrice1PlusThreeTicks 'F'
///五档价
#define THOST_OPT_FiveLevelPrice 'G'


///ContingentCondition
///立即
#define THOST_CC_Immediately '1'
///止损
#define THOST_CC_Touch '2'
///止赢
#define THOST_CC_TouchProfit '3'
///预埋单
#define THOST_CC_ParkedOrder '4'
///最新价大于条件价
#define THOST_CC_LastPriceGreaterThanStopPrice '5'
///最新价大于等于条件价
#define THOST_CC_LastPriceGreaterEqualStopPrice '6'
///最新价小于条件价
#define THOST_CC_LastPriceLesserThanStopPrice '7'
///最新价小于等于条件价
#define THOST_CC_LastPriceLesserEqualStopPrice '8'
///卖一价大于条件价
#define THOST_CC_AskPriceGreaterThanStopPrice '9'
///卖一价大于等于条件价
#define THOST_CC_AskPriceGreaterEqualStopPrice 'A'
///卖一价小于条件价
#define THOST_CC_AskPriceLesserThanStopPrice 'B'
///卖一价小于等于条件价
#define THOST_CC_AskPriceLesserEqualStopPrice 'C'
///买一价大于条件价
#define THOST_CC_BidPriceGreaterThanStopPrice 'D'
///买一价大于等于条件价
#define THOST_CC_BidPriceGreaterEqualStopPrice 'E'
///买一价小于条件价
#define THOST_CC_BidPriceLesserThanStopPrice 'F'
///买一价小于等于条件价
#define THOST_CC_BidPriceLesserEqualStopPrice 'H'

///ForceCloseReason
///非强平
#define THOST_FCC_NotForceClose '0'
///资金不足
#define THOST_FCC_LackDeposit '1'
///客户超仓
#define THOST_FCC_ClientOverPositionLimit '2'
///会员超仓
#define THOST_FCC_MemberOverPositionLimit '3'
///持仓非整数倍
#define THOST_FCC_NotMultiple '4'
///违规
#define THOST_FCC_Violation '5'
///其它
#define THOST_FCC_Other '6'
///自然人临近交割
#define THOST_FCC_PersonDeliv '7'


///ProductClass
///期货
#define THOST_PC_Futures '1'
///期货期权
#define THOST_PC_Options '2'
///组合
#define THOST_PC_Combination '3'
///即期
#define THOST_PC_Spot '4'
///期转现
#define THOST_PC_EFP '5'
///现货期权
#define THOST_PC_SpotOption '6'

///InstLifePhase
///未上市
#define THOST_FTDC_IP_NotStart '0'
///上市
#define THOST_FTDC_IP_Started '1'
///停牌
#define THOST_FTDC_IP_Pause '2'
///到期
#define THOST_FTDC_IP_Expired '3'

///MaxMarginSideAlgorithm
///不使用大额单边保证金算法
#define THOST_FTDC_MMSA_NO '0'
///使用大额单边保证金算法
#define THOST_FTDC_MMSA_YES '1'

///同步请求
#define THOST_RT_SYNC	    '0'
///异步请求
#define THOST_RT_ASYNC      '1'
///异步初始化请求
#define THOST_RT_ASYNC_INIT '2'

///流控超时时间(毫秒)
#define THOST_FLOWCTL_TIMEOUT_MS		200
///同步请求等待超时时间(秒)
#define THOST_SYNC_RQ_TIMEOUT_S			10
///断开连接睡眠时间
#define THOST_DISCONNECT_SLEEP_TIME_S	5

///PositionDate
///今日持仓
#define THOST_PSD_Today		'1'
///历史持仓
#define THOST_PSD_History	'2'
///所有持仓
#define THOST_PSD_ALL		'3'

///PosiDirection
///净
#define THOST_PD_Net '1'
///多头
#define THOST_PD_Long '2'
///空头
#define THOST_PD_Short '3'


///InstrumentStatusType是一个合约交易状态类型
///开盘前
#define THOST_IS_BeforeTrading '0'
///非交易
#define THOST_IS_NoTrading '1'
///连续交易
#define THOST_IS_Continous '2'
///集合竞价报单
#define THOST_IS_AuctionOrdering '3'
///集合竞价价格平衡
#define THOST_IS_AuctionBalance '4'
///集合竞价撮合
#define THOST_IS_AuctionMatch '5'
///收盘
#define THOST_IS_Closed '6'

///深度行情
struct CThostDepthMarketDataField
{
	///合约代码
	string				InstrumentID;
	///交易所代码
	string				ExchangeID;
	///最新价
	double				LastPrice;
	///上次结算价
	double				PreSettlementPrice;
	///昨收盘
	double				PreClosePrice;
	///昨持仓量
	double				PreOpenInterest;
	///今开盘
	double				OpenPrice;
	///最高价
	double				HighestPrice;
	///最低价
	double				LowestPrice;
	///数量
	int					Volume;
	///成交金额
	double				Turnover;
	///持仓量
	double				OpenInterest;
	///今收盘
	double				ClosePrice;
	///本次结算价
	double				SettlementPrice;
	///涨停板价
	double				UpperLimitPrice;
	///跌停板价
	double				LowerLimitPrice;
	///昨虚实度
	double				PreDelta;
	///今虚实度
	double				CurrDelta;
	///最后修改时间
	int					UpdateTime;
	///最后修改毫秒
	int					UpdateMillisec;
	///申买价一
	double				BidPrice1;
	///申买量一
	int					BidVolume1;
	///申卖价一
	double				AskPrice1;
	///申卖量一
	int					AskVolume1;
	///申买价二
	double				BidPrice2;
	///申买量二
	int					BidVolume2;
	///申卖价二
	double				AskPrice2;
	///申卖量二
	int					AskVolume2;
	///申买价三
	double				BidPrice3;
	///申买量三
	int					BidVolume3;
	///申卖价三
	double				AskPrice3;
	///申卖量三
	int					AskVolume3;
	///申买价四
	double				BidPrice4;
	///申买量四
	int					BidVolume4;
	///申卖价四
	double				AskPrice4;
	///申卖量四
	int					AskVolume4;
	///申买价五
	double				BidPrice5;
	///申买量五
	int					BidVolume5;
	///申卖价五
	double				AskPrice5;
	///申卖量五
	int					AskVolume5;
	///当日均价
	double				AveragePrice;
	///业务日期
	int					ActionDay;
	///交易日
	int					TradingDay;
};

///订单请求
struct CThostOrderRequestField {
	///经纪公司代码
	string					BrokerID;
	///投资者代码
	string					InvestorID;
	///合约代码
	string					InstrumentID;
	///报单引用
	string					OrderRef;
	///用户代码
	string					UserID;
	///报单价格条件
	char					OrderPriceType;
	///买卖方向
	char					Direction;
	///组合开平标志
	string					CombOffsetFlag;
	///组合投机套保标志
	string					CombHedgeFlag;
	///价格
	double					LimitPrice;
	///数量
	int						VolumeTotalOriginal;
	///有效期类型
	char					TimeCondition;
	///GTD日期
	string					GTDDate;
	///成交量类型
	char					VolumeCondition;
	///最小成交量
	int						MinVolume;
	///触发条件
	char					ContingentCondition;
	///止损价
	double					StopPrice;
	///强平原因
	char					ForceCloseReason;
	///自动挂起标志
	int						IsAutoSuspend;
	///业务单元
	string					BusinessUnit;
	///请求编号
	int						RequestID;
	///用户强评标志
	int						UserForceClose;
	///互换单标志
	int						IsSwapOrder;
};

///订单回报
struct CThostOrderReportField : public CThostOrderRequestField {
	///内部生成用来标识唯一订单
	long						OrderId;	
	///报单编号
	string						OrderSysID;
	///交易所代码
	string						ExchangeID;
	///已成交数量
	int							FilledVolume;
	///最近一笔成交的数量
	int							LastFilledVolume;
	///最近一笔成交的价格
	double						LastFilledPrice;
	///成交均价
	double						AveragePrice;
	///报单状态
	char						OrderStatus;
	///交易日
	string						TradingDay;
	///报单日期
	string						InsertDate;
	///委托时间
	string						InsertTime;
	///最近成交时间				
	string						LastFilledTime;
	///撤销时间
	string						CancelTime;
	///前置编号
	int							FrontID;
	///会话编号
	int							SessionID;
	///用户端产品信息
	string						UserProductInfo;
	///状态信息
	string						StatusMsg;

	bool operator < (const CThostOrderReportField &orderReportField) const 
	{
		if (strcmp(InsertTime.c_str(), orderReportField.InsertTime.c_str()) < 0)
		{
			return true;
		}

		if (strcmp(InsertTime.c_str(), orderReportField.InsertTime.c_str())> 0)
		{
			return false;
		}

		if (strcmp(OrderSysID.c_str(), orderReportField.OrderSysID.c_str()) <= 0)
		{
			return true;
		}
		return false;
	}
};

struct CThostTradeField {
	///经纪公司代码
	string						BrokerID;
	///投资者代码
	string						InvestorID;
	///合约代码
	string						InstrumentID;
	///报单引用
	string						OrderRef;
	///用户代码
	string						UserID;
	///交易所代码
	string						ExchangeID;
	///成交编号
	string						TradeID;
	///买卖方向
	char						Direction;
	///报单编号
	string						OrderSysID;
	///开平标志
	char						OffsetFlag;
	///投机套保标志
	char						HedgeFlag;
	///价格
	double						Price;
	///数量
	int							Volume;
	///成交时期
	string						TradeDate;
	///成交时间
	string						TradeTime;
	///交易日
	string						TradingDay;
	///结算编号
	string						SettlementID;
	///经纪公司报单编号
	string						BrokerOrderSeq;

	bool operator < (const CThostTradeField &tradeField) const 
	{ 
		if (TradeID.length() < tradeField.TradeID.length())
		{
			return true;
		}

		if (TradeID.length() > tradeField.TradeID.length())
		{
			return false;
		}

		if (strcmp(TradeID.c_str(), tradeField.TradeID.c_str()) <= 0)
		{
			return true;
		}
		return false;
	}
};

///资金账户
struct CThostTradingAccountField
{
	///经纪公司代码
	string						BrokerID;
	///投资者帐号
	string						AccountID;
	///上次结算准备金
	double						PreBalance;
	///上次占用的保证金
	double						PreMargin;
	///入金金额
	double						Deposit;
	///出金金额
	double						Withdraw;
	///冻结的保证金
	double						FrozenMargin;
	///冻结的资金
	double						FrozenCash;
	///冻结的手续费
	double						FrozenCommission;
	///当前保证金总额
	double						CurrMargin;
	///资金差额
	double						CashIn;
	///手续费
	double						Commission;
	///平仓盈亏
	double						CloseProfit;
	///持仓盈亏
	double						PositionProfit;
	///期货结算准备金
	double						Balance;
	///可用资金
	double						Available;
	///可取资金
	double						WithdrawQuota;
	///交易日
	int							TradingDay;
};

struct CThostInvestorPositionField {
	///合约代码
	string						InstrumentID;
	///经纪公司代码
	string						BrokerID;
	///投资者代码
	string						InvestorID;
	///持仓多空方向
	char						PosiDirection;
	///投机套保标志
	char						HedgeFlag;
	///持仓日期
	char						PositionDate;
	///总持仓
	int							Position;
	///上日持仓
	int							YdPosition;
	///今日持仓
	int							TodayPosition;
	///多头冻结
	int							LongFrozen;
	///空头冻结
	int							ShortFrozen;
	///开仓冻结金额
	double						LongFrozenAmount;
	///开仓冻结金额
	double						ShortFrozenAmount;
	///开仓量
	int							OpenVolume;
	///平仓量
	int							CloseVolume;
	///开仓金额
	double						OpenAmount;
	///平仓金额
	double						CloseAmount;
	///持仓成本
	double						PositionCost;
	///上次占用的保证金
	double						PreMargin;
	///占用的保证金
	double						UseMargin;
	///冻结的保证金
	double						FrozenMargin;
	///冻结的资金
	double						FrozenCash;
	///冻结的手续费
	double						FrozenCommission;
	///资金差额
	double						CashIn;
	///手续费
	double						Commission;
	///平仓盈亏
	double						CloseProfit;
	///持仓盈亏
	double						PositionProfit;
	///上次结算价
	double						PreSettlementPrice;
	///本次结算价
	double						SettlementPrice;
	///交易日
	string						TradingDay;
	///结算编号
	int							SettlementID;
	///开仓成本
	double						OpenCost;
	///交易所保证金
	double						ExchangeMargin;
	///逐日盯市平仓盈亏
	double						CloseProfitByDate;
	///逐笔对冲平仓盈亏
	double						CloseProfitByTrade;
};

///合约
struct CThostInstrumentField
{
	///合约代码
	string						InstrumentID;
	///交易所代码
	string						ExchangeID;
	///合约名称
	string						InstrumentName;
	///产品代码
	string						ProductID;
	///产品类型
	char						ProductClass;
	///交割年份
	int							DeliveryYear;
	///交割月
	int							DeliveryMonth;
	///市价单最大下单量
	int							MaxMarketOrderVolume;
	///市价单最小下单量
	int							MinMarketOrderVolume;
	///限价单最大下单量
	int							MaxLimitOrderVolume;
	///限价单最小下单量
	int							MinLimitOrderVolume;
	///合约数量乘数
	int							VolumeMultiple;
	///最小变动价位
	double						PriceTick;
	///创建日
	string						CreateDate;
	///上市日
	string						OpenDate;
	///到期日
	string						ExpireDate;
	///开始交割日
	string						StartDelivDate;
	///结束交割日
	string						EndDelivDate;
	///合约生命周期状态
	char						InstLifePhase;
	///当前是否交易
	int							IsTrading;
	///多头保证金率
	double						LongMarginRatio;
	///空头保证金率
	double						ShortMarginRatio;
	///是否使用大额单边保证金算法
	char						MaxMarginSideAlgorithm;
};

///合约保证金率
struct CThostInstrumentMarginRateField
{
	///合约代码
	string						InstrumentID;
	///经纪公司代码
	string						BrokerID;
	///投资者代码
	string						InvestorID;
	///投机套保标志
	char						HedgeFlag;
	///多头保证金率
	double						LongMarginRatioByMoney;
	///多头保证金费
	double						LongMarginRatioByVolume;
	///空头保证金率
	double						ShortMarginRatioByMoney;
	///空头保证金费
	double						ShortMarginRatioByVolume;
	///是否相对交易所收取
	int							IsRelative;
};

///合约手续费率
struct CThostInstrumentCommissionRateField
{
	///合约代码
	string						InstrumentID;
	///经纪公司代码
	string						BrokerID;
	///投资者代码
	string						InvestorID;
	///开仓手续费率
	double						OpenRatioByMoney;
	///开仓手续费
	double						OpenRatioByVolume;
	///平仓手续费率
	double						CloseRatioByMoney;
	///平仓手续费
	double						CloseRatioByVolume;
	///平今手续费率
	double						CloseTodayRatioByMoney;
	///平今手续费
	double						CloseTodayRatioByVolume;
};

///合约状态
struct CThostInstrumentStatusField
{
	///交易所代码
	string						ExchangeID;
	///合约在交易所的代码
	string						ExchangeInstID;
	///合约代码
	string						InstrumentID;
	///合约交易状态
	char						InstrumentStatus;
	///进入本状态时间
	string						EnterTime;
};

///回测参数
struct CThostBackTestParamsField
{
	///合约列表
	vector<string>									InstrumentIDList;
	///开始回测日期
	int												StartDate;
	///结束回测日期			
	int												EndDate;
	///回测数据类型
	char											PeriodType;
	///回测数据周期值
	int												PeriodValue;
	///初始资金	
	double											InitialBalance;
	///合约信息
	vector<CThostInstrumentField>					InstrumentInfoList;
	///合约保证金列表
	vector<CThostInstrumentMarginRateField>			InstrumentMarginRateList;
	///合约手续费列表
	vector<CThostInstrumentCommissionRateField>		InstrumentCommissionRateList;
	///数据库地址
	string											SqlHost;
	///数据库端口
	int												SqlPort;
	///数据库用户
	string											SqlUser;
	///数据库密码
	string											SqlPassword;
	///数据库
	string											SqlDB;
};

///回测指标
struct CThostStrategyEvalResultField
{
	///策略年化收益率
	double						AnnualizedReturns;
	///参考标准年化收益率
	double						BenchmarkReturns;
	///阿尔法
	double						Alpha;
	///贝塔
	double						Beta;
	///策略收益波动率
	double						Volatility;
	///夏普比率
	double						SharpRatio;
	///信息比率
	double						InformationRatio;
	///最大回撤
	double						MaxDrawDown;
	///胜率
	double						WinRatio;
	///最大连续上涨天数
	double						MaxContinuousWinDays;
	///最大连续 下跌天数
	double						MaxContinuousLossDays;
};

struct CThostDataTimeStamp {
	///自然日yyMMdd
	int		date;
	///时间 HHmmss
	int		time;
	///毫秒
	int		millisec;
	///重载大于号
	bool operator > (const CThostDataTimeStamp &datatimestamp) const 
	{ 
		if (this->date > datatimestamp.date)
		{
			return true;
		}

		if (this->date == datatimestamp.date)
		{
			if (this->time > datatimestamp.time)
			{
				return true;
			}

			if (this->time == datatimestamp.time)
			{
				if (this->millisec > datatimestamp.millisec)
				{
					return true;
				}
			}
		}
		return false;
	}
};

///THOST_ENDMASK_*
///未结束
#define THOST_ENDMASK_NOEND			1
///当天交易日结束
#define THOST_ENDMASK_TRADINGDAYEND	2
///所有的数据结束
#define	THOST_ENDMASK_ALLEND		4


//THOST_EXCHAGEID_*
#define THOST_EXCHANGEID_CFFEX	"CFFEX"
#define THOST_EXCHANGEID_SHFE	"SHFE"
#define THOST_EXCHANGEID_DCE	"DCE"
#define THOST_EXCHANGEID_CZCE	"CZCE"
#define THOST_EXCHANGEID_SGE	"SGE"


//THOST_SECTIONTIME_*
#define THOST_SECTIONTIME_090000 90000
#define THOST_SECTIONTIME_091500 91500
#define THOST_SECTIONTIME_093000 93000
#define THOST_SECTIONTIME_101500 101500
#define THOST_SECTIONTIME_103000 103000
#define THOST_SECTIONTIME_113000 113000
#define THOST_SECTIONTIME_130000 130000
#define THOST_SECTIONTIME_133000 133000
#define THOST_SECTIONTIME_145900 145900
#define THOST_SECTIONTIME_150000 150000
#define THOST_SECTIONTIME_151500 151500
#define THOST_SECTIONTIME_153000 153000

#define THOST_SECTIONTIME_200000 200000
#define THOST_SECTIONTIME_210000 210000
#define THOST_SECTIONTIME_230000 230000
#define THOST_SECTIONTIME_233000 233000
#define THOST_SECTIONTIME_240000 240000
#define THOST_SECTIONTIME_000000 0
#define THOST_SECTIONTIME_010000 10000
#define THOST_SECTIONTIME_023000 23000

#define THOST_OFFSETTIME_030000 30000

#define THOST_EXCEPTIONPRICE_MAX 10000000
#define THOST_EXCEPTIONPRICE_MIN -10000000

inline void FixExceptionPrice(double &price, double fixedPrice = 0)
{
	if (price > THOST_EXCEPTIONPRICE_MAX || price < THOST_EXCEPTIONPRICE_MIN)
	{
		price = fixedPrice;
	}
}

inline bool THOST_IS_DAYTRADINGTIME(string symbol, string exchangeid, int time)
{
	if (exchangeid == THOST_EXCHANGEID_SGE)
	{
		if ((time >= THOST_SECTIONTIME_090000 && time <= THOST_SECTIONTIME_113000) || 
			(time >= THOST_SECTIONTIME_133000 && time <= THOST_SECTIONTIME_153000))
		{
			return true;
		}
	}
	else if (exchangeid == THOST_EXCHANGEID_CFFEX)
	{
		if (symbol == "T" || symbol == "TF")
		{
			if ((time >= THOST_SECTIONTIME_091500 && time <= THOST_SECTIONTIME_113000) || 
				(time >= THOST_SECTIONTIME_130000 && time <= THOST_SECTIONTIME_151500))
			{
				return true;
			}
		}
		else
		{
			if ((time >= THOST_SECTIONTIME_093000 && time <= THOST_SECTIONTIME_113000) || 
				(time >= THOST_SECTIONTIME_130000 && time <= THOST_SECTIONTIME_150000))
			{
				return true;
			}
		}

	} 
	else
	{
		if ((time >= THOST_SECTIONTIME_090000 && time <= THOST_SECTIONTIME_101500) || 
			(time >= THOST_SECTIONTIME_103000 && time <= THOST_SECTIONTIME_113000) || 
			(time >= THOST_SECTIONTIME_133000 && time <= THOST_SECTIONTIME_150000))
		{
			return true;
		}
	}
	return false;
}

inline bool THOST_IS_NIGHTTRADINGTIME(string symbol, string exchangeid, int time)
{
	if (exchangeid == THOST_EXCHANGEID_SGE)
	{
		if ((time >= THOST_SECTIONTIME_200000 && time <= THOST_SECTIONTIME_240000) || 
			(time >= THOST_SECTIONTIME_000000 && time <= THOST_SECTIONTIME_023000))
		{
			return true;
		}
	}
	else if (exchangeid == THOST_EXCHANGEID_SHFE)
	{
		if (symbol == "ru" || symbol == "rb")
		{
			if (time >= THOST_SECTIONTIME_210000 && time <= THOST_SECTIONTIME_230000)
			{
				return true;
			}
		}
		else if (symbol == "au" || symbol == "ag")
		{
			if ((time >= THOST_SECTIONTIME_210000 && time <= THOST_SECTIONTIME_240000) || 
				(time >= THOST_SECTIONTIME_000000 && time <= THOST_SECTIONTIME_023000))
			{
				return true;
			}
		}
		else
		{
			if ((time >= THOST_SECTIONTIME_210000 && time <= THOST_SECTIONTIME_240000) || 
				(time >= THOST_SECTIONTIME_000000 && time <= THOST_SECTIONTIME_010000))
			{
				return true;
			}
		}	
	}
	else if (exchangeid != THOST_EXCHANGEID_CFFEX)
	{
		if (time >= THOST_SECTIONTIME_210000 && time <= THOST_SECTIONTIME_233000)
		{
			return true;
		}
	}
	return false;
}

inline bool THOST_IS_TRADINGTIME(string symbol, string exchangeid, int time)
{
	if (THOST_IS_DAYTRADINGTIME(symbol, exchangeid, time) || 
		THOST_IS_NIGHTTRADINGTIME(symbol, exchangeid, time))
	{
		return true;
	}
	return false;
}

inline bool THOST_IS_SECTIONBEGIN(string symbol, string exchangeid, int time)
{
	if (exchangeid == THOST_EXCHANGEID_SGE)
	{
		if (time == THOST_SECTIONTIME_090000 || time == THOST_SECTIONTIME_133000 || time == THOST_SECTIONTIME_200000)
		{
			return true;
		}
	}
	else if (exchangeid == THOST_EXCHANGEID_CFFEX)
	{
		if (time == THOST_SECTIONTIME_130000)
		{
			return true;
		}

		if (symbol == "T" || symbol == "TF")
		{
			if (time == THOST_SECTIONTIME_091500)
			{
				return true;
			}
		}
		else if (time == THOST_SECTIONTIME_093000)
		{
			return true;
		}
	}
	else
	{
		if (time == THOST_SECTIONTIME_090000 || time == THOST_SECTIONTIME_103000 || 
			time == THOST_SECTIONTIME_133000 || time == THOST_SECTIONTIME_210000)
		{
			return true;
		}
	}
	return false;
}

inline bool THOST_IS_DAYSECTIONEND(string symbol, string exchangeid, int time)
{
	if (exchangeid == THOST_EXCHANGEID_SGE)
	{
		if (time == THOST_SECTIONTIME_113000)
		{
			return true;
		}
	}
	else if (exchangeid == THOST_EXCHANGEID_CFFEX)
	{
		if (time == THOST_SECTIONTIME_113000)
		{
			return true;
		}
	}
	else
	{
		if (time == THOST_SECTIONTIME_101500 || time == THOST_SECTIONTIME_113000)
		{
			return true;
		}
	}
	return false;
}

inline bool THOST_IS_NIGHTSECTIONEND(string symbol, string exchangeid, int time)
{
	if (exchangeid == THOST_EXCHANGEID_SGE)
	{
		if (time == THOST_SECTIONTIME_023000)
		{
			return true;
		}
	}
	else if (exchangeid == THOST_EXCHANGEID_SHFE)
	{
		if (symbol == "ru" || symbol == "rb")
		{
			if (time == THOST_SECTIONTIME_230000)
			{
				return true;
			}
		}
		else if (symbol == "au" || symbol == "ag")
		{
			if (time == THOST_SECTIONTIME_023000)
			{
				return true;
			}
		}
		else
		{
			if (time == THOST_SECTIONTIME_010000)
			{
				return true;
			}
		}	
	}
	else
	{
		if (time == THOST_SECTIONTIME_233000)
		{
			return true;
		}
	}
	return false;
}

inline bool THOST_IS_SECTIONEND(string symbol, string exchangeid, int time)
{
	if (THOST_IS_DAYSECTIONEND(symbol, exchangeid, time) || 
		THOST_IS_NIGHTSECTIONEND(symbol, exchangeid, time))
	{
		return true;
	}
	return false;
}

inline int MARKETDATA_TIMESTAMP_CMP(int LastActionDay, int LastTime, int LastMillisec, int CurActionDay, int CurTime, int CurMillisec)
{
	long lLastMarketDataTimeStamp = (long)LastActionDay * 1000000000 + LastTime * 1000 + LastMillisec;
	long lCurMarketDataTimeStamp = (long)CurActionDay * 1000000000 + CurTime * 1000 + CurMillisec;

	if (lLastMarketDataTimeStamp < lCurMarketDataTimeStamp)
	{
		return -1;
	}
	else if (lLastMarketDataTimeStamp == lCurMarketDataTimeStamp)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

inline int MARKETDATA_TIMESTAMP_CMP(CThostDepthMarketDataField *pLastMarketData, CThostDepthMarketDataField *pCurMarketData)
{
	long lLastMarketDataTimeStamp = (long)pLastMarketData->ActionDay * 1000000000 + pLastMarketData->UpdateTime * 1000 + pLastMarketData->UpdateMillisec;
	long lCurMarketDataTimeStamp = (long)pCurMarketData->ActionDay * 1000000000 + pCurMarketData->UpdateTime * 1000 + pCurMarketData->UpdateMillisec;

	if (lLastMarketDataTimeStamp < lCurMarketDataTimeStamp)
	{
		return -1;
	}
	else if (lLastMarketDataTimeStamp == lCurMarketDataTimeStamp)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

inline bool ISDIRTYDATA(int PrevDate, int PrevTime, int CurDate, int CurTime)
{
	if (PrevDate < CurDate)
	{
		return false;
	}
	else if (PrevDate == CurDate)
	{
		if (((PrevTime + THOST_OFFSETTIME_030000) % THOST_SECTIONTIME_240000) < 
			((CurTime + THOST_OFFSETTIME_030000) % THOST_SECTIONTIME_240000))
		{
			return false;
		}
	}
	return true;
}

///KBAR PERIODTYPE
#define		THOST_KBAR_PERIOD_TYPE_TICK			't'
#define		THOST_KBAR_PERIOD_TYPE_SECOND		'S'
#define		THOST_KBAR_PERIOD_TYPE_MINUTE		'M'
#define		THOST_KBAR_PERIOD_TYPE_HOUR			'H'
#define		THOST_KBAR_PERIOD_TYPE_DAY			'd'
#define		THOST_KBAR_PERIOD_TYPE_WEEK			'w'
#define		THOST_KBAR_PERIOD_TYPE_MONTH		'm'
#define		THOST_KBAR_PERIOD_TYPE_YEAD			'y'

///KBarStatus
#define		THOST_KBAR_STATUS_MASK				15
#define		THOST_KBAR_STATUS_NULL				1
#define		THOST_KBAR_STATUS_OPEN				2
#define		THOST_KBAR_STATUS_CLOSE				4
#define     THOST_KBAR_STATUS_TRADINGCLOSE      8

#define		KBAR_ISNULL(X)					X&THOST_KBAR_STATUS_NULL
#define		KBAR_ISOPEN(X)					X&THOST_KBAR_STATUS_OPEN
#define		KBAR_ISCLOSE(X)					X&THOST_KBAR_STATUS_CLOSE
#define		KBAR_ISTRADINGCLOSE(X)			X&THOST_KBAR_STATUS_TRADINGCLOSE

struct CKBarField {
	CKBarField()
	{
		Reset();
	}

	CKBarField(string sInstrumentID, string sSymbol, string sExchangeID, char cPeriodType, int nPeriodValue, int nOffset)
	{
		Reset();
		PeriodType = cPeriodType;
		PeriodValue = nPeriodValue;
		Symbol = sSymbol;
		InstrumentID = sInstrumentID;
		ExchangeID = sExchangeID;
		Offset = nOffset;
		Status = THOST_KBAR_STATUS_NULL;
	}

	void Reset()
	{
		TradingDate = 0;
		Date = 0;
		BeginTime = 0;
		EndTime = 0;
		OpenPrice = 0;
		HighPrice = 0;
		LowPrice = 0;
		ClosePrice = 0;
		AskPrice = 0;
		AskVolume = 0;
		BidPrice = 0;
		BidVolume = 0;
		Volume = 0;
		OpenInterest = 0;
		Ticks = 0;
		TWapPrice = 0;
		VWapPrice = 0;
		PreClosePrice = 0;
		PreVolume = 0;
		PreOpenInterest = 0;
		Offset = 0;
		IsFill = false;
		Status = THOST_KBAR_STATUS_NULL;
		BarIndex = 0;
	}

	///周期类型
	char						PeriodType;
	///周期值
	int							PeriodValue;
	///品种代码
	string						Symbol;
	///合约代码
	string						InstrumentID;
	///交易所ID
	string						ExchangeID;
	///自然日期
	int							Date;
	///开始时间
	int							BeginTime;
	///结束时间
	int							EndTime;
	///开盘价
	double						OpenPrice;
	///最高价
	double						HighPrice;
	///最低价
	double						LowPrice;
	////收盘价
	double						ClosePrice;				
	///卖一价
	double						AskPrice;
	///卖一量
	int							AskVolume;
	///买一价
	double						BidPrice;
	///买一量
	int							BidVolume;
	///成交量
	int							Volume;
	///持仓量的差
	double						OpenInterest;
	///tick个数
	int							Ticks;
	///twap价
	double						TWapPrice;
	///vwap价
	double						VWapPrice;
	///前收盘价
	double						PreClosePrice;
	///前成交量
	int							PreVolume;
	///前持仓量
	double						PreOpenInterest;
	///偏移量,单位为秒
	int							Offset;
	///是否填充
	bool						IsFill;
	///kbar状态
	int							Status;
	///索引值
	int							BarIndex;
	///交易日期
	int							TradingDate;

	bool operator < (const CKBarField &kbarField) const 
	{ 
		if (Date < kbarField.Date)
		{
			return true;
		}
		if (Date == kbarField.Date)
		{
			if (BeginTime < kbarField.BeginTime)
			{
				return true;
			}
			else if (BeginTime == kbarField.BeginTime)
			{
				if (strcmp(InstrumentID.c_str(), kbarField.InstrumentID.c_str()) <= 0)
				{
					return true;
				}
			}
		}
		return false;
	}
};

struct CKBarDailyField : public CKBarField {
	CKBarDailyField() : CKBarField()
	{
		PreSettlePrice = 0;
		LimitUpPrice = 0;
		LimitDownPrice = 0;
		Rank = 0;
	}

	CKBarDailyField(string sInstrumentID, string sSymbol, string sExchangeID) 
		: CKBarField(sInstrumentID, sSymbol, sExchangeID, THOST_KBAR_PERIOD_TYPE_DAY, 1, 0)
	{
		PreSettlePrice = 0;
		LimitUpPrice = 0;
		LimitDownPrice = 0;
		Rank = 0;
	}

	///昨日结算
	double						PreSettlePrice;
	///涨停价
	double						LimitUpPrice;
	///跌停价
	double						LimitDownPrice;
	///品种排名
	int							Rank;
};

class CThostMdHandlerSpi {
public:
	///深度行情通知
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData){};
};

class CThostTraderHandlerSpi {
public:
	///订单回报
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrder){};

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostInstrumentStatusField *pInstrumentStatus) {};

	///初始化完成
	virtual void OnRtnInitializeFinished(int ApiType){};
};

class CKBarHandlerSpi { 
public:
	///kbar回调
	virtual void OnRtnKBar(CKBarField *pKBar){};
};

///合约信息表
typedef map<string, CThostInstrumentField>					InstrumentInfoMap;
///合约保证金表
typedef map<string, CThostInstrumentMarginRateField>		InstrumentMarginRateMap;
///合约手续费表
typedef map<string, CThostInstrumentCommissionRateField>	InstrumentComissionRateMap;
///订单回报表
typedef map<string, CThostOrderReportField*>				OrderReportMap;
///用户持仓表
typedef map<string, CThostInvestorPositionField>			InvestorPositionMap;
///用户持仓列表
typedef vector<CThostInvestorPositionField>					InvestorPositionVec;
///订单回报列表
typedef vector<CThostOrderReportField>						OrderReportVec;
///成交列表
typedef vector<CThostTradeField>							TradeVec;
///请求类型表
typedef map<int,char>										ReqTypeMap;
///kbar表
typedef map< string, vector<CKBarField*> >					KBarMap;
///kbar快照表
typedef map<string, CKBarField>								KBarSnapshotMap;
///日线表
typedef map<string,	CKBarDailyField*>						KBarDailyMap;
///行情快照表
typedef map<string, CThostDepthMarketDataField>				DepthMarketDataSnapshotMap;

#endif
