// KeyClient.cpp : CKeyClient 的实现

#include "stdafx.h"
#include "KeyClient.h"
#include "../common/logger.hpp"


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
	proxy->SOF_SetSignMethod(static_cast<int>(SignMethod));
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetSignMethod(LONG * SignMethod)
{
	*SignMethod = proxy->SOF_GetSignMethod();
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SetEncryptMethod(LONG EncryptMethod)
{
	proxy->SOF_SetEncryptMethod(static_cast<int>(EncryptMethod));
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetUserList(BSTR* userList)
{
	*userList = proxy->SOF_GetUserList();
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_ExportUserCert(BSTR CertID, BSTR * rv)
{
	*rv = proxy->SOF_ExportExChangeUserCert(CertID);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_Login(BSTR CertID, BSTR PassWd, LONG * rv)
{
	*rv = proxy->SOF_Login(CertID, PassWd);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_ChangePassWd(BSTR CertID, BSTR OldPassWd, BSTR NewPassWd, BOOL * rv)
{
	*rv = proxy->SOF_ChangePassWd(CertID, OldPassWd, NewPassWd);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_ExportExChangeUserCert(BSTR CertID, BSTR * rv)
{
	*rv = proxy->SOF_ExportExChangeUserCert(CertID);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetCertInfo(BSTR Cert, LONG Type, BSTR * rv)
{
	*rv = proxy->SOF_GetCertInfo(Cert, static_cast<short>(Type));
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid, BSTR * rv)
{
	*rv = proxy->SOF_GetCertInfoByOid(Cert, Oid);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetUserInfo(BSTR Cert, LONG Type, BSTR * rv)
{
	*rv = proxy->SOF_GetUserInfo(Cert, static_cast<short>(Type));
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_ValidateCert(BSTR Cert, BOOL * rv)
{
	*rv = proxy->SOF_ValidateCert(Cert);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SignData(BSTR Cert, BSTR InData, BSTR * rv)
{
	*rv = proxy->SOF_SignData(Cert, InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue, BOOL * rv)
{
	*rv = proxy->SOF_VerifySignedData(Cert, InData, SignValue);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SignFile(BSTR Cert, BSTR InFile, BSTR * rv)
{
	*rv = proxy->SOF_SignFile(Cert, InFile);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue, BOOL * rv)
{
	*rv = proxy->SOF_VerifySignedFile(Cert, InFile, SignValue);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_EncryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	*rv = proxy->SOF_EncryptData(SymmKey, Indata);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_DecryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	*rv = proxy->SOF_DecryptData(SymmKey, Indata);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	*rv = proxy->SOF_EncryptFile(SymmKey, InFile, OutFile);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	*rv = proxy->SOF_DecryptFile(SymmKey, InFile, OutFile);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_PubKeyEncrypt(BSTR Cert, BSTR InData, BSTR * rv)
{
	*rv = proxy->SOF_PubKeyEncrypt(Cert, InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_PriKeyDecrypt(BSTR CertID, BSTR InData, BSTR * rv)
{
	*rv = proxy->SOF_PriKeyDecrypt(CertID, InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SignDataByP7(BSTR CertID, BSTR InData, BSTR * rv)
{
	*rv = proxy->SOF_SignDataByP7(CertID, InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedDataByP7(BSTR P7Data, BOOL * rv)
{
	*rv = proxy->SOF_VerifySignedDataByP7(P7Data);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetP7SignDataInfo(BSTR P7Data, LONG type, BSTR * rv)
{
	*rv = proxy->SOF_GetP7SignDataInfo(P7Data, type);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_SignDataXML(BSTR CertID, BSTR InData, BSTR * rv)
{
	*rv = proxy->SOF_SignDataXML(CertID, InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_VerifySignedDataXML(BSTR InData, BOOL * rv)
{
	*rv = proxy->SOF_VerifySignedDataXML(InData);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GetXMLSignatureInfo(BSTR XMLSignedData, LONG type, BSTR * rv)
{
	*rv = proxy->SOF_GetXMLSignatureInfo(XMLSignedData, type);
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_CheckSupport(LONG * rv)
{
	*rv = proxy->SOF_CheckSupport();
	return S_OK;
}

STDMETHODIMP CKeyClient::SOF_GenRandom(LONG len, BSTR * rv)
{
	*rv = proxy->SOF_GenRandom(len);
	return S_OK;
}
