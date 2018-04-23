#include "redismanager.h"
#include <boost/thread.hpp>

CRedisManager::CRedisManager()
{
	m_pRedisContext = NULL;
	m_redisHost = "127.0.0.1";
	m_redisPort = 6379;
}

CRedisManager::~CRedisManager()
{
}

int	CRedisManager::Connect()
{
	int i;
	for (i = 0; i < m_reconnectCount; ++i)
	{
		printf("REDIS:try %d connecting.\n", i+1);
		m_pRedisContext = redisConnect(m_redisHost.c_str(), m_redisPort);
		if (m_pRedisContext == NULL || m_pRedisContext->err) 
		{
			continue;
		}
		else
		{
			break;
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}		
	if (i >= m_reconnectCount)
	{
		return 1;
	}
	printf("REDIS:redis connected\n");
	return 0;
}

int CRedisManager::Disconnect()
{
}

redisReply* CRedisManager::Command(const char *format, ...)
{
	char strRedisCommand[5000];
	memset(strRedisCommand, '\0', sizeof(strRedisCommand));
	va_list argPtr;
	va_start( argPtr, format );
	vsprintf( strRedisCommand, format, argPtr );
	va_end( argPtr );
	
	for (int i = 0; i < m_reconnectCount; ++i)
	{
		redisReply *reply = (redisReply*)redisCommand(m_pRedisContext, strRedisCommand);
		if (reply == NULL)
		{
			printf("REDIS:LPUSH depthmarketdata command error, connection maybe disconnect. %s\n", strRedisCommand);
			redisFree(m_pRedisContext);
			if (!Connect())
			{
				continue;
			}
			return NULL;
		}
		return reply;
	}
}