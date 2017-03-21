#pragma once

#include "define.h"
#include <wtypes.h>
#include "openssl\md5.h"
#include "openssl\evp.h"
#include "openssl\sha.h"
#include "openssl\rsa.h"
#include "openssl\des.h"
#include "openssl\des_old.h"
#include "openssl\x509.h"
#include "openssl\rand.h"
#include "openssl\hmac.h"
#include "openssl\rc4.h"
#include "openssl\ossl_typ.h"

PCCERT_CONTEXT g_pMySignCert = NULL;
HCERTSTORE g_hMyCertStore = NULL;


class Util
{
public:
	static long get_pub_key_from_cert(
			IN BYTE* pbCertData,
			IN long dwCertDataLen,
			OUT BYTE  *pbN,
			INOUT int* pdwNLen,
			OUT BYTE *pbE,
			INOUT int* pdwELen
		)
	{
		X509 *pX509 = NULL;
		EVP_PKEY *pKey = NULL;
		const BYTE *p = NULL;
		BYTE bN[512] = { 0 };
		BYTE bE[160] = { 0 };
		int iE = sizeof(bE);
		int iN = sizeof(bN);

		try
		{
			pX509 = X509_new();
			p = &(pbCertData[0]);
			pX509 = d2i_X509(NULL, &p, dwCertDataLen);
			pKey = X509_get_pubkey(pX509);

			iE = BN_bn2bin(pKey->pkey.rsa->e, bE);
			iN = BN_bn2bin(pKey->pkey.rsa->n, bN);

			if (pbN)
			{
				if (*pdwNLen>iN)
				{
					memcpy(pbN, bN, iN);
				}
			}
			*pdwNLen = iN;
			if (pbE)
			{
				if (*pdwELen>iE)
				{
					memcpy(pbE, bE, iE);
				}
			}
			*pdwELen = iE;

			if (pX509 != NULL)
			{
				X509_free(pX509);
			}

		}
		catch (...)
		{

		}
		return 0;
	}


	static DWORD load_my_sign_cert()
	{
		DWORD dwRet, dwSize, PropId = CERT_KEY_PROV_INFO_PROP_ID;
		CRYPT_KEY_PROV_INFO* pKeyInfo;
		BOOL bRet, bSigFound = FALSE, bExchangeFound = FALSE;
		PCCERT_CONTEXT pCertContext = NULL, m_pMyCert = NULL;

		if (NULL != m_pMyCert)
		{
			CertFreeCertificateContext(m_pMyCert);
			m_pMyCert = NULL;
		}

		if (NULL != g_pMySignCert)
		{
			CertFreeCertificateContext(g_pMySignCert);
			CertCloseStore(g_hMyCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
			g_pMySignCert = NULL;
			g_hMyCertStore = NULL;
		}

		g_hMyCertStore = CertOpenSystemStore(NULL, L"MY");
		if (NULL == g_hMyCertStore)
		{
			return GetLastError();
		}

		while (1)
		{
			pCertContext = CertFindCertificateInStore(g_hMyCertStore, 0, 0, CERT_FIND_PROPERTY, &PropId, pCertContext);
			if (NULL == pCertContext)
			{
				break;
			}

			bRet = CertGetCertificateContextProperty(pCertContext, CERT_KEY_PROV_INFO_PROP_ID, NULL, &dwSize);
			if (FALSE == bRet)
			{
				continue;
			}

			pKeyInfo = (CRYPT_KEY_PROV_INFO*)HeapAlloc(GetProcessHeap(), 0, dwSize);
			if (NULL == pKeyInfo)
			{
				continue;
			}

			bRet = CertGetCertificateContextProperty(pCertContext,
				CERT_KEY_PROV_INFO_PROP_ID,
				pKeyInfo,
				&dwSize);
			if (FALSE == bRet)
			{
				HeapFree(GetProcessHeap(), 0, pKeyInfo);
				continue;
			}

			dwRet = wcscmp(pKeyInfo->pwszProvName, L"HaiTai Cryptographic Service Provider");

			BYTE bUsage;
			bRet = CertGetIntendedKeyUsage(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
				pCertContext->pCertInfo, &bUsage, 1);

			// 		if(0 == dwRet)
			// 		{
			// 			if((AT_KEYEXCHANGE == pKeyInfo->dwKeySpec)
			// 				||(FALSE != (bUsage & CERT_DATA_ENCIPHERMENT_KEY_USAGE)))
			// 			{
			// 				m_pMyCert = CertDuplicateCertificateContext(pCertContext);
			// 
			// 				bExchangeFound = TRUE;
			// 
			// 			}
			// 		}
			if (0 == dwRet)
			{
				if ((FALSE != (bUsage & CERT_DIGITAL_SIGNATURE_KEY_USAGE))
					|| (AT_SIGNATURE == pKeyInfo->dwKeySpec))
				{
					g_pMySignCert = CertDuplicateCertificateContext(pCertContext);

					bSigFound = TRUE;
					break;

				}
			}

			//HeapFree(GetProcessHeap(), 0, pKeyInfo);
		}

		// 	CertCloseStore(m_hMyCertStore, CERT_CLOSE_STORE_FORCE_FLAG);
		// 	m_hMyCertStore = NULL;

		return 1;
	}



	static DWORD sign_message(void *pIn, DWORD dwInLen, void *pOut, PDWORD pdwOutLen)
	{
		if (NULL == pIn)
		{
			return 1;
		}

		if (NULL == pdwOutLen)
		{
			return 2;
		}

		CRYPT_SIGN_MESSAGE_PARA  SigParams;
		BOOL	bRet;
		const BYTE* MessageArray[] = { (const BYTE *)pIn };

		SigParams.cbSize = sizeof(CRYPT_SIGN_MESSAGE_PARA);
		SigParams.dwMsgEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
		SigParams.pSigningCert = g_pMySignCert;
		SigParams.HashAlgorithm.pszObjId = szOID_RSA_SHA1RSA;
		SigParams.HashAlgorithm.Parameters.cbData = NULL;
		SigParams.cMsgCert = 1;
		SigParams.rgpMsgCert = &(g_pMySignCert);
		SigParams.cAuthAttr = 0;
		SigParams.dwInnerContentType = 0;
		SigParams.cMsgCrl = 0;
		SigParams.cUnauthAttr = 0;
		SigParams.dwFlags = 0;
		SigParams.pvHashAuxInfo = NULL;
		SigParams.rgAuthAttr = NULL;

		bRet = CryptSignMessage(&SigParams, FALSE, 1, (const BYTE **)&pIn, &dwInLen, (BYTE *)pOut, pdwOutLen);

		if (FALSE == bRet)
		{
			return GetLastError();
		}

		return 0;
	}



	static DWORD verify_message(void *pSignedMessage, DWORD dwSignedMessage, void *pDecodedMessage, PDWORD pdwDecodedMessage)
	{
		if (NULL == pSignedMessage)
		{
			return 1;
		}

		if (NULL == pdwDecodedMessage)
		{
			return 2;
		}

		CRYPT_VERIFY_MESSAGE_PARA	VerifyPara;

		VerifyPara.cbSize = sizeof(CRYPT_VERIFY_MESSAGE_PARA);
		VerifyPara.dwMsgAndCertEncodingType = PKCS_7_ASN_ENCODING | X509_ASN_ENCODING;
		VerifyPara.hCryptProv = 0;
		VerifyPara.pfnGetSignerCertificate = NULL;
		VerifyPara.pvGetArg = NULL;

		BOOL bRet = CryptVerifyMessageSignature(&VerifyPara, 0, (BYTE *)pSignedMessage, dwSignedMessage, (BYTE *)pDecodedMessage, pdwDecodedMessage, NULL);

		if (FALSE == bRet)
		{
			return GetLastError();

		}

		return 0;
	}


};

