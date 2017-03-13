#include "stdafx.h"
#include "USBKeyClient.h"
#include "../common/define.h"


USBKeyClient::USBKeyClient()
{
}


USBKeyClient::~USBKeyClient()
{
}


void USBKeyClient::help(BSTR* v)
{
	if (v != NULL)
	{
		//::SysFreeString(*v);
		*v = NULL;
	}

	*v = ::SysAllocString(L"help: This ActiveX control is used to manage CA from USBKey device.");
}

/* 1
原型：	int SOF_GetVersion(unsigned int *puiVersion)
描述：	获取接口的版本号
参数：	puiVersion [out]	版本号
返回值：	0	成功
非0	失败，返回错误代码
备注：	版本号的格式为：0xAAAABBBB,其中AAAA为主版本号，BBBB为次版本号。
*/
int USBKeyClient::SOF_GetVersion(OUT unsigned int *puiVersion)
{
	*puiVersion = 121;
	return SOR_OK;
}


/* 2
原型：	Void SOF_SetSignMethod(int SignMethod);
描述：	设置控件签名使用的签名算法；使用非对称加密算法时，也是通过这个函数来设置
参数：	SignMethod[in] 签名算法标识，详见表5“算法代码表”
返回值：	无
*/
void SOF_SetSignMethod(int SignMethod)
{

}


/* 3
原型：	int SOF_GetSignMethod();
描述：	获得控件签名使用的签名算法
参数：	无
返回值：	获得当前的签名算法代码	详见表5“算法代码表”。
*/
int SOF_GetSignMethod()
{
	return 0;
}

/* 4
原型：	Void SOF_SetEncryptMethod (int EncryptMethod) ;
描述：	设置控件使用的对称加解密算法。
参数：	EncryptMethod	[IN] 对称加解密算法，详见表5“算法代码表”
返回值：	无
*/
void SOF_SetEncryptMethod(int EncryptMethod)
{

}

/* 5
原型：	int SOF_GetEncryptMethod();
描述：	获得控件使用的对称加解密算法代码。详见表5“算法代码表”。
参数：	无
返回值：	当前控件使用的加密算法
*/
int SOF_GetEncryptMethod()
{
	return 0;
}

/* 6
原型：	BSTR SOF_GetUserList();
描述：	取得当前已安装证书的用户列表
参数：	无
返回值：	BSTR ret  用户列表字符串	数据格式：(用户1||标识1&&&用户2||标识2&&&…)
*/
BSTR SOF_GetUserList()
{
	return NULL;
}

/* 7
原型：	BSTR SOF_ExportUserCert(BSTR CertID);
描述：	根据证书实体唯一标识，获取Base64编码的证书字符串。
参数：	BSTR CertID [in]	输入参数，证书实体唯一标识
返回值：	BSTR rv	证书字符串
空	失败空值
备注：	如果是双证书，导出的是签名证书。
*/
BSTR SOF_ExportUserCert(BSTR CertID)
{
	return NULL;
}

/* 8
原型：	int SOF_Login(BSTR CertID, BSTR PassWd)
描述：	校验证书口令
参数：	BSTR CertID [in]	输入参数，证书实体唯一标识
BSTR PassWd [in]	输入参数，口令
返回值：	成功返回0；失败时返回密码重试的剩余次数；锁死时返回 -1
*/
int SOF_Login(BSTR CertID, BSTR PassWd)
{
	return 0;
}


/* 9
原型：	BOOL  SOF_ChangePassWd(BSTR CertID, BSTR OldPassWd，BSTR NewPassWd)
描述：	修改证书口令
参数：	BSTR CertID [in]	输入参数，证书实体唯一标识
BSTR OldPassWd [in]	输入参数，旧口令
BSTR NewPassWd [in]	输入参数，新口令
返回值：	成功 TRUE，失败 FALSE
*/
BOOL  SOF_ChangePassWd(BSTR CertID, BSTR OldPassWd, BSTR NewPassWd)
{
	return true;
}

/* 10
原型：	BSTR SOF_ExportExChangeUserCert (BSTR CertID) ;
描述：	根据证书实体唯一标识，获取Base64编码的证书字符串。指定获取加密（交换）证书
参数：	BSTR CertID[in]	证书实体唯一标识
返回值：	BSTR rv	获取Base64编码的证书字符串
空值		失败
*/
BSTR SOF_ExportExChangeUserCert(BSTR CertID)
{
	return NULL;
}

/* 11
原型：	BSTR SOF_GetCertInfo(BSTR Cert, short Type);
描述：	获取证书信息
参数：	BSTR Cert[in]	Base64编码的证书
short Type[in]	获取信息的类型，TYPE参数见表3“证书信息解析代码表”。
返回值：	BSTR	ret		证书信息
空值				失败
*/
BSTR SOF_GetCertInfo(BSTR Cert, short Type)
{
	return NULL;
}


/* 12
原型：	BSTR SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid) ;
描述：	根据OID获取证书私有扩展项信息
参数：	BSTR Cert[in]	Base64编码的证书
BSTR Oid [in]	私有扩展对象ID，比如“1.2.156.xxx”
返回值：	BSTR ret	证书OID对应的值
空值			失败
*/
BSTR SOF_GetCertInfoByOid(BSTR Cert, BSTR Oid)
{
	return NULL;
}


/* 13
原型：	BSTR SOF_GetUserInfo (BSTR CertId, short type) ;
描述：	获得用户信息
参数：	BSTR Certid[in]	证书标识
type[in]	信息类别，参数和意义见表4“证书对应信息表”。
返回值：	BSTR ret	type对应的值
空值			失败
*/
BSTR SOF_GetUserInfo(BSTR CertId, short type)
{
	return NULL;
}

/* 14
原型：	BOOL SOF_ValidateCert(BSTR Cert);
描述：	验证证书有效性
参数：	BSTR Cert[in]	Base64编码的证书
返回值：	TRUE	成功
FALSE	失败
空值	失败
*/
BOOL SOF_ValidateCert(BSTR Cert)
{
	return true;
}

/* 15
原型：	BSTR SOF_SignData(BSTR CertID, BSTR InData) ;
描述：	对字符串数据进行数字签名，签名格式为PKCS#1
参数：	BSTR CertID[in]	证书标识
BSTR  InData[in] 签名原文
返回值：	BSTR  ret	签名结果, BASE64编码
空值	失败
*/
BSTR SOF_SignData(BSTR CertID, BSTR InData)
{
	return NULL;
}

/* 16
原型：	BOOL SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue) ;
描述：	验证数字签名
参数：	BSTR Cert[in]	签名者证书，BASE64编码
BSTR  InData[in]	签名原文
BSTR  SignValue[in]	签名值，BASE64编码
返回值：	TRUE	成功
FALSE	失败
*/
BOOL SOF_VerifySignedData(BSTR Cert, BSTR InData, BSTR SignValue)
{
	return true;
}

/* 17
原型：	BSTR SOF_SignFile(BSTR CertID, BSTR InFile) ;
描述：	对文件数字签名
参数：	BSTR CertID[in]	证书标识
BSTR  InFile[in]	签名原文文件路径
返回值：	BSTR ret	签名结果
空值	失败
*/
BSTR SOF_SignFile(BSTR CertID, BSTR InFile)
{
	return NULL;
}

/* 18
原型：	BOOL SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue) ;
描述：	验证文件数字签名
参数：	BSTR Cert[in]	签名者证书
BSTR  InFile[in]	签名原文文件路径
BSTR  SignValue[in]	签名值
返回值：	TRUE	成功
FALSE	失败
*/
BOOL SOF_VerifySignedFile(BSTR Cert, BSTR InFile, BSTR SignValue)
{
	return true;
}

/* 19
原型：	BSTR SOF_EncryptData(BSTR SymmKey, BSTR Indata) ;
描述：	使用对称算法加密数据
参数：	BSTR Key[in]	加密密钥
BSTR Indata[in]	待加密的明文
返回值：	BSTR rv	加密后的密文
空值		失败
*/
BSTR SOF_EncryptData(BSTR SymmKey, BSTR Indata)
{
	return NULL;
}

/* 20
原型：	BSTR SOF_DecryptData(BSTR SymmKey, BSTR Indata) ;
描述：	使用对称算法解密数据
参数：	BSTR SymmKey[in]	解密密钥
BSTR Indata[in]	待解密的密文
返回值：	BSTR rv	解密后的明文
空值		失败
*/
BSTR SOF_DecryptData(BSTR SymmKey, BSTR Indata)
{
	return NULL;
}

/* 21
原型：	BOOL SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile) ;
描述：	使用对称算法加密文件
参数：	BSTR SymmKey[in]	加密密钥
BSTR InFile[in]	待加密的明文文件路径
BSTR OutFile[in]	密文文件保存路径
返回值：	TRUE	成功
FALSE	失败
*/
BOOL SOF_EncryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile)
{
	return true;
}

/* 22
原型：	BOOL SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile) ;
描述：	使用对称算法解密文件
参数：	BSTR SymmKey[in]	解密密钥
BSTR InFile[in]	待解密的密文文件路径
BSTR OutFile[in]	明文文件保存路径
返回值：	TRUE	成功
FALSE	失败
*/
BOOL SOF_DecryptFile(BSTR SymmKey, BSTR InFile, BSTR OutFile)
{
	return true;
}

/* 23
原型：	BSTR SOF_PubKeyEncrypt(BSTR Cert, BSTR InData) ;
描述：	使用证书对数据加密。（PKCS#1格式）
参数：	BSTR Cert[in]	证书
BSTR InData[in]	待加密的数据
返回值：	BSTR  rv	成功加密后的密文
空值	失败
备注：	因为是PKCS#1格式，故加密的数据长度要小于证书的位数。比如1024位的证书，InData长度必须小于128
*/
BSTR SOF_PubKeyEncrypt(BSTR Cert, BSTR InData)
{
	return NULL;
}

/* 24
原型：	BSTR SOF_PriKeyDecrypt(BSTR CertID, BSTR InData) ;
描述：	使用证书对数据加密。（PKCS#1格式）
参数：	BSTR Cert[in]
BSTR InData[in]
返回值：	BSTR  rv
空值
备注：	因为是PKCS#1格式，故加密的数据长度要小于证书的位数。比如1024位的证书，InData长度必须小于128
*/
BSTR SOF_PriKeyDecrypt(BSTR CertID, BSTR InData)
{
	return NULL;
}

/* 25
原型：	BSTR SOF_SignDataByP7(BSTR CertID, BSTR InData)
描述：	对字符串数据进行数字签名，签名格式为PKCS#7
参数：	BSTR CertID[in]	证书标识
BSTR  InData[in]	签名原文
返回值：	BSTR  ret 	签名结果
空值	失败
备注：	PKCS#7签名结果包含原文+签名者证书+签名值。
*/
BSTR SOF_SignDataByP7(BSTR CertID, BSTR InData)
{
	return NULL;
}

/* 26
原型：	BOOL SOF_VerifySignedDataByP7 (BSTR P7Data) ;
描述：	验证数字签名
参数：	BSTR P7Data[in]	PKCS#7签名包
返回值：	TRUE	成功
FALSE	失败
*/
BOOL SOF_VerifySignedDataByP7(BSTR P7Data)
{
	return true;
}

/* 27
原型：	BSTR SOF_GetP7SignDataInfo ( BSTR P7Data，short type) ;
描述：	解析PKCS#7签名包的信息，可获得原文、签名值、签名证书等信息
参数：	BSTR P7Data[in]	PKCS#7签名包
short type[in]	类型
返回值：	BSTR ret	成功
空值	失败
备注：	类型：1：原文；2：签名者证书；3：签名值
*/
BSTR SOF_GetP7SignDataInfo(BSTR P7Data, short type)
{
	return NULL;
}

/* 28
原型：	BSTR SOF_SignDataXML(BSTR CertID, BSTR InData) ;
描述：	对XML数据进行数字签名，输出符合国际标准的XML签名结果
参数：	BSTR CertID[in]	证书标识
BSTR  InData[in]	签名原文，XML格式
返回值：	BSTR  ret	签名结果
空值	失败
备注：	XML签名标准见 http://www.w3.org/TR/xmldsig-core/
*/
BSTR SOF_SignDataXML(BSTR CertID, BSTR InData)
{
	return NULL;
}

/* 29
原型：	BOOL  SOF_VerifySignedDataXML (BSTR InData) ;
描述：	验证xml签名
参数：	BSTR  InData[in]	XML签名值
返回值：	TRUE	成功
FALSE	失败
备注：	XML签名标准见 http://www.w3.org/TR/xmldsig-core/
*/
BOOL  SOF_VerifySignedDataXML(BSTR InData)
{
	return true;
}

/* 30
原型：	BSTR  SOF_GetXMLSignatureInfo (BSTR XMLSignedData,short type) ;
描述：	解析XML签名数据，获取签名值、XML原文、证书等信息
参数：	BSTR  XMLSignedData[in]
Type[in]
返回值：	各项对应的信息
备注：	type可选的参数和意义：1：xml原文；2：摘要；3：签名值；4：签名证书；5：摘要算法；6：签名算法。失败返回空值
*/
BSTR  SOF_GetXMLSignatureInfo(BSTR XMLSignedData, short type)
{
	return NULL;
}

/* 31
原型：	Int SOF_CheckSupport();
描述：	检查控件是否支持当前操作系统和浏览器版本
参数：	无
返回值：	int rv	0表示支持，1表示不支持。
*/
int SOF_CheckSupport()
{
	return 0;
}

/* 32
原型：	BSTR SOF_GenRandom(short len);
描述：	产生随机数
参数：	int RanddomLen[in]	待产生的随机数长度（bytes，字节长度）
返回值：	BSTR rv	随机数值（Base64编码后的）
*/
BSTR SOF_GenRandom(short len)
{
	return NULL;
}