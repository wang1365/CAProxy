#pragma once
#include "IClient.h"
class USBKeyClient :
	public IClient
{
public:
	USBKeyClient();
	virtual ~USBKeyClient();

	virtual void help(BSTR* v);
	virtual int SOF_GetVersion(OUT unsigned int *puiVersion);
};

