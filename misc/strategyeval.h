/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file strategyeval.h
///@brief 策略评估定义
///@history 
///20161024@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef INC_STRATEGYEVAL_H
#define INC_STRATEGYEVAL_H

#include "../thostapi/thostapistruct.h"

class CThostStrategyEval {
public:
	CThostStrategyEval(){}

	~CThostStrategyEval(){}

public:
	///设置评估样本
	void SetEvalSample(vector<double> &vecBalance, vector<double> &vecBenchmarkIndex);

	///开始计算指标
	void StartEval();

	///获取评估结果
	void GetEvalResult(CThostStrategyEvalResultField &strategyEvalResult);

private:
	///计算均值
	double Mean(vector<double> &vecData);

	///计算方差
	double Variance(vector<double> &vecData);

	///计算标准差
	double StandardDeviation(vector<double> &vecData);

	///计算协方差
	double CoVariance(vector<double> &vecX, vector<double> &vecY);

private:
	///资金列表,加上初始资金n+1个,n为交易日的个数
	vector<double>					m_vecBalance;
	///参考指数列表,初始指数为第一个交易日的昨收指数
	vector<double>					m_vecBenchmarkIndex;
	///策略评估结果
	CThostStrategyEvalResultField	m_strategyEvalResult;
};

#ifdef _MISC_AS_TEMPLATE_
#include "strategyeval.cpp"
#endif

#endif // end INC_STRATEGYEVAL_H