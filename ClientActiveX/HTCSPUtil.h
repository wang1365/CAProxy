


#include "stdafx.h"


#define HTLOGGER HSLogTime(__FUNCTION__), HSLog



DWORD HS_ConnectDevice(HANDLE *phCard);
DWORD HS_HashData(HANDLE hCard, BYTE* pbData, DWORD dwDataLen, BYTE *pbOutData);
DWORD HS_HashData_Verify(BYTE* pbData, DWORD dwDataLen, BYTE *pbOutData, DWORD *pdwOutDataLen);

void  reverse(BYTE *pdata, DWORD len);

DWORD  HexToStr(IN CONST BYTE *pbHex, IN DWORD dwHexLen, OUT BYTE *pbStr);
DWORD HS_ListCertID(char *pszCertID);
DWORD HS_ChangeCertID(IN char *pszCertID, OUT char *pszDeviceName, OUT char *pszConName);


DWORD HSLogTime(char *pszFuntionName);

DWORD HSLog(char *pszData, ...);
DWORD HS_Init();
DWORD HS_GetDeviceHandle(IN char *pszDeviceName, OUT HANDLE *phCard);

//BOOL MakeXML_Data(IN char *strDataTobeSign, IN char *strDigest, IN char *strCert, 
//				  IN char *strSignature, OUT char *szBuf, OUT int *nLen);
//
//bool GetXML_Data(IN char *szBuf, IN int nLen, 
//				 OUT char *strDataTobeSign_OUT, OUT char *strDigest_OUT, 
//				 OUT char *strCert_OUT, OUT char *strSignature_OUT);

DWORD HS_ListECertID(char *pszECertID);
DWORD HS_ListSCertID(char *pszSCertID);
DWORD HS_GetCertType(HANDLE hCard, DWORD *pdwCertType);

