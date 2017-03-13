// KeyClient.cpp : CKeyClient µÄÊµÏÖ

#include "stdafx.h"
#include "KeyClient.h"


// CKeyClient



STDMETHODIMP CKeyClient::help(BSTR* v)
{
	if (v != NULL)
	{
		::SysFreeString(*v);
		*v = NULL;
	}

	*v = ::SysAllocString(L"This ActiveX control is used to manage CA from USBKey device.");

	return S_OK;
}
