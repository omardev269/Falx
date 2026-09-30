// BismIllahIRRahmaanIRRaheem
/* real-time d3d11-related debug support system */

#ifndef RTD3D11DEBUG_H
#define RTD3D11DEBUG_H
#include <graphics/graphicsdef.h>
#ifdef FLX_TRY_D3D11
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <runtime/rtdebug.h>
#define FLX_SMART_CHECK_HRESULT(x) FLX_SMART_CHECK(SUCCEEDED(x), "HRESULT error - Direct3D11")
#define FLX_CHECK_HRESULT(x) FLX_CHECK(SUCCEEDED(x), "HRESULT error - Direct3D11")
#define FLX_ASSERT_HRESULT(x) FLX_ASSERT(SUCCEEDED(x))
#endif // FLX_TRY_D3D11
#endif // !RTD3D11DEBUG_H