/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file strategyeval.h
///@brief ������������
///@history 
///20161024@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_STRATEGYEVAL_H
#define INC_STRATEGYEVAL_H

#include "../thostapi/thostapistruct.h"

class CThostStrategyEval {
public:
	CThostStrategyEval(){}

	~CThostStrategyEval(){}

public:
	///������������
	void SetEvalSample(vector<double> &vecBalance, vector<double> &vecBenchmarkIndex);

	///��ʼ����ָ��
	void StartEval();

	///��ȡ�������
	void GetEvalResult(CThostStrategyEvalResultField &strategyEvalResult);

private:
	///�����ֵ
	double Mean(vector<double> &vecData);

	///���㷽��
	double Variance(vector<double> &vecData);

	///�����׼��
	double StandardDeviation(vector<double> &vecData);

	///����Э����
	double CoVariance(vector<double> &vecX, vector<double> &vecY);

private:
	///�ʽ��б�,���ϳ�ʼ�ʽ�n+1��,nΪ�����յĸ���
	vector<double>					m_vecBalance;
	///�ο�ָ���б�,��ʼָ��Ϊ��һ�������յ�����ָ��
	vector<double>					m_vecBenchmarkIndex;
	///�����������
	CThostStrategyEvalResultField	m_strategyEvalResult;
};

#ifdef _MISC_AS_TEMPLATE_
#include "strategyeval.cpp"
#endif

#endif // end INC_STRATEGYEVAL_H