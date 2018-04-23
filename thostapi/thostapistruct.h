/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file thostapistruct.h
///@brief �����˿ͻ��˽ӿ�ʹ�õ�ҵ�����ݽṹ
///@history 
///20160818@�쿡		�������ļ�
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

///�ز�ģʽ
#define THOST_API_MODE_BACKTEST		1
///ʵ�̽���ģʽ
#define THOST_API_MODE_TRADING		2

///ctp
#define THOST_API_TYPE_CTP			1
///fs
#define THOST_API_TYPE_FS			2

///Direction ��������
///��
#define THOST_D_Long	'0'
///��
#define THOST_D_Short	'1'

///OF ��ƽ��
///����
#define THOST_OF_Open '0'
///ƽ��
#define THOST_OF_Close '1'
///ǿƽ
#define THOST_OF_ForceClose '2'
///ƽ��
#define THOST_OF_CloseToday '3'
///ƽ��
#define THOST_OF_CloseYesterday '4'
///ǿ��
#define THOST_OF_ForceOff '5'
///����ǿƽ
#define THOST_OF_LocalForceClose '6'

///HedgeFlag��һ��Ͷ���ױ���־����
///Ͷ��
#define THOST_HF_Speculation '1'
///����
#define THOST_HF_Arbitrage '2'
///�ױ�
#define THOST_HF_Hedge '3'

///VolumeCondition
///�κ�����
#define THOST_VC_AV '1'
///��С����
#define THOST_VC_MV '2'
///ȫ������
#define THOST_VC_CV '3'

///TimeCondition
///������ɣ�������
#define THOST_TC_IOC '1'
///������Ч
#define THOST_TC_GFS '2'
///������Ч
#define THOST_TC_GFD '3'
///ָ������ǰ��Ч
#define THOST_TC_GTD '4'
///����ǰ��Ч
#define THOST_TC_GTC '5'
///���Ͼ�����Ч
#define THOST_TC_GFA '6'

///OrderStatus
///broker���ն���
#define THOST_OST_Recieved 'a'
///���������ն���
#define THOST_OST_Submitted '3'
///���ֳɽ�
#define THOST_OST_Filled '1'
///��ȫ�ɽ�
#define THOST_OST_Complete '0'
///�����ɹ�
#define THOST_OST_Canceled '5'
///���ڳ���
#define THOST_OST_Canceling '6'
///����ʧ��
#define THOST_OST_CancelFailed '7'
///��
#define THOST_OST_Error '8'


///OrderPriceType
///�����
#define THOST_OPT_AnyPrice '1'
///�޼�
#define THOST_OPT_LimitPrice '2'
///���ż�
#define THOST_OPT_BestPrice '3'
///���¼�
#define THOST_OPT_LastPrice '4'
///���¼۸����ϸ�1��ticks
#define THOST_OPT_LastPricePlusOneTicks '5'
///���¼۸����ϸ�2��ticks
#define THOST_OPT_LastPricePlusTwoTicks '6'
///���¼۸����ϸ�3��ticks
#define THOST_OPT_LastPricePlusThreeTicks '7'
///��һ��
#define THOST_OPT_AskPrice1 '8'
///��һ�۸����ϸ�1��ticks
#define THOST_OPT_AskPrice1PlusOneTicks '9'
///��һ�۸����ϸ�2��ticks
#define THOST_OPT_AskPrice1PlusTwoTicks 'A'
///��һ�۸����ϸ�3��ticks
#define THOST_OPT_AskPrice1PlusThreeTicks 'B'
///��һ��
#define THOST_OPT_BidPrice1 'C'
///��һ�۸����ϸ�1��ticks
#define THOST_OPT_BidPrice1PlusOneTicks 'D'
///��һ�۸����ϸ�2��ticks
#define THOST_OPT_BidPrice1PlusTwoTicks 'E'
///��һ�۸����ϸ�3��ticks
#define THOST_OPT_BidPrice1PlusThreeTicks 'F'
///�嵵��
#define THOST_OPT_FiveLevelPrice 'G'


///ContingentCondition
///����
#define THOST_CC_Immediately '1'
///ֹ��
#define THOST_CC_Touch '2'
///ֹӮ
#define THOST_CC_TouchProfit '3'
///Ԥ��
#define THOST_CC_ParkedOrder '4'
///���¼۴���������
#define THOST_CC_LastPriceGreaterThanStopPrice '5'
///���¼۴��ڵ���������
#define THOST_CC_LastPriceGreaterEqualStopPrice '6'
///���¼�С��������
#define THOST_CC_LastPriceLesserThanStopPrice '7'
///���¼�С�ڵ���������
#define THOST_CC_LastPriceLesserEqualStopPrice '8'
///��һ�۴���������
#define THOST_CC_AskPriceGreaterThanStopPrice '9'
///��һ�۴��ڵ���������
#define THOST_CC_AskPriceGreaterEqualStopPrice 'A'
///��һ��С��������
#define THOST_CC_AskPriceLesserThanStopPrice 'B'
///��һ��С�ڵ���������
#define THOST_CC_AskPriceLesserEqualStopPrice 'C'
///��һ�۴���������
#define THOST_CC_BidPriceGreaterThanStopPrice 'D'
///��һ�۴��ڵ���������
#define THOST_CC_BidPriceGreaterEqualStopPrice 'E'
///��һ��С��������
#define THOST_CC_BidPriceLesserThanStopPrice 'F'
///��һ��С�ڵ���������
#define THOST_CC_BidPriceLesserEqualStopPrice 'H'

///ForceCloseReason
///��ǿƽ
#define THOST_FCC_NotForceClose '0'
///�ʽ���
#define THOST_FCC_LackDeposit '1'
///�ͻ�����
#define THOST_FCC_ClientOverPositionLimit '2'
///��Ա����
#define THOST_FCC_MemberOverPositionLimit '3'
///�ֲַ�������
#define THOST_FCC_NotMultiple '4'
///Υ��
#define THOST_FCC_Violation '5'
///����
#define THOST_FCC_Other '6'
///��Ȼ���ٽ�����
#define THOST_FCC_PersonDeliv '7'


///ProductClass
///�ڻ�
#define THOST_PC_Futures '1'
///�ڻ���Ȩ
#define THOST_PC_Options '2'
///���
#define THOST_PC_Combination '3'
///����
#define THOST_PC_Spot '4'
///��ת��
#define THOST_PC_EFP '5'
///�ֻ���Ȩ
#define THOST_PC_SpotOption '6'

///InstLifePhase
///δ����
#define THOST_FTDC_IP_NotStart '0'
///����
#define THOST_FTDC_IP_Started '1'
///ͣ��
#define THOST_FTDC_IP_Pause '2'
///����
#define THOST_FTDC_IP_Expired '3'

///MaxMarginSideAlgorithm
///��ʹ�ô��߱�֤���㷨
#define THOST_FTDC_MMSA_NO '0'
///ʹ�ô��߱�֤���㷨
#define THOST_FTDC_MMSA_YES '1'

///ͬ������
#define THOST_RT_SYNC	    '0'
///�첽����
#define THOST_RT_ASYNC      '1'
///�첽��ʼ������
#define THOST_RT_ASYNC_INIT '2'

///���س�ʱʱ��(����)
#define THOST_FLOWCTL_TIMEOUT_MS		200
///ͬ������ȴ���ʱʱ��(��)
#define THOST_SYNC_RQ_TIMEOUT_S			10
///�Ͽ�����˯��ʱ��
#define THOST_DISCONNECT_SLEEP_TIME_S	5

///PositionDate
///���ճֲ�
#define THOST_PSD_Today		'1'
///��ʷ�ֲ�
#define THOST_PSD_History	'2'
///���гֲ�
#define THOST_PSD_ALL		'3'

///PosiDirection
///��
#define THOST_PD_Net '1'
///��ͷ
#define THOST_PD_Long '2'
///��ͷ
#define THOST_PD_Short '3'


///InstrumentStatusType��һ����Լ����״̬����
///����ǰ
#define THOST_IS_BeforeTrading '0'
///�ǽ���
#define THOST_IS_NoTrading '1'
///��������
#define THOST_IS_Continous '2'
///���Ͼ��۱���
#define THOST_IS_AuctionOrdering '3'
///���Ͼ��ۼ۸�ƽ��
#define THOST_IS_AuctionBalance '4'
///���Ͼ��۴��
#define THOST_IS_AuctionMatch '5'
///����
#define THOST_IS_Closed '6'

///�������
struct CThostDepthMarketDataField
{
	///��Լ����
	string				InstrumentID;
	///����������
	string				ExchangeID;
	///���¼�
	double				LastPrice;
	///�ϴν����
	double				PreSettlementPrice;
	///������
	double				PreClosePrice;
	///��ֲ���
	double				PreOpenInterest;
	///����
	double				OpenPrice;
	///��߼�
	double				HighestPrice;
	///��ͼ�
	double				LowestPrice;
	///����
	int					Volume;
	///�ɽ����
	double				Turnover;
	///�ֲ���
	double				OpenInterest;
	///������
	double				ClosePrice;
	///���ν����
	double				SettlementPrice;
	///��ͣ���
	double				UpperLimitPrice;
	///��ͣ���
	double				LowerLimitPrice;
	///����ʵ��
	double				PreDelta;
	///����ʵ��
	double				CurrDelta;
	///����޸�ʱ��
	int					UpdateTime;
	///����޸ĺ���
	int					UpdateMillisec;
	///�����һ
	double				BidPrice1;
	///������һ
	int					BidVolume1;
	///������һ
	double				AskPrice1;
	///������һ
	int					AskVolume1;
	///����۶�
	double				BidPrice2;
	///��������
	int					BidVolume2;
	///�����۶�
	double				AskPrice2;
	///��������
	int					AskVolume2;
	///�������
	double				BidPrice3;
	///��������
	int					BidVolume3;
	///��������
	double				AskPrice3;
	///��������
	int					AskVolume3;
	///�������
	double				BidPrice4;
	///��������
	int					BidVolume4;
	///��������
	double				AskPrice4;
	///��������
	int					AskVolume4;
	///�������
	double				BidPrice5;
	///��������
	int					BidVolume5;
	///��������
	double				AskPrice5;
	///��������
	int					AskVolume5;
	///���վ���
	double				AveragePrice;
	///ҵ������
	int					ActionDay;
	///������
	int					TradingDay;
};

///��������
struct CThostOrderRequestField {
	///���͹�˾����
	string					BrokerID;
	///Ͷ���ߴ���
	string					InvestorID;
	///��Լ����
	string					InstrumentID;
	///��������
	string					OrderRef;
	///�û�����
	string					UserID;
	///�����۸�����
	char					OrderPriceType;
	///��������
	char					Direction;
	///��Ͽ�ƽ��־
	string					CombOffsetFlag;
	///���Ͷ���ױ���־
	string					CombHedgeFlag;
	///�۸�
	double					LimitPrice;
	///����
	int						VolumeTotalOriginal;
	///��Ч������
	char					TimeCondition;
	///GTD����
	string					GTDDate;
	///�ɽ�������
	char					VolumeCondition;
	///��С�ɽ���
	int						MinVolume;
	///��������
	char					ContingentCondition;
	///ֹ���
	double					StopPrice;
	///ǿƽԭ��
	char					ForceCloseReason;
	///�Զ������־
	int						IsAutoSuspend;
	///ҵ��Ԫ
	string					BusinessUnit;
	///������
	int						RequestID;
	///�û�ǿ����־
	int						UserForceClose;
	///��������־
	int						IsSwapOrder;
};

///�����ر�
struct CThostOrderReportField : public CThostOrderRequestField {
	///�ڲ�����������ʶΨһ����
	long						OrderId;	
	///�������
	string						OrderSysID;
	///����������
	string						ExchangeID;
	///�ѳɽ�����
	int							FilledVolume;
	///���һ�ʳɽ�������
	int							LastFilledVolume;
	///���һ�ʳɽ��ļ۸�
	double						LastFilledPrice;
	///�ɽ�����
	double						AveragePrice;
	///����״̬
	char						OrderStatus;
	///������
	string						TradingDay;
	///��������
	string						InsertDate;
	///ί��ʱ��
	string						InsertTime;
	///����ɽ�ʱ��				
	string						LastFilledTime;
	///����ʱ��
	string						CancelTime;
	///ǰ�ñ��
	int							FrontID;
	///�Ự���
	int							SessionID;
	///�û��˲�Ʒ��Ϣ
	string						UserProductInfo;
	///״̬��Ϣ
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
	///���͹�˾����
	string						BrokerID;
	///Ͷ���ߴ���
	string						InvestorID;
	///��Լ����
	string						InstrumentID;
	///��������
	string						OrderRef;
	///�û�����
	string						UserID;
	///����������
	string						ExchangeID;
	///�ɽ����
	string						TradeID;
	///��������
	char						Direction;
	///�������
	string						OrderSysID;
	///��ƽ��־
	char						OffsetFlag;
	///Ͷ���ױ���־
	char						HedgeFlag;
	///�۸�
	double						Price;
	///����
	int							Volume;
	///�ɽ�ʱ��
	string						TradeDate;
	///�ɽ�ʱ��
	string						TradeTime;
	///������
	string						TradingDay;
	///������
	string						SettlementID;
	///���͹�˾�������
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

///�ʽ��˻�
struct CThostTradingAccountField
{
	///���͹�˾����
	string						BrokerID;
	///Ͷ�����ʺ�
	string						AccountID;
	///�ϴν���׼����
	double						PreBalance;
	///�ϴ�ռ�õı�֤��
	double						PreMargin;
	///�����
	double						Deposit;
	///������
	double						Withdraw;
	///����ı�֤��
	double						FrozenMargin;
	///������ʽ�
	double						FrozenCash;
	///�����������
	double						FrozenCommission;
	///��ǰ��֤���ܶ�
	double						CurrMargin;
	///�ʽ���
	double						CashIn;
	///������
	double						Commission;
	///ƽ��ӯ��
	double						CloseProfit;
	///�ֲ�ӯ��
	double						PositionProfit;
	///�ڻ�����׼����
	double						Balance;
	///�����ʽ�
	double						Available;
	///��ȡ�ʽ�
	double						WithdrawQuota;
	///������
	int							TradingDay;
};

struct CThostInvestorPositionField {
	///��Լ����
	string						InstrumentID;
	///���͹�˾����
	string						BrokerID;
	///Ͷ���ߴ���
	string						InvestorID;
	///�ֲֶ�շ���
	char						PosiDirection;
	///Ͷ���ױ���־
	char						HedgeFlag;
	///�ֲ�����
	char						PositionDate;
	///�ֲܳ�
	int							Position;
	///���ճֲ�
	int							YdPosition;
	///���ճֲ�
	int							TodayPosition;
	///��ͷ����
	int							LongFrozen;
	///��ͷ����
	int							ShortFrozen;
	///���ֶ�����
	double						LongFrozenAmount;
	///���ֶ�����
	double						ShortFrozenAmount;
	///������
	int							OpenVolume;
	///ƽ����
	int							CloseVolume;
	///���ֽ��
	double						OpenAmount;
	///ƽ�ֽ��
	double						CloseAmount;
	///�ֲֳɱ�
	double						PositionCost;
	///�ϴ�ռ�õı�֤��
	double						PreMargin;
	///ռ�õı�֤��
	double						UseMargin;
	///����ı�֤��
	double						FrozenMargin;
	///������ʽ�
	double						FrozenCash;
	///�����������
	double						FrozenCommission;
	///�ʽ���
	double						CashIn;
	///������
	double						Commission;
	///ƽ��ӯ��
	double						CloseProfit;
	///�ֲ�ӯ��
	double						PositionProfit;
	///�ϴν����
	double						PreSettlementPrice;
	///���ν����
	double						SettlementPrice;
	///������
	string						TradingDay;
	///������
	int							SettlementID;
	///���ֳɱ�
	double						OpenCost;
	///��������֤��
	double						ExchangeMargin;
	///���ն���ƽ��ӯ��
	double						CloseProfitByDate;
	///��ʶԳ�ƽ��ӯ��
	double						CloseProfitByTrade;
};

///��Լ
struct CThostInstrumentField
{
	///��Լ����
	string						InstrumentID;
	///����������
	string						ExchangeID;
	///��Լ����
	string						InstrumentName;
	///��Ʒ����
	string						ProductID;
	///��Ʒ����
	char						ProductClass;
	///�������
	int							DeliveryYear;
	///������
	int							DeliveryMonth;
	///�м۵�����µ���
	int							MaxMarketOrderVolume;
	///�м۵���С�µ���
	int							MinMarketOrderVolume;
	///�޼۵�����µ���
	int							MaxLimitOrderVolume;
	///�޼۵���С�µ���
	int							MinLimitOrderVolume;
	///��Լ��������
	int							VolumeMultiple;
	///��С�䶯��λ
	double						PriceTick;
	///������
	string						CreateDate;
	///������
	string						OpenDate;
	///������
	string						ExpireDate;
	///��ʼ������
	string						StartDelivDate;
	///����������
	string						EndDelivDate;
	///��Լ��������״̬
	char						InstLifePhase;
	///��ǰ�Ƿ���
	int							IsTrading;
	///��ͷ��֤����
	double						LongMarginRatio;
	///��ͷ��֤����
	double						ShortMarginRatio;
	///�Ƿ�ʹ�ô��߱�֤���㷨
	char						MaxMarginSideAlgorithm;
};

///��Լ��֤����
struct CThostInstrumentMarginRateField
{
	///��Լ����
	string						InstrumentID;
	///���͹�˾����
	string						BrokerID;
	///Ͷ���ߴ���
	string						InvestorID;
	///Ͷ���ױ���־
	char						HedgeFlag;
	///��ͷ��֤����
	double						LongMarginRatioByMoney;
	///��ͷ��֤���
	double						LongMarginRatioByVolume;
	///��ͷ��֤����
	double						ShortMarginRatioByMoney;
	///��ͷ��֤���
	double						ShortMarginRatioByVolume;
	///�Ƿ���Խ�������ȡ
	int							IsRelative;
};

///��Լ��������
struct CThostInstrumentCommissionRateField
{
	///��Լ����
	string						InstrumentID;
	///���͹�˾����
	string						BrokerID;
	///Ͷ���ߴ���
	string						InvestorID;
	///������������
	double						OpenRatioByMoney;
	///����������
	double						OpenRatioByVolume;
	///ƽ����������
	double						CloseRatioByMoney;
	///ƽ��������
	double						CloseRatioByVolume;
	///ƽ����������
	double						CloseTodayRatioByMoney;
	///ƽ��������
	double						CloseTodayRatioByVolume;
};

///��Լ״̬
struct CThostInstrumentStatusField
{
	///����������
	string						ExchangeID;
	///��Լ�ڽ������Ĵ���
	string						ExchangeInstID;
	///��Լ����
	string						InstrumentID;
	///��Լ����״̬
	char						InstrumentStatus;
	///���뱾״̬ʱ��
	string						EnterTime;
};

///�ز����
struct CThostBackTestParamsField
{
	///��Լ�б�
	vector<string>									InstrumentIDList;
	///��ʼ�ز�����
	int												StartDate;
	///�����ز�����			
	int												EndDate;
	///�ز���������
	char											PeriodType;
	///�ز���������ֵ
	int												PeriodValue;
	///��ʼ�ʽ�	
	double											InitialBalance;
	///��Լ��Ϣ
	vector<CThostInstrumentField>					InstrumentInfoList;
	///��Լ��֤���б�
	vector<CThostInstrumentMarginRateField>			InstrumentMarginRateList;
	///��Լ�������б�
	vector<CThostInstrumentCommissionRateField>		InstrumentCommissionRateList;
	///���ݿ��ַ
	string											SqlHost;
	///���ݿ�˿�
	int												SqlPort;
	///���ݿ��û�
	string											SqlUser;
	///���ݿ�����
	string											SqlPassword;
	///���ݿ�
	string											SqlDB;
};

///�ز�ָ��
struct CThostStrategyEvalResultField
{
	///�����껯������
	double						AnnualizedReturns;
	///�ο���׼�껯������
	double						BenchmarkReturns;
	///������
	double						Alpha;
	///����
	double						Beta;
	///�������沨����
	double						Volatility;
	///���ձ���
	double						SharpRatio;
	///��Ϣ����
	double						InformationRatio;
	///���س�
	double						MaxDrawDown;
	///ʤ��
	double						WinRatio;
	///���������������
	double						MaxContinuousWinDays;
	///������� �µ�����
	double						MaxContinuousLossDays;
};

struct CThostDataTimeStamp {
	///��Ȼ��yyMMdd
	int		date;
	///ʱ�� HHmmss
	int		time;
	///����
	int		millisec;
	///���ش��ں�
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
///δ����
#define THOST_ENDMASK_NOEND			1
///���콻���ս���
#define THOST_ENDMASK_TRADINGDAYEND	2
///���е����ݽ���
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

	///��������
	char						PeriodType;
	///����ֵ
	int							PeriodValue;
	///Ʒ�ִ���
	string						Symbol;
	///��Լ����
	string						InstrumentID;
	///������ID
	string						ExchangeID;
	///��Ȼ����
	int							Date;
	///��ʼʱ��
	int							BeginTime;
	///����ʱ��
	int							EndTime;
	///���̼�
	double						OpenPrice;
	///��߼�
	double						HighPrice;
	///��ͼ�
	double						LowPrice;
	////���̼�
	double						ClosePrice;				
	///��һ��
	double						AskPrice;
	///��һ��
	int							AskVolume;
	///��һ��
	double						BidPrice;
	///��һ��
	int							BidVolume;
	///�ɽ���
	int							Volume;
	///�ֲ����Ĳ�
	double						OpenInterest;
	///tick����
	int							Ticks;
	///twap��
	double						TWapPrice;
	///vwap��
	double						VWapPrice;
	///ǰ���̼�
	double						PreClosePrice;
	///ǰ�ɽ���
	int							PreVolume;
	///ǰ�ֲ���
	double						PreOpenInterest;
	///ƫ����,��λΪ��
	int							Offset;
	///�Ƿ����
	bool						IsFill;
	///kbar״̬
	int							Status;
	///����ֵ
	int							BarIndex;
	///��������
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

	///���ս���
	double						PreSettlePrice;
	///��ͣ��
	double						LimitUpPrice;
	///��ͣ��
	double						LimitDownPrice;
	///Ʒ������
	int							Rank;
};

class CThostMdHandlerSpi {
public:
	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostDepthMarketDataField *pDepthMarketData){};
};

class CThostTraderHandlerSpi {
public:
	///�����ر�
	virtual void OnRtnOrderReport(CThostOrderReportField *pOrder){};

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CThostInstrumentStatusField *pInstrumentStatus) {};

	///��ʼ�����
	virtual void OnRtnInitializeFinished(int ApiType){};
};

class CKBarHandlerSpi { 
public:
	///kbar�ص�
	virtual void OnRtnKBar(CKBarField *pKBar){};
};

///��Լ��Ϣ��
typedef map<string, CThostInstrumentField>					InstrumentInfoMap;
///��Լ��֤���
typedef map<string, CThostInstrumentMarginRateField>		InstrumentMarginRateMap;
///��Լ�����ѱ�
typedef map<string, CThostInstrumentCommissionRateField>	InstrumentComissionRateMap;
///�����ر���
typedef map<string, CThostOrderReportField*>				OrderReportMap;
///�û��ֱֲ�
typedef map<string, CThostInvestorPositionField>			InvestorPositionMap;
///�û��ֲ��б�
typedef vector<CThostInvestorPositionField>					InvestorPositionVec;
///�����ر��б�
typedef vector<CThostOrderReportField>						OrderReportVec;
///�ɽ��б�
typedef vector<CThostTradeField>							TradeVec;
///�������ͱ�
typedef map<int,char>										ReqTypeMap;
///kbar��
typedef map< string, vector<CKBarField*> >					KBarMap;
///kbar���ձ�
typedef map<string, CKBarField>								KBarSnapshotMap;
///���߱�
typedef map<string,	CKBarDailyField*>						KBarDailyMap;
///������ձ�
typedef map<string, CThostDepthMarketDataField>				DepthMarketDataSnapshotMap;

#endif
