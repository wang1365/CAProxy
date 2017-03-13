#pragma once

#define IN
#define OUT
#define INOUT


/** 客户端控件和COM组件的函数返回值代码表 ***/
#define SOR_OK					0	//成功
#define SOR_UnknownErr			0X0B000001	//异常错误
#define SOR_NotSupportYetErr	0X0B000002	//不支持的服务
#define SOR_FileErr				0X0B000003	//文件操作错误
#define SOR_ProviderTypeErr		0X0B000004	//服务提供者参数类型错误
#define SOR_LoadProviderErr		0X0B000005	//导入服务提供者接口错误
#define SOR_LoadDevMngApiErr	0X0B000006	//导入设备管理接口错误
#define SOR_AlgoTypeErr			0X0B000007	//算法类型错误
#define SOR_NameLenErr			0X0B000008	//名称长度错误
#define SOR_KeyUsageErr			0X0B000009	//密钥用途错误
#define SOR_ModulusLenErr		0X0B000010	//模的长度错误
#define SOR_NotInitializeErr	0X0B000011	//未初始化
#define SOR_ObjErr				0X0B000012	//对象错误
#define SOR_MemoryErr			0X0B000100	//内存错误
#define SOR_TimeoutErr			0X0B000101	//服务超时
#define SOR_IndataLenErr		0X0B000200	//输入数据长度错误
#define SOR_IndataErr			0X0B000201	//输入数据错误
#define SOR_GenRandErr			0X0B000300	//生成随机数错误
#define SOR_HashObjErr			0X0B000301	//HASH对象错
#define SOR_HashErr				0X0B000302	//HASH运算错误
#define SOR_GenRsaKeyErr		0X0B000303	//产生RSA密钥错
#define SOR_RsaModulusLenErr	0X0B000304	//RSA密钥模长错误
#define SOR_CspImprtPubKeyErr	0X0B000305	//CSP服务导入公钥错误
#define SOR_RsaEncErr			0X0B000306	//RSA加密错误
#define SOR_RSGDecErr			0X0B000307	//RSA解密错误
#define SOR_HashNotEqualErr		0X0B000308	//HASH值不相等
#define SOR_KeyNotFountErr		0X0B000309	//密钥未发现
#define SOR_CertNotFountErr		0X0B000310	//证书未发现
#define SOR_NotExportErr		0X0B000311	//对象未导出
#define SOR_DecryptPadErr		0X0B000400	//解密时做补丁错误
#define SOR_MacLenErr			0X0B000401	//MAC长度错误
#define SOR_KeyInfoTypeErr		0X0B000402	//密钥类型错误
#define SOR_NULLPointerErr		0X0B000403	//某一个参数为空指针
#define SOR_APPNOTFOUNDErr		0X0B000404	//没有找到该应用
#define SOR_CERTENCODEErr		0X0B000405	//证书编码格式错误。
#define SOR_CERTINVALIDErr		0X0B000406	//证书无效，不是可信ca颁发的证书。
#define SOR_CERTHASEXPIREDErr	0X0B000407	//证书已过期。
#define SOR_CERTREVOKEDErr		0X0B000408	//证书已经被吊销。
#define SOR_SIGNDATAErr			0X0B000409	//签名失败。
#define SOR_VERIFYSIGNDATAErr	0X0B000410	//验证签名失败
#define SOR_READFILEErr			0X0B000411	//读文件异常，可能文件不存在或没有读取权限等。
#define SOR_WRITEFILEErr		0X0B000412	//写文件异常，可能文件不存在或没有写权限等
#define SOR_SECRETSEGMENTErr	0X0B000413	//门限算法密钥分割失败。
#define SOR_SECERTRECOVERYErr	0X0B000414	//门限恢复失败。
#define SOR_ENCRYPTDATAErr		0X0B000415	//对数据的对称加密失败
#define SOR_DECRYPTDATAErr		0X0B000416	//对称算法的数据解密失败。
#define SOR_PKCS7ENCODEErr		0X0B000417	//PKCS#7编码格式错误
#define SOR_XMLENCODEErr		0X0B000418	//不是合法的xml编码数据
#define SOR_PARAMETERNOTSUPPORTErr	0X0B000419	//不支持的参数

#define SOR_LoginErr			0X0B000420	//登录失败
#define SOR_AddP7BErr			0X0B000421	//添加信任列表失败
#define SOR_CertRevokedErr		0X0B000422	//证书已注销



/***** 证书信息解析代码表 *****/
#define SGD_GET_CERT_VERSION	0x00000001	//证书版本
#define SGD_GET_CERT_SERIAL		0x00000002	//证书序列号
#define SGD_GET_CERT_SIGNALG	0x00000003	//证书签名算法标识
#define SGD_GET_CERT_ISSUER_C	0x00000004	//证书颁发者国家(C)
#define SGD_GET_CERT_ISSUER_O	0x00000005	//证书颁发者组织名(O)
#define SGD_GET_CERT_ISSUER_OU	0x00000006	//证书颁发者部门名(OU)
#define SGD_GET_CERT_ISSUER_S	0x00000007	//证书颁发者所在的省、自治区、直辖市(S)
#define SGD_GET_CERT_ISSUER_CN	0x00000008	//证书颁发者通用名称(CN)
#define SGD_GET_CERT_ISSUER_L	0x00000009	//证书颁发者所在的城市、地区(L)
#define SGD_GET_CERT_ISSUER_E	0x00000010	//证书颁发者Email
#define SGD_GET_CERT_NOTBEFORE	0x00000011	//证书有效期：起始日期
#define SGD_GET_CERT_AFTER		0x00000012	//证书有效期：终止日期
#define SGD_GET_CERT_SUBJECT_C	0x00000013	//证书拥有者国家(C )
#define SGD_GET_CERT_SUBJECT_O	0x00000014	//证书拥有者组织名(O)
#define SGD_GET_CERT_SUBJECT_OU	0x00000015	//证书拥有者部门名(OU)
#define SGD_GET_CERT_SUBJECT_S	0x00000016	//证书拥有者所在的省、自治区、直辖市(S)
#define SGD_GET_CERT_SUBJECT_CN	0x00000017	//证书拥有者通用名称(CN)
#define SGD_GET_CERT_SUBJECT_L	0x00000018	//证书拥有者所在的城市、地区(L)
#define SGD_GET_CERT_SUBJECT_E	0x00000019	//证书拥有者Email
#define SGD_GET_CERT_ISSUER_DN	0x00000020	//证书颁发者DN
#define SGD_GET_CERT_SUBJECT_DN	0x00000021	//证书拥有者DN
#define SGD_GET_CERT_DER_PUBKEY	0x00000022	//证书公钥信息
#define SGD_GET_CERT_DER_EXTENSIONS		0x00000023	//证书扩展项信息
#define SGD_EXT_AUTHORITYKEYIDENTIFIER	0x00000024	//颁发者密钥标识符
#define SGD_EXT_SUBJECTKEYIDENTIFIER	0x00000025	//证书持有者密钥标识符
#define SGD_EXT_KEYUSAGE				0x00000026	//密钥用途
#define SGD_EXT_PRIVATEKEYUSAGEPERIOD	0x00000027	//私钥有效期
#define SGD_EXT_CERTIFICATEPOLICIES		0x00000028	//证书策略
#define SGD_EXT_POLICYMAPPINGS			0x00000029	//策略映射
#define SGD_EXT_BASICCONSTRAINTS		0x00000030	//基本限制
#define SGD_EXT_POLICYCONSTRAINTS		0x00000031	//策略限制
#define SGD_EXT_EXTKEYUSAGE				0x00000032	//扩展密钥用途
#define SGD_EXT_CRLDISTRIBUTIONPO		0x00000033	//CRL发布点
#define SGD_EXT_NETSCAPE_CERT_TYPE		0x00000034	//netscape属性
#define SGD_EXT_CERT_UNIQUEID			0x00000035	//证书实体唯一标识
#define SGD_EXT_IDENTIFYCARDNUMBER		0x00000036	//个人身份证号码
#define SGD_EXT_INURANCENUMBER			0x00000037	//个人社会保险号
#define SGD_EXT_ICREGISTRATIONNUMBER	0x00000038	//企业工商注册号
#define SGD_EXT_ORGANIZATIONCODE		0x00000039	//企业组织机构代码
#define SGD_EXT_TAXATIONNUMBER			0x00000040	//税务登记证号


/***** 证书对应信息表 *****/
#define CERT_SUBJECT		1	//用户名
#define CERT_UNIQUEID		2	//证书实体唯一标识
#define CERT_DEPT			3	//部门
#define CERT_ISSUE			4	//颁发者DN
#define CERT_DEVICETYPE		8	//证书介质类型
#define CERT_CATYPE			9	//CA类型
#define CERT_KEYTYPE		10	//用户证书密钥类型，（双证或单证）
#define CERT_DEVICENAME		13	//用户证书介质名称
#define CERT_DEVICEPROVIDER	14	//用户证书介质提供者即csp名称
#define CERT_DEVICEAFFIX	15	//用户证书介质附加库
#define CERT_SIGNPATH		16	//用户签名证书路径
#define CERT_EXCHPATH		17	//用户加密证书路径
#define CERT_SIGNPFXPATH	18	//用户签名P12证书路径
#define CERT_EXCHPFXPATH	19	//用户加密P12证书路径
#define CERT_UNIQUEIDOID	22	//用户证书UniqueID的OID


/***** 对称算法标识 *****/
#define SGD_SM1_ECB		0x00000101	//SM1算法ECB加密模式
#define SGD_SM1_CBC		0x00000102	//SM1算法CBC加密模式
#define SGD_SM1_CFB		0x00000104	//SM1算法CFB加密模式
#define SGD_SM1_OFB		0x00000108	//SM1算法OFB加密模式
#define SGD_SM1_MAC		0x00000110	//SM1算法MAC加密模式
#define SGD_SSF33_ECB	0x00000201	//SSF33算法ECB加密模式
#define SGD_SSF33_CBC	0x00000202	//SSF33算法CBC加密模式
#define SGD_SSF33_CFB	0x00000204	//SSF33算法CFB加密模式
#define SGD_SSF33_OFB	0x00000208	//SSF33算法OFB加密模式
#define SGD_SSF33_MAC	0x00000210	//SSF33算法MAC加密模式


/***** 非对称算法标识 *****/
#define SGD_SM2_1	0x00020100	//SM2签名算法
#define SGD_SM2_2	0x00020200	//SM2密钥交换协议
#define SGD_SM2_3	0x00020400	//SM2加密算法
#define SGD_RSA		0x00010000	//RSA算法


/***** 杂凑算法标识 *****/
#define SGD_SM3		0x00000001	//SM3杂凑算法
#define SGD_SHA1	0x00000002	//SHA1杂凑算法
#define SGD_SHA256	0x00000004	//SHA256杂凑算法


/***** 签名算法标识 *****/
#define SGD_SHA1_RSA	0x00001002	//Sha1RSA算法
#define SGD_SHA256_RSA	0x00001004	//Sha256RSA算法
#define SGD_SM3_RSA		0x00001001	//SM3RSA算法
#define SGD_SM3_SM2		0x00020101	//SM3SM2算法
