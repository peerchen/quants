#include "../misc/strategyeval.h"
#include <cmath>
#include <cstdio>

class CTestStrategyEval {
public:
	CTestStrategyEval(string testName) 
	{
		m_testName = testName;
	}

	~CTestStrategyEval() {};

public:
	int Start()
	{
		CThostStrategyEval *pThostStrategyEval = new CThostStrategyEval();
		vector<double> vecBechmarkIndex;
		const double eps = 0.00001;
		///sample1
		vector<double> vecBalance;
		vecBalance.push_back(10000);
		vecBalance.push_back(10150);
		vecBalance.push_back(10050);
		vecBalance.push_back(10250);
		vecBalance.push_back(10200);
		vecBalance.push_back(10600);
		vecBalance.push_back(10220);
		vecBalance.push_back(10450);
		vecBalance.push_back(10180);
		vecBalance.push_back(10210);
		vecBalance.push_back(10150);
		
		vecBechmarkIndex.push_back(3016.85);
		vecBechmarkIndex.push_back(2988.09);
		vecBechmarkIndex.push_back(2994.92);
		vecBechmarkIndex.push_back(3049.38);
		vecBechmarkIndex.push_back(3060.69);
		vecBechmarkIndex.push_back(3054.02);
		vecBechmarkIndex.push_back(3054.30);
		vecBechmarkIndex.push_back(3043.56);
		vecBechmarkIndex.push_back(3036.60);
		vecBechmarkIndex.push_back(3027.90);
		vecBechmarkIndex.push_back(3039.01);
		
		CThostStrategyEvalResultField exceptionStrategyEvalResult;
		exceptionStrategyEvalResult.AnnualizedReturns = 0.375;
		exceptionStrategyEvalResult.Volatility = 0.34469;
		exceptionStrategyEvalResult.SharpRatio = 1.00091;
		exceptionStrategyEvalResult.MaxDrawDown = 0.04245;
		exceptionStrategyEvalResult.WinRatio = 0.5;
		exceptionStrategyEvalResult.MaxContinuousWinDays = 1;
		exceptionStrategyEvalResult.MaxContinuousLossDays = 1;
		exceptionStrategyEvalResult.BenchmarkReturns = 0.18364;
		exceptionStrategyEvalResult.Alpha = 0.33559;
		exceptionStrategyEvalResult.Beta = 0.06127;
		exceptionStrategyEvalResult.InformationRatio = 0.67507;

		pThostStrategyEval->SetEvalSample(vecBalance, vecBechmarkIndex);
		pThostStrategyEval->StartEval();
		CThostStrategyEvalResultField strategyEvalResult;
		pThostStrategyEval->GetEvalResult(strategyEvalResult);

		int ans1 = 0;
		if (fabs(exceptionStrategyEvalResult.AnnualizedReturns - strategyEvalResult.AnnualizedReturns) < eps &&
			fabs(exceptionStrategyEvalResult.Volatility - strategyEvalResult.Volatility) < eps &&
			fabs(exceptionStrategyEvalResult.SharpRatio - strategyEvalResult.SharpRatio) < eps &&
			fabs(exceptionStrategyEvalResult.MaxDrawDown - strategyEvalResult.MaxDrawDown) < eps &&
			fabs(exceptionStrategyEvalResult.WinRatio - strategyEvalResult.WinRatio) < eps &&
			fabs(exceptionStrategyEvalResult.MaxContinuousWinDays - strategyEvalResult.MaxContinuousWinDays) < eps &&
			fabs(exceptionStrategyEvalResult.MaxContinuousLossDays - strategyEvalResult.MaxContinuousLossDays) < eps &&
			fabs(exceptionStrategyEvalResult.BenchmarkReturns - strategyEvalResult.BenchmarkReturns) < eps &&
			fabs(exceptionStrategyEvalResult.Alpha - strategyEvalResult.Alpha) < eps &&
			fabs(exceptionStrategyEvalResult.Beta - strategyEvalResult.Beta) < eps &&
			fabs(exceptionStrategyEvalResult.InformationRatio - strategyEvalResult.InformationRatio) < eps)
		{
			ans1 =1;
		}

		///sample2
		vecBalance.clear();
		vecBalance.push_back(10000);
		int initBalance = 10000;
		for (int i = 0; i < 20; ++i)
		{
			if (i % 2 == 0)
			{
				initBalance += 100;
			}
			else
			{
				initBalance -= 50;
			}
			vecBalance.push_back(initBalance);
		}
		vecBechmarkIndex.push_back(3012.82);
		vecBechmarkIndex.push_back(3015.83);
		vecBechmarkIndex.push_back(3050.17);
		vecBechmarkIndex.push_back(2992.00);
		vecBechmarkIndex.push_back(2994.32);
		vecBechmarkIndex.push_back(2979.34);
		vecBechmarkIndex.push_back(2953.39);
		vecBechmarkIndex.push_back(2971.28);
		vecBechmarkIndex.push_back(2978.46);
		vecBechmarkIndex.push_back(2982.43);

		exceptionStrategyEvalResult.AnnualizedReturns = 0.625;
		exceptionStrategyEvalResult.Volatility = 0.11564;
		exceptionStrategyEvalResult.SharpRatio = 5.14542;
		exceptionStrategyEvalResult.MaxDrawDown = 0.00495;
		exceptionStrategyEvalResult.WinRatio = 0.5;
		exceptionStrategyEvalResult.MaxContinuousWinDays = 1;
		exceptionStrategyEvalResult.MaxContinuousLossDays = 1;
		exceptionStrategyEvalResult.BenchmarkReturns = -0.14262;
		exceptionStrategyEvalResult.Alpha = 0.60116;
		exceptionStrategyEvalResult.Beta = 0.03567;
		exceptionStrategyEvalResult.InformationRatio = 4.56087;

		pThostStrategyEval->SetEvalSample(vecBalance, vecBechmarkIndex);
		pThostStrategyEval->StartEval();
		pThostStrategyEval->GetEvalResult(strategyEvalResult);

		int ans2 = 0;
		if (fabs(exceptionStrategyEvalResult.AnnualizedReturns - strategyEvalResult.AnnualizedReturns) < eps &&
			fabs(exceptionStrategyEvalResult.Volatility - strategyEvalResult.Volatility) < eps &&
			fabs(exceptionStrategyEvalResult.SharpRatio - strategyEvalResult.SharpRatio) < eps &&
			fabs(exceptionStrategyEvalResult.MaxDrawDown - strategyEvalResult.MaxDrawDown) < eps &&
			fabs(exceptionStrategyEvalResult.WinRatio - strategyEvalResult.WinRatio) < eps &&
			fabs(exceptionStrategyEvalResult.MaxContinuousWinDays - strategyEvalResult.MaxContinuousWinDays) < eps &&
			fabs(exceptionStrategyEvalResult.MaxContinuousLossDays - strategyEvalResult.MaxContinuousLossDays) < eps &&
			fabs(exceptionStrategyEvalResult.BenchmarkReturns - strategyEvalResult.BenchmarkReturns) < eps &&
			fabs(exceptionStrategyEvalResult.Alpha - strategyEvalResult.Alpha) < eps &&
			fabs(exceptionStrategyEvalResult.Beta - strategyEvalResult.Beta) < eps &&
			fabs(exceptionStrategyEvalResult.InformationRatio - strategyEvalResult.InformationRatio) < eps)
		{
			ans2 =1;
		}

// 		printf ("%.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f %.5f\n", 
// 			strategyEvalResult.AnnualizedReturns,
// 			strategyEvalResult.Volatility,
// 			strategyEvalResult.SharpRatio,
// 			strategyEvalResult.MaxDrawDown,
// 			strategyEvalResult.WinRatio,
// 			strategyEvalResult.MaxContinuousWinDays,
// 			strategyEvalResult.MaxContinuousLossDays,
// 			strategyEvalResult.BenchmarkReturns,
// 			strategyEvalResult.Alpha,
// 			strategyEvalResult.Beta,
// 			strategyEvalResult.InformationRatio);
		///sample3
		
		delete pThostStrategyEval;
		int ans = ans1+ans2 == 2 ? 0 : 1;
		printf ("%s:%d\n", m_testName.c_str(), ans);
		return ans;
	}
private:
	string m_testName;

};
