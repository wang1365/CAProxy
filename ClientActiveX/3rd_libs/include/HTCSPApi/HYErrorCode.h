/**********************************************************************
	文件（模块）名：HYErrorCode.h
	版本：V0.1
	作者：Zhl
	内容说明：虎跃项目使用的错误码定义
	修订：
	创建 2010-04-08 Zhl
**********************************************************************/
#ifndef _HY_ERROR_CODE_H
#define _HY_ERROR_CODE_H


#define HT_RETURN_SUCCEED				0x00000000	//返回成功

#define HKI_ERR_DEV_INDEX				0x80100070	//设备索引号超界
#define HKI_ERR_DEV_INUSE				0x80100071	//该设备已处于连接状态，请等待并重新尝试连接
#define HKI_ERR_DEV_READERLEN			0x80100072	//取设备个数时发现设备名超长
#define HKI_ERR_DEV_INVALIDCTX			0x80100073	//释放设备时，指定的设备句柄无效，您可能已调用过HKDisconnectDev函数
#define HKI_ERR_DEV_BYNAME				0x80100074	//找不到指定的设备名或该设备不可用
#define HKI_ERR_DEV_INVALIDINDEX		0x80100075	//无效的设备索引号
#define HKI_ERR_DEV_NOTFOUND			0x80100076	//未发现该设备
#define HKI_ERR_DEV_ATR_LENGTH			0x80100077	//取序列号时，所得ATR的长度有误。应为2-32字节。
#define HKI_ERR_DEV_ATR_FLAG			0x80100078	//取序列号时，所得ATR的首字节标志不正确。应为3B或3F
#define HKI_ERR_DEV_ATR_VALUE			0x80100079	//取序列号时，不能识别ATR内容的格式。可能没有找到0x8888标志
#define HKI_ERR_DEV_HID_CRC				0x80100081	//HID设备通信时发生奇偶校验错误
#define HKI_ERR_DEV_HID_TIMEOUT			0x80100082	//HID设备通信时发生超时错误
#define HKI_ERR_DEV_HID_RESET			0x80100083	//HID设备通信时发生卡片复位信息TS或BCC错误
#define HKI_ERR_DEV_HID_LEN				0x80100084	//HID设备通信时发生长度错误
#define HKI_ERR_DEV_HID_BCC				0x80100085	//HID设备通信中，计算机发送的命令系列校验码错
#define HKI_ERR_DEV_HID_WRITE			0x80100086	//HID设备写操作错误
#define HKI_ERR_DEV_HID_READ			0x80100087	//HID设备读操作错误
#define HKI_ERR_CRYPT_INVALIDPARAM		0x801000A8	////给定的函数参数在无效值域
#define HKI_ERR_MEM_NULLPTR				0x801000A9	//作为参数的指针为空
#define HKI_ERR_MEM_BUFTOOSMALL			0x801000AA	//分配的内存空间不够
#define HKI_ERR_EVT_INVALID				0x801000AB	//无效的设备插拔处理事件句柄
#define HKI_ERR_MUTEX_OBJHOST			0x801000AC	//当前申请服务的对象过多，已无剩余资源可用
#define HKI_ERR_UNKOWN					0x801000AD	//未定义错误
#define HKI_PIN_LEN_ERROR				0x801000AF	//口令长度错误
#define HKI_PIN_RETRY_NUM_ERROR			0x801000B0	//口令错误计数次数错误
#define HKI_DEVICE_NAME_LEN_ERROR		0x801000B1	//设备名字长度错误
#define HKI_FILE_TYPE_ERROR				0x801000B2	//文件类型错误
#define HKI_WRITE_DATA_LEN_ERROR		0x801000B3	//写入的数据长度错误
#define HKI_READ_DATA_LEN_ERROR			0x801000B4	//读出的数据长度错误
#define HKI_RSA_PADDING_TYPE_ERROR		0x801000B5	//RSA补码类型错误
#define HKI_RSA_DATA_LEN_ERROR			0x801000B6	//RSA数据长度错误
#define HKI_ACCESS_TYPE_ERROR			0x801000B7	//权限类型错误
#define HKI_INPUT_DATA_LEN_ERROR		0x801000B8	//SSF33/DES算法输入的数据长度错误，必须为16/8的倍数
#define HKI_DEVICE_CMS_DIFFER_ERROR		0x801000B9	//设备缓冲区的大小和设备类型不符
#define HKI_KEY_LEN_ERROR				0x801000BA	//SSF33密钥长度错误 ， 以前也使用过 0x801000BE HKI_SSF33KEY_LEN_ERROR
#define HKI_KEY_ID_ERROR				0x801000BB	//SSF33密钥ID错误 ， 以前也使用过  HKI_SSF33KEY_ID_ERROR
#define HKI_INVALID_PARAM_TYPE			0x801000BC	//非法的参数类型
#define HKI_RET_DATA_LEN_ERROR			0x801000BD	//设备返回的数据长度错误
#define HKI_ERROR_NOT_ENOUGH_MEMORY		0x801000BF	//内存不足
#define HKI_ENCRYPT_DECRYPT_MODE_ERROR	0x801000C0	//加解密模式错误
#define HKI_STRTOHEX_INPUT_DATA_ERROR	0x801000C1	//字符转换数据错误
#define HKI_NOT_SUPPORT_ERROR			0x801000C2	//功能不支持
#define HKI_FILE_ID_INVALID_ERROR		0x801000C3	//文件ID取值范围不合法
#define HKI_PADDING_INVALID_ERROR		0x801000C4	//数据的补码有误
#define HKI_NOT_SUPPORT_SYM				0x801000C5	//不支持该算法

#define HK_LENGTH_INVALID_ERROR			0x80106700	//写入数据的长度错误  , 以前也使用过0x8010676F。
#define HK_ID_EXIST_ERROR				0x80106AEF	//标识符已存在
#define HK_NOT_SUPPORT_ERROR			0x80106AF0	//功能不支持
#define HK_ID_NOT_FOUND_ERROR			0x80106AF1	//未找到文件
#define HK_NOT_ENOUGH_SIZE				0x80106AF3	//空间不够
#define HK_X_TIMES_LEFT					0x80106430	//--0x8010643E	校验口令错误，还有x次机会，x可以从0xF~0x1
#define HK_X_TIMES_LEFT					0x80106430	//--0x8010643E	校验口令错误，还有x次机会
#define HK_1_TIMES_LEFT					0x80106430	//--0x8010643E	校验口令错误，还有x次机会
#define HK_2_TIMES_LEFT					0x80106431	//--0x8010643E	校验口令错误，还有x次机会
#define HK_3_TIMES_LEFT					0x80106432	//--0x8010643E	校验口令错误，还有x次机会
#define HK_4_TIMES_LEFT					0x80106433	//--0x8010643E	校验口令错误，还有x次机会
#define HK_5_TIMES_LEFT					0x80106434	//--0x8010643E	校验口令错误，还有x次机会
#define HK_6_TIMES_LEFT					0x80106435	//--0x8010643E	校验口令错误，还有x次机会
#define HK_7_TIMES_LEFT					0x80106436	//--0x8010643E	校验口令错误，还有x次机会
#define HK_8_TIMES_LEFT					0x80106437	//--0x8010643E	校验口令错误，还有x次机会
#define HK_9_TIMES_LEFT					0x80106438	//--0x8010643E	校验口令错误，还有x次机会
#define HK_A_TIMES_LEFT					0x80106439	//--0x8010643E	校验口令错误，还有x次机会
#define HK_B_TIMES_LEFT					0x8010643A	//--0x8010643E	校验口令错误，还有x次机会
#define HK_C_TIMES_LEFT					0x8010643B	//--0x8010643E	校验口令错误，还有x次机会
#define HK_D_TIMES_LEFT					0x8010643C	//--0x8010643E	校验口令错误，还有x次机会
#define HK_E_TIMES_LEFT					0x8010643D	//--0x8010643E	校验口令错误，还有x次机会
#define HK_F_TIMES_LEFT					0x8010643E	//--0x8010643E	校验口令错误，还有x次机会
#define HK_NOT_ACCESS_ERROR				0x801069F1	//安全条件不满足
#define HK_PIN_LOCKED_ERROR				0x801069F2	//口令被锁死
#define HT_PARAM_ERROR					0x88000001	//參數錯誤
#define HT_CONTEXT_ERROR				0x88000002	//PC/SC上下文異常
#define HT_LIST_READER_ERROR			0x88000003	//枚舉硬體設備失敗
#define HT_LINK_ERROR					0x88000004	//連接硬體設備失敗
#define HT_UNLINK_ERROR					0x88000005	//斷開硬體設備連接失敗
#define HT_RESET_ERROR					0x88000006	//復位失敗
#define HT_PCSC_ERROR					0x88000007	//PC/SC通訊錯誤
#define HT_UNKNOWN_ERROR				0x88000008	//未知錯誤
#define HT_DATA_ERROR					0x88000020	//資料錯誤
#define HT_DATA_LEN_ERROR				0x88000021	//資料長度錯誤
#define HT_CREATE_MF_ERROR				0x88000030	//創建MF錯誤
#define HT_ACTIVE_MF_ERROR				0x88000031	//啟動MF錯誤
#define HT_DELETE_MF_ERROR				0x88000032	//刪除MF錯誤
#define HT_CREATE_DF_ERROR				0x88000033	//創建DF錯誤
#define HT_CREATE_EF_ERROR				0x88000034	//創建檔錯誤
#define HT_NO_MF						0x88000035	//沒有MF
#define HT_NO_DF						0x88000036	//沒有DF
#define HT_NO_EF						0x88000037	//該文件不存在
#define HT_EF_EXIST						0x88000038	//文件已存在
#define HT_HW_NO_SPACE					0x88000039	//硬體沒有空間
#define HT_HW_INVALID_FORMAT			0x8800003A	//硬體的格式不對
#define HT_WRITE_KEY_ERROR				0x8800003B	//寫密鑰錯誤
#define HT_PIN_LEN_ERROR				0x8800003C	//pin的長度錯誤
#define HT_PIN_ERROR					0x8800003D	//pin錯誤
#define HT_PIN_LOCK						0x8800003E	//pin被鎖死
#define HT_READ_ERROR					0x8800003F	//讀錯誤
#define HT_WRITE_ERROR					0x88000040	//寫錯誤
#define HT_RSA_MODE_ERROR				0x88000041	//RSA模式錯誤
#define HT_LABEL_LEN_ERROR				0x88000042	//Label（S-Key名字）長度錯誤
#define HT_NEED_RIGHT_ERROR				0x88000043	//執行該功能需要許可權
#define HT_COMMAND_ERROR				0x88000044	//硬體指令返回碼錯誤
#define HT_SERIAL_ATR_ERROR				0x88000045	//取序列號時，所得ATR的有誤
#define HT_DONT_SUPPORT_ERROR			0x88000046	//不支援該功能
#define HT_CSP_CONTAINER_EXIST			0x88000050	//CSP密鑰容器已存在
#define HT_CSP_SILENT_ERROR				0x88000051	//用於在CSP不允許彈出UI（用戶介面）時
#define HT_CSP_NO_KEY					0x88000052	//沒有找到RSA密鑰對或相關參數不正確
#define HT_CSP_ERROR					0x88000053	//調用CSP失敗
#define HT_INSTALL_ERROR				0x88000060	//沒有找到安裝項
#define HT_USER_CANCEL					0x88000061	//在UI（用戶介面）上用戶選擇了“取消”按鈕
#define HT_LOAD_DLL_ERROR				0x88000062	//載入動態庫失敗
#define HT_SYS_VER_ERROR				0x88000063	//作業系統版本為98,me，不支援該功能
#define HT_P12_ERROR					0x88000064	//p12證書不能正確解析
#define HT_P12_PWD_ERROR				0x88000065	//p12證書口令錯誤
#define HT_TOO_MORE_FILE_CREATE			0x88000066	//創建了過多的用戶自定義檔
#define HT_NO_CACHED_DATA   			0x88000067	//缓存数据不存在
#define HT_NO_CONTAINER   				0x88000068	//容器不存在
#define HT_INVALID_PARAMETER			0xE0020002	//参数错误
#define HT_BUFFER_NOT_ENOUGH			0xE0020003	//参数空间不足
#define HT_HOST_MEMORY					0xE0020004	//主机内存不足
#define HT_FUNC_NOT_SUPPORT				0xE0020005	//功能不支持
#define HT_DEVICE_NUM_TOO_LARGE			0xE0020006	//设备个数超限
#define HT_INVALID_HANDLE				0xE0020007	//句柄无效
#define HT_DEVICE_NO_RESPONSE			0xE0020008	//设备无响应
#define HT_NO_DEVICE					0xE0020009	//没有设备
#define HT_DEVICE_NOT_IDENTIFIED		0xE002000A	//设备不能识别(复位信息不对)
#define HT_BAD_CHECKSUM					0xE002000B	//通信校验错误
#define HT_ERR_SEND_DATA				0xE002000C	//发送数据过程中出错
#define HT_ERR_READ_DATA				0xE002000D	//读取数据过程中出错
#define HT_COMM_TIMEOUT					0xE002000E	//读取数据过程中发生超时
#define HT_BAD_DATA_FORMAT				0xE002000F	//数据格式不能识别
#define HT_NO_TOKEN						0xE0020010	//没有智能卡
#define HT_COS_NOT_RECOGNIZED			0xE0020011	//不能识别的COS
#define HT_ERR_DEVICE_BYNAME			0xE0020012	//系统设备名称错误
#define HT_WAIT_TIME_OUT				0xE0020013	//等待超时
#define HT_SERIAL_NULL					0xE0020015	//KEY内的序列号为空，可能是没有刻序列号
#define HT_NOT_RIGHT_PCODE			    0xE0020016	//PCODE号不匹配
#define HT_NOT_NEED_PCODE			    0xE0020017	//不需要比较PCODE
#define HT_ENC_ERROR				    0xE0020018	//加密失败
#define HT_DEC_ERROR				    0xE0020019	//解密失败

#endif _HY_ERROR_CODE_H