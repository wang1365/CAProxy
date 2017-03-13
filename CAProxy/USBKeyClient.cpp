#include "stdafx.h"
#include "USBKeyClient.h"
#include "define.h"


USBKeyClient::USBKeyClient()
{
}


USBKeyClient::~USBKeyClient()
{
}


void USBKeyClient::help(BSTR* v)
{
	if (v != NULL)
	{
		::SysFreeString(*v);
		*v = NULL;
	}

	*v = ::SysAllocString(L"help: This ActiveX control is used to manage CA from USBKey device.");
}

int USBKeyClient::SOF_GetVersion(OUT unsigned int *puiVersion)
{
	return SOR_OK;
}