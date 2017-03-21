#pragma once

#include "logger.hpp"
#include "HTCSPApi\HTCSPApi.h"


class USBKeyAPIWrapper
{
private:
	USBKeyAPIWrapper() {}
	~USBKeyAPIWrapper() {}

private:
	static tstring DLLNAME;
	HMODULE m_hModule = NULL;
	HANDLE *m_phCard = NULL;

public:
	static USBKeyAPIWrapper* GetInstance() {
		static USBKeyAPIWrapper *service;
		if (service == nullptr) {
			service = new USBKeyAPIWrapper();
			if (!service->init()) {
				delete service;
				service = nullptr;
			}
		}
	
		return service;
	}

private:
	bool init() {
		if (m_hModule == NULL) {
			m_hModule = ::LoadLibrary(GetDllFromRegedit().c_str());
		}

		if (NULL == m_hModule) {
			LOG_ERROR("::LoadLibrary usbkey.dll failed!");
			return false;
		}
		return true;
	}

	static tstring GetDllFromRegedit() {
		// TODO
		DLLNAME = TEXT("usbkey.dll");
		return DLLNAME;
	}

	
public:
	typedef DWORD(*FUN_HSListContainers)(HANDLE, LPSTR, DWORD*, DWORD*);
	DWORD x_HSListContainers
	(
		IN HANDLE hCard,
		OUT LPSTR pszContainersName,
		IN	OUT DWORD *pdwContainersNameLen,
		OUT DWORD *pdwContainerNum
	);

	typedef DWORD(*FUN_HSGetContainerName)(HANDLE, DWORD, LPSTR, DWORD*, DWORD*);
	DWORD x_HSGetContainerName
	(
		IN HANDLE hCard,
		IN DWORD dwContainerIndex,
		OUT LPSTR pszContainerName,
		IN OUT DWORD *pdwContainerNameLen,
		OUT DWORD *pdwContainerParam
	);

	typedef DWORD(*FUN_HSReadCert)(HANDLE, LPCSTR, DWORD, BYTE*, DWORD*);
	DWORD x_HSReadCert
	(
		IN HANDLE hCard,
		IN LPCSTR szContainerName,
		IN DWORD dwKeySpec,
		OUT BYTE *pbCert,
		IN OUT DWORD *pdwCertLen
	);
	
	typedef DWORD(*FUN_HSVerifyUserPin)(HANDLE, LPCSTR, DWORD*);
	DWORD x_HSVerifyUserPin
	(
		IN HANDLE hCard,
		IN LPCSTR szUserPin,
		OUT DWORD *pdwRetryNum
	);

	typedef DWORD(*FUN_HSChangeUserPin)(HANDLE, LPCSTR, LPCSTR, DWORD*);
	DWORD x_HSChangeUserPin
	(
		IN HANDLE hCard,
		IN LPCSTR szOldPin,
		IN LPCSTR szNewPin,
		OUT DWORD *pdwRetryNum
	);

	typedef DWORD(*FUN_HSRSASign)(HANDLE, LPCSTR, DWORD, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSASign
	(
		IN HANDLE hCard,
		IN LPCSTR szContainerName,
		IN DWORD dwKeySpec,
		IN DWORD dwPadMode,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSRSAVerifySoft)(HTCSP_RSA_PUBLIC_ST, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSAVerifySoft
	(
		IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st,
		IN DWORD dwPadMode,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSRSAEncryptSoft)(HTCSP_RSA_PUBLIC_ST, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSAEncryptSoft
	(
		IN HTCSP_RSA_PUBLIC_ST ht_RSA_pub_st,
		IN DWORD dwPadMode,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSRSADecryptSoft)(HTCSP_RSA_PRIVATE_ST, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSADecryptSoft
	(
		IN HTCSP_RSA_PRIVATE_ST ht_RSA_pri_st,
		IN DWORD dwPadMode,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSRSADecrypt)(HANDLE, LPCSTR, DWORD, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSADecrypt
	(
		IN HANDLE hCard,
		IN LPCSTR szContainerName,
		IN DWORD dwKeySpec,
		IN DWORD dwPadMode,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSGetSerial)(HANDLE, BYTE[]);
	DWORD x_HSGetSerial
	(
		IN HANDLE hCard,
		OUT BYTE bSerial[SERIAL_LEN]
	);

	typedef DWORD(*FUN_HSConnectDev)(LPCSTR, HANDLE*);
	DWORD x_HSConnectDev
	(
		IN LPCSTR szDevName,
		OUT HANDLE *phCard
	);

	typedef DWORD(*FUN_HSDisconnectDev)(HANDLE);
	DWORD x_HSDisconnectDev
	(
		IN HANDLE hCard
	);

	typedef DWORD(*FUN_HSListReaders)(LPSTR, DWORD*, DWORD*);
	DWORD x_HSListReaders
	(
		OUT LPSTR pszReaderName,
		IN OUT DWORD *pdwReaderNameLen,
		OUT DWORD *pdwReaderNum
	);

	typedef DWORD(*FUN_HSGetDevState)(LPCSTR, DWORD*);
	DWORD x_HSGetDevState
	(
		IN LPCSTR szReaderName,
		OUT DWORD *pdwState
	);

	typedef DWORD(*FUN_HSSetDevChgEvent)(LPCSTR, DEVINSERTED, DEVREMOVED, void*, HANDLE*);
	DWORD x_HSSetDevChgEvent
	(
		IN LPCSTR szReaderName,
		IN DEVINSERTED InsertProc,
		IN DEVREMOVED RemoveProc,
		IN void *pProcParam,
		OUT HANDLE *phEvtHandler
	);

	typedef DWORD(*FUN_HSGetContainerNum)(HANDLE, DWORD*);
	DWORD x_HSGetContainerNum
	(
		IN HANDLE hCard,
		OUT DWORD *pdwContainerNum
	);

	typedef DWORD(*FUN_HSHashPadding)(HANDLE, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSHashPadding
	(
		IN HANDLE hCard,
		IN DWORD dwHashAlgID,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSSHA1)(CONST BYTE *, DWORD, BYTE[]);
	DWORD x_HSSHA1
	(
		IN CONST BYTE *pbData,
		IN DWORD dwDataLen,
		OUT BYTE bHash[20]
	);

	typedef DWORD(*FUN_HSRSAPad)(DWORD, DWORD, DWORD, BYTE*, DWORD, BYTE*, DWORD*);
	DWORD x_HSRSAPad
	(
		IN DWORD dwRSAOpMode,
		IN DWORD dwPadMode,
		IN DWORD dwNLen,
		IN BYTE *pbInData,
		IN DWORD dwInDataLen,
		OUT BYTE *pbOutData,
		IN OUT DWORD *pdwOutDataLen
	);

	typedef DWORD(*FUN_HSGenRandom)(HANDLE, DWORD, BYTE *);
	DWORD x_HSGenRandom
	(
		IN HANDLE hCard,
		IN DWORD dwRandomLen,
		OUT BYTE *pbRandom
	);
};

