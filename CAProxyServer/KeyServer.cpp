// KeyServer.cpp : CKeyServer µÄÊµÏÖ

#include "stdafx.h"
#include "KeyServer.h"


// CKeyServer

STDMETHODIMP CKeyServer::SOF_SetCertTrustList(BSTR CTLAltName, BSTR CTLContent, LONG CTLContentLen)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_QueryCertTrustListAltNames(BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_QueryCertTrustList(BSTR CTLAltName, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_DelCertTrustList(BSTR CTLAltName)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SetWebAppName(BSTR WebAppName)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SetSignMethod(LONG SignMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetSignMethod(LONG * SignMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SetEncryptMethod(LONG EncryptMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetEncryptMethod(LONG * EncryptMethod)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetServerCertificate(LONG CertUsage, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GenRandom(LONG len, LONG * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetCertInfo(BSTR Cert, LONG Type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_ValidateCert(BSTR Cert, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SignData(BSTR Cert, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SignFile(BSTR Cert, BSTR InFile, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_EncryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_DecryptData(BSTR SymmKey, BSTR Indata, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_PubKeyEncrypt(BSTR Cert, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_PriKeyDecrypt(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SignDataByP7(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_VerifySignedDataByP7(BSTR P7Data, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetP7SignDataInfo(BSTR P7Data, LONG type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_SignDataXML(BSTR CertID, BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_VerifySignedDataXML(BSTR InData, BOOL * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetXMLSignatureInfo(BSTR XMLSignedData, LONG type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_CreateTimeStampRequest(BSTR InData, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_VerifyTimeStamp(BSTR content, BSTR tsResponseData, LONG * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetTimeStampInfo(BSTR tsResponseData, LONG type, BSTR * rv)
{
	return E_NOTIMPL;
}

STDMETHODIMP CKeyServer::SOF_GetLastError(LONG * rv)
{
	return E_NOTIMPL;
}
