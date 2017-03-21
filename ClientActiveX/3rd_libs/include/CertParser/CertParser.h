#ifndef  __CA_CERT_PASER_
#define __CA_CERT_PASER_
/*
* certpaser.h
* parse certificate
* by goo @ 2009-8-27
*
*/

#include "config.h"
class CCertParser
{
private:
	PCCERT_CONTEXT m_pCertContext;
	unsigned int m_dwLastError;

	tstring GetSubString(tstring sSrc,tstring sFind);
	tstring GetTimeStr(FILETIME ft);
	tstring GetKeyUage(BYTE ku);
	tstring GetHexStr(BYTE* pData,DWORD dwLen);
	BYTE* DecodeExtObj(PCERT_EXTENSION ext);
	string GetOid(int nType);
public:
	CCertParser(PCCERT_CONTEXT pCert);
	CCertParser();
	~CCertParser(void);

	tstring GetCertVersion();
	tstring GetCertSerialNum();
	tstring GetCertPeriod(BOOL bNotBefore = TRUE);
	tstring GetCertSubject();
	tstring GetCertIssuer();
	tstring GetSubjectAndIssuerInfo(int nType);
	tstring GetKeyUage();

	tstring GetCertInfoByOid(tstring sOid);
	tstring GetCertSignatureAlg();
	tstring GetCertUniqueId(BOOL bSubject = TRUE);
	tstring GetCertExt(int nType ,string oid = "");
	tstring GetPubKey();

	int CheckCRL(BYTE* pData,DWORD dwData);
};

#endif //__CA_CERT_PASER_
