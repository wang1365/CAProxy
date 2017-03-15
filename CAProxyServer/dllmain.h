// dllmain.h: 模块类的声明。

class CCAProxyServerModule : public ATL::CAtlDllModuleT< CCAProxyServerModule >
{
public :
	DECLARE_LIBID(LIBID_CAProxyServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CAPROXYSERVER, "{F464E0DF-B9D4-46A9-85B2-D94E0859C8C8}")
};

extern class CCAProxyServerModule _AtlModule;
