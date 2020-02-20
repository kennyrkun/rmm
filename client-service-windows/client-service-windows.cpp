// client-service-windows.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "clientservicewindows_i.h"


using namespace ATL;

#include <stdio.h>

class CclientservicewindowsModule : public ATL::CAtlServiceModuleT< CclientservicewindowsModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_clientservicewindowsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CLIENTSERVICEWINDOWS, "{39333cde-5937-4aae-907e-4ff2fefd3714}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
		// Suggested - PKT Level Authentication,
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY
		// and an appropriate non-null Security Descriptor.

		return S_OK;
	}
};

CclientservicewindowsModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

