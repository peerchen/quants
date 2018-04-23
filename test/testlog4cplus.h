#include <stdio.h>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../misc/loghandler.h"

using namespace boost::uuids;
using namespace std;


class CTestLog4cplus {
public:
	CTestLog4cplus(){}
	~CTestLog4cplus(){}

public:
	void Start()
	{
		m_pLogHandler = new CLogHandler();
		m_pLogHandler->InitLogger("testLog4cplus");
		//m_pLogHandler->AddFileLogger("./testLog4cplus.log");
		m_pLogHandler->AddConsoleLogger();

		index = 0;
		boost::thread thrd1(boost::bind(&CTestLog4cplus::LogInfo, this));
		boost::thread thrd2(boost::bind(&CTestLog4cplus::LogInfo, this));
		boost::thread thrd3(boost::bind(&CTestLog4cplus::LogInfo, this));
	}

	void LogInfo()
	{
		for (int i = 0; i < 10000; ++i)
		{
// 			boost::mutex::scoped_lock lock(io_mutex);
// 			index++;
// 			lock.unlock();
			m_pLogHandler->Info("%d hello world", i+1);
		}
		m_pLogHandler->Info("End.");
	}

private:
	CLogHandler *m_pLogHandler;
	int index;
	boost::mutex io_mutex;
	boost::condition_variable cond;
	boost::mutex exit_mutex;
	boost::uuids::random_generator generator;
};