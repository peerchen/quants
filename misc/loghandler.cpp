/////////////////////////////////////////////////////////////////////////
///@system 量化平台
///@author 徐俊
///@file loghandler.h
///@brief 日志功能的实现
///@history 
///20160818@徐俊		创建该文件
/////////////////////////////////////////////////////////////////////////
 
#include "loghandler.h"

CLogHandler::CLogHandler()
{
	m_pszloggerName = NULL;
}

CLogHandler::~CLogHandler()
{
	if (m_pszloggerName != NULL)
	{
		delete m_pszloggerName;
		m_pszloggerName = NULL;
	}
}

void CLogHandler::InitLogger(const char *pszLoggerName)
{
	if (pszLoggerName != NULL)
	{
		m_pszloggerName = new char[strlen(pszLoggerName)+1];
		memset(m_pszloggerName, '\0',strlen(pszLoggerName)+1);
		memcpy(m_pszloggerName, pszLoggerName, strlen(pszLoggerName));
		m_logger = Logger::getInstance(LOG4CPLUS_TEXT(m_pszloggerName));
	}
}

void CLogHandler::AddFileLogger(const char *pszLogFileName)
{
	if (m_pszloggerName != NULL)
	{
		SharedAppenderPtr fileAppend(new FileAppender(LOG4CPLUS_TEXT(pszLogFileName), LOG4CPLUS_FSTREAM_NAMESPACE::ios::app, true));
		std::auto_ptr<Layout> simpleLayout(new PatternLayout(LOG4CPLUS_TEXT("%D %t [%p] %m%n")));   
		fileAppend->setLayout(simpleLayout);
		m_logger.addAppender(fileAppend);      
		m_logger.setLogLevel(ALL_LOG_LEVEL); 
	}
}

void CLogHandler::AddConsoleLogger()
{
	if (m_pszloggerName != NULL)
	{
		SharedObjectPtr<Appender> consoleAppend (new ConsoleAppender());    
		std::auto_ptr<Layout> simpleLayout(new PatternLayout(LOG4CPLUS_TEXT("%D %t [%p] %m%n")));   
		consoleAppend->setLayout(simpleLayout);   
		m_logger.addAppender(consoleAppend);      
		m_logger.setLogLevel(ALL_LOG_LEVEL);
	}   
}

void CLogHandler::Info(const char *format, ...)
{
	char buffer[1000];
	memset(buffer, '\0', sizeof(buffer));
	va_list argPtr;
	va_start( argPtr, format );
	vsprintf( buffer, format, argPtr );
	va_end( argPtr );
	LOG4CPLUS_INFO(m_logger, LOG4CPLUS_TEXT(buffer));
}

void CLogHandler::Error(const char *format, ...)
{
	char buffer[1000];
	memset(buffer, '\0', sizeof(buffer));
	va_list argPtr;
	va_start( argPtr, format );
	vsprintf( buffer, format, argPtr );
	va_end( argPtr );
	LOG4CPLUS_ERROR(m_logger, LOG4CPLUS_TEXT(buffer));
}

void CLogHandler::Debug(const char *format, ...)
{
	char buffer[1000];
	memset(buffer, '\0', sizeof(buffer));
	va_list argPtr;
	va_start( argPtr, format );
	vsprintf( buffer, format, argPtr );
	va_end( argPtr );
	LOG4CPLUS_DEBUG(m_logger, LOG4CPLUS_TEXT(buffer));
}
