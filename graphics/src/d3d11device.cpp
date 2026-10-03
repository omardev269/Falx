// BismIllahIRRahmaanIRRaheem
/* D3D11 device implementation */

#include "d3d11device.h"
#include "rtd3d11debug.h"
#include <string>
#ifdef FLX_TRY_D3D11
bit falx::D3D11GraphicsDevice::Create(bit aAllowSoftwareRendering, IWindow* aiWindow)
{
	{
		m_ClearColor = { 0.4, 0.6, 1.0, 1.0 };
		i_Device = NULL;
		i_Context = NULL;
		m_Warp = false;
		m_MsaaLevels = 1;
		m_MsaaQuality = 1;
	}
#ifdef FLX_DEBUG
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_Context))) {
#else
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_Context))) {
#endif // FLX_DEBUG
		FLX_LOG("Direct3D11 hardware acceleration unavailable\n");
		m_Warp = true;
		if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_WARP, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &i_Device, &m_FeatureLevel, &i_Context)) || aAllowSoftwareRendering) {
			FLX_LOG("Direct3D11 device creation failed\n");
			return false;
		}
	}
	FLX_LOG("Successfully created a Direct3D11 device (thankfully!)\n");
	if (!m_Warp) {
		if (m_FeatureLevel >= D3D_FEATURE_LEVEL_10_1){
			m_MsaaLevels = 2;
			FLX_SMART_CHECK_HRESULT(i_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 2, &m_MsaaQuality), "Device does not support 2x MSAA while reporting feature level >= 10_1");
			FLX_LOG("MSAA enabled\n");
		}
		else {
			FLX_LOG("MSAA disabled\n");
		}
	}
	else {
		FLX_LOG("MSAA disabled\n");
	}
	{
		DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;
		FLX_ZMEM(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		m_SwapChainDesc.Windowed = TRUE;
		m_SwapChainDesc.BufferCount = 1;
		m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_SwapChainDesc.SampleDesc.Count = m_MsaaLevels;
		m_SwapChainDesc.SampleDesc.Quality = m_MsaaQuality - 1;
		m_SwapChainDesc.OutputWindow = (HWND)aiWindow->GetWindowsIdentifier();
		m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		FLX_SMART_CHECK_HRESULT(i_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&i_DxgiDevice), "DXGI device retrieval failed");
		FLX_SMART_CHECK_HRESULT(i_DxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&i_Adapter), "DXGI adapter retrieval failed");
		FLX_SMART_CHECK_HRESULT(i_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&i_Factory), "DXGI factory retrieval failed");
		FLX_SMART_CHECK_HRESULT(i_Factory->CreateSwapChain(i_Device, &m_SwapChainDesc, &i_SwapChain), "DXGI swapchain creation failed");
	}
	{
		// create main RTV
		ID3D11Texture2D* i_DxgiTexture;
		FLX_SMART_CHECK_HRESULT(i_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&i_DxgiTexture), "DXGI RTO texture recieval failed");
		FLX_SMART_CHECK_HRESULT(i_Device->CreateRenderTargetView(i_DxgiTexture, NULL, &i_DxgiRepView), "Main RTV creation failed");
		i_DxgiTexture->Release();
	}
	return true;
}
void falx::D3D11GraphicsDevice::BeginFrame()
{
	i_Context->ClearRenderTargetView(i_DxgiRepView, (const float32*)&m_ClearColor.x);
}

void falx::D3D11GraphicsDevice::EndFrame()
{
	FLX_SMART_CHECK_HRESULT(i_SwapChain->Present(0, 0), "DXGI swapchain presentation failed");
}

void falx::D3D11GraphicsDevice::Dismiss()
{
	i_DxgiRepView->Release();
	i_SwapChain->Release();
	i_Factory->Release();
	i_Adapter->Release();
	i_DxgiDevice->Release();
	i_Context->Release();
	i_Device->Release();
}


#endif // FLX_TRY_D3D11

