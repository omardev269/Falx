// BismIllahIRRahmaanIRRaheem
/* D3D11 device class decl header */

#ifndef D3D11DEVICE_FLX_H
#define D3D11DEVICE_FLX_H
#include <graphics/device.h>
#ifdef FLX_TRY_D3D11
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
namespace falx {
	class D3D11GraphicsDevice : public IGraphicsDevice {
		float4 m_ClearColor;
		ID3D11Device* i_Device;
		ID3D11DeviceContext* i_Context;
		IDXGIAdapter* i_Adapter;
		IDXGISwapChain* i_SwapChain;
		uint32 m_MsaaQuality;
		uint32 m_MsaaLevels;
		D3D_FEATURE_LEVEL m_FeatureLevel;
		bit m_Warp;
	public:
		bit Create(bit aAllowSoftwareRendering, IWindow* aiWindow) override;
		void BeginFrame() override;
		void EndFrame() override;
		void Dismiss() override;
	};
}
#endif
#endif