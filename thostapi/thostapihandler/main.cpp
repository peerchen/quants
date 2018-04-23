#include "thostapihandler.h"
#include "thostapibthandler.h"

CThostApi* CThostApi::CreateThostApi(int nApiMode)
{
	CThostApi *pThostApi = NULL;
	///ʵ��ģʽ
	if (THOST_API_MODE_TRADING == nApiMode)
	{
		pThostApi = new CThostApiHandler();
	}

	///�ز�ģʽ
	if (THOST_API_MODE_BACKTEST == nApiMode)
	{
		pThostApi = new CThostApiBTHandler();
	}

	return pThostApi;
}
