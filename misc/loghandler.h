/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file loghandler.h
///@brief 定义了日志功能头文件,基于log4cplus库实现
///@history 
///20160818@徐俊		创建该文件
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
	///根据日志对象名初始化对象
	void InitLogger(const char *pszLoggerName);

	///增加文件的日志
	void AddFileLogger(const char *pszLogFileName);

	///增加控制台的日志
	void AddConsoleLogger();

	///输出info日志
	void Info(const char *format, ...);

	///输出error日志
	void Error(const char *format, ...);

	///输出debug日志
	void Debug(const char *format, ...);

private:
	///日志对象名
	char				*m_pszloggerName;
	///log4cplus处理对象
	log4cplus::Logger	m_logger;
};


#ifdef _MISC_AS_TEMPLATE_
#include "loghandler.cpp"
#endif

#endif
