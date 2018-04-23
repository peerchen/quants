/////////////////////////////////////////////////////////////////////////
///@system ����ƽ̨
///@author �쿡
///@file loghandler.h
///@brief ��������־����ͷ�ļ�,����log4cplus��ʵ��
///@history 
///20160818@�쿡		�������ļ�
/////////////////////////////////////////////////////////////////////////

#ifndef INC_LOGHANDLE_H
#define INC_LOGHANDLE_H

#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <log4cplus/logger.h>
#include <log4cplus/layout.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

class CLogHandler {
public:
	CLogHandler();
	~CLogHandler();

public:
	///������־��������ʼ������
	void InitLogger(const char *pszLoggerName);

	///�����ļ�����־
	void AddFileLogger(const char *pszLogFileName);

	///���ӿ���̨����־
	void AddConsoleLogger();

	///���info��־
	void Info(const char *format, ...);

	///���error��־
	void Error(const char *format, ...);

	///���debug��־
	void Debug(const char *format, ...);

private:
	///��־������
	char				*m_pszloggerName;
	///log4cplus�������
	log4cplus::Logger	m_logger;
};


#ifdef _MISC_AS_TEMPLATE_
#include "loghandler.cpp"
#endif

#endif
