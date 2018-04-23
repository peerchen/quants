#include "mysqlmanager.h"

int CMysqlManager::Connect(const char *host, const char *user, const char *password, const char *db, unsigned int port, const char *unix_socket, unsigned long clientflag)
{
	mysql_init(&m_mysqlconn);
	if (!mysql_real_connect(&m_mysqlconn,host, user, password, db, port, unix_socket, clientflag))
	{
		return 1;
	}
	m_bIsConnect = true;
	char reconnect = 1;
	mysql_options(&m_mysqlconn, MYSQL_OPT_RECONNECT, (char *)&reconnect);
	return 0;
}

int CMysqlManager::Execute(const char *query, unsigned long length)
{
	if (mysql_real_query(&m_mysqlconn, query, length)) 
	{
		return 1;
	}
	return 0;
}

bool CMysqlManager::IsConnect()
{
	return m_bIsConnect;
}

const char* CMysqlManager::GetErrMsg()
{
	return mysql_error(&m_mysqlconn);
}

MYSQL_RES* CMysqlManager::GetResult()
{
	return mysql_store_result(&m_mysqlconn);
}

void CMysqlManager::ReleaseResult()
{
	do
	{
		MYSQL_RES *pResult = mysql_store_result(&m_mysqlconn);
		mysql_free_result(pResult);
	}while( !mysql_next_result(&m_mysqlconn));
}