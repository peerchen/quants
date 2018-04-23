#include "teststrategyeval.h"
#include "testsimulationtrade.h"
#include "testsgitmd.h"
#include "testsgittrade.h"
#include "testlog4cplus.h"
#include "testkbarhandler.h"

using namespace fstech;
int main()
{
	/*
	CTestStrategyEval *pTestStrategyEval = new CTestStrategyEval("teststrategyeval");
	pTestStrategyEval->Start();

	CTestSimulationTrade *pSimulationTrade = new CTestSimulationTrade("testSimulationTrade");
	pSimulationTrade->Start();
	
	CTestSGITMd *pTestsgitmd = new CTestSGITMd();
	pTestsgitmd->Start();

	CTestSGITTrade *pTestsgittrade = new CTestSGITTrade();
	pTestsgittrade->Start();
	*/

	/*
	CTestLog4cplus *pTestLog4cplus = new CTestLog4cplus();
	pTestLog4cplus->Start();
	*/
	
	CTestKBarHandler *pKBarHandler = new CTestKBarHandler();
	pKBarHandler->Start();

	boost::mutex						m_exitMutex;
	boost::condition_variable			m_exitCond;
	boost::mutex::scoped_lock lock(m_exitMutex);
	m_exitCond.wait(lock);
	return 0;
}