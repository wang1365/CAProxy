#include "stdafx.h"
#include "USBKeyAPIWrapper.h"



#define GET_METHOD(MethodType, methodName);\
	MethodType pMethod = (MethodType)::GetProcAddress(m_hModule, methodName);\
	if (NULL == pMethod) {\
		LOG_ERROR("Get function address failed");\
		return S_FALSE;\
	}

#define INVOKE_METHOD(...) return (*pMethod)(__VA_ARGS__)


tstring USBKeyAPIWrapper::DLLNAME = TEXT("usbkey.dll");

DWORD USBKeyAPIWrapper::x_HSListContainers
(
	IN HANDLE hCard,
	OUT LPSTR pszContainersName,
	IN	OUT DWORD *pdwContainersNameLen,
	OUT DWORD *pdwContainerNum
) {
	GET_METHOD(FUN_HSListContainers, "HSListContainers");
	INVOKE_METHOD(hCard, pszContainersName, pdwContainersNameLen, pdwContainerNum);
}


DWORD USBKeyAPIWrapper::x_HSVerifyUserPin
(
	IN HANDLE hCard,
	IN LPCSTR szUserPin,
	OUT DWORD *pdwRetryNum
) {
	GET_METHOD(FUN_HSVerifyUserPin, "HSVerifyUserPin");
	INVOKE_METHOD(hCard, szUserPin, pdwRetryNum);
}

DWORD USBKeyAPIWrapper::x_HSChangeUserPin
(
	IN HANDLE hCard,
	IN LPCSTR szOldPin,
	IN LPCSTR szNewPin,
	OUT DWORD *pdwRetryNum
) {
	GET_METHOD(FUN_HSChangeUserPin, "HSChangeUserPin");
	INVOKE_METHOD(hCard, szOldPin, szNewPin, pdwRetryNum);
}


DWORD USBKeyAPIWrapper::x_HSGetContainerName
(
	IN HANDLE hCard,
	IN DWORD dwContainerIndex,
	OUT LPSTR pszContainerName,
	IN OUT DWORD *pdwContainerNameLen,
	OUT DWORD *pdwContainerParam
) {
	GET_METHOD(FUN_HSGetContainerName, "HSGetContainerName");
	INVOKE_METHOD(hCard, dwContainerIndex, pszContainerName, pdwContainerNameLen, pdwContainerParam);
}

DWORD USBKeyAPIWrapper::x_HSReadCert
(
	IN HANDLE hCard,
	IN LPCSTR szContainerName,
	IN DWORD dwKeySpec,
	OUT BYTE *pbCert,
	IN OUT DWORD *pdwCertLen
) {
	GET_METHOD(FUN_HSReadCert, "HSReadCert");
	INVOKE_METHOD(hCard, szContainerName, dwKeySpec, pbCert, pdwCertLen);
}



DWORD USBKeyAPIWrapper::x_HSRSASign
(
	IN HANDLE hCard,
	IN LPCSTR szContainerName,
	IN DWORD dwKeySpec,
	IN DWORD dwPadMode,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSASign, "HSRSASign");
	INVOKE_METHOD(hCard, szContainerName, dwKeySpec, dwPadMode, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}


DWORD USBKeyAPIWrapper::x_HSRSAVerifySoft
(
	IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st,
	IN DWORD dwPadMode,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSAVerifySoft, "HSRSAVerifySoft");
	INVOKE_METHOD(ht_RSA_pub_st, dwPadMode, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}

DWORD USBKeyAPIWrapper::x_HSRSAEncryptSoft
(
	IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st,
	IN DWORD dwPadMode,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSAEncryptSoft, "HSRSAEncryptSoft");
	INVOKE_METHOD(ht_RSA_pub_st, dwPadMode, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}

DWORD USBKeyAPIWrapper::x_HSRSADecryptSoft
(
	IN HTCSP_RSA_PRIVATE_ST ht_RSA_pri_st,
	IN DWORD dwPadMode,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSADecryptSoft, "HSRSADecryptSoft");
	INVOKE_METHOD(ht_RSA_pri_st, dwPadMode, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}

DWORD USBKeyAPIWrapper::x_HSRSADecrypt
(
	IN HANDLE hCard,
	IN LPCSTR szContainerName,
	IN DWORD dwKeySpec,
	IN DWORD dwPadMode,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSADecrypt, "HSRSADecrypt");
	INVOKE_METHOD(hCard, szContainerName, dwKeySpec, dwPadMode, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}

DWORD USBKeyAPIWrapper::x_HSGetSerial
(
	IN HANDLE hCard,
	OUT BYTE bSerial[SERIAL_LEN]
) {
	GET_METHOD(FUN_HSGetSerial, "HSGetSerial");
	INVOKE_METHOD(hCard, bSerial);
}

DWORD USBKeyAPIWrapper::x_HSConnectDev
(
	IN LPCSTR szDevName,
	OUT HANDLE *phCard
) {
	GET_METHOD(FUN_HSConnectDev, "HSConnectDev");
	INVOKE_METHOD(szDevName, phCard);
}


DWORD USBKeyAPIWrapper::x_HSDisconnectDev
(
	IN HANDLE hCard
) {
	GET_METHOD(FUN_HSDisconnectDev, "HSDisconnectDev");
	INVOKE_METHOD(hCard);
}

DWORD USBKeyAPIWrapper::x_HSListReaders
(
	OUT LPSTR pszReaderName,
	IN OUT DWORD *pdwReaderNameLen,
	OUT DWORD *pdwReaderNum
) {
	GET_METHOD(FUN_HSListReaders, "HSListReaders");
	INVOKE_METHOD(pszReaderName, pdwReaderNameLen, pdwReaderNum);
}


DWORD USBKeyAPIWrapper::x_HSGetDevState
(
	IN LPCSTR szReaderName,
	OUT DWORD *pdwState
) {
	GET_METHOD(FUN_HSGetDevState, "HSGetDevState");
	INVOKE_METHOD(szReaderName, pdwState);
}

DWORD USBKeyAPIWrapper::x_HSSetDevChgEvent
(
	IN LPCSTR szReaderName,
	IN DEVINSERTED InsertProc,
	IN DEVREMOVED RemoveProc,
	IN void *pProcParam,
	OUT HANDLE *phEvtHandler
) {
	GET_METHOD(FUN_HSSetDevChgEvent, "HSSetDevChgEvent");
	INVOKE_METHOD(szReaderName, InsertProc, RemoveProc, pProcParam, phEvtHandler);
}


DWORD USBKeyAPIWrapper::x_HSGetContainerNum
(
	IN HANDLE hCard,
	OUT DWORD *pdwContainerNum
) {
	GET_METHOD(FUN_HSGetContainerNum, "HSGetContainerNum");
	INVOKE_METHOD(hCard, pdwContainerNum);
}

DWORD USBKeyAPIWrapper::x_HSHashPadding
(
	IN HANDLE hCard,
	IN DWORD dwHashAlgID,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSHashPadding, "HSHashPadding");
	INVOKE_METHOD(hCard, dwHashAlgID, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}


DWORD USBKeyAPIWrapper::x_HSSHA1
(
	IN CONST BYTE *pbData,
	IN DWORD dwDataLen,
	OUT BYTE bHash[20]
) {
	GET_METHOD(FUN_HSSHA1, "HSSHA1");
	INVOKE_METHOD(pbData, dwDataLen, bHash);
}

DWORD USBKeyAPIWrapper::x_HSRSAPad
(
	IN DWORD dwRSAOpMode,
	IN DWORD dwPadMode,
	IN DWORD dwNLen,
	IN BYTE *pbInData,
	IN DWORD dwInDataLen,
	OUT BYTE *pbOutData,
	IN OUT DWORD *pdwOutDataLen
) {
	GET_METHOD(FUN_HSRSAPad, "HSRSAPad");
	INVOKE_METHOD(dwRSAOpMode, dwPadMode, dwNLen, pbInData, dwInDataLen, pbOutData, pdwOutDataLen);
}

DWORD USBKeyAPIWrapper::x_HSGenRandom
(
	IN HANDLE hCard,
	IN DWORD dwRandomLen,
	OUT BYTE *pbRandom
) {
	GET_METHOD(FUN_HSGenRandom, "HSGenRandom");
	INVOKE_METHOD(hCard, dwRandomLen, pbRandom);
}