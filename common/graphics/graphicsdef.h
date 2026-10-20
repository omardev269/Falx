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
#ifdef FLX_DEBUG
#define FLX_ALLOW_IMGUI
#else
// -RELEDIT
#define FLX_ALLOW_IMGUI
#endif // FLX_DEBUG
namespace falx {
	enum GraphicsAPI {
		GRAPHICS_API_D3D11
	};
	// to be filled in with the newest supported graphics API
	inline enum GraphicsAPI g_GraphicsAPI;
#ifdef FLX_TRY_D3D11
	inline bit g_GraphicsAPIAvailable = true;
#else
	inline bit g_GraphicsAPIAvailable = false;
#endif // FLX_TRY_D3D11
	inline bit g_GraphicsDeviceAvailable = false;
	// Only one callsite please - RAM is not a luxury currently
	inline void GetGraphicsAPI() {
#ifdef FLX_TRY_D3D11
		HMODULE h_Module = LoadLibraryA("d3d11.dll");
		if (h_Module) {
			g_GraphicsAPI = GRAPHICS_API_D3D11;
			g_GraphicsAPIAvailable = true;
			FreeLibrary(h_Module);
		}
		else {
			g_GraphicsAPIAvailable = false;
		}
#else
		g_GraphicsAPIAvailable = false;
#endif // FLX_TRY_D3D11
	}
}
#endif // !GRAPHICSDEF_H