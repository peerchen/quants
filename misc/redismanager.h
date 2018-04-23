/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file redismanager.h
///@brief ������redis������ص���
///@history 
///20161130@�쿡		�������ļ�
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
	///redis�����ı���
	redisContext*						m_pRedisContext;
	///redis����ip
	string								m_redisHost;
	///redis����port
	int									m_redisPort;
	///��������
	int									m_reconnectCount;
};

#ifdef _MISC_AS_TEMPLATE_
#include "redismanager.cpp"
#endif

#endif //end INC_REDISMANAGER_H

