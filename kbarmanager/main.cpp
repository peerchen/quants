#include "kbarmanager.h"

int main(int argc, char* argv[])
{
	CKBarManager *pKBarManager = new CKBarManager();
	pKBarManager->Start(argc, argv);
	pKBarManager->Wait();
	return 0;
}