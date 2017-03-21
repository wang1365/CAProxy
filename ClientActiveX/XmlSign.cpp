#include "StdAfx.h"
#include "XmlSign.h"
#include "base64_m.h"

#include "sofdefine.h"
#include "stringutil.h"

#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/sax/SAXException.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/util/ParseException.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/framework/MemBufFormatTarget.hpp"
// XSEC	 
#include "xsec/utils/XSECPlatformUtils.hpp"
#include "xsec/framework/XSECProvider.hpp"
#include "xsec/dsig/DSIGReference.hpp"
#include "xsec/framework/XSECException.hpp"
#include "xsec/dsig/DSIGSignature.hpp"
#include "xsec/dsig/DSIGKeyInfoX509.hpp"
#include "xsec/dsig/DSIGKeyInfoValue.hpp"
#include "xsec/enc/WinCAPI/WinCAPICryptoKeyRSA.hpp"
#include "xsec/enc/WinCAPI/WinCAPICryptoProvider.hpp"
#include "xsec/enc/XSECCryptoException.hpp"
#include "xsec/dsig/DSIGObject.hpp"
		 
#include "xsec/canon/XSECC14n20010315.hpp"

#pragma comment(lib,"xsec_1.lib")
#pragma comment(lib,"../xerces-c_2_8_0-x86-windows-vc_8_0/lib/xerces-c_2.lib")

XERCES_CPP_NAMESPACE_USE
typedef xercesc_2_8::DOMDocument DOMDOC ;


XMLCh * outputDoc(DOMImplementation * impl, DOMDOC * doc)
{
	//XMLFormatTarget *formatTarget = new LocalFileFormatTarget(lpSaveDocFilepath);

	DOMWriter  *theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();

	theSerializer->setEncoding(MAKE_UNICODE_STRING("UTF-8"));
	if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, false))
		theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, false);

	XMLCh * p = theSerializer->writeToString(*doc);

	delete theSerializer;
	//delete formatTarget;

	return p;
}

//int CXmlSign::CodePageConvert(UINT SrcCodePage, LPCSTR pBuff, int iBuffLen, UINT DestCodePage, char* &lpCodePage)
//{
//	int iWideCharCnt = MultiByteToWideChar(SrcCodePage, 0, pBuff, iBuffLen, NULL, 0);
//	LPWSTR lpszWideChar = new wchar_t[iWideCharCnt + 1];
//	memset(lpszWideChar, 0, (iWideCharCnt + 1) * sizeof(WCHAR));
//	iWideCharCnt = MultiByteToWideChar(SrcCodePage, 0, pBuff, iBuffLen, lpszWideChar, iWideCharCnt);
//
//	if(DestCodePage == 54936 
//		&& !IsValidCodePage(54936))
//		DestCodePage = 936;
//
//	int iDestCnt = WideCharToMultiByte(DestCodePage, 0, lpszWideChar, iWideCharCnt, NULL, 0, NULL, NULL);
//	lpCodePage = new char[iDestCnt + 1];
//	memset(lpCodePage, 0, iDestCnt + 1);
//	iDestCnt = WideCharToMultiByte(DestCodePage, 0, lpszWideChar, iWideCharCnt, lpCodePage, iDestCnt, NULL, NULL);
//
//	delete []lpszWideChar; 
//	return iDestCnt;
//}
//
//
//char* CXmlSign::UTF_8ToGB2312(char *pText, int* pLen)
//{
//	char * r = NULL;
//	*pLen = CodePageConvert(CP_UTF8,pText,*pLen,CP_ACP,r);
//	return r;
//}
//
//char* CXmlSign::GB2312ToUTF_8(char *pText, int* pLen)
//{
//	char * r = NULL;
//	*pLen = CodePageConvert(CP_ACP,pText,*pLen,CP_UTF8,r);
//	return r;
//}

// Unicode 转换成UTF-8 
//void CXmlSign::UnicodeToUTF_8(char* pOut,WCHAR* pText)
//{
//	// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
//	char* pchar = (char *)pText;
//
//	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
//	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
//	pOut[2] = (0x80 | (pchar[0] & 0x3F));
//
//	return;
//}

// GB2312 转换成　Unicode
//void CXmlSign::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer)
//{
//	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
//	return;
//}

//char* CXmlSign::GB2312ToUTF_8(char *pText, int* pLen)
//{
//	char buf[4];
//	int nLen = *pLen;
//	char* rst = new char[nLen + (nLen >> 2) + 2];
//
//	memset(buf,0,4);
//	memset(rst,0,nLen + (nLen >> 2) + 2);
//
//	int i = 0;
//	int j = 0;      
//	while(i < nLen)
//	{
//		//如果是英文直接复制就可以
//		if( *(pText + i) >= 0)
//		{
//			rst[j++] = pText[i++];
//		}
//		else
//		{
//			WCHAR pbuffer;
//			Gb2312ToUnicode(&pbuffer,pText+i);
//
//			UnicodeToUTF_8(buf,&pbuffer);
//
//			//unsigned short int tmp = 0;
//			/*tmp = */rst[j] = buf[0];
//			/*tmp = */rst[j+1] = buf[1];
//			/*tmp = */rst[j+2] = buf[2];
//
//			j += 3;
//			i += 2;
//		}
//	}
//	rst[j] = '\0';
//
//	*pLen = j;
//
//	return rst;
//}

BOOL CXmlSign::verify(const char* szSignedXmlString,int nLen)
{
	MemBufInputSource * pStream = NULL;
	XercesDOMParser * pParser = NULL;
	XSECProvider prov;
	DSIGSignature *pSig = NULL;
	HCRYPTPROV hCryptProv = NULL; 
	HCRYPTKEY hKey = NULL;
	PCCERT_CONTEXT pCert = NULL;
	WinCAPICryptoKeyRSA * pRsa = NULL;
	DOMDOC *pDoc = NULL;
	BOOL bRet = FALSE;

	try
	{
		if(nLen>0)
			pStream = new MemBufInputSource((XMLByte*)szSignedXmlString,nLen,"xml");

		pParser = new XercesDOMParser;
		pParser->setDoNamespaces(true);
		pParser->setValidationScheme(XercesDOMParser::Val_Never);
		pParser->setDoSchema(false);
		pParser->setCreateEntityReferenceNodes(false);

		if(nLen>0)
			pParser->parse(*pStream);
		else
			pParser->parse(szSignedXmlString);

		pDoc = pParser->getDocument();
		if(pDoc == NULL)
		{
			m_dwError = SOR_XMLENCODEErr;
			goto err;
		}

		pSig = prov.newSignatureFromDOM(pDoc);

		pSig->load();

		DSIGKeyInfoList * kil = pSig->getKeyInfoList();

		if(!(CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0 | CRYPT_SILENT))) 
		{
			if (!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_SILENT))
			{
				//g_sErrmsg = "获取公钥句柄失败：CryptAcquireContext";
				m_dwError = SOR_VERIFYSIGNDATAErr;
				goto err;
			}
		}

		if (kil->item(0)->getKeyInfoType() == DSIGKeyInfo::KEYINFO_X509) 
		{
			XSECCryptoBase64 * b64 = XSECPlatformUtils::g_cryptoProvider->base64();
			unsigned char b64Str[2048];
			unsigned int outputLen = 0;

			safeBuffer sb;
			sb.sbXMLChIn(((DSIGKeyInfoX509 *) kil->item(0))->getCertificateItem(0));
			b64->decodeInit();
			outputLen = b64->decode(sb.rawBuffer(),sb.sbRawBufferSize(),b64Str,2048);
			outputLen += b64->decodeFinish(&b64Str[outputLen], 2048 - outputLen);
			pCert = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING ,b64Str,outputLen);
			PCERT_INFO pCertInfo = pCert->pCertInfo;
			if(!(CryptImportPublicKeyInfoEx(hCryptProv,X509_ASN_ENCODING,&(pCertInfo->SubjectPublicKeyInfo),CALG_RSA_SIGN|CALG_RSA_KEYX,0,NULL,&hKey)))
			{
				//g_sErrmsg = "导入公钥信息失败：CryptImportPublicKeyInfoEx";
				m_dwError = SOR_VERIFYSIGNDATAErr;
				goto err;
			}
			pRsa = new WinCAPICryptoKeyRSA(hCryptProv,hKey);
		}
		else if(kil->item(0)->getKeyInfoType() == DSIGKeyInfo::KEYINFO_VALUE_RSA)
		{
			DSIGKeyInfoValue* dkiv = (DSIGKeyInfoValue *) kil->item(0);
			const XMLCh* modulus = dkiv->getRSAModulus();
			const XMLCh* exponent = dkiv->getRSAExponent();
			pRsa = new WinCAPICryptoKeyRSA(hCryptProv);
			pRsa->loadPublicExponentBase64BigNums(XMLString::transcode(exponent),
				XMLString::stringLen(exponent));
			pRsa->loadPublicModulusBase64BigNums(XMLString::transcode(modulus),
				XMLString::stringLen(modulus));
			pRsa->importKey();
		}
		else
		{
			//g_sErrmsg = "未知的或不支持的\"KeyInfo\"信息";
			m_dwError = SOR_VERIFYSIGNDATAErr;
			goto err;
		}
		pSig->setSigningKey(pRsa);
		bRet = pSig->verify();
		if(!bRet)
			m_dwError = SOR_VERIFYSIGNDATAErr;
	}
	//catch(ParseException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	//catch(XSECCryptoException e)
	//{
	//	//g_sErrmsg = (char*)e.getMsg();
	//	bRet = FALSE;
	//}
	//catch(XSECException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMsg());
	//	bRet = FALSE;
	//}
	//catch(DOMException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	//catch(SAXException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	catch(...)
	{
		//g_sErrmsg = "未知错误";
		m_dwError = SOR_UnknownErr;
	}

err:
	if(pSig)
		prov.releaseSignature(pSig);	
	// 	if(pDoc)
	// 		pDoc->release();
	if(pParser)
		delete pParser;
	if (pCert)
		CertFreeCertificateContext(pCert);
	if (hKey) 
		CryptDestroyKey(hKey);
	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	if(pStream)
		delete pStream;

	return bRet;
}

wstring CXmlSign::sign(PCCERT_CONTEXT pCert,const char* szXmlString,int nLen )
{
	DWORD dwKeySpec = 0;
	HCRYPTPROV hCryptProv = NULL; 
	DOMDOC *pDoc = NULL;
	DOMDOC  *pOrinDoc = NULL;
	XSECProvider prov;
	DSIGSignature *pSig = NULL;
	DSIGReference *pRef = NULL;
	DOMElement *pSigNode = NULL;
	WinCAPICryptoKeyRSA *pRsa = NULL;
	XercesDOMParser * pParser = NULL;
	MemBufInputSource * pStream = NULL;

	wstring sRet = L"";

	try
	{
		if(nLen>0)
			pStream = new MemBufInputSource((XMLByte*)szXmlString,nLen,"xml");
		pParser = new XercesDOMParser;
		pParser->setDoNamespaces(true);
		pParser->setValidationScheme(XercesDOMParser::Val_Never);
		pParser->setDoSchema(false);
		pParser->setCreateEntityReferenceNodes(true);
		if(nLen>0)
			pParser->parse(*pStream);
		else
			pParser->parse(szXmlString);

		pOrinDoc = pParser->getDocument();

		XMLCh tempStr[100];
		XMLString::transcode("Core", tempStr, 99);    
		DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

		pDoc = impl->createDocument();
		/*
		* Now we have a document, create a signature for it.
		*/
		pSig = prov.newSignature();
		pSigNode = pSig->createBlankSignature(pDoc, 
			DSIGConstants::s_unicodeStrURIC14N_NOC,
			DSIGConstants::s_unicodeStrURIRSA_SHA1);

		DOMElement * rootElem = pOrinDoc->getDocumentElement();
		if(rootElem == NULL)
		{
			//g_sErrmsg = "没有获取到XML的根结点";
			m_dwError = SOR_XMLENCODEErr;
			goto err;
		}

		pDoc->setStandalone(false);
		pDoc->appendChild(pDoc->createTextNode(DSIGConstants::s_unicodeStrNL));
		pDoc->appendChild(pSigNode);

		// Add an object
		DSIGObject * obj = pSig->appendObject();
		obj->setId(MAKE_UNICODE_STRING("data"));
		DOMNode* node = pDoc->importNode(rootElem,TRUE);
		obj->appendChild(node);

		/*
		* Add some references
		*/
		pRef = pSig->createReference(MAKE_UNICODE_STRING("#data"),
			DSIGConstants::s_unicodeStrURISHA1);
		//pRef->appendEnvelopedSignatureTransform();

		XSECCryptoBase64 * b64 = XSECPlatformUtils::g_cryptoProvider->base64();

			unsigned char b64Str[4096];
			unsigned int outputLen = 0;
			b64->encodeInit();
			outputLen = b64->encode((unsigned char *) ((PCCERT_CONTEXT)pCert)->pbCertEncoded, ((PCCERT_CONTEXT)pCert)->cbCertEncoded, b64Str, 4096);
			outputLen += b64->encodeFinish(&b64Str[outputLen], 4096 - outputLen);
			b64Str[outputLen] = '\0';
			safeBuffer sb = (safeBuffer)(char*)b64Str;

			// Append an X509Data
			DSIGKeyInfoX509 * x509 = pSig->appendX509Data();
			x509->appendX509Certificate(sb.sbStrToXMLCh());

			if(!(CryptAcquireCertificatePrivateKey((PCCERT_CONTEXT)pCert, 0, NULL, &hCryptProv, &dwKeySpec, NULL))) 
			{
				if (pCert)
					CertFreeCertificateContext(pCert);
				
				m_dwError = SOR_SIGNDATAErr;
				goto err;
			}

			pRsa = new WinCAPICryptoKeyRSA(hCryptProv,dwKeySpec,TRUE);

			pSig->setSigningKey(pRsa);
			pSig->sign();
		

		// Output the document post signature if necessary
		sRet = outputDoc(impl,pDoc);
	}
	//catch(ParseException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	//catch(XSECCryptoException e)
	//{
	//	//g_sErrmsg = (char*)e.getMsg();
	//	bRet = FALSE;
	//}
	//catch(XSECException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMsg());
	//	bRet = FALSE;
	//}
	//catch(DOMException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	//catch(SAXException e)
	//{
	//	//g_sErrmsg = XMLString::transcode(e.getMessage());
	//	bRet = FALSE;
	//}
	catch(...)
	{
		//g_sErrmsg = "未知错误";
		//MessageBox(NULL,_T("exception"),_T("xml"),MB_OK);
		m_dwError = SOR_UnknownErr;
	}
err:
	if(pSig)
		prov.releaseSignature(pSig);
	if(pDoc)
		pDoc->release();
	if(hCryptProv)
		CryptReleaseContext(hCryptProv, 0);
	if(pStream)
		delete pStream;

	return sRet;
}

wstring CXmlSign::parse(const char* szSignedXmlString,int nType)
{
	MemBufInputSource * pStream = NULL;
	XercesDOMParser * pParser = NULL;
	XSECProvider prov;
	DSIGSignature *pSig = NULL;
	DOMDOC *pDoc = NULL;
	DSIGKeyInfoList * kil = NULL;
	DSIGReferenceList* rl = NULL;
	wstring sRet = L"";
	XMLByte * pHash = NULL;
	unsigned int nHashLen = 0;
	DSIGObject* pObj = NULL;
	wstring sTemp = L"";
	char* pTemp = NULL;
	int len = 0;

	Base64 b64;
	/*DOMElement * ele = NULL;
	DOMNode* node = NULL;

	DOMNodeList* nl = NULL;
	DOMText* tn = NULL;*/

	try
	{
		int nLen = (int)strlen(szSignedXmlString);
		pStream = new MemBufInputSource((XMLByte*)szSignedXmlString,nLen,"xml");
		pParser = new XercesDOMParser;
		pParser->setDoNamespaces(true);
		pParser->setValidationScheme(XercesDOMParser::Val_Never);
		pParser->setDoSchema(false);
		pParser->setCreateEntityReferenceNodes(false);
		pParser->parse(*pStream);

		pDoc = pParser->getDocument();
		if(pDoc == NULL)
		{
			m_dwError = SOR_XMLENCODEErr;
			goto err;
		}

		pSig = prov.newSignatureFromDOM(pDoc);
		pSig->load();
		size_t pos = 0;
		switch (nType)
		{
		case 1: //原文
			sRet = CStringUtil::s2ws(CStringUtil::utf8_to_gb2312(szSignedXmlString));
			pObj = pSig->getObjectItem(0);
			sTemp = L"<";
			sTemp += ((DOMElement*)pObj->getElement())->getNodeName();
			sTemp += L" Id=\"";
			sTemp += pObj->getId();
			sTemp += L"\">";
			pos = sRet.find(sTemp);
			if(pos == string::npos)
			{
				m_dwError = SOR_XMLENCODEErr;
				break;
			}

			sRet = sRet.substr(pos+sTemp.length());
			sTemp =L"</";
			sTemp += ((DOMElement*)pObj->getElement())->getNodeName();
			sTemp += L">";
			pos = sRet.find(sTemp);
			if(pos == string::npos)
			{
				m_dwError = SOR_XMLENCODEErr;
				break;
			}
			sRet = sRet.substr(0,pos);

			break;
		case 2: //摘要
			pHash = new XMLByte[256];
			rl = pSig->getReferenceList();
			nHashLen = rl->item(0)->readHash(pHash,256);
			sRet = CStringUtil::s2ws(b64.base64_encode(pHash,nHashLen));
			delete[] pHash;
			break;
		case 3://签名值
			sRet = pSig->getSignatureValue();
			break;
		case 4://签名证书
			kil = pSig->getKeyInfoList();
			if (kil->item(0)->getKeyInfoType() == DSIGKeyInfo::KEYINFO_X509) 
			{
				sRet = ((DSIGKeyInfoX509 *) kil->item(0))->getCertificateItem(0);
			}
			break;
		case 5://摘要算法
			pHash = new XMLByte[256];
			rl = pSig->getReferenceList();
			nHashLen = rl->item(0)->readHash(pHash,256);
			delete[] pHash;
			if(nHashLen == 20)
				sRet = L"SHA1";
			else if(nHashLen == 16)
				sRet = L"MD5";
			else
				sRet = L"UNKNOWN";
			/*switch(pSig->getHashMethod())
			{
			case HASH_NONE:
				sRet = _T("HASH_NONE");
				break;
			case HASH_SHA1:
				sRet = _T("SHA1");
				break;
			case HASH_MD5:
				sRet = _T("MD5");
				break;
			case HASH_SHA224:
				sRet = _T("SHA224");
				break;
			case HASH_SHA256:
				sRet = _T("SHA256");
				break;
			case HASH_SHA384:
				sRet = _T("SHA384");
				break;
			case HASH_SHA512:
				sRet = _T("SHA512");
				break;
			}*/
			break;
		case 6://签名算法
			pHash = new XMLByte[256];
			rl = pSig->getReferenceList();
			nHashLen = rl->item(0)->readHash(pHash,256);
			delete[] pHash;
			if(nHashLen == 20)
				sRet = L"RSA-SHA1";
			else if(nHashLen == 16)
				sRet = L"RSA-MD5";
			else
				sRet = L"UNKNOWN";
			/*switch (pSig->getSignatureMethod())
			{
			case SIGNATURE_NONE:
				sRet = _T("SIGNATURE_NONE");
				break;
			case SIGNATURE_DSA:
				sRet = _T("DSA");
				break;
			case SIGNATURE_HMAC:
				sRet = _T("Hash MAC");
				break;
			case SIGNATURE_RSA:*/
				//sRet = _T("RSA");
				//break;
			//}
			break;
		default://
			m_dwError = SOR_PARAMETERNOTSUPPORTErr;
			break;
		}
	}
	catch(...)
	{
		m_dwError = SOR_UnknownErr;
	}
err:
	if(pSig)
		prov.releaseSignature(pSig);
	if(pDoc)
		pDoc->release();
	if(pStream)
		delete pStream;
	return sRet;
}

wstring CXmlSign::ParseSignedXml(const char* szSignedXmlString,int nType)
{
	try 
	{
		m_dwError = 0;
		XMLPlatformUtils::Initialize();
		XSECPlatformUtils::Initialise();

		// Setup for Windows Crypt API
		WinCAPICryptoProvider * cp;
		// First set windows as the crypto provider
		cp = new WinCAPICryptoProvider();
		XSECPlatformUtils::SetCryptoProvider(cp);
	}
	catch (...) 
	{
		//g_sErrmsg = XMLString::transcode(e.getMessage());
		m_dwError = SOR_UnknownErr;
		return L"";
	}

	wstring sRet = parse(CStringUtil::gb2312_to_utf8(szSignedXmlString).c_str(),nType);
	XSECPlatformUtils::Terminate();
	XMLPlatformUtils::Terminate();

	return sRet;
}

wstring CXmlSign::SignXmlFile(void* pCert,const char* szXmlFileName)
{
	try 
	{
		m_dwError = 0;
		XMLPlatformUtils::Initialize();
		XSECPlatformUtils::Initialise();

		// Setup for Windows Crypt API
		WinCAPICryptoProvider * cp;
		// First set windows as the crypto provider
		cp = new WinCAPICryptoProvider();
		XSECPlatformUtils::SetCryptoProvider(cp);
	}
	catch (...) 
	{
		//g_sErrmsg = XMLString::transcode(e.getMessage());
		m_dwError = SOR_UnknownErr;
		return L"";
	}

	wstring sRet = sign((PCCERT_CONTEXT)pCert,szXmlFileName);

	XSECPlatformUtils::Terminate();
	XMLPlatformUtils::Terminate();

	return sRet;
}

wstring CXmlSign::SignXmlString(void* pCert,const char* szXmlString)
{
	try {
		m_dwError = 0;
		XMLPlatformUtils::Initialize();
		XSECPlatformUtils::Initialise();

		// Setup for Windows Crypt API
		WinCAPICryptoProvider * cp;
		// First set windows as the crypto provider
		cp = new WinCAPICryptoProvider();
		XSECPlatformUtils::SetCryptoProvider(cp);
	}
	catch (...) {
		//g_sErrmsg = XMLString::transcode(e.getMessage());
		m_dwError = SOR_UnknownErr;
		return L"";
	}

	//MessageBoxA(NULL,szXmlString,"xml",MB_OK);
	
	string stmp = CCommon::GB2312ToUTF_8(szXmlString);
	wstring sRet = sign((PCCERT_CONTEXT)pCert,stmp.c_str(),(int)stmp.length());

	XSECPlatformUtils::Terminate();
	XMLPlatformUtils::Terminate();

	return sRet;
}

BOOL CXmlSign::VerifyXmlFile(const char* szSignedXmlFileName)
{
	try {
		m_dwError = 0;
		XMLPlatformUtils::Initialize();
		XSECPlatformUtils::Initialise();

		// Setup for Windows Crypt API
		WinCAPICryptoProvider * cp;
		// First set windows as the crypto provider
		cp = new WinCAPICryptoProvider();
		XSECPlatformUtils::SetCryptoProvider(cp);
	}
	catch (...) {
		//g_sErrmsg = XMLString::transcode(e.getMessage());
		m_dwError = SOR_UnknownErr;
		return FALSE;
	}

	BOOL b = verify(szSignedXmlFileName);

	XSECPlatformUtils::Terminate();
	XMLPlatformUtils::Terminate();

	return b;
}

BOOL CXmlSign::VerifyXmlString(const char* szSignedXmlString)
{
	try {
		m_dwError = 0;
		XMLPlatformUtils::Initialize();
		XSECPlatformUtils::Initialise();

		// Setup for Windows Crypt API
		WinCAPICryptoProvider * cp;
		// First set windows as the crypto provider
		cp = new WinCAPICryptoProvider();
		XSECPlatformUtils::SetCryptoProvider(cp);
	}
	catch (...) {
		//g_sErrmsg = XMLString::transcode(e.getMessage());
		m_dwError = SOR_UnknownErr;
		return FALSE;
	}

	string stmp = CCommon::GB2312ToUTF_8(szSignedXmlString);
	BOOL b = verify(stmp.c_str(),(int)stmp.length());
	//BOOL b = verify(pTemp,szServer,nPort,nLen);

	//if(pTemp)
	//	delete []pTemp;

	XSECPlatformUtils::Terminate();
	XMLPlatformUtils::Terminate();

	return b;
}

//LPCSTR WINAPI gtGetErrMsg()
//{
//	return g_sErrmsg.c_str();
//}

CXmlSign::CXmlSign()
: m_dwAlgo(0)
, m_dwError(0)
{

}

CXmlSign::CXmlSign(unsigned int dwAlgo)
: m_dwAlgo(dwAlgo)
, m_dwError(0)
{
}

CXmlSign::~CXmlSign(void)
{
}