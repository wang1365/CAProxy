#pragma once

//#include <xercesc/util/PlatformUtils.hpp>
//#include <xercesc/dom/DOM.hpp>
//
//
//typedef xercesc_2_8::DOMDocument DOMDOC ;

#include <string>
using namespace std;

class CXmlSign
{
private:
	unsigned int m_dwError;
	unsigned int m_dwAlgo;
	//XMLCh * outputDoc(DOMImplementation * impl, DOMDOC * doc);
	wstring sign(PCCERT_CONTEXT pCert,const char* szXmlString,int nLen = 0);
	BOOL verify(const char* szSignedXmlString,int nLen = 0);
	wstring parse(const char* szSignedXmlString,int nType = 1);

	//int CodePageConvert(UINT SrcCodePage, LPCSTR pBuff, int iBuffLen, UINT DestCodePage, char* &lpCodePage);
	//void UnicodeToUTF_8(char* pOut,WCHAR* pText);
	//void Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	//char* GB2312ToUTF_8(char *pText, int* pLen);
	//char* UTF_8ToGB2312(char *pText, int* pLen);

public:
	CXmlSign();
	CXmlSign(unsigned int dwAlgo);
	~CXmlSign(void);

	unsigned int GetLastError(){return m_dwError;};

	wstring SignXmlFile(void* pCert,const char* szXmlFileName);
	wstring SignXmlString(void* pCert,const char* szXmlString);
	BOOL VerifyXmlFile(const char* szSignedXmlFileName);
	BOOL VerifyXmlString(const char* szSignedXmlString);

	wstring ParseSignedXml(const char* szSignedXmlString,int nType);
};

