// CAProxyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <assert.h>

#ifdef DEBUG
	#ifdef _WIN64
	#import "../x64/Debug/CAProxy.dll" no_namespace
	#else
	#import "../Debug/CAProxy.dll" no_namespace
	#endif
#else // DEBUG
	#ifdef _WIN64
	#import "../x64/Release/CAProxy.dll" no_namespace
	#else
	#import "../Release/CAProxy.dll" no_namespace
	#endif
#endif

void test_activex() {
	
	CLSID clsid;
	if (S_OK == CLSIDFromProgID(OLESTR("CAProxy.IKeyClient.1"), &clsid))
	{ 
		ATL::CComPtr<IKeyClient> pMyActiveX;
		if (S_OK == pMyActiveX.CoCreateInstance(clsid)) {
			BSTR result = pMyActiveX->help();
			wprintf(L"%s", result);
			::SysFreeString(result);

			LONG version = pMyActiveX->SOF_GetVersion();
			wprintf(L"%d", version);

			assert(S_OK == pMyActiveX->SOF_SetSignMethod(12L));
		}
	}
}

int main()
{
	CoInitialize(NULL);
	test_activex();
	CoUninitialize();

	getchar();
    return 0;
}

