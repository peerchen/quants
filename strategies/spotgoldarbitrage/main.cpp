#include "spotgoldarbitrage.h"

int main()
{
	CSpotGoldArbitrage *pSpotGoldArbitrage = new CSpotGoldArbitrage();
	pSpotGoldArbitrage->Start();
	pSpotGoldArbitrage->Wait();
	return 0;
}