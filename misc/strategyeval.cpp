#include <cmath>
#include "strategyeval.h"

void CThostStrategyEval::SetEvalSample(vector<double> &vecBalance, vector<double> &vecBenchmarkIndex)
{
	m_vecBalance = vecBalance;
	m_vecBenchmarkIndex = vecBenchmarkIndex;
}

void CThostStrategyEval::StartEval()
{
	memset(&m_strategyEvalResult, 0, sizeof(CThostStrategyEvalResultField));
	int nTradingDayCount = m_vecBalance.size() - 1;
	memset(&m_strategyEvalResult, 0, sizeof(m_strategyEvalResult));
	///计算年化收益率
	vector<double>::iterator iter_Begin = m_vecBalance.begin();
	vector<double>::iterator iter_End = m_vecBalance.end() - 1;
	m_strategyEvalResult.AnnualizedReturns = ((*iter_End) / (*iter_Begin) - 1) / nTradingDayCount * 250;

	vector<double> vecDailyReturns;
	for (int i = 1; i < m_vecBalance.size(); ++i)
	{
		vecDailyReturns.push_back(m_vecBalance[i] / m_vecBalance[i-1] - 1);
	}

	///计算策略收益波动率
	m_strategyEvalResult.Volatility = sqrt(250.0) * StandardDeviation(vecDailyReturns);

	///计算夏普比率
	m_strategyEvalResult.SharpRatio = (m_strategyEvalResult.AnnualizedReturns - 0.03) / m_strategyEvalResult.Volatility;

	///计算最大回撤
	m_strategyEvalResult.MaxDrawDown = 1- m_vecBalance[1] / m_vecBalance[0];
	double maxBalance = m_vecBalance[0] > m_vecBalance[1] ? m_vecBalance[0] : m_vecBalance[1];
	for (int i = 2; i < m_vecBalance.size(); ++i)
	{
		double tempDrawDown = 1 - m_vecBalance[i] / maxBalance;
		if (m_vecBalance[i] > maxBalance)
		{
			maxBalance = m_vecBalance[i];
		}
		if (tempDrawDown > m_strategyEvalResult.MaxDrawDown)
		{
			m_strategyEvalResult.MaxDrawDown = tempDrawDown;
		}
	}
	if (m_strategyEvalResult.MaxDrawDown <= 0)
	{
		m_strategyEvalResult.MaxDrawDown = 0;
	}
	
	///计算胜率
	int WinDays = 0;
	for (int i = 0; i < vecDailyReturns.size(); ++i)
	{
		if (vecDailyReturns[i] > 0)
		{
			WinDays++;
		}
	}
	m_strategyEvalResult.WinRatio =  1.0 * WinDays / nTradingDayCount;

	///计算最大连续上涨天数
	m_strategyEvalResult.MaxContinuousWinDays = 0;
	int tempContinuousWinDays = 0;
	for (int i = 0; i < vecDailyReturns.size(); )
	{
		if (vecDailyReturns[i] >= 0)
		{
			tempContinuousWinDays++;
		}
		else
		{
			if (tempContinuousWinDays > m_strategyEvalResult.MaxContinuousWinDays)
			{
				m_strategyEvalResult.MaxContinuousWinDays = tempContinuousWinDays;
			}
			tempContinuousWinDays = 0;
		}
		i++;
	}

	///计算最大连续亏损天数
	m_strategyEvalResult.MaxContinuousLossDays = 0;
	int tempContinuousLossDays = 0;
	for (int i = 0; i < vecDailyReturns.size(); )
	{
		if (vecDailyReturns[i] <= 0)
		{
			tempContinuousLossDays++;
		}
		else
		{
			if (tempContinuousLossDays > m_strategyEvalResult.MaxContinuousLossDays)
			{
				m_strategyEvalResult.MaxContinuousLossDays = tempContinuousLossDays;
			}
			tempContinuousLossDays = 0;
		}
		i++;
	}

	if (m_vecBenchmarkIndex.size() == m_vecBalance.size())
	{
		vector<double> vecBenchmarkIndexReturns;
		for (int i = 1; i < m_vecBenchmarkIndex.size(); ++i)
		{
			vecBenchmarkIndexReturns.push_back(m_vecBenchmarkIndex[i] / m_vecBenchmarkIndex[i-1] - 1);
		}
		///计算参考标准年化收益率
		vector<double>::iterator iter_BenchmarkBegin = m_vecBenchmarkIndex.begin();
		vector<double>::iterator iter_BenchmarkEnd = m_vecBenchmarkIndex.end() - 1;
		m_strategyEvalResult.BenchmarkReturns = ((*iter_BenchmarkEnd) / (*iter_BenchmarkBegin) - 1) / nTradingDayCount * 250;

		///计算beta
		double BenchmarkVariance = Variance(vecBenchmarkIndexReturns);
		double CoVarianceBenchmarkAndDailyReturns = CoVariance(vecBenchmarkIndexReturns, vecDailyReturns);
		m_strategyEvalResult.Beta = CoVarianceBenchmarkAndDailyReturns / BenchmarkVariance;

		///计算alpha
		m_strategyEvalResult.Alpha = (m_strategyEvalResult.AnnualizedReturns - 0.03) - m_strategyEvalResult.Beta * (m_strategyEvalResult.BenchmarkReturns - 0.03);

		///计算信息比率
		vector<double> vecDiffDailyReturnsAndBenchmark;
		for (int i = 0; i < vecDailyReturns.size(); ++i)
		{
			vecDiffDailyReturnsAndBenchmark.push_back(vecDailyReturns[i] - vecBenchmarkIndexReturns[i]);
		}
		double meanDailyReturnsAndBenchmark = Mean(vecDiffDailyReturnsAndBenchmark);
		double stdevDailyReturnsAndBenchmark = StandardDeviation(vecDiffDailyReturnsAndBenchmark);
		m_strategyEvalResult.InformationRatio = 250 * meanDailyReturnsAndBenchmark / (sqrt(250.0) * stdevDailyReturnsAndBenchmark);
	}
}

void CThostStrategyEval::GetEvalResult(CThostStrategyEvalResultField &strategyEvalResult)
{
	strategyEvalResult = m_strategyEvalResult;
}

double CThostStrategyEval::Mean(vector<double> &vecData)
{
	double sum = 0;
	for (int i = 0; i < vecData.size(); ++i)
	{
		sum += vecData[i];
	}
	return sum / vecData.size();
}

double CThostStrategyEval::Variance(vector<double> &vecData)
{
	double variance = 0;
	double mean = Mean(vecData);
	for (int i = 0; i < vecData.size(); ++i)
	{
		variance += (vecData[i] - mean) * (vecData[i] - mean);		
	}

	variance /= vecData.size();
	return variance;
}

double CThostStrategyEval::CoVariance(vector<double> &vecX, vector<double> &vecY)
{
	double meanX = Mean(vecX);
	double meanY = Mean(vecY);

	double covariance = 0;
	for (int i = 0; i < vecX.size(); ++i)
	{
		covariance += (vecX[i] - meanX) * (vecY[i] - meanY);
	}

	covariance /= (vecX.size() - 1);
	return covariance;
}

double CThostStrategyEval::StandardDeviation(vector<double> &vecData)
{
	double variance = Variance(vecData);
	double stdev = sqrt(variance);
	return stdev;
}