#pragma once

#include <string>  
#include <comutil.h>  
using namespace std;

class CStringUtil
{
public:
	static string ws2s(const wstring& ws)
	{
		_bstr_t t = ws.c_str();
		char* pchar = (char*)t;
		string result = pchar;
		return result;
	}

	static wstring s2ws(const string& s)
	{
		_bstr_t t = s.c_str();
		wchar_t* pwchar = (wchar_t*)t;
		wstring result = pwchar;
		return result;
	}

	//UTF-8到GB2312的转换
	static string utf8_to_gb2312(const char* utf8)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;

		string result = str;
		delete[] str;
		return result;
	}

	//GB2312到UTF-8的转换
	static string gb2312_to_utf8(const char* gb2312)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;

		string result = str;
		delete[] str;
		return result;
	}
};