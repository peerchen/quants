#include "servermodel.h"
#include <cstdio>
#ifdef WIN32
#include <process.h> 
#else
#include <unistd.h>  
#endif


CServerModel::CServerModel()
{
	m_pid = 0;
	m_pszProcessName = NULL;
	m_pszProcessInfoPathName = NULL;
}

CServerModel::~CServerModel()
{
	if (m_pszProcessName != NULL)
	{
		free(m_pszProcessName);
		m_pszProcessName = NULL;
	}

	if (m_pszProcessInfoPathName != NULL)
	{
		free(m_pszProcessInfoPathName);
		m_pszProcessInfoPathName = NULL;
	}
}

int CServerModel::Start(int argc, char* argv[])
{
	///获取进程id
	m_pid = (int) getpid();

	///获取进程名
	int beginpos, endpos;
	for (beginpos = strlen(argv[0]) - 1; beginpos >= 0; beginpos--)
	{
		if (argv[0][beginpos] == '/' || argv[0][beginpos] == '\\')
		{
			break;
		}
	}
	beginpos++;
	if (beginpos < 0)
	{
		beginpos = 0;
	}

	for (endpos = strlen(argv[0]) - 1; endpos >= 0; endpos--)
	{
		if (argv[0][endpos] == '.')
		{
			break;
		}
	}
	endpos--;
	if (endpos < 0)
	{
		endpos = strlen(argv[0]) - 1;
	}
	
	int nProcessNameLen = endpos - beginpos + 1;
	if (nProcessNameLen > 0)
	{
		m_pszProcessName = (char*) malloc(nProcessNameLen+1);
		memset(m_pszProcessName, '\0', nProcessNameLen);
		strncpy(m_pszProcessName, argv[0] + beginpos, nProcessNameLen);
		m_pszProcessName[nProcessNameLen] = '\0';
	}

	int nPathProcessInfoLen = nProcessNameLen + 40;
	m_pszProcessInfoPathName = (char*) malloc(nPathProcessInfoLen);
	memset(m_pszProcessInfoPathName, '\0', nPathProcessInfoLen);
	sprintf(m_pszProcessInfoPathName, "%s_%d", m_pszProcessName, m_pid);
	return 0;
}


int CServerModel::Wait()
{
	boost::mutex::scoped_lock lock(m_exitMutex);
	m_exitCond.wait(lock);
	return 0;
}

int CServerModel::Stop()
{
	return 0;
}

int CServerModel::GetPID()
{
	return m_pid;
}

const char* CServerModel::GetProcessName()
{
	return m_pszProcessName;
}

const char* CServerModel::GetProcessInfoPathName()
{
	return m_pszProcessInfoPathName;
}
