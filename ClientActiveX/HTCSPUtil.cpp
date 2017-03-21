
#include "StdAfx.h"
#include "HTCSPUtil.h"
#include "HTCSPApi/HTCSPApi.h"
#include "HTCSPApi/HYErrorCode.h"

//#include "TinyXML/tinyxml.h"
//#include "TinyXML/tinystr.h"

#pragma comment(lib, "HTCSPApi/HTCSPApi.lib")

#pragma warning(push)
#pragma warning( disable : 4996 )

char g_szReaderName[1024] = { 0 };
DWORD g_dwProcessID = 0;
DWORD g_dwInitFlag = 0;
HANDLE g_hCard = NULL;

typedef struct _DEVICE_HCARD_{
	DWORD dwState;
	char szDeviceName[0x40];
	HANDLE hCard;
	char szReservd[0x40];
}DEVICE_HCARD;

DEVICE_HCARD Device_st[0x10] = {0};



DWORD HS_TestDevice(HANDLE hCard)
{
	DWORD dwRet = 0;
	BYTE bSerial[SERIAL_LEN] = {0};

	try
	{
		dwRet = HSGetSerial(hCard, bSerial);
	}
	catch (...)
	{
		dwRet = 1;
	}

	return dwRet;
}



DWORD _stdcall InsertProc(void *pParam)
{
	DWORD dwRet = 0, i = 0;
	HANDLE hCard = NULL;
	char *p = NULL;

	p = (char *)pParam;

	for (i=0; i<10; i++)
	{
		if (Device_st[i].dwState == 0)
		{
			dwRet = HSConnectDev(p, &hCard);
			strcpy(Device_st[i].szDeviceName, p);
			Device_st[i].hCard = hCard;
			Device_st[i].dwState = 1;
			return 0;
		}
	}

	return 0;
}


DWORD _stdcall RemoveProc(void *pParam)
{
	DWORD dwRet = 0, i = 0;

	for (i=0; i<10; i++)
	{
		if (Device_st[i].dwState == 1)
		{
			dwRet = memcmp((const char *)pParam, Device_st[i].szDeviceName, strlen((const char *)pParam));
			if (dwRet == 0)
			{
				dwRet = HSDisconnectDev(Device_st[i].hCard);
				Device_st[i].hCard = NULL;
				memset(Device_st[i].szDeviceName, 0x00, 0x40);
				Device_st[i].dwState = 0;
				return 0;
			}
		}
	}

	return 0;
}


DWORD HS_Init()
{
	if (g_dwInitFlag == 1)
	{
		return 0;
	}
	DWORD dwRet = 0, dwReaderNamesLen = 1024, dwReaderNum = 0, dwState = 0;
	char *p = NULL;

	try
	{
		if(dwRet = HSListReaders(g_szReaderName, &dwReaderNamesLen, &dwReaderNum))
		{
			return dwRet;
		}

		p = g_szReaderName;
		while(p && *p!=0x00)
		{
			if(dwRet = HSGetDevState(p, &dwState))
				return dwRet;
			if(dwState == HGS_STATE_PRESENT)
			{
				if(dwRet = InsertProc(p))
					return dwRet;
			}
			if(dwRet = HSSetDevChgEvent(p, InsertProc, RemoveProc, p, NULL))
			{
				return dwRet;
			}
			p+=strlen(p)+1;
		}
	}
	catch (...)
	{
	}

	g_dwInitFlag = 1;

	return 0;
}

DWORD HS_GetDeviceHandle(IN char *pszDeviceName, OUT HANDLE *phCard)
{
	DWORD dwRet = 0, i = 0;
	HANDLE hCard = NULL;

	for (i=0; i<10; i++)
	{
		if (Device_st[i].dwState == 1)
		{
			dwRet = memcmp(pszDeviceName, Device_st[i].szDeviceName, strlen(pszDeviceName));
			if (dwRet == 0)
			{
				*phCard = Device_st[i].hCard;
				return 0;
			}
		}
	}

	return 1;
}


DWORD HS_ConnectDevice(HANDLE *phCard)
{
	if (g_hCard != NULL)
	{
		*phCard = g_hCard;
		return 0;
	}
	DWORD dwRet = 0, dwReaderNameLen = 0, dwReaderNum = 0, i = 0, dwDevState = 0;
	char szReaderName[1024] = {0};

	memset(szReaderName, 0x00, sizeof(szReaderName));
	dwReaderNameLen = sizeof(szReaderName);
	dwRet = HSListReaders(szReaderName, &dwReaderNameLen, &dwReaderNum);

	while(szReaderName[i] != 0x00)
	{
		dwDevState = 0x00;
		dwRet = HSGetDevState(&szReaderName[i], &dwDevState);

		if(dwDevState == HGS_STATE_PRESENT)
			break;
		i = i + strlen(&szReaderName[i]) + 1;
	}
	if(szReaderName[i] == 0x00)
	{
		dwRet = HKI_ERR_DEV_NOTFOUND;
	}

	dwRet = HSConnectDev(&szReaderName[i], phCard);
	g_hCard = *phCard;
	return dwRet;
}


DWORD HS_HashData(HANDLE hCard, BYTE* pbData, DWORD dwDataLen, BYTE *pbOutData)
{
	DWORD dwRet = 0, dwOutDataLen = 256, dwRelOutDataLen = 256;
	BYTE bHashData[60] = {0}, bOutData[256] = {0}, bRelOutData[256] = {0};

	dwRet = HSSHA1(pbData, dwDataLen, bHashData);
	dwRet = HSHashPadding(hCard, 1, bHashData, 20, bOutData, &dwOutDataLen);
	dwRet = HSRSAPad(3, 1, 0x80, bOutData, dwOutDataLen, bRelOutData, &dwRelOutDataLen);
	memcpy(pbOutData, bRelOutData, dwRelOutDataLen);

	return dwRet;
}


DWORD HS_HashData_Verify(BYTE* pbData, DWORD dwDataLen, BYTE *pbOutData, DWORD *pdwOutDataLen)
{
	DWORD dwRet = 0, dwOutDataLen = 256, dwRelOutDataLen = 256;
	BYTE bHashData[60] = {0}, bOutData[256] = {0}, bRelOutData[256] = {0};
	HANDLE hCard = NULL;

	dwRet = HS_ConnectDevice(&hCard);

	dwRet = HSSHA1(pbData, dwDataLen, bHashData);
	dwRet = HSHashPadding(hCard, 1, bHashData, 20, bOutData, &dwOutDataLen);
	*pdwOutDataLen = dwOutDataLen;
	memcpy(pbOutData, bOutData, dwRelOutDataLen);

	HSDisconnectDev(hCard);

	return dwRet;
}


void  reverse(BYTE *pdata, DWORD len)
{
	int i=0, j=len-1;
	BYTE  temp=0;

	while (i < j)
	{
		temp = pdata[i];
		pdata[i] = pdata[j];
		pdata[j] = temp;
		i++;
		j--;
	}
}



DWORD  HexToStr(IN CONST BYTE *pbHex, IN DWORD dwHexLen, OUT BYTE *pbStr)
{
	DWORD i = 0;
	for(i=0; i<dwHexLen; i++)
	{
		if (((pbHex[i]&0xf0)>>4)>=0 && ((pbHex[i]&0xf0)>>4)<=9)
			pbStr[2*i]=((pbHex[i]&0xf0)>>4)+0x30;
		else if (((pbHex[i]&0xf0)>>4)>=10 && ((pbHex[i]&0xf0)>>4)<=16)
			pbStr[2*i]=((pbHex[i]&0xf0)>>4)+0x37;
		else 
			return -1;	//won't happen

		if ((pbHex[i]&0x0f)>=0 && (pbHex[i]&0x0f)<=9)
			pbStr[2*i+1]=(pbHex[i]&0x0f)+0x30;
		else if ((pbHex[i]&0x0f)>=10 && (pbHex[i]&0x0f)<=16)
			pbStr[2*i+1]=(pbHex[i]&0x0f)+0x37;
		else 
			return -1;  //won't happen
	}
	return 0;
}

DWORD HS_ListECertID(char *pszECertID)
{
	DWORD dwRet = 0, dwReaderNameLen = 0, dwReaderNum = 0, i = 0, dwDevState = 0;
	DWORD dwConNameLen = 1024, dwConParam = 0, dwConNum = 0, j = 0;
	char szReaderName[1024] = {0}, szConName[1024] = {0};
	char szCertID[8000] = {0};
	HANDLE hCard = NULL;

	memset(szReaderName, 0x00, sizeof(szReaderName));
	dwReaderNameLen = sizeof(szReaderName);
	dwRet = HSListReaders(szReaderName, &dwReaderNameLen, &dwReaderNum);

	while(szReaderName[i] != 0x00)
	{
		dwDevState = 0x00;
		dwRet = HSGetDevState(&szReaderName[i], &dwDevState);

		if(dwDevState == HGS_STATE_PRESENT)
		{
			dwRet = HSConnectDev(&szReaderName[i], &hCard);

			dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);
			if (dwConNum == 0)
			{
				strcat(szCertID, &szReaderName[i]);
				strcat(szCertID, "&&&");
			}
			else
			{
				for (j=0; j<dwConNum; j++)
				{
					memset(szConName, 0x00, 1024);
					dwConNameLen = 1024;
					dwRet = HSGetContainerName(hCard, j, szConName, &dwConNameLen, &dwConParam);
					if (dwConParam == 0x00000013)
					{
						strcat(szCertID, &szReaderName[i]);
						strcat(szCertID, "||");
						strcat(szCertID, szConName);
						strcat(szCertID, "&&&");
					}
					
				}
			}
			dwRet = HSDisconnectDev(hCard);
		}

		i = i + strlen(&szReaderName[i]) + 1;
	}

	strcpy(pszECertID, szCertID);

	return dwRet;
}


DWORD HS_ListSCertID(char *pszSCertID)
{
	DWORD dwRet = 0, dwReaderNameLen = 0, dwReaderNum = 0, i = 0, dwDevState = 0;
	DWORD dwConNameLen = 1024, dwConParam = 0, dwConNum = 0, j = 0;
	char szReaderName[1024] = {0}, szConName[1024] = {0};
	char szCertID[8000] = {0};
	HANDLE hCard = NULL;

	memset(szReaderName, 0x00, sizeof(szReaderName));
	dwReaderNameLen = sizeof(szReaderName);
	dwRet = HSListReaders(szReaderName, &dwReaderNameLen, &dwReaderNum);

	while(szReaderName[i] != 0x00)
	{
		dwDevState = 0x00;
		dwRet = HSGetDevState(&szReaderName[i], &dwDevState);

		if(dwDevState == HGS_STATE_PRESENT)
		{
			dwRet = HSConnectDev(&szReaderName[i], &hCard);

			dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);
			if (dwConNum == 0)
			{
				strcat(szCertID, &szReaderName[i]);
				strcat(szCertID, "&&&");
			}
			else
			{
				for (j=0; j<dwConNum; j++)
				{
					memset(szConName, 0x00, 1024);
					dwConNameLen = 1024;
					dwRet = HSGetContainerName(hCard, j, szConName, &dwConNameLen, &dwConParam);
					if (dwConParam == 0x00130000)
					{
						strcat(szCertID, &szReaderName[i]);
						strcat(szCertID, "||");
						strcat(szCertID, szConName);
						strcat(szCertID, "&&&");
					}

				}
			}
			dwRet = HSDisconnectDev(hCard);
		}

		i = i + strlen(&szReaderName[i]) + 1;
	}

	strcpy(pszSCertID, szCertID);

	return dwRet;
}


DWORD HS_ListCertID(char *pszCertID)
{
	DWORD dwRet = 0, dwReaderNameLen = 0, dwReaderNum = 0, i = 0, dwDevState = 0;

	char szECertID[1024] = {0}, szSCertID[1024] = {0};

	HS_ListSCertID(szSCertID);
	HS_ListECertID(szECertID);

	strcpy(pszCertID, szSCertID);
	strcat(pszCertID, szECertID);

	return 0;


// 	DWORD dwConNameLen = 1024, dwConParam = 0, dwConNum = 0, j = 0;
// 	char szReaderName[1024] = {0}, szConName[1024] = {0};
// 	char szCertID[8000] = {0};
// 	HANDLE hCard = NULL;
// 
// 	memset(szReaderName, 0x00, sizeof(szReaderName));
// 	dwReaderNameLen = sizeof(szReaderName);
// 	dwRet = HSListReaders(szReaderName, &dwReaderNameLen, &dwReaderNum);
// 
// 	while(szReaderName[i] != 0x00)
// 	{
// 		dwDevState = 0x00;
// 		dwRet = HSGetDevState(&szReaderName[i], &dwDevState);
// 
// 		if(dwDevState == HGS_STATE_PRESENT)
// 		{
// 			dwRet = HSConnectDev(&szReaderName[i], &hCard);
// 
// 			dwRet = HSListContainers(hCard, szConName, &dwConNameLen, &dwConNum);
// 			if (dwConNum == 0)
// 			{
// 				strcat(szCertID, &szReaderName[i]);
// 				strcat(szCertID, "&&&");
// 			}
// 			else
// 			{
// 				for (j=0; j<dwConNum; j++)
// 				{
// 					memset(szConName, 0x00, 1024);
// 					dwConNameLen = 1024;
// 					dwRet = HSGetContainerName(hCard, j, szConName, &dwConNameLen, &dwConParam);
// 					strcat(szCertID, &szReaderName[i]);
// 					strcat(szCertID, "||");
// 					strcat(szCertID, szConName);
// 					strcat(szCertID, "&&&");
// 				}
// 			}
// 			dwRet = HSDisconnectDev(hCard);
// 		}
// 
// 		i = i + strlen(&szReaderName[i]) + 1;
// 	}
// 
// 	strcpy(pszCertID, szCertID);
// 
// 	return dwRet;
}


DWORD HS_ChangeCertID(IN char *pszCertID, OUT char *pszDeviceName, OUT char *pszConName)
{
	DWORD dwRet = 0, dwCertIDLen = 0, i = 0;
	char szDeviceName[1000] = {0};

	strcpy(szDeviceName, pszCertID);
	dwCertIDLen = strlen(szDeviceName);
	if (dwCertIDLen > 100)
	{
		return 1;
	}

	for (i=0; i<dwCertIDLen; i++)
	{
		if ((szDeviceName[i] == '|') && (szDeviceName[i+1] != '|'))
		{
			strcpy(pszConName, &szDeviceName[i+1]);
			memcpy(pszDeviceName, szDeviceName, (i-1));
			return 0;
		}
	}

	return 1;
}




DWORD HSLogTime(char *pszFuntionName)
{
	//WaitForSingleObject(g_hMutexHSLog, INFINITE);
	//EnterCriticalSection(&g_cs);

	char szData[100*1024] = {0};
	char szTime[100] = {0};
	SYSTEMTIME st;
	DWORD dwProcessID = 0;

	if (g_dwProcessID == 0)
	{
		g_dwProcessID = GetCurrentProcessId();
	}

	GetLocalTime(&st);
	sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%2d:%03d] 0x%08x ", st.wYear, 
		st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, g_dwProcessID);

	FILE *fp=NULL;
	if((fp =fopen("C:\\SXCALogFile.txt", "a+")) !=NULL)
	{
		fprintf(fp, szTime);
		fprintf(fp, pszFuntionName);
		fprintf(fp, " ");
		fclose(fp);
	}

	return 0;
}


DWORD HSLog(char *pszData, ...)
{
	char szData[100*1024] = {0};
	va_list args;

	va_start(args, pszData);
	vsnprintf(szData, 100*1024, pszData, args);

	FILE *fp=NULL;
	if((fp =fopen("C:\\SXCALogFile.txt", "a+")) !=NULL)
	{
		fprintf(fp, szData);
		fprintf(fp, "\n");
		fclose(fp);
	}

	va_end(args);

	//ReleaseMutex(g_hMutexHSLog);
	return 0;
}


//BOOL MakeXML_Data(IN char *strDataTobeSign, IN char *strDigest, IN char *strCert, 
//				  IN char *strSignature, OUT char *szBuf, OUT int *nLen)
//{
//	DWORD dwPos = 0;
//	try
//	{
//		//创建一个XML的文档对象。
//		TiXmlDocument *myDocument = new TiXmlDocument();
//		//创建一个根元素并连接。
//		TiXmlElement *RootElement = new TiXmlElement("Signature");
//		myDocument->LinkEndChild(RootElement);
//		RootElement->SetAttribute("xmlns", "http://www.w3.org/2000/09/xmldsig#");
//		//创建SignedInfo并连接。
//		TiXmlElement *SignedInfoElement = new TiXmlElement("SignedInfo");
//		RootElement->LinkEndChild(SignedInfoElement);
//		//创建CanonicalizationMethod
//		TiXmlElement *CanonicalizationMethodElement = new TiXmlElement("CanonicalizationMethod");
//		SignedInfoElement->LinkEndChild(CanonicalizationMethodElement);
//		//设置CanonicalizationMethodElement元素的属性。
//		CanonicalizationMethodElement->SetAttribute("Algorithm", "http://www.w3.org/TR/2001/REC-xml-c14n-20010315");
//
//		//创建SignatureMethod
//		TiXmlElement *SignatureMethodElement = new TiXmlElement("SignatureMethod");
//		SignedInfoElement->LinkEndChild(SignatureMethodElement);
//		//设置SignatureMethodElement元素的属性。
//		SignatureMethodElement->SetAttribute("Algorithm", "http://www.w3.org/2000/09/xmldsig#rsa-sha1");
//
//		//创建Reference
//		TiXmlElement *ReferenceElement = new TiXmlElement("Reference");
//		SignedInfoElement->LinkEndChild(ReferenceElement);
//		//设置ReferenceElement元素的属性。
//		ReferenceElement->SetAttribute("URI", "#data");
//		//创建DigestMethod
//		TiXmlElement *DigestMethodElement = new TiXmlElement("DigestMethod");
//		ReferenceElement->LinkEndChild(DigestMethodElement);
//		//设置ReferenceElement元素的属性。
//		DigestMethodElement->SetAttribute("Algorithm", "http://www.w3.org/2000/09/xmldsig#sha1");
//		//创建DigestValue
//		TiXmlElement *DigestValueElement = new TiXmlElement("DigestValue");
//		ReferenceElement->LinkEndChild(DigestValueElement);
//		TiXmlText *DigestValueContent = new TiXmlText(strDigest);
//		DigestValueElement->LinkEndChild(DigestValueContent);
//
//		//签名值：
//		TiXmlElement *SignatureValueElement = new TiXmlElement("SignatureValue");
//		RootElement->LinkEndChild(SignatureValueElement);
//		TiXmlText *SignatureValueContent = new TiXmlText(strSignature);
//		SignatureValueElement->LinkEndChild(SignatureValueContent);
//
//		//KeyInfor:
//		TiXmlElement *KeyInfoElement = new TiXmlElement("KeyInfo");
//		RootElement->LinkEndChild(KeyInfoElement);
//		//X509Data
//		TiXmlElement *X509DataElement = new TiXmlElement("X509Data");
//		KeyInfoElement->LinkEndChild(X509DataElement);
//		//X509Certificate
//		TiXmlElement *X509CertificateElement = new TiXmlElement("X509Certificate");
//		X509DataElement->LinkEndChild(X509CertificateElement);
//		TiXmlText *X509CertificateContent = new TiXmlText(strCert);
//		X509CertificateElement->LinkEndChild(X509CertificateContent);
//
//		//Object:
//		TiXmlElement *ObjectElement = new TiXmlElement("Object");
//		RootElement->LinkEndChild(ObjectElement);
//		ObjectElement->SetAttribute("Id", "data");
//		TiXmlText *ObjectDataContent = new TiXmlText(strDataTobeSign);
//		ObjectElement->LinkEndChild(ObjectDataContent);
//
//		//先保存到临时文件：
//		char szPathBuffer[MAX_PATH] = {0};
//		char szTempName[MAX_PATH] = {0};
//		// Get the temp path.
//		DWORD dwRet = GetTempPath(MAX_PATH,     // length of the buffer
//			szPathBuffer); // buffer for path 
//		if (dwRet == 0)
//		{
//			//Error
//		}
//
//		// Create a temporary file. 
//		dwRet = GetTempFileName(szPathBuffer, // directory for tmp files
//			"NEW",        // temp file name prefix 
//			0,            // create unique name 
//			szTempName);  // buffer for name 
//		myDocument->SaveFile(szTempName);
//
//		FILE *fp = fopen(szTempName, "rb");
//
//
//		if(szBuf)
//		{
//			strcpy(szBuf, "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>");
//			dwPos = strlen(szBuf);
//			*nLen = fread(&szBuf[dwPos], 1, 1024*200, fp);
//			*nLen = *nLen+dwPos;
//		}
//		fclose(fp);
//	}
//	catch (...)
//	{
//		return false;
//	}
//	return true;
//}




//void parseElement( TiXmlNode* pElem,
//				  OUT char *strDataTobeSign, OUT char *strDigest, OUT char *strCert, OUT char *strSignature)
//{
//	char szBuf2[0x1000] = {0};
//	char szBuf3[0x300] = {0};
//
//	if ( NULL == pElem )
//	{
//		return;
//	}
//
//	TiXmlNode*    pElement = pElem->FirstChild();
//
//	for ( ; pElement; pElement = pElement->NextSibling() )
//	{
//		int nType = pElement->Type();
//
//		switch ( nType )
//		{
//		case TiXmlNode::ELEMENT:
//			parseElement( pElement, strDataTobeSign, strDigest, strCert, strSignature);
//			strcpy(szBuf3, pElement->Value());
//			if(0 == strcmp(pElement->Value(), "DigestValue"))
//				strcpy(strDigest, pElement->ToElement()->GetText());
//			if(0 == strcmp(pElement->Value(), "SignatureValue"))
//				strcpy(strSignature, pElement->ToElement()->GetText());
//			if(0 == strcmp(pElement->Value(), "X509Certificate"))
//				strcpy(strCert, pElement->ToElement()->GetText());
//			if(0 == strcmp(pElement->Value(), "Object"))
//			{
//				strcpy(strDataTobeSign, pElement->ToElement()->GetText());
//			}
//			break;
//		case TiXmlNode::TEXT:
//			strcpy(szBuf2, pElement->Value());
//			break;
//		default:
//			break;
//		}
//	}
//}
//
//void ParseXmlDocument(char *szXMLFile, 
//					  OUT char *strDataTobeSign, OUT char *strDigest, OUT char *strCert, OUT char *strSignature)   
//{   
//	TiXmlDocument doc(szXMLFile);   
//	doc.LoadFile();   
//	TiXmlElement *root = doc.RootElement();  
//
//	parseElement(root, strDataTobeSign, strDigest, strCert, strSignature);	   
//}  

#include <string>
using namespace std;
void string_replace(std::string & strBig, const std::string & strsrc, const std::string &strdst) 
{
	string::size_type pos=0;
	string::size_type srclen=strsrc.size();
	string::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != string::npos)
	{
		strBig.erase(pos, srclen);
		strBig.insert(pos, strdst);
		pos += dstlen;
	}
}
//bool GetXML_Data(IN char *szBuf_in, IN int nLen_in, 
//				 OUT char *strDataTobeSign_OUT, OUT char *strDigest_OUT, 
//				 OUT char *strCert_OUT, OUT char *strSignature_OUT)
//{
//	std::string str(szBuf_in);
//	string_replace(str, "<xml>", "&lt;xml&gt;");
//	string_replace(str, "</xml>", "&lt;/xml&gt;");
//	
//	char szBuf[1024 *200] = {0};
//	int nLen = str.size();
//	memcpy(szBuf, str.c_str(), str.size());
//
//	//先保存到临时文件：
//	char szPathBuffer[MAX_PATH] = {0};
//	char szTempName[MAX_PATH] = {0};
//	// Get the temp path.
//	DWORD dwRet = GetTempPath(MAX_PATH,     // length of the buffer
//		szPathBuffer); // buffer for path 
//	if (dwRet == 0)
//	{
//		//Error
//	}
//
//	// Create a temporary file. 
//	dwRet = GetTempFileName(szPathBuffer, // directory for tmp files
//		"NEW",        // temp file name prefix 
//		0,            // create unique name 
//		szTempName);  // buffer for name 
//
//	FILE *fp = fopen(szTempName, "wb");
//	if(szBuf)
//		fwrite(szBuf, 1, nLen, fp);
//	fclose(fp);
//
//	ParseXmlDocument(szTempName, strDataTobeSign_OUT, strDigest_OUT, strCert_OUT, strSignature_OUT);
//
//	return true;
//}

DWORD HS_GetCertType(HANDLE hCard, DWORD *pdwCertType)
{
	DWORD dwRet = 0, dwConNum = 0, i = 0, dwConNameLen = 0, dwConParam = 0;
	DWORD dwFlag1 = 0, dwFlag2 = 0;
	char szConName[1024] = {0};

	dwRet = HSGetContainerNum(hCard, &dwConNum);

	for (i=0; i<dwConNum; i++)
	{
		memset(szConName, 0x00, 1024);
		dwConNameLen = 1024;
		dwConParam = 0;
		dwRet = HSGetContainerName(hCard, i, szConName, &dwConNameLen, &dwConParam);
		if (dwConParam == 0x00130000)
		{
			dwFlag1 = 1;
		}
		else if (dwConParam == 0x00000013)
		{
			dwFlag2 = 1;
		}
	}

	*pdwCertType = dwFlag1+dwFlag2;

	return 0;
}

#pragma warning(pop)
