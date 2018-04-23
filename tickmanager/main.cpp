#include "tickmanager.h"

int main(int argc, char* argv[])
{
	CTickManager *pTickManager = new CTickManager();
	pTickManager->Start(argc, argv);
	pTickManager->Wait();
	return 0;
}