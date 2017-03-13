// dllmain.h: 模块类的声明。

class CCAProxyModule : public ATL::CAtlDllModuleT< CCAProxyModule >
{
public :
	DECLARE_LIBID(LIBID_CAProxyLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CAPROXY, "{016DC079-A488-4A76-83B7-E7D598C86D43}")
};

extern class CCAProxyModule _AtlModule;
