/////////////////////////////////////////////////////////////////////////////////////////////
// �ļ���:		mysqlmanager.h
// ������:		�쿡
// ����ʱ��:	19/01/2016
// ��������:	mysql�Ķ�д����
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef INC_MYSQLMANAGER_H
#define INC_MYSQLMANAGER_H

#ifdef WIN32
#include "winsock.h" 
#endif
#include <mysql.h> 

class CMysqlManager {
public:
	CMysqlManager()
	{
		m_bIsConnect = false;
	}
	virtual ~CMysqlManager(){}
	
public:
	///�������ݿ�
	int Connect(const char *host, const char *user, const char *password, const char *db, unsigned int port,
		const char *unix_socket, unsigned long clientflag);

	///ִ��mysql���
	int Execute(const char *query, unsigned long length);

	///��ȡ�Ƿ�����
	bool IsConnect();

	///��ȡmysql���ִ�д�����Ϣ 
	const char* GetErrMsg();
	
	///��ȡmysql����ִ�н��
	MYSQL_RES* GetResult();

	///�ͷ�mysql������ڴ�
	void ReleaseResult();

private:
	///mysql���Ӷ���
	MYSQL m_mysqlconn;

	///�Ƿ�����
	bool  m_bIsConnect;
};

#ifdef _MISC_AS_TEMPLATE_
#include "mysqlmanager.cpp"
#endif
#endif