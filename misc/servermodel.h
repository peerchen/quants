/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file servermodel.h
///@brief 定义了驻留服务的基类
///@history 
///20161124@徐俊		创建该文件
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
	///启动函数
	virtual int Start(int argc, char* argv[]);

	///等待函数
	virtual int Wait();

	///停止函数
	virtual int Stop();

	///获取进程id
	int GetPID();

	///获取进程名称
	const char* GetProcessName();

	///获取路径进程名
	const char* GetProcessInfoPathName();

private:
	///进程id
	int									m_pid;
	///进程名称
	char								*m_pszProcessName;
	///路径中加上进程信息
	char								*m_pszProcessInfoPathName;
	///程序退出的锁
	boost::mutex						m_exitMutex;
	///退出的条件变量
	boost::condition_variable			m_exitCond;

};

#ifdef _MISC_AS_TEMPLATE_
#include "servermodel.cpp"
#endif

#endif //end INC_SERVERMODEL_H