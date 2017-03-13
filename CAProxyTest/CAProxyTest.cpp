// CAProxyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

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
	HRESULT hResult1 = CLSIDFromProgID(OLESTR("CAProxy.IKeyClient.1"), &clsid);
	ATL::CComPtr<IKeyClient> pMyActiveX;
	HRESULT hResult2 = pMyActiveX.CoCreateInstance(clsid);
	BSTR result = ::SysAllocStringLen(L"", 256);
	BSTR p1 = ::SysAllocStringLen(L"", 256);
	pMyActiveX->help(&p1);
	wprintf(L"%s", p1);
}

int main()
{
	CoInitialize(NULL);
	test_activex();
	CoUninitialize();

	getchar();
    return 0;
}

