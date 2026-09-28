// BismIllahIRRahmaanIRRaheem
/* common graphics-related definitions */

#ifndef GRAPHICSDEF_H
#define GRAPHICSDEF_H
#include <runtime/enginedef.h>
#include <runtime/rtdebug.h>
#ifdef FLX_WIN32
#include <Windows.h>
#define FLX_TRY_D3D11
#endif // FLX_WIN32
namespace falx {
	enum GraphicsAPI {
		GRAPHICS_API_D3D11
	};
	// to be filled in with the newest supported graphics API
	enum GraphicsAPI g_GraphicsAPI;
	// Only one callsite please - RAM is not a luxury currently
	inline void GetGraphicsAPI() {
#ifdef FLX_TRY_D3D11
		HMODULE h_Module = LoadLibraryA("d3d11.dll");
		if (h_Module) {
			g_GraphicsAPI = GRAPHICS_API_D3D11;
			FreeLibrary(h_Module);
		}
		else {
			FLX_QUIT("Device unsupported");
		}
#endif // FLX_TRY_D3D11
		{
			FLX_QUIT("Device unsupported");
		}
	}
}
#endif // !GRAPHICSDEF_H