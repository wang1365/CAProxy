// KeyClient.cpp : CKeyClient µÄÊµÏÖ

#include "stdafx.h"
#include "KeyClient.h"


// CKeyClient



STDMETHODIMP CKeyClient::help(BSTR* v)
{
	proxy->help(v);
	return S_OK;
}
