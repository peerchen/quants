/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file servermodel.h
///@brief ������פ������Ļ���
///@history 
///20161124@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_SERVERMODEL_H
#define INC_SERVERMODEL_H

#include <boost/thread.hpp>

using namespace std;

class CServerModel {
public:
	CServerModel();
	~CServerModel();

public:
	///��������
	virtual int Start(int argc, char* argv[]);

	///�ȴ�����
	virtual int Wait();

	///ֹͣ����
	virtual int Stop();

	///��ȡ����id
	int GetPID();

	///��ȡ��������
	const char* GetProcessName();

	///��ȡ·��������
	const char* GetProcessInfoPathName();

private:
	///����id
	int									m_pid;
	///��������
	char								*m_pszProcessName;
	///·���м��Ͻ�����Ϣ
	char								*m_pszProcessInfoPathName;
	///�����˳�����
	boost::mutex						m_exitMutex;
	///�˳�����������
	boost::condition_variable			m_exitCond;

};

#ifdef _MISC_AS_TEMPLATE_
#include "servermodel.cpp"
#endif

#endif //end INC_SERVERMODEL_H