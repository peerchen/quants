/////////////////////////////////////////////////////////////////////////////////////////////
// 文件名:		mysqlmanager.h
// 创建者:		徐俊
// 创建时间:	19/01/2016
// 内容描述:	mysql的读写操作
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
	///连接数据库
	int Connect(const char *host, const char *user, const char *password, const char *db, unsigned int port,
		const char *unix_socket, unsigned long clientflag);

	///执行mysql语句
	int Execute(const char *query, unsigned long length);

	///获取是否连接
	bool IsConnect();

	///获取mysql语句执行错误信息 
	const char* GetErrMsg();
	
	///获取mysql语句的执行结果
	MYSQL_RES* GetResult();

	///释放mysql结果的内存
	void ReleaseResult();

private:
	///mysql连接对象
	MYSQL m_mysqlconn;

	///是否连接
	bool  m_bIsConnect;
};

#ifdef _MISC_AS_TEMPLATE_
#include "mysqlmanager.cpp"
#endif
#endif