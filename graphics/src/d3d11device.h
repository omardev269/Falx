// BismIllahIRRahmaanIRRaheem
/* D3D11 device class decl header */

#ifndef D3D11DEVICE_FLX_H
#define D3D11DEVICE_FLX_H
#include <graphics/device.h>
#ifdef FLX_TRY_D3D11
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
#include <d3d11gbuffer.h>
#include <vector>
#define FLX_RTVD3D11 ID3D11RenderTargetView
namespace falx {
	class D3D11GraphicsDevice : public IGraphicsDevice {
		float4 m_ClearColor;
		ID3D11Device* i_Device;
		ID3D11DeviceContext* i_Context;
		// -
		
		ID3D11BlendState* i_BlendState;
#ifdef FLX_DEBUG
		// debug overlay (ie.. ImGui)
		GBufferD3D11 m_DebugOverlay;
#endif // FLX_DEBUG

		// rectangle buffer
		
		ID3D11Buffer* i_RectangleVertexBuffer;
		ID3D11Buffer* i_RectangleIndexBuffer;
		ID3D11VertexShader* i_RectangleVertexShader;
		ID3D11PixelShader* i_RectanglePixelShader;
		ID3D11InputLayout* i_RectangleInputLayout;

		//-
		
		std::vector<ID3D11ShaderResourceView*> i_Gbuffers;
		// DSS States

		ID3D11DepthStencilState* i_DssEnabled;
		ID3D11DepthStencilState* i_DssDisabled;
		// -
		
		// output view
		ID3D11RenderTargetView* i_DxgiRepView;
		// - dxgi
		
		IDXGIDevice* i_DxgiDevice;
		IDXGIAdapter* i_Adapter;
		IDXGIFactory* i_Factory;
		IDXGISwapChain* i_SwapChain;
		// -

		HWND h_Window;
		uint32 m_MsaaQuality;
		uint32 m_MsaaLevels;
		D3D_FEATURE_LEVEL m_FeatureLevel;
		bit m_Warp;
	public:
		bit Create(bit aAllowSoftwareRendering, IWindow* aiWindow) override;
		void BeginFrame() override;
		void EndFrame() override;
		FLX_INLINEFUNC void AddGBuffer(ID3D11ShaderResourceView* aiGbuffer) { i_Gbuffers.push_back(aiGbuffer); }
		FLX_INLINEFUNC ID3D11Device* GetDevice() { return i_Device; }
		FLX_INLINEFUNC ID3D11DeviceContext* GetContext() { return i_Context; }
		void Dismiss() override;
	};
}
#endif // FLX_TRY_D3D11
#endif // !D3D11DEVICE_FLX_H