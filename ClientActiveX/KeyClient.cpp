// KeyClient.cpp : CKeyClient 的实现

#include "stdafx.h"
#include "KeyClient.h"
#include "define.h"
#include "HTCSPUtil.h"
#include "SKFAPI/SKFAPI.h"
#include "Base64.h"
#include "comutil.h"
#include "stringutil.h"
#include "util.hpp"

// CKeyClient



STDMETHODIMP CKeyClient::help(BSTR* v)
{
	return S_OK;
}


/* 1
原型：	int SOF_GetVersion(unsigned int *puiVersion)
描述：	获取接口的版本号
参数：	puiVersion [out]	版本号
返回值：	0	成功
非0	失败，返回错误代码
备注：	版本号的格式为：0xAAAABBBB,其中AAAA为主版本号，BBBB为次版本号。
*/
STDMETHODIMP CKeyClient::SOF_GetVersion(LONG* puiVersion)
{
	*puiVersion = 0x00010003;

	return S_OK;
}

// 2
STDMETHODIMP CKeyClient::SOF_SetSignMethod(LONG signMethod)
{
	this->_signMethod = signMethod;
	return S_OK;
}

// 3
STDMETHODIMP CKeyClient::SOF_GetSignMethod(LONG *signMethod)
{
	*signMethod = this->_signMethod;
	return S_OK;
}

// 4
STDMETHODIMP CKeyClient::SOF_SetEncryptMethod(LONG encryptMethod)
{
	this->_encryptMethod = encryptMethod;
	return S_OK;
}

// 5
STDMETHODIMP CKeyClient::SOF_GetEncryptMethod(LONG* encryptMethod)
{
	*encryptMethod = this->_encryptMethod;
	return S_OK;
}

/* 6
原型：	BSTR SOF_GetUserList();
描述：	取得当前已安装证书的用户列表
参数：	无
返回值：	BSTR ret  用户列表字符串	数据格式：(用户1||标识1&&&用户2||标识2&&&…)
*/
STDMETHODIMP CKeyClient::SOF_GetUserList(BSTR* userList)
{
	HS_Init();

	DWORD dwRet = 0, dwCertIDLen = 0;
	char* szCertID = new char[8000];

	dwRet = HS_ListCertID(szCertID);
	dwCertIDLen = strlen(szCertID);

	WCHAR wRetData[1024];
	memset(wRetData, 0x00, 1024);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szCertID, -1, (LPWSTR)wRetData, dwCertIDLen);
	nDataLen = wcslen(wRetData);
	*userList = SysAllocStringLen(wRetData, nDataLen);
	delete[]szCertID;

	return S_OK;
}

// 7
STDMETHODIMP CKeyClient::SOF_ExportUserCert(BSTR CertID, BSTR * strCertContext)
{
	HS_Init();

	DWORD dwRet = 0, dwConNameLen = 1024, dwConNum = 1024, i = 0, dwCertLen = 6000;
	DWORD dwBase64Len = 10 * 1024, dwConParam = 0;
	HANDLE hCard = NULL;
	BYTE bCert[6000] = { 0 }, bBase64[10 * 1024] = { 0 };
	 
	char szConName[1024] = { 0 };

	char szDeviceName[256] = { 0 }, szContainerName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szContainerName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);

	for (i = 0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if ((dwConParam & 0x00020000) == 0x00020000)
		{
			dwRet = memcmp(szConName, szContainerName, strlen(szConName));
			if (0 == dwRet)
			{
				dwRet = HSReadCert(hCard, szConName, AT_SIGNATURE, bCert, &dwCertLen);
				if (dwRet == 0)
				{
					CBase64Codec::Encode(bCert, dwCertLen, bBase64, &dwBase64Len);

					WCHAR *wRetData = new WCHAR[dwBase64Len * 4];
					memset(wRetData, 0x00, dwBase64Len * 4);
					int nDataLen = 0;
					MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase64, -1, (LPWSTR)wRetData, dwBase64Len);
					nDataLen = wcslen(wRetData);
					*strCertContext = SysAllocStringLen(wRetData, nDataLen);
					delete[]wRetData;

					return S_OK;
				}
				break;
			}
		}
	}

	*strCertContext = SysAllocString(L"Fail");
	return S_OK;
}

// 8
STDMETHODIMP CKeyClient::SOF_Login(BSTR CertID, BSTR PassWd, int *nRetryCount)
{
	HS_Init();

	DWORD dwRet = 0, dwRetryCount = 0;
	HANDLE hCard = NULL;
	char *pPassWord = NULL;

	pPassWord = _com_util::ConvertBSTRToString(PassWd);

	char szDeviceName[256] = { 0 }, szConName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if ((pCertID == NULL) || (pPassWord == NULL))
	{
		*nRetryCount = 0xFF;
		return S_FALSE;
	}

	if ((strlen(pPassWord) == 0) || (strlen(pCertID) == 0))
	{
		*nRetryCount = 0xFF;
		return S_FALSE;
	}

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szConName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HSVerifyUserPin(hCard, pPassWord, &dwRetryCount);
	if (dwRet == 0x8800003D)
	{
		*nRetryCount = dwRetryCount;
	}
	else if (dwRet == 0x8800003E)
	{
		*nRetryCount = -1;
	}
	else if (dwRet == 0)
	{
		*nRetryCount = 0;
	}
	else
	{
		*nRetryCount = 0xFF;
	}

	return S_OK;
}

// 9
STDMETHODIMP CKeyClient::SOF_ChangePassWd(BSTR CertID, BSTR OldPassWd, BSTR NewPassWd, VARIANT_BOOL * pbResult)
{
	HS_Init();

	DWORD dwRet = 0, dwRetryCount = 0;
	char *pOldPIN = NULL, *pNewPIN = NULL, *pCertID = NULL;
	HANDLE hCard = NULL;
	WCHAR wszRetryCount[200] = { 0 };
	char szDeviceName[256] = { 0 }, szConName[256] = { 0 };

	try
	{
		pCertID = _com_util::ConvertBSTRToString(CertID);
		pOldPIN = _com_util::ConvertBSTRToString(OldPassWd);
		pNewPIN = _com_util::ConvertBSTRToString(NewPassWd);

		if ((pCertID == NULL) || (pOldPIN == NULL) || (pNewPIN == NULL))
		{
			*pbResult = FALSE;
			return S_FALSE;
		}

		if (strlen(pCertID) > 100)
		{
			*pbResult = FALSE;
			return S_FALSE;
		}

		dwRet = HS_ChangeCertID(pCertID, szDeviceName, szConName);
		if (dwRet != 0)
		{
			return S_FALSE;
		}

		dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

		dwRet = HSChangeUserPin(hCard, pOldPIN, pNewPIN, &dwRetryCount);
		if (dwRet == 0)
		{
			*pbResult = TRUE;
			return S_OK;
		}
	}
	catch (...)
	{
		*pbResult = FALSE;
		return S_OK;
	}

	*pbResult = FALSE;
	return S_OK;
}

// 10
STDMETHODIMP CKeyClient::SOF_ExportExChangeUserCert(BSTR CertID, BSTR * strCert)
{
	HS_Init();

	DWORD dwRet = 0, dwConNameLen = 1024, dwConNum = 1024, i = 0, dwCertLen = 6000;
	DWORD dwBase64Len = 10 * 1024, dwConParam = 0;
	HANDLE hCard = NULL;
	BYTE bCert[6000] = { 0 }, bBase64[10 * 1024] = { 0 };
	CBase64Codec Base64;
	char szConName[1024] = { 0 };

	char szDeviceName[256] = { 0 }, szContainerName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szContainerName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);

	for (i = 0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if ((dwConParam & 0x00000002) == 0x00000002)
		{
			dwRet = memcmp(szConName, szContainerName, strlen(szConName));
			if (0 == dwRet)
			{
				dwRet = HSReadCert(hCard, szConName, AT_KEYEXCHANGE, bCert, &dwCertLen);
				if (dwRet == 0)
				{
					Base64.Encode(bCert, dwCertLen, bBase64, &dwBase64Len);

					WCHAR *wRetData = new WCHAR[dwBase64Len * 4];
					memset(wRetData, 0x00, dwBase64Len * 4);
					int nDataLen = 0;
					MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase64, -1, (LPWSTR)wRetData, dwBase64Len);
					nDataLen = wcslen(wRetData);
					*strCert = SysAllocStringLen(wRetData, nDataLen);
					delete[]wRetData;

					return S_OK;
				}
				break;
			}
		}
	}

	*strCert = SysAllocString(L"Fail");
	return S_OK;
}

// 11
STDMETHODIMP CKeyClient::SOF_GetCertInfo(BSTR Cert, LONG usType, BSTR * strCertInfo)
{
	//HS_Init();

	DWORD /*dwRet = 0, dwLen = 0, */ dwRelCertLen = 3000, dwCertLen = 0;
	PCCERT_CONTEXT pCertContext = NULL;
	//SYSTEMTIME CertTime = {0};
	//FILETIME LocalTime = {0};
	/*char szRetBuf[2048] = {0}, szCert[3000] = {0},*p = NULL;*/
	char*p = NULL;
	wchar_t szRetBuf[2048] = { 0 };
	BYTE bCertData[3000] = { 0 };

	p = _com_util::ConvertBSTRToString(Cert);
	dwCertLen = strlen(p);

	CBase64Codec::Decode((BYTE *)p, dwCertLen, bCertData, &dwRelCertLen);

	pCertContext = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		bCertData, dwRelCertLen);
	if (NULL == pCertContext)
	{
		return S_FALSE;
	}

	try
	{

		//CCertParser parser(pCertContext);  //Link failed with Certsel.lib
		//wstring sret = L"";

		//switch (usType)
		//{
		//case SGD_GET_CERT_VERSION://证书版本
		//{
		//	sret = parser.GetCertVersion();
		//	//sprintf(szRetBuf, "V%d", (pCertContext->pCertInfo->dwVersion +1));
		//}
		//break;
		//case SGD_GET_CERT_SERIAL://证书序列号
		//{
		//	sret = parser.GetCertSerialNum();
		//	//BYTE bySN[0x40] = {0};
		//	//memcpy(bySN, pCertContext->pCertInfo->SerialNumber.pbData, pCertContext->pCertInfo->SerialNumber.cbData);
		//	////逆序，符合规范
		//	//reverse(bySN, pCertContext->pCertInfo->SerialNumber.cbData);
		//	//for(int i=0;i<pCertContext->pCertInfo->SerialNumber.cbData;i++)
		//	//{
		//	//	sprintf(&(szRetBuf[i*2]), "%02X", bySN[i]);
		//	//}
		//}
		//break;
		//case SGD_GET_CERT_SIGNALG://证书签名算法标识
		//{
		//	sret = parser.GetCertSignatureAlg();
		//	//strcpy(szRetBuf, pCertContext->pCertInfo->SignatureAlgorithm.pszObjId);
		//	//dwLen = strlen(szRetBuf);
		//}
		//break;
		//case SGD_GET_CERT_ISSUER_C://证书颁发者国家(C)	
		//						   //strcpy(szRetBuf, "CN");
		//						   //dwLen = strlen(szRetBuf);
		//						   //dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
		//						   //CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,CERT_NAME_ISSUER_FLAG, NULL, szRetBuf, dwLen + 1);
		//						   //break;
		//case SGD_GET_CERT_ISSUER_O://证书颁发者组织名(O)
		//						   //strcpy(szRetBuf, "JSCA");
		//						   //dwLen = strlen(szRetBuf);
		//						   //dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
		//						   //CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, szRetBuf, dwLen + 1);
		//						   //break;
		//case SGD_GET_CERT_ISSUER_OU://证书颁发者部门名(OU)
		//							//{
		//							//	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_ORGANIZATIONAL_UNIT_NAME, NULL, 0);
		//							//	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_ORGANIZATIONAL_UNIT_NAME,  szRetBuf, dwLen + 1);
		//							//}
		//							//break;
		//case SGD_GET_CERT_ISSUER_S://证书颁发者所在的省、自治区、直辖市(S)
		//						   //{
		//						   //	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, szOID_STATE_OR_PROVINCE_NAME, NULL, 0);
		//						   //	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_STATE_OR_PROVINCE_NAME,  szRetBuf, dwLen + 1);
		//						   //}
		//						   //break;
		//case SGD_GET_CERT_ISSUER_CN:	//证书颁发者通用名称(CN)
		//								//dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
		//								//CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, szRetBuf, dwLen + 1);
		//								//break;
		//case SGD_GET_CERT_ISSUER_L://证书颁发者所在的城市、地区(L)
		//						   //{
		//						   //	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, szOID_LOCALITY_NAME, NULL, 0);
		//						   //	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_LOCALITY_NAME,  szRetBuf, dwLen + 1);
		//						   //}
		//						   //break;
		//case SGD_GET_CERT_ISSUER_E://证书颁发者Email
		//						   //{
		//						   //	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, szOID_RSA_emailAddr, NULL, 0);
		//						   //	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_RSA_emailAddr,  szRetBuf, dwLen + 1);
		//						   //}


		//						   //break;

		//case SGD_GET_CERT_SUBJECT_C://证书拥有者国家(C )
		//							//{
		//							//	strcpy(szRetBuf, "CN");
		//							//	dwLen = strlen(szRetBuf);
		//							//	//dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
		//							//	//CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,0, NULL, szRetBuf, dwLen + 1);
		//							//}
		//							//break;
		//case SGD_GET_CERT_SUBJECT_O://证书拥有者组织名(O)
		//							/*{
		//							dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, szOID_ORGANIZATION_NAME, NULL, 0);
		//							CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, szOID_ORGANIZATION_NAME, szRetBuf, dwLen + 1);
		//							}
		//							break;*/
		//case SGD_GET_CERT_SUBJECT_OU://证书拥有者部门名(OU)
		//							 /*{
		//							 dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_ORGANIZATIONAL_UNIT_NAME, NULL, 0);
		//							 CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_ORGANIZATIONAL_UNIT_NAME,  szRetBuf, dwLen + 1);
		//							 }
		//							 break;*/
		//case SGD_GET_CERT_SUBJECT_S://证书拥有者所在的省、自治区、直辖市(S)
		//							/*{
		//							dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_STATE_OR_PROVINCE_NAME, NULL, 0);
		//							CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_STATE_OR_PROVINCE_NAME,  szRetBuf, dwLen + 1);
		//							}
		//							break;
		//							*/
		//case SGD_GET_CERT_SUBJECT_L://证书拥有者所在的城市、地区(L)
		//							/*{
		//							dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_LOCALITY_NAME, NULL, 0);
		//							CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_LOCALITY_NAME,  szRetBuf, dwLen + 1);
		//							}*/
		//							//break;	
		//case SGD_GET_CERT_SUBJECT_E://证书拥有者Email
		//							/*{
		//							dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_RSA_emailAddr, NULL, 0);
		//							CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_RSA_emailAddr,  szRetBuf, dwLen + 1);
		//							}*/

		//							//break;
		//case SGD_GET_CERT_SUBJECT_CN://证书拥有者通用名称(CN)
		//							 //dwLen = CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
		//							 //CertGetNameString(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE,0, NULL,  szRetBuf, dwLen + 1);

		//	sret = parser.GetSubjectAndIssuerInfo(usType);
		//	break;
		//case SGD_GET_CERT_NOTBEFORE://证书有效期：起始日期
		//	sret = parser.GetCertPeriod();
		//	/*FileTimeToLocalFileTime(&pCertContext->pCertInfo->NotBefore, &LocalTime);
		//	FileTimeToSystemTime(&LocalTime, &CertTime);
		//	sprintf(szRetBuf, "%d-%d-%d", CertTime.wYear, CertTime.wMonth, CertTime.wDay);*/
		//	break;
		//case SGD_GET_CERT_AFTER://证书有效期：终止日期
		//						/*FileTimeToLocalFileTime(&pCertContext->pCertInfo->NotAfter,&LocalTime);
		//						FileTimeToSystemTime(&LocalTime, &CertTime);
		//						sprintf(szRetBuf, "%d-%d-%d", CertTime.wYear, CertTime.wMonth, CertTime.wDay);*/
		//	sret = parser.GetCertPeriod(FALSE);
		//	break;
		//case SGD_GET_CERT_ISSUER_DN://证书颁发者DN
		//	sret = parser.GetCertIssuer();
		//	/*{
		//	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, szOID_COMMON_NAME, NULL, 0);
		//	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_COMMON_NAME,  szRetBuf, dwLen + 1);
		//	}*/
		//	break;
		//case SGD_GET_CERT_SUBJECT_DN://证书拥有者DN
		//	sret = parser.GetCertSubject();
		//	//{
		//	//	//dwLen = CertGetNameString(pCertContext,CERT_NAME_DNS_TYPE,0,NULL,szRetBuf,2048);
		//	//	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_COMMON_NAME, NULL, 0);
		//	//	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_COMMON_NAME,  szRetBuf, dwLen + 1);
		//	//}
		//	break;
		//case SGD_GET_CERT_DER_PUBKEY://证书公钥信息
		//{
		//	for (size_t i = 0; i<pCertContext->pCertInfo->SubjectPublicKeyInfo.PublicKey.cbData; i++)
		//	{
		//		swprintf(&(szRetBuf[i * 2]), sizeof(szRetBuf) - i*2, L"%02X", pCertContext->pCertInfo->SubjectPublicKeyInfo.PublicKey.pbData[i]);
		//	}
		//	sret = szRetBuf;
		//}
		//break;
		//case SGD_GET_CERT_DER_EXTENSIONS://证书扩展项信息
		//{
		//	//done
		//}
		//break;
		//case SGD_EXT_AUTHORITYKEYIDENTIFIER:	//颁发者密钥标识符
		//										//{
		//										//	for(int i=0;i<pCertContext->pCertInfo->cExtension;i++)
		//										//	{
		//										//		if(0 == memcmp("2.5.29.35", pCertContext->pCertInfo->rgExtension[i].pszObjId, strlen(pCertContext->pCertInfo->rgExtension[i].pszObjId)))
		//										//		{
		//										//			// 密钥用法KeyUsage

		//										//			CERT_AUTHORITY_KEY_ID2_INFO pValue;

		//										//			if(FALSE == CryptDecodeObjectEx( X509_ASN_ENCODING, "2.5.29.35", pCertContext->pCertInfo->rgExtension[i].Value.pbData,
		//										//				pCertContext->pCertInfo->rgExtension[i].Value.cbData, CRYPT_DECODE_NOCOPY_FLAG, NULL, NULL, &dwLen))
		//										//			{
		//										//				break;
		//										//			}

		//										//			CryptDecodeObjectEx( X509_ASN_ENCODING, "2.5.29.35", pCertContext->pCertInfo->rgExtension[i].Value.pbData,
		//										//				pCertContext->pCertInfo->rgExtension[i].Value.cbData, CRYPT_DECODE_NOCOPY_FLAG, NULL, &pValue, &dwLen);

		//										//			HexToStr(pValue.KeyId.pbData, pValue.KeyId.cbData, (BYTE *)szRetBuf);
		//										//		}
		//										//	}

		//										//	// 			dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, CERT_NAME_ISSUER_FLAG, szOID_AUTHORITY_KEY_IDENTIFIER2, NULL, 0);
		//										//	// 			CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,CERT_NAME_ISSUER_FLAG, szOID_AUTHORITY_KEY_IDENTIFIER2,  szRetBuf, dwLen + 1);

		//										//}
		//										//break; 
		//case SGD_EXT_SUBJECTKEYIDENTIFIER://证书持有者密钥标识符
		//								  /*{
		//								  dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_AUTHORITY_KEY_IDENTIFIER2, NULL, 0);
		//								  CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_AUTHORITY_KEY_IDENTIFIER2,  szRetBuf, dwLen + 1);
		//								  }
		//								  break ;*/
		//case SGD_EXT_KEYUSAGE://密钥用途
		//					  //{
		//					  //	// 			dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_KEY_USAGE, NULL, 0);
		//					  //	// 			CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_KEY_USAGE,  szRetBuf, dwLen + 1);

		//					  //	for(int i=0;i<pCertContext->pCertInfo->cExtension;i++)
		//					  //	{
		//					  //		if(0 == memcmp("2.5.29.15", pCertContext->pCertInfo->rgExtension[i].pszObjId, strlen(pCertContext->pCertInfo->rgExtension[i].pszObjId)))
		//					  //		{
		//					  //			// 密钥用法KeyUsage

		//					  //			DWORD dwUsage = 0;

		//					  //			if(CertGetIntendedKeyUsage(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pCertContext->pCertInfo, (BYTE *)&dwUsage, 1))
		//					  //			{
		//					  //				sprintf(szRetBuf, "%X", dwUsage);
		//					  //			}
		//					  //		}
		//					  //	}
		//					  //}

		//					  //break; 
		//case SGD_EXT_PRIVATEKEYUSAGEPERIOD://私钥有效期
		//								   /*FileTimeToLocalFileTime(&pCertContext->pCertInfo->NotAfter,&LocalTime);
		//								   FileTimeToSystemTime(&LocalTime, &CertTime);
		//								   sprintf(szRetBuf, "%d-%d-%d", CertTime.wYear, CertTime.wMonth, CertTime.wDay);
		//								   break; 	*/
		//case SGD_EXT_CERTIFICATEPOLICIES://证书策略
		//								 /*{
		//								 dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_CERT_POLICIES, NULL, 0);
		//								 CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_CERT_POLICIES,  szRetBuf, dwLen + 1);
		//								 }
		//								 break;	 	*/
		//case SGD_EXT_POLICYMAPPINGS://策略映射
		//							/*{
		//							dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_POLICY_MAPPINGS, NULL, 0);
		//							CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_POLICY_MAPPINGS,  szRetBuf, dwLen + 1);
		//							}
		//							break;	 */
		//case SGD_EXT_BASICCONSTRAINTS://基本限制
		//							  //{//szOID_NAME_CONSTRAINTS
		//							  //	dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_BASIC_CONSTRAINTS2, NULL, 0);
		//							  //	CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_BASIC_CONSTRAINTS2,  szRetBuf, dwLen + 1);
		//							  //}
		//							  //break;	 	
		//case SGD_EXT_POLICYCONSTRAINTS://策略限制
		//							   /*{
		//							   dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_BASIC_CONSTRAINTS, NULL, 0);
		//							   CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_BASIC_CONSTRAINTS,  szRetBuf, dwLen + 1);
		//							   }
		//							   break;	 	*/
		//case SGD_EXT_EXTKEYUSAGE://扩展密钥用途
		//						 /*{
		//						 dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_ENHANCED_KEY_USAGE, NULL, 0);
		//						 CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_ENHANCED_KEY_USAGE,  szRetBuf, dwLen + 1);
		//						 }
		//						 break;	*/
		//case SGD_EXT_CRLDISTRIBUTIONPO://CRL发布点
		//							   /*{
		//							   dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_CRL_DIST_POINTS, NULL, 0);
		//							   CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_CRL_DIST_POINTS,  szRetBuf, dwLen + 1);
		//							   }
		//							   break;*/
		//case SGD_EXT_NETSCAPE_CERT_TYPE://netscape属性
		//								/*{
		//								dwLen = CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_SGC_NETSCAPE, NULL, 0);
		//								CertGetNameString(pCertContext, CERT_NAME_ATTR_TYPE,0, szOID_SGC_NETSCAPE,  szRetBuf, dwLen + 1);
		//								}
		//								break;*/
		//case SGD_EXT_CERT_UNIQUEID://证书实体唯一标识
		//						   /*{
		//						   memcpy(szRetBuf, pCertContext->pCertInfo->SubjectUniqueId.pbData, pCertContext->pCertInfo->SubjectUniqueId.cbData);
		//						   }*/
		//	sret = parser.GetCertExt(usType);
		//	break;
		//case SGD_EXT_IDENTIFYCARDNUMBER://个人身份证号码
		//	break;
		//case SGD_EXT_INURANCENUMBER://个人社会保险号
		//	break;
		//case SGD_EXT_ICREGISTRATIONNUMBER://企业工商注册号
		//	break;
		//case SGD_EXT_ORGANIZATIONCODE://企业组织机构代码
		//	break;
		//case SGD_EXT_TAXATIONNUMBER://税务登记证号
		//	break;
		//default:
		//	CertFreeCertificateContext(pCertContext);
		//	return S_FALSE;
		//}

		//CertFreeCertificateContext(pCertContext);
		//if (!sret.empty())
		//{
		//	*strCertInfo = SysAllocString(sret.c_str());
		//	return S_OK;
		//}
	}
	catch (...)
	{
		*strCertInfo = NULL;
	}

	/*WCHAR wRetData[1024] ;
	memset( wRetData , 0x00 , 1024);
	int nDataLen = 0;
	dwLen = strlen(szRetBuf);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szRetBuf, -1, (LPWSTR)wRetData, dwLen);
	nDataLen=wcslen(wRetData);
	*strCertInfo=SysAllocStringLen(wRetData, nDataLen);*/

	return S_OK;
}

// 12
STDMETHODIMP CKeyClient::SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid, BSTR * strCertExInfo)
{
	//if ((Oid == NULL) || (Cert == NULL))
	//{
	//	return S_FALSE;
	//}

	//DWORD /*dwRet = 0, */dwCertLen = 0, dwRelCertLen = 4000;//, dwOutDataLen = 0;
	//char *pCert = NULL, *pOid = NULL, szOutData[1024] = { 0 };
	//CBase64Codec Base64;
	//BYTE bCertData[4000] = { 0 };
	//PCCERT_CONTEXT pCertContext = NULL;
	////PCERT_EXTENSION pExtension = NULL;
	////char szRetBuf[400] = {0};
	////DWORD dwLen = 400;

	//pCert = _com_util::ConvertBSTRToString(Cert);

	////if ((pOid == NULL) || (pCert == NULL))
	////{
	////	return S_FALSE;
	////}

	////if ((strlen(pOid) == 0) || (strlen(pCert) == 0))
	////{
	////	return S_FALSE;
	////}

	//dwCertLen = strlen(pCert);
	//Base64.Decode((BYTE *)pCert, dwCertLen, bCertData, &dwRelCertLen);

	//pCertContext = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, bCertData, dwRelCertLen);

	//if (pCertContext == NULL)
	//	return S_FALSE;

	//try
	//{
	//	CCertParser parser(pCertContext);
	//	*strCertExInfo = SysAllocString(parser.GetCertExt(0, CStringUtil::ws2s(Oid).c_str()).c_str());
	//}
	//catch (...)
	//{
	//	*strCertExInfo = NULL;
	//}

	//CertFreeCertificateContext(pCertContext);
	return S_OK;
}

// 13
STDMETHODIMP CKeyClient::SOF_GetUserInfo(BSTR CertID, LONG ustype, BSTR * strUserInfo)
{
//	HS_Init();
//
//	DWORD dwRet = 0, dwType = 0, dwLableLen = 0;
//	HANDLE hCard = NULL;
//	BYTE bCert[3000] = { 0 };
//	DWORD dwCertLen = 3000, dwRetBufLen = 3000, dwSize = 0, dwLen = 0, dwConParam = 0;
//	char szRetBuf[3000] = { 0 };
//	size_t bufSize = sizeof(szRetBuf) - 1;
//	PCCERT_CONTEXT pCertContext = NULL;
//	BOOL bRet = FALSE;
//	CRYPT_KEY_PROV_INFO* pKeyInfo;
//	BYTE bySN[0x40] = { 0 };
//
//#define STRCPY_BUFF(A) strncpy_s(szRetBuf, bufSize, A, strlen(A))
//
//	char szDeviceName[256] = { 0 }, szConName[256] = { 0 }, *pCertID = NULL;
//	pCertID = _com_util::ConvertBSTRToString(CertID);
//
//	if (strlen(pCertID) > 100)
//	{
//		return S_FALSE;
//	}
//
//	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szConName);
//	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);
//
//	dwRet = HSGetContainerParam(hCard, szConName, &dwConParam);
//	if (dwConParam == 0x00130000)
//	{
//		dwRet = HSReadCert(hCard, szConName, AT_SIGNATURE, bCert, &dwCertLen);
//	}
//	else if (dwConParam == 0x00000013)
//	{
//		dwRet = HSReadCert(hCard, szConName, AT_KEYEXCHANGE, bCert, &dwCertLen);
//	}
//	else
//	{
//		return S_OK;
//	}
//
//	pCertContext = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
//		bCert, dwCertLen);
//
//	dwType = ustype;
//
//	CCertParser parser(pCertContext);
//
//#define GET_NAME CertGetNameStringA
//
//	switch (dwType)
//	{
//	case CERT_SUBJECT://用户名
//		dwLen = GET_NAME(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, NULL, 0);
//		GET_NAME(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, 0, NULL, szRetBuf, dwLen + 1);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_UNIQUEID://证书实体唯一标识
//		*strUserInfo = SysAllocString(parser.GetCertExt(0, "1.2.86.21.1.1").c_str());
//		return S_OK;
//		//memcpy(bySN, pCertContext->pCertInfo->SerialNumber.pbData, pCertContext->pCertInfo->SerialNumber.cbData);
//		////逆序，符合规范
//		//reverse(bySN, pCertContext->pCertInfo->SerialNumber.cbData);
//		//for(int i=0;i<pCertContext->pCertInfo->SerialNumber.cbData;i++)
//		//{
//		//	sprintf(&(szRetBuf[i*2]), "%02X", bySN[i]);
//		//}
//		//dwLen = strlen(szRetBuf);
//		//break;
//	case CERT_DEPT://部门
//		dwLen = GET_NAME(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_ORGANIZATIONAL_UNIT_NAME, NULL, 0);
//		GET_NAME(pCertContext, CERT_NAME_ATTR_TYPE, 0, szOID_ORGANIZATIONAL_UNIT_NAME, szRetBuf, dwLen + 1);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_ISSUE://颁发者DN
//		dwLen = GET_NAME(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG, NULL, NULL, 0);
//		GET_NAME(pCertContext, CERT_NAME_SIMPLE_DISPLAY_TYPE, CERT_NAME_ISSUER_FLAG,
//			NULL, szRetBuf, dwLen + 1);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_DEVICETYPE://证书介质类型
//		STRCPY_BUFF("HaiHey 3000");
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_CATYPE://CA类型
//		*strUserInfo = SysAllocString(parser.GetSubjectAndIssuerInfo(SGD_GET_CERT_ISSUER_CN).c_str());
//		//strcpy(szRetBuf, "FJCA");
//		//dwLen = strlen(szRetBuf);
//		//break;
//		return S_OK;
//	case CERT_KEYTYPE://用户证书密钥类型，（双证或单证）
//		dwConParam = 0;
//		dwRet = HS_GetCertType(hCard, &dwConParam);
//		if (dwConParam == 2) {
//			STRCPY_BUFF("双证");
//		}
//		else {
//			STRCPY_BUFF("单证");
//		}
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_DEVICENAME://用户证书介质名称
//		dwLableLen = 100;
//		dwRet = HSReadLabel(hCard, szRetBuf, &dwLableLen);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_DEVICEPROVIDER://用户证书介质提供者即csp名称
//		STRCPY_BUFF("HaiTai Cryptographic Service Provider");
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_DEVICEAFFIX://用户证书介质附加库
//		STRCPY_BUFF("GEC00001.dll;HTCSPApi.dll;SKFAPI.dll");
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_SIGNPATH://用户签名证书路径
//		HS_ListSCertID(szRetBuf);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_EXCHPATH://用户加密证书路径
//		HS_ListECertID(szRetBuf);
//		dwLen = strlen(szRetBuf);
//		break;
//	case CERT_SIGNPFXPATH://用户签名P12证书路径
//		*strUserInfo = NULL;
//		return S_OK;
//	case CERT_EXCHPFXPATH://用户加密P12证书路径
//		*strUserInfo = NULL;
//		return S_OK;
//	case CERT_UNIQUEIDOID://用户证书UniqueID的OID
//		STRCPY_BUFF("1.2.86.21.1.1");
//		dwLen = strlen(szRetBuf);
//		break;
//	}
//
//	WCHAR *wRetData = new WCHAR[dwLen * 4];
//	memset(wRetData, 0x00, dwLen * 4);
//	int nDataLen = 0;
//	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)szRetBuf, -1, (LPWSTR)wRetData, dwLen);
//	nDataLen = wcslen(wRetData);
//	*strUserInfo = SysAllocStringLen(wRetData, nDataLen);
//	delete[]wRetData;
//
	return S_OK;
}

// 14 
STDMETHODIMP CKeyClient::SOF_ValidateCert(BSTR Cert, BOOL * pbResult)
{
	DWORD dwRet = 0, dwCertLen = 0, dwRelCertLen = 6000, dwFrom = 0, dwTo = 0, dwCurrent = 0;
	BYTE bCertData[6000] = { 0 };
	PCCERT_CONTEXT pCertContext = NULL;
	char *p = NULL, szTimeFrom[256] = { 0 }, szTimeTo[256] = { 0 }, szCurTime[256] = { 0 };

	SYSTEMTIME CertTime = { 0 };
	FILETIME LocalTime = { 0 };

	p = _com_util::ConvertBSTRToString(Cert);
	dwCertLen = strlen(p);

	CBase64Codec::Decode((BYTE *)p, dwCertLen, bCertData, &dwRelCertLen);

	pCertContext = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
		bCertData, dwRelCertLen);

	if (pCertContext == NULL)
		return S_FALSE;

	LONG l = CertVerifyTimeValidity(NULL, pCertContext->pCertInfo);//有效期验证
	if (l == 0)
		*pbResult = TRUE;
	else
		*pbResult = FALSE;
	
	return S_OK;
}

// 15
STDMETHODIMP CKeyClient::SOF_SignData(BSTR CertID, BSTR InData, BSTR * strDecData)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0;
	BYTE byBase64Data[2048] = { 0 };
	DWORD dwBase64DataLen = 2048;
	DWORD dwSize = 0;
	BYTE bCertData[3072] = { 0 };
	BYTE bOutData[256] = { 0 };
	DWORD dwOutDataLen = 256, i = 0;
	HANDLE hCard = NULL;
	char szConName[1024] = { 0 }, *p = NULL;
	DWORD dwConNameLen = 1024, dwConParam = 0, dwConNum = 0;
	BYTE bInData[256] = { 0 };

	p = _com_util::ConvertBSTRToString(InData);

	dwInDataLen = strlen(p);

	char szDeviceName[256] = { 0 }, szContainerName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szContainerName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HS_HashData(hCard, (BYTE *)p, dwInDataLen, bInData);
	dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);

	for (i = 0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if ((dwConParam & 0x00010000) == 0x00010000)
		{
			dwRet = memcmp(szConName, szContainerName, strlen(szConName));
			if (0 == dwRet)
			{
				dwRet = HSRSASign(hCard, szConName, AT_SIGNATURE, 3, bInData, 128, bOutData, &dwOutDataLen);
				break;
			}
		}
	}

	CBase64Codec::Encode(bOutData, dwOutDataLen, byBase64Data, &dwBase64DataLen);

	WCHAR *wRetData = new WCHAR[dwBase64DataLen * 4];
	memset(wRetData, 0x00, dwBase64DataLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)byBase64Data, -1, (LPWSTR)wRetData, dwBase64DataLen);
	nDataLen = wcslen(wRetData);
	*strDecData = SysAllocStringLen(wRetData, nDataLen);
	delete[] wRetData;

	return S_OK;
}

// 16
STDMETHODIMP CKeyClient::SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue, VARIANT_BOOL * usResult)
{
	HS_Init();

	DWORD dwOutDataLen = 256, dwInDataLen = 0, dwCertLen = 0, dwRet = 0;
	HANDLE hCard = NULL;
	char szConName[1024] = { 0 }, *p = NULL;
	DWORD dwRelCertLen = 6000, dwELen = 256, dwNlen = 256, dwRelSignLen = 256;
	DWORD dwOutData2 = 256, dwSignLen = 256, dwVerifyLen = 256;
	CBase64Codec Base64;
	HTCSP_RSA_PUBLIC_ST rsa_pub_st = { 0 };
	BYTE bOutData2[256] = { 0 };
	BYTE bRelCert[5 * 1024] = { 0 }, bOutData[256] = { 0 }, bRelSign[256] = { 0 };
	BYTE *bInData = NULL;

	p = _com_util::ConvertBSTRToString(InData);
	dwInDataLen = strlen(p);
	bInData = new BYTE[dwInDataLen + 100];
	memset(bInData, 0x00, dwInDataLen + 100);
	memcpy(bInData, p, dwInDataLen);

	p = NULL;
	p = _com_util::ConvertBSTRToString(Cert);
	dwCertLen = strlen(p);
	Base64.Decode((BYTE *)p, dwCertLen, bRelCert, &dwRelCertLen);

	dwRet = Util::get_pub_key_from_cert(bRelCert, dwRelCertLen, &rsa_pub_st.byModulus[128], (int *)&dwNlen,
		rsa_pub_st.bypublicExponent, (int *)&dwELen);

	reverse(rsa_pub_st.bypublicExponent, 4);
	rsa_pub_st.dwbits = 1024;

	dwRet = HS_HashData_Verify(bInData, dwInDataLen, bOutData, &dwVerifyLen);

	p = NULL;
	p = _com_util::ConvertBSTRToString(SignValue);
	dwSignLen = strlen(p);
	Base64.Decode((BYTE *)p, dwSignLen, bRelSign, &dwRelSignLen);

	dwRet = HSRSAVerifySoft(rsa_pub_st, 1, bRelSign, 128, bOutData2, &dwOutData2);

	if (dwVerifyLen == dwOutData2)
	{
		dwRet = memcmp(bOutData, bOutData2, dwVerifyLen);
		if (dwRet == 0)
		{
			*usResult = 1;
			delete[]bInData;
			return TRUE;
		}
	}

	*usResult = 0;
	delete[]bInData;
	return S_OK;
}

// 17
STDMETHODIMP CKeyClient::SOF_SignFile(BSTR CertID, BSTR InFile, BSTR * strSignData)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, i = 0, dwConNameLen = 1024, dwConNum = 0, dwConParam = 0;
	CW2A szInFile(InFile);
	BYTE bOutData[256] = { 0 }, bRelOutData[256] = { 0 };
	char szFileIn[_MAX_PATH] = { 0 }, szConName[1024] = { 0 }, *p = NULL;
	HANDLE hCard = NULL;
	DWORD dwOutDataLen = 256, dwRetryNum = 0, dwFileSize = 0;
	BYTE *bInData = NULL;

	p = _com_util::ConvertBSTRToString(InFile);

	FILE *fpIn = NULL;
	fopen_s(&fpIn, p, "rb");
	fseek(fpIn, 0, SEEK_END);
	dwFileSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	bInData = new BYTE[dwFileSize + 0x20];
	memset(bInData, 0x00, (dwFileSize + 0x20));

	dwInDataLen = fread(bInData, 1, dwFileSize, fpIn);
	fclose(fpIn);

	char szDeviceName[256] = { 0 }, szContainerName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szContainerName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HS_HashData(hCard, bInData, dwInDataLen, bOutData);

	dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);

	for (i = 0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if ((dwConParam & 0x00010000) == 0x00010000)
		{
			dwRet = memcmp(szConName, szContainerName, strlen(szConName));
			if (0 == dwRet)
			{
				dwRet = HSRSASign(hCard, szConName, AT_SIGNATURE, 3,
					bOutData, 128, bRelOutData, &dwOutDataLen);
				break;
			}
		}
	}

	CBase64Codec Base64;
	BYTE bBase[1024] = { 0 };
	DWORD dwBaseLen = 1024;

	Base64.Encode(bRelOutData, dwOutDataLen, bBase, &dwBaseLen);

	WCHAR *wRetData = new WCHAR[dwBaseLen * 4];
	memset(wRetData, 0x00, dwBaseLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase, -1, (LPWSTR)wRetData, dwBaseLen);
	nDataLen = wcslen(wRetData);
	*strSignData = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;

	delete[]bInData;

	return S_OK;
}

// 18
STDMETHODIMP CKeyClient::SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue, VARIANT_BOOL * usResult)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, dwCertLen = 0, dwRelCertLen = 6000, dwFileSize = 0;
	BYTE *bInData = NULL, bOutData[256] = { 0 }, bOutData2[256] = { 0 };
	char szFileIn[_MAX_PATH] = { 0 }, *p = NULL;
	HTCSP_RSA_PUBLIC_ST rsa_pub_st = { 0 };
	BYTE bRelCert[5 * 1024] = { 0 };
	DWORD dwNlen = 256, dwELen = 256, dwOutData2 = 256;
	CBase64Codec Base64;
	BYTE bSign[1000] = { 0 }, bRelSign[1000] = { 0 };
	DWORD dwSignLen = 1000, dwRelSignLen = 1000, dwVerifyLen = 0;

	p = _com_util::ConvertBSTRToString(InFile);

	FILE *fpIn = NULL;
	if (0 != fopen_s(&fpIn, p, "rb")) {
		return S_FALSE;
	}

	fseek(fpIn, 0, SEEK_END);
	dwFileSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	bInData = new BYTE[dwFileSize + 0x20];
	memset(bInData, 0x00, (dwFileSize + 0x20));

	dwInDataLen = fread(bInData, 1, dwFileSize, fpIn);
	fclose(fpIn);

	p = NULL;
	p = _com_util::ConvertBSTRToString(Cert);
	dwCertLen = strlen(p);
	Base64.Decode((BYTE *)p, dwCertLen, bRelCert, &dwRelCertLen);

	dwRet = Util::get_pub_key_from_cert(bRelCert, dwRelCertLen, &rsa_pub_st.byModulus[128], (int *)&dwNlen,
		rsa_pub_st.bypublicExponent, (int *)&dwELen);

	reverse(rsa_pub_st.bypublicExponent, 4);
	rsa_pub_st.dwbits = 1024;

	dwRet = HS_HashData_Verify(bInData, dwInDataLen, bOutData, &dwVerifyLen);

	p = NULL;
	p = _com_util::ConvertBSTRToString(SignValue);
	dwSignLen = strlen(p);
	Base64.Decode((BYTE *)p, dwSignLen, bRelSign, &dwRelSignLen);

	dwRet = HSRSAVerifySoft(rsa_pub_st, 1, bRelSign, 128, bOutData2, &dwOutData2);

	if (dwVerifyLen == dwOutData2)
	{
		dwRet = memcmp(bOutData, bOutData2, dwVerifyLen);
		if (dwRet == 0)
		{
			*usResult = 1;
			delete[]bInData;
			return S_OK;
		}
	}

	*usResult = 0;
	delete[]bInData;
	return S_FALSE;
}

// 19
STDMETHODIMP CKeyClient::SOF_EncryptData(BSTR SymmKey, BSTR Indata, BSTR* strEncData)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, dwOutDataLen = 0, dwDeviceNameLen = 1024;
	HANDLE hCard = NULL, hKey = NULL;
	BYTE *pbOutData = NULL, *pbInData = NULL;
	BOOL bFlags = FALSE;
	char szDeviceName[1024] = { 0 };
	DEVHANDLE hDev = NULL;
	char szOutData[10 * 1024] = { 0 }, *p = NULL;
	BYTE bKey[0x20] = { 0 };
	DWORD dwKeyLen = 0, dwBaseLen = 20 * 1024, dwBlockNum = 0;
	CBase64Codec Base64;
	BYTE bBase[20 * 1024] = { 0 };
	WCHAR wszRetryCount[200] = { 0 };

	pbInData = new BYTE[1024 * 1024 * 10];
	memset(pbInData, 0x00, 1024 * 1024 * 10);

	p = _com_util::ConvertBSTRToString(SymmKey);
	dwKeyLen = strlen(p);
	memcpy(bKey, p, 16);

	p = NULL;
	p = _com_util::ConvertBSTRToString(Indata);
	dwInDataLen = strlen(p);
	memcpy(pbInData, p, dwInDataLen);

	if (dwInDataLen % 16)
	{
		dwBlockNum = dwInDataLen / 16;
		dwBlockNum = dwBlockNum + 1;
		dwInDataLen = dwBlockNum * 16;
	}

	pbOutData = new BYTE[dwInDataLen + 0x20];
	memset(pbOutData, 0x00, dwInDataLen + 0x20);
	dwOutDataLen = dwInDataLen + 0x20;

	dwRet = SKF_EnumDev(TRUE, szDeviceName, &dwDeviceNameLen);

	dwRet = SKF_ConnectDev(szDeviceName, &hDev);

	dwRet = SKF_SetSymmKey(hDev, bKey, SGD_SM1_ECB/*m_dwEncryptMethod*/, &hKey);

	dwRet = SKF_Encrypt(hKey, pbInData, dwInDataLen, pbOutData, &dwOutDataLen);

	Base64.Encode(pbOutData, dwOutDataLen, bBase, &dwBaseLen);

	WCHAR *wRetData = new WCHAR[dwBaseLen * 4];
	memset(wRetData, 0x00, dwBaseLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase, -1, (LPWSTR)wRetData, dwBaseLen);
	nDataLen = wcslen(wRetData);
	*strEncData = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;

	delete[]pbOutData;
	delete[]pbInData;

	return S_OK;
}

// 20
STDMETHODIMP CKeyClient::SOF_DecryptData(BSTR SymmKey, BSTR Indata, BSTR * strSrcData)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, dwOutDataLen = 0, dwDeviceNameLen = 1024;
	HANDLE hCard = NULL, hKey = NULL;
	BOOL bFlags = FALSE;
	char szDeviceName[1024] = { 0 }, *p = NULL;
	DEVHANDLE hDev = NULL;
	CBase64Codec Base64;
	DWORD dwBaseLen = 10 * 1024, dwKeyLen = 0;
	BYTE bKey[0x40] = { 0 };
	BYTE *bOutData = NULL, *bBase = NULL, *bInData = NULL;


	bOutData = new BYTE[1024 * 1024 * 10];
	memset(bOutData, 0x00, 1024 * 1024 * 10);

	bBase = new BYTE[1024 * 1024 * 10];
	memset(bBase, 0x00, 1024 * 1024 * 10);

	bInData = new BYTE[1024 * 1024 * 10];
	memset(bInData, 0x00, 1024 * 1024 * 10);


	p = _com_util::ConvertBSTRToString(SymmKey);
	dwKeyLen = strlen(p);
	memcpy(bKey, p, 16);

	p = NULL;
	p = _com_util::ConvertBSTRToString(Indata);
	dwInDataLen = strlen(p);
	memcpy(bInData, p, dwInDataLen);
	dwOutDataLen = dwInDataLen;

	Base64.Decode(bInData, dwInDataLen, bBase, &dwBaseLen);

	dwRet = SKF_EnumDev(TRUE, szDeviceName, &dwDeviceNameLen);
	dwRet = SKF_ConnectDev(szDeviceName, &hDev);
	dwRet = SKF_SetSymmKey(hDev, bKey, SGD_SM1_ECB/*m_dwEncryptMethod*/, &hKey);
	dwRet = SKF_Decrypt(hKey, bBase, dwBaseLen, bOutData, &dwOutDataLen);

	WCHAR *wRetData = new WCHAR[dwOutDataLen * 4];
	memset(wRetData, 0x00, dwOutDataLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bOutData, -1, (LPWSTR)wRetData, dwOutDataLen);
	nDataLen = wcslen(wRetData);
	*strSrcData = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;

	delete[]bOutData;
	delete[]bBase;
	delete[]bInData;

	return S_OK;
}

// 21
STDMETHODIMP CKeyClient::SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, VARIANT_BOOL * pbResult)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, dwOutDataLen = 0, dwDeviceNameLen = 1024, dwFileSize = 0;
	DWORD dwBlockNum = 0, dwTotalLen = 0;
	HANDLE hCard = NULL, hKey = NULL;
	BYTE *pbOutData = NULL;
	BOOL bFlags = FALSE;
	char szDeviceName[1024] = { 0 };
	DEVHANDLE hDev = NULL;
	BYTE bKey[0x40] = { 0 };
	BYTE *bInData = NULL, *bOutData = NULL;
	char szFileIn[_MAX_PATH] = { 0 }, szFileOut[_MAX_PATH] = { 0 };
	char *pKey = NULL, *pInFile = NULL, *pOutFile = NULL;

	pKey = _com_util::ConvertBSTRToString(SymmKey);
	memcpy(bKey, pKey, 16);

	pInFile = _com_util::ConvertBSTRToString(InFile);
	pOutFile = _com_util::ConvertBSTRToString(OutFile);

	FILE *fpIn = NULL, *fpOut = NULL;
	if (0 != fopen_s(&fpIn, pInFile, "rb")) {
		return S_FALSE;
	}

	fseek(fpIn, 0, SEEK_END);
	dwFileSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	if (0 != fopen_s(&fpOut, pOutFile, "wb")) {
		fclose(fpIn);
		return S_FALSE;
	}

	bInData = new BYTE[dwFileSize + 10];
	memset(bInData, 0x00, dwFileSize + 10);

	bOutData = new BYTE[dwFileSize + 10];
	memset(bOutData, 0x00, dwFileSize + 10);

	dwInDataLen = fread(bInData, 1, dwFileSize, fpIn);
	dwTotalLen = dwInDataLen;

	if (dwInDataLen % 16)
	{
		dwBlockNum = dwInDataLen / 16;
		dwBlockNum = dwBlockNum + 1;
		dwTotalLen = 16 * dwBlockNum;
		memset((bInData + dwInDataLen), 0x00, (dwTotalLen - dwInDataLen));
	}

	dwRet = SKF_EnumDev(TRUE, szDeviceName, &dwDeviceNameLen);
	dwRet = SKF_ConnectDev(szDeviceName, &hDev);
	dwRet = SKF_SetSymmKey(hDev, bKey, SGD_SM1_ECB/*m_dwEncryptMethod*/, &hKey);

	dwOutDataLen = dwTotalLen + 0x20;
	dwRet = SKF_Encrypt(hKey, bInData, dwTotalLen, bOutData, &dwOutDataLen);

	fwrite(bOutData, 1, dwOutDataLen, fpOut);
	fclose(fpIn);
	fclose(fpOut);

	delete[]bInData;
	delete[]bOutData;

	*pbResult = TRUE;
	return S_OK;
}

 // 22
STDMETHODIMP CKeyClient::SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile, VARIANT_BOOL * pbResult)
{
	HS_Init();

	DWORD dwRet = 0, dwInDataLen = 0, dwOutDataLen = 0, dwDeviceNameLen = 1024;
	DWORD dwBlockNum = 0, dwTotalLen = 0, dwFileSize = 0;
	HANDLE hCard = NULL, hKey = NULL;
	BYTE *pbOutData = NULL;
	BOOL bFlags = FALSE;
	char szDeviceName[1024] = { 0 };
	DEVHANDLE hDev = NULL;
	CW2A szKey(SymmKey);
	CW2A szInFile(InFile);
	CW2A szOutFile(OutFile);
	BYTE *bOutData = NULL;
	BYTE *bInData = NULL;
	char szFileIn[_MAX_PATH] = { 0 }, szFileOut[_MAX_PATH] = { 0 };
	FILE *fpIn = NULL, *fpOut = NULL;
	BYTE bRelKey[0x20] = { 0 }, bKey[0x20] = { 0 };
	char *pKey = NULL, *pInFile = NULL, *pOutFile = NULL;

	pKey = _com_util::ConvertBSTRToString(SymmKey);
	memcpy(bKey, pKey, 16);

	pInFile = _com_util::ConvertBSTRToString(InFile);
	pOutFile = _com_util::ConvertBSTRToString(OutFile);

	if (0 != fopen_s(&fpIn, pInFile, "rb")) {
		return S_FALSE;
	}
	fseek(fpIn, 0, SEEK_END);
	dwFileSize = ftell(fpIn);
	fseek(fpIn, 0, SEEK_SET);

	if (0 != fopen_s(&fpOut, pOutFile, "wb")) {
		fclose(fpIn);
		return S_FALSE;
	}

	bInData = new BYTE[dwFileSize + 10];
	memset(bInData, 0x00, dwFileSize + 10);

	bOutData = new BYTE[dwFileSize + 10];
	memset(bOutData, 0x00, dwFileSize + 10);

	dwInDataLen = fread(bInData, 1, dwFileSize, fpIn);
	dwTotalLen = dwInDataLen;

	dwRet = SKF_EnumDev(TRUE, szDeviceName, &dwDeviceNameLen);
	dwRet = SKF_ConnectDev(szDeviceName, &hDev);
	dwRet = SKF_SetSymmKey(hDev, bKey, SGD_SM1_ECB/*m_dwEncryptMethod*/, &hKey);

	dwOutDataLen = dwTotalLen + 0x20;
	dwRet = SKF_Decrypt(hKey, bInData, dwTotalLen, bOutData, &dwOutDataLen);

	fwrite(bOutData, 1, dwOutDataLen, fpOut);

	fclose(fpIn);
	fclose(fpOut);

	delete[]bInData;
	delete[]bOutData;

	*pbResult = TRUE;
	return S_OK;
}

// 23
STDMETHODIMP CKeyClient::SOF_PubKeyEncrypt(BSTR Cert, BSTR InData, BSTR* strEncData)
{
	HS_Init();

	DWORD dwRet = 0, dwCertLen = 0, dwDataLen = 0, dwNlen = 256, dwELen = 256, dwOutDataLen = 256;
	CW2A szCert(Cert);
	CW2A szData(InData);
	BYTE bN[256] = { 0 }, bE[256] = { 0 };
	HTCSP_RSA_PUBLIC_ST rsa_pub = { 0 };
	DEVHANDLE hDev = NULL;
	BYTE bOutData[256] = { 0 };
	BYTE bRelCert[3000] = { 0 };
	char szaaaa[1000] = { 0 };
	char *p = NULL;
	DWORD dwRelCertLen = 3000;
	CBase64Codec Base64;

	p = _com_util::ConvertBSTRToString(Cert);

	dwCertLen = strlen(p);
	Base64.Decode((const BYTE *)p, dwCertLen, bRelCert, &dwRelCertLen);
	dwRet = Util::get_pub_key_from_cert(bRelCert, dwRelCertLen, bN, (int *)&dwNlen, bE, (int *)&dwELen);
	reverse(bE, 4);

	memcpy(&rsa_pub.byModulus[128], bN, dwNlen);
	memcpy(rsa_pub.bypublicExponent, bE, 4);
	rsa_pub.dwbits = 1024;

	p = NULL;
	p = _com_util::ConvertBSTRToString(InData);
	dwDataLen = strlen(p);

	dwRet = HSRSAEncryptSoft(rsa_pub, 1, (BYTE *)p, dwDataLen, bOutData, &dwOutDataLen);

	BYTE bBase64[2000] = { 0 };
	DWORD dwBase64Len = 2000;
	Base64.Encode(bOutData, dwOutDataLen, bBase64, &dwBase64Len);

	WCHAR *wRetData = new WCHAR[dwBase64Len * 4];
	memset(wRetData, 0x00, dwBase64Len * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase64, -1, (LPWSTR)wRetData, dwBase64Len);
	nDataLen = wcslen(wRetData);
	*strEncData = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;

	return S_OK;
}

// 24
STDMETHODIMP CKeyClient::SOF_PriKeyDecrypt(BSTR CertID, BSTR InData, BSTR* strSrcData)
{
	HS_Init();

	DWORD dwRet = 0, dwContainerNameLen = 0, i = 0, dwSize = 0, dwFlags = 0;
	CW2A szCertId(CertID);
	CW2A szData(InData);
	DEVHANDLE hDev = NULL;
	HCONTAINER hContainer = NULL;
	BYTE bCertData[3072] = { 0 };
	BYTE bOutData[256] = { 0 };
	DWORD dwOutDataLen = 256, dwInDataLen = 0;
	HANDLE hCard = NULL;
	char szConName[1024] = { 0 }, *p = NULL;
	CBase64Codec Base64;
	BYTE bRelInData[2000] = { 0 };
	DWORD dwConNameLen = 1024, dwConParam = 0, dwConNum = 0, dwRelInDataLen = 2000;

	p = _com_util::ConvertBSTRToString(InData);
	dwInDataLen = strlen(p);

	Base64.Decode((BYTE *)p, dwInDataLen, bRelInData, &dwRelInDataLen);

	char szDeviceName[256] = { 0 }, szContainerName[256] = { 0 }, *pCertID = NULL;
	pCertID = _com_util::ConvertBSTRToString(CertID);

	if (strlen(pCertID) > 100)
	{
		return S_FALSE;
	}

	dwRet = HS_ChangeCertID(pCertID, szDeviceName, szContainerName);
	dwRet = HS_GetDeviceHandle(szDeviceName, &hCard);

	dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);

	for (i = 0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if ((dwConParam & 0x00000001) == 0x00000001)
		{
			dwRet = memcmp(szConName, szContainerName, strlen(szConName));
			if (0 == dwRet)
			{
				dwRet = HSRSADecrypt(hCard, szConName, AT_KEYEXCHANGE, RSA_PKCS1_PADDING,
					bRelInData, dwRelInDataLen, bOutData, &dwOutDataLen);
				break;
			}
		}
	}

	WCHAR *wRetData = new WCHAR[dwOutDataLen * 4];
	memset(wRetData, 0x00, dwOutDataLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bOutData, -1, (LPWSTR)wRetData, dwOutDataLen);
	nDataLen = wcslen(wRetData);
	*strSrcData = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;
	return S_OK;
}

// 25
STDMETHODIMP CKeyClient::SOF_SignDataByP7(BSTR CertID, BSTR InData, BSTR* strSignResult)
{
	HS_Init();

	char *p = NULL, szOutData[102400] = { 0 };
	DWORD dwInDataLen = 0, dwOutDataLen = 102400, dwRet = 0, dw02 = 10240;
	CBase64Codec Base64;
	BYTE bOutData[10240] = { 0 };

	p = _com_util::ConvertBSTRToString(InData);
	dwInDataLen = strlen(p);

	dwRet = Util::load_my_sign_cert();

	dwRet = Util::sign_message(p, dwInDataLen, szOutData, &dwOutDataLen);

	// 	FILE *fp = NULL;
	// 	fp = fopen("C:\\p7.txt", "wb");
	// 	dwRet = fwrite(szOutData, 1, dwOutDataLen, fp);
	// 	fclose(fp);

	BYTE bBase64[20000] = { 0 };
	DWORD dwBase64Len = 20000;
	Base64.Encode((const BYTE *)szOutData, dwOutDataLen, bBase64, &dwBase64Len);

	WCHAR *wRetData = new WCHAR[dwBase64Len * 4];
	memset(wRetData, 0x00, dwBase64Len * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase64, -1, (LPWSTR)wRetData, dwBase64Len);
	nDataLen = wcslen(wRetData);
	*strSignResult = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;
	return S_OK;
}

// 26
STDMETHODIMP CKeyClient::SOF_VerifySignedDataByP7(BSTR P7Data, VARIANT_BOOL * pbResult)
{
	HS_Init();

	char *p = NULL;
	BYTE bRelP7Data[10240] = { 0 }, bOutData[10240] = { 0 };
	DWORD dwRelP7DataLen = 10240, dwOutDataLen = 10240, dwInDataLen = 0, dwRet = 0;

	p = _com_util::ConvertBSTRToString(P7Data);
	dwInDataLen = strlen(p);

	CBase64Codec::Decode((BYTE *)p, dwInDataLen, bRelP7Data, &dwRelP7DataLen);
	dwRet = Util::verify_message(bRelP7Data, dwRelP7DataLen, bOutData, &dwOutDataLen);
	*pbResult = (dwRet == 0) ? TRUE : FALSE;

	return S_OK;
}

// 27
STDMETHODIMP CKeyClient::SOF_GetP7SignDataInfo(BSTR P7Data, LONG ustype, BSTR * strDataInfo)
{
	HS_Init();

	char *p = NULL, szConName[1024] = { 0 };
	BYTE bRelP7Data[10240] = { 0 }, bOutData[10240] = { 0 }, bHashData[1024] = { 0 };
	DWORD dwRelP7DataLen = 10240, dwOutDataLen = 10240, dwInDataLen = 0, dwRet = 0, dwType = 0;
	DWORD i = 0, dwConNum = 0, dwConNameLen = 1024, dwConParam = 0, dwBase64Len = 10240;
	HANDLE hCard = NULL;
	BYTE bSignData[256] = { 0 }, bCert[3000] = { 0 }, bBase[10240] = { 0 };
	PKCS7 *pPKCS7 = NULL;
	BYTE *p01 = bRelP7Data;
	BYTE bySignatur[0x100] = { 0 };

	p = _com_util::ConvertBSTRToString(P7Data);
	dwInDataLen = strlen(p);

	CBase64Codec::Decode((BYTE *)p, dwInDataLen, bRelP7Data, &dwRelP7DataLen);

	dwRet = Util::verify_message(bRelP7Data, dwRelP7DataLen, bOutData, &dwOutDataLen);

	pPKCS7 = d2i_PKCS7(NULL, (const unsigned char **)&p01, dwRelP7DataLen);

	dwType = ustype;
	if (1 == dwType)
	{
		WCHAR *wRetData = new WCHAR[dwOutDataLen * 4];
		memset(wRetData, 0x00, dwOutDataLen * 4);
		int nDataLen = 0;
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bOutData, -1, (LPWSTR)wRetData, dwOutDataLen);
		nDataLen = wcslen(wRetData);
		*strDataInfo = SysAllocStringLen(wRetData, nDataLen);
		delete[]wRetData;
	}
	else if (2 == dwType)
	{
		PKCS7_SIGN_ENVELOPE *pSignEnv = pPKCS7->d.signed_and_enveloped;

		X509 *pCert = sk_X509_value(pSignEnv->cert, 0);
		BYTE *ppCert = bCert;
		int nCertLen = i2d_X509(pCert, &ppCert);

		CBase64Codec::Encode(bCert, nCertLen, bBase, &dwBase64Len);

		WCHAR *wRetData = new WCHAR[dwBase64Len * 4];
		memset(wRetData, 0x00, dwBase64Len * 4);
		int nDataLen = 0;
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase, -1, (LPWSTR)wRetData, dwBase64Len);
		nDataLen = wcslen(wRetData);
		*strDataInfo = SysAllocStringLen(wRetData, nDataLen);
		delete[]wRetData;
	}
	else if (3 == dwType)
	{
		PKCS7_SIGNED *sign = pPKCS7->d.sign;
		//int nSignCount = sk_PKCS7_SIGNER_INFO_num(sign->signer_info);

		PKCS7_SIGNER_INFO *signInfo = sk_PKCS7_SIGNER_INFO_value(sign->signer_info, 0);

		memcpy(bySignatur, signInfo->enc_digest->data, signInfo->enc_digest->length);

		CBase64Codec::Encode(bySignatur, 0x80, bBase, &dwBase64Len);

		WCHAR *wRetData = new WCHAR[0x80 * 4];
		memset(wRetData, 0x00, 0x80 * 4);
		int nDataLen = 0;
		MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase, -1, (LPWSTR)wRetData, 0x80 * 4);
		nDataLen = wcslen(wRetData);
		*strDataInfo = SysAllocStringLen(wRetData, nDataLen);
		delete[]wRetData;
	}
	return S_OK;
}

// 28
STDMETHODIMP CKeyClient::SOF_SignDataXML(BSTR CertID, BSTR InData, BSTR * strXmlData)
{
//#if TEST
//	CreateCert();
//#else
//	HS_Init();
//	Util::load_my_sign_cert();
//#endif
//
//	CXmlSign xml;
//	wstring wret = xml.SignXmlString((void*)g_pMySignCert, CStringUtil::ws2s(InData).c_str());
//
//	*strXmlData = SysAllocString(wret.c_str());

	return S_OK;
}

// 29
STDMETHODIMP CKeyClient::SOF_VerifySignedDataXML(BSTR InData, VARIANT_BOOL * pbResult)
{
	//CXmlSign xml;
	//*pbResult = xml.VerifyXmlString(CStringUtil::ws2s(InData).c_str());
	return S_OK;
}

// 30
STDMETHODIMP CKeyClient::SOF_GetXMLSignatureInfo(BSTR XMLSignedData, LONG ustype, BSTR * strItemInfo)
{
	//CXmlSign xml;
	//*strItemInfo = SysAllocString(xml.ParseSignedXml(CStringUtil::ws2s(XMLSignedData).c_str(), ustype).c_str());
	return S_OK;
}

// 31
STDMETHODIMP CKeyClient::SOF_CheckSupport(LONG * usResult)
{
	HS_Init();
	*usResult = 0;

	return S_OK;
}

// 32
STDMETHODIMP CKeyClient::SOF_GenRandom(LONG usLen, BSTR* strNum)
{
	HS_Init();

	DWORD dwRet = 0, dwRandomLen = 0, dwBaseLen = 512;
	HANDLE hCard = NULL;
	BYTE bRandom[256] = { 0 }, bBase[512] = { 0 };
	CBase64Codec Base64;
	dwRandomLen = usLen;

	dwRet = HS_ConnectDevice(&hCard);

	dwRet = HSGenRandom(hCard, dwRandomLen, bRandom);

	Base64.Encode(bRandom, dwRandomLen, bBase, &dwBaseLen);

	WCHAR *wRetData = new WCHAR[dwBaseLen * 4];
	memset(wRetData, 0x00, dwBaseLen * 4);
	int nDataLen = 0;
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)bBase, -1, (LPWSTR)wRetData, dwBaseLen);
	nDataLen = wcslen(wRetData);
	*strNum = SysAllocStringLen(wRetData, nDataLen);
	delete[]wRetData;
	return S_OK;
}
