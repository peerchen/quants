/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file redismanager.h
///@brief 定义了redis交互相关的类
///@history 
///20161130@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
#ifndef INC_REDISMANAGER_H
#define INC_REDISMANAGER_H
#include <hiredis.h>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

class CRedisManager
{
public:
	CRedisManager();
	~CRedisManager();

public:

	void SetRedisServer(string strRedisHost, int nRedisPort, int nReconnectCount = 3)
	{
		m_redisHost = strRedisHost;
		m_redisPort = nRedisPort;
		m_reconnectCount = nReconnectCount;
	}

	int	Connect();

	int Disconnect();

	redisReply* Command(const char *format, ...);

private:
	///redis上下文变量
	redisContext*						m_pRedisContext;
	///redis服务ip
	string								m_redisHost;
	///redis服务port
	int									m_redisPort;
	///重连次数
	int									m_reconnectCount;
};

#ifdef _MISC_AS_TEMPLATE_
#include "redismanager.cpp"
#endif

#endif //end INC_REDISMANAGER_H

