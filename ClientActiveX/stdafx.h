// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED

#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的


#define ATL_NO_ASSERT_ON_DESTROY_NONEXISTENT_WINDOW

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include "CertParser\CertParser.h"
#include "HTCSPApi\HTCSPApi.h"

#pragma comment(lib, "comsuppwd.lib")
//#pragma comment(lib,"crypt32.lib")
//#pragma comment(lib, "libs/HTCSPApi/lib/HTCSPApi.lib")
#pragma comment(lib, "SKFAPI/SKFAPI.lib")
#pragma comment(lib, "openssl/libeay32.lib")
#pragma comment(lib, "CertParser/Certsel.lib")

extern DWORD g_dwAlgID;
extern HANDLE g_hCard;
extern char g_szReaderName[1024];
extern DWORD g_dwInitFlag;
