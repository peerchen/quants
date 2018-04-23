#include "thostapihandler.h"
#include "thostapibthandler.h"

CThostApi* CThostApi::CreateThostApi(int nApiMode)
{
	CThostApi *pThostApi = NULL;
	///实盘模式
	if (THOST_API_MODE_TRADING == nApiMode)
	{
		pThostApi = new CThostApiHandler();
	}

	///回测模式
	if (THOST_API_MODE_BACKTEST == nApiMode)
	{
		pThostApi = new CThostApiBTHandler();
	}

	return pThostApi;
}
