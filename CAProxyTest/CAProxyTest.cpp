// CAProxyTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include <iostream>

using namespace std;

#ifdef DEBUG
	#ifdef _WIN64
	#import "../x64/Debug/CAProxy.dll" no_namespace
	#else
	#import "../Debug/CAProxy.dll" no_namespace
	#endif
#else // DEBUG
	#ifdef _WIN64
	#import "../x64/Release/CAProxy.dll" no_namespace
	#else
	#import "../Release/CAProxy.dll" no_namespace
	#endif
#endif

void test_activex() {
	
	CLSID clsid;
	if (S_OK == CLSIDFromProgID(OLESTR("CAProxy.IKeyClient.1"), &clsid))
	{ 
		ATL::CComPtr<IKeyClient> pMyActiveX;
		if (S_OK == pMyActiveX.CoCreateInstance(clsid)) {
			BSTR result = pMyActiveX->help();
			wprintf(L"%s\n", result);
			::SysFreeString(result);

			long myversion = 0;
			LONG version = pMyActiveX->SOF_GetVersion();
			wprintf(L"version is :%d\n", version);

			assert(S_OK == pMyActiveX->SOF_SetSignMethod(12L));
		}
	}
}

#include "openssl/x509.h"
#include <fstream>
#include <openssl/evp.h> 
#pragma comment(lib, "openssl.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "capi.lib")
#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "dasync.lib")

char * Base64Decode(const char *input, int length, char* out, bool with_new_line)
{
	BIO * b64 = NULL;
	BIO * bmem = NULL;
	char * buffer = (char *)malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	int out_len = BIO_read(bmem, buffer, length);
	memcpy(out, buffer, out_len);

	BIO_free_all(bmem);
	free(buffer);

	return buffer;
}


void test_openssl()
{	
	std::ifstream f("C:\\OpenSSL-Win32\\certs\\rootcert.pem");
	const char *pem = "MIIC7DCCAdSgAwIBAgIBAjANBgkqhkiG9w0BAQsFADASMRAwDgYDVQQDDAdSb290IENBMCAXDTE2MDExNTA4MTk0OVoYDzIxMTYwMTE2MDgxOTQ5WjANMQswCQYDVQQDDAJDQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAJadpD0ASxxfxsvdj9IxsogVzMSGLFziaYuE9KejU9+R479RifvwfBANO62sNWJ19X//9G5UjwWmkiOzn1k50DkYsBBA3mJzik6wjt/c58lBIlSEgAgpvDU8ht8w3t20JP9+YqXAeugqFj/Wl9rFQtsvaWSRywjXVlp5fxuEQelNnXcJEKhsKTNExsBUZebo4/J1BWpklWzA9P0lYW5INvDAAwcF1nzlEf0Y6Eot03IMNyg2MTE4hehxjdgCSci8GYnFirE/ojXqqpAcZGh7r2dqWgZUD1Dh+bT2vjrUzj8eTH3GdzI+oljt29102JIUaqj3yzRYkah8FLF9CLNNsUcCAwEAAaNQME4wHQYDVR0OBBYEFLQRM/HX4l73U54gIhBPhga/H8leMB8GA1UdIwQYMBaAFI71Ja8em2uEPXyAmslTnE1y96NSMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBADnZ9uXGAdwfNC3xuERIlBwgLROeBRGgcfHWdXZB/tWkIM9ox88wYKWynanPbra4n0zhepooKt+naeY2HLR8UgwT6sTi0Yfld9mjytA8/DP6AcqtIDDf60vNI00sgxjgZqofVayA9KShzIPzjBec4zI1sg5YzoSNyH28VXFstEpi8CVtmRYQHhc2gDI9MGge4sHRYwaIFkegzpwcEUnp6tTVe9ZvHawgsXF/rCGfH4M6uNO0D+9Md1bdW7382yOtWbkyibsugqnfBYCUH6hAhDlfYzpba2Smb0roc6Crq7HR5HpEYY6qEir9wFMkD5MZsWrNRGRuzd5am82J+aaHz/4=";
	char *decoded = new char[3096];
	//f.read(buffer, 3096);
	//f.close();
	//cout << buffer << endl;
	//int len = EVP_DecodeBlock((unsigned char*)decoded, (const unsigned char*)buffer, strlen(buffer));
	//char* decoded =  Base64Decode(buffer, strlen(buffer), false);
	Base64Decode(pem, strlen(pem), decoded, false);
	if (decoded == NULL) {
		return;
	}

	X509* x = d2i_X509(NULL, (const unsigned char**)&decoded, 3096);
	//free(decoded);
	//decoded = NULL;
	if (x == NULL) {
		return;
	}

	long version = X509_get_version(x);
	cout << "Cert version is: " << version << endl;

	ASN1_INTEGER *sn = X509_get_serialNumber(x);
	cout << "sn->data: " << sn->data << endl;
	cout << "sn->flags: " << sn->flags << endl;
	cout << "sn->length: " << sn->length << endl;
	cout << "sn->type: " << sn->type << endl;

	X509_NAME *issuer = X509_get_issuer_name(x);
	X509_NAME *subject = X509_get_subject_name(x);
	ASN1_TIME *from = X509_get_notBefore(x);
	ASN1_TIME *to = X509_get_notAfter(x);
	EVP_PKEY *pkey = X509_get_pubkey(x);
	
	
	
}

int main()
{
	test_openssl();

	//CoInitialize(NULL);
	//test_activex();
	//CoUninitialize();

	getchar();
    return 0;
}

