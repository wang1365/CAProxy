#ifndef _CA_CONFIG_H_
#define _CA_CONFIG_H_

#ifndef __WINCRYPT_H__
#include <WinCrypt.h>
#endif

#include <comutil.h>
//#include "SXException.h"


//#ifdef _TSTRING_

#include <string>
using namespace std;

//#ifdef UNICODE
typedef  std::wstring tstring;
//#else
//typedef  std::string  tstring;
//#endif

//#endif

#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"comsuppw.lib")

#endif