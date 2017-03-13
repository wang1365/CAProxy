#pragma once
#include "define.h"

class IClient
{
public:
	virtual void help(BSTR* v) = 0;
	virtual int SOF_GetVersion(OUT unsigned int *puiVersion) = 0;
};

