// KeyClient.cpp : CKeyClient 的实现

#include "stdafx.h"
#include "KeyClient.h"
#include "logger.hpp"


// CKeyClient



STDMETHODIMP CKeyClient::help(BSTR* v)
{
	proxy->help(v);
	return S_OK;
}


STDMETHODIMP CKeyClient::SOF_GetVersion(LONG* puiVersion)
{
	// TODO: 在此添加实现代码
	proxy->SOF_GetVersion((unsigned int*)(puiVersion));
	Logger::getInstance().setLevel(Logger::FATAL);
	Logger::getInstance().error("This is a error test");


	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SetSignMethod(LONG SignMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetSignMethod(LONG * SignMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_SetEncryptMethod(LONG EncryptMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetUserList(BSTR* userList)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_ExportUserCert(BSTR CertID, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_Login(BSTR CertID, BSTR PassWd, LONG * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_ChangePassWd(BSTR CertID, BSTR OldPassWd, BSTR NewPassWd, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_ExportExChangeUserCert(BSTR CertID, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetCertInfo(BSTR Cert, LONG Type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetUserInfo(BSTR Cert, LONG Type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_ValidateCert(BSTR Cert, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_SignData(BSTR Cert, BSTR InData, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_SignFile(BSTR Cert, BSTR InFile, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_EncryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_DecryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_PubKeyEncrypt(BSTR Cert, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_PriKeyDecrypt(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_SignDataByP7(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedDataByP7(BSTR P7Data, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetP7SignDataInfo(BSTR P7Data, LONG type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_SignDataXML(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedDataXML(BSTR InData, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GetXMLSignatureInfo(BSTR XMLSignedData, LONG type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_CheckSupport(LONG * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyClient::SOF_GenRandom(LONG len, BSTR * rv)
{
	return E_NOTIMPL;
}
