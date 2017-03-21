// CAProxyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <iostream>

using namespace std;

#ifdef DEBUG
	#import "../Debug/ClientActiveX.dll" no_namespace
#else // DEBUG
	#import "../Release/ClientActiveX.dll" no_namespace
#endif

void test_activex() {
	
	CLSID clsid;
	if (S_OK == CLSIDFromProgID(OLESTR("CAProxy.IKeyClient.1"), &clsid))
	{ 
		ATL::CComPtr<IKeyClient> pMyActiveX;
		if (S_OK == pMyActiveX.CoCreateInstance(clsid)) {
			BSTR result = pMyActiveX->help();
			wprintf(L"%s\n", result);
			::SysFreeString(result);

			long myversion = 0;
			LONG version = pMyActiveX->SOF_GetVersion();
			wprintf(L"version is :%d\n", version);

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

