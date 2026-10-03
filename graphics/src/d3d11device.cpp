// BismIllahIRRahmaanIRRaheem
/* D3D11 device implementation */

#include "d3d11device.h"
#include "rtd3d11debug.h"
#include <string>
#ifdef FLX_TRY_D3D11
static uint32 g_D3D11VertexShaderBytecode[] = { 1128421444, 2048115713, 4039072426, 205861471, 841364194, 1, 844, 6, 56, 252,
472, 596, 672, 756, 963538753, 188, 188, 4294836736, 148, 40,
2359296, 2359296, 2359296, 2359296, 2359297, 0, 4294836736, 83886161, 2685337601, 1065353216,
0, 1056964608, 0, 33554463, 2147483653, 2416902144, 33554463, 2147549189, 2416902145, 67108868,
2147680256, 2427060224, 2699296769, 2699100161, 50331653, 2147680256, 2162425856, 2695495681, 50331650, 3221487616,
2153054208, 2147483648, 50331650, 3221422080, 2430861312, 2699296768, 33554433, 3221749760, 2684354561, 33554433,
3758292992, 2430861313, 65535, 1380206675, 212, 65600, 53, 50331743, 1052786, 0,
50331743, 1052722, 1, 67108967, 1057010, 0, 1, 50331749, 1056818, 1,
33554536, 1, 83886134, 1048594, 0, 1052714, 0, 83886134, 1048610, 0,
16385, 1065353216, 167772175, 1056834, 0, 16386, 1056964608, 1056964608, 0, 0,
1048646, 0, 83886134, 1056818, 0, 1052742, 0, 83886134, 1056898, 0,
16385, 1065353216, 83886134, 1056818, 1, 1052742, 1, 16777278, 1413567571, 116,
7, 1, 0, 4, 1, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1178944594,
68, 0, 0, 0, 28, 4294837248, 2304, 28, 1919117645, 1718580079,
1378361460, 1279795241, 1394625619, 1701077352, 1866670194, 1818849389, 824210021, 3223088, 1313297225, 76,
2, 8, 56, 0, 0, 3, 0, 1799, 65, 0,
0, 3, 1, 771, 1230196560, 1313818964, 1480938496, 1380929347, 2880110660, 1313297231,
80, 2, 8, 56, 0, 1, 3, 0, 15, 68,
0, 0, 3, 1, 3075, 1348425299, 1414091599, 5132105, 1129858388, 1146244943,
2880154368, };
static uint32 g_D3D11PixelShaderBytecode[] = { 1128421444, 1375268566, 1200379889, 3503995554, 238599292, 1, 696, 6, 56, 164,
272, 396, 556, 644, 963538753, 100, 100, 4294902272, 60, 40,
2621440, 2621440, 2621440, 2359297, 2621440, 0, 4294902272, 33554463, 2147483648, 2952986624,
33554463, 2415919104, 2685339648, 50331714, 2148466688, 2967732224, 2699298816, 33554433, 2148468736, 2162425856,
65535, 1380206675, 100, 64, 25, 50331738, 1073152, 0, 67115096, 1077248,
0, 21845, 50335842, 1052722, 1, 50331749, 1057010, 0, 150995013, 1057010,
0, 1052742, 1, 1080902, 0, 1073152, 0, 16777278, 1413567571, 116,
2, 0, 0, 2, 0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1178944594,
152, 0, 0, 2, 28, 4294902784, 2304, 112, 92, 3,
0, 0, 0, 0, 1, 1, 102, 2, 5, 4,
4294967295, 0, 1, 13, 1632853863, 1701605485, 1600585842, 1954047316, 6648437, 1919117645,
1718580079, 1378361460, 1279795241, 1394625619, 1701077352, 1866670194, 1818849389, 824210021, 3223088, 1313297225,
80, 2, 8, 56, 0, 1, 3, 0, 15, 68,
0, 0, 3, 1, 771, 1348425299, 1414091599, 5132105, 1129858388, 1146244943,
2880154368, 1313297231, 44, 1, 8, 32, 0, 0, 3, 0,
15, 1415534163, 1162302017, 2880110676, };
bit falx::D3D11GraphicsDevice::Create(bit aAllowSoftwareRendering, IWindow* aiWindow)
{
	{
		m_ClearColor = { 0.4, 0.6, 1.0, 1.0 };
		i_Device = NULL;
		i_Context = NULL;
		m_Warp = false;
		m_MsaaLevels = 1;
		m_MsaaQuality = 1;
		i_RectangleVertexBuffer = NULL;
		i_RectangleIndexBuffer = NULL;
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
	{
		D3D11_BLEND_DESC m_BlendDesc;
		FLX_ZMEM(&m_BlendDesc, sizeof(D3D11_BLEND_DESC));
		m_BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		m_BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		m_BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		m_BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		m_BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		m_BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		m_BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		m_BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateBlendState(&m_BlendDesc, &i_BlendState), "Debug overlay blend state creation failed");
	}
	{
		D3D11_BUFFER_DESC m_RectangleVertexBufferDesc;
		FLX_ZMEM(&m_RectangleVertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_RectangleVertexBufferDesc.ByteWidth = sizeof(float32) * 5 * 4;
		m_RectangleVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA m_RectangleVertexBufferData;
		FLX_ZMEM(&m_RectangleVertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		float32 m_RectangleVertices[20] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 1.0f
		};
		m_RectangleVertexBufferData.pSysMem = m_RectangleVertices;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateBuffer(&m_RectangleVertexBufferDesc, &m_RectangleVertexBufferData, &i_RectangleVertexBuffer), "Rectangle vertex buffer creation failed");
	}
	{
		D3D11_BUFFER_DESC m_RectangleIndexBufferDesc;
		FLX_ZMEM(&m_RectangleIndexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_RectangleIndexBufferDesc.ByteWidth = sizeof(uint16) * 6;
		m_RectangleIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA m_RectangleIndexBufferData;
		FLX_ZMEM(&m_RectangleIndexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		uint16 m_RectangleIndices[6] = {
			0, 1, 3,
			3, 2, 0
		};
		m_RectangleIndexBufferData.pSysMem = m_RectangleIndices;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateBuffer(&m_RectangleIndexBufferDesc, &m_RectangleIndexBufferData, &i_RectangleIndexBuffer), "Rectangle index buffer creation failed");
	}
	{
		D3D11_DEPTH_STENCIL_DESC m_DssEnabledDesc;
		FLX_ZMEM(&m_DssEnabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		m_DssEnabledDesc.DepthEnable = TRUE;
		m_DssEnabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		m_DssEnabledDesc.DepthFunc = D3D11_COMPARISON_LESS;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateDepthStencilState(&m_DssEnabledDesc, &i_DssEnabled), "Depth stencil state creation failed");
	}
	{
		D3D11_DEPTH_STENCIL_DESC m_DssDisabledDesc;
		FLX_ZMEM(&m_DssDisabledDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		m_DssDisabledDesc.DepthEnable = FALSE;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateDepthStencilState(&m_DssDisabledDesc, &i_DssDisabled), "Depth stencil state creation failed");
	}
	{
		FLX_SMART_CHECK_HRESULT(i_Device->CreateVertexShader(g_D3D11VertexShaderBytecode, sizeof(g_D3D11VertexShaderBytecode), NULL, &i_RectangleVertexShader), "Rectangle vertex shader creation failed");
		FLX_SMART_CHECK_HRESULT(i_Device->CreatePixelShader(g_D3D11PixelShaderBytecode, sizeof(g_D3D11PixelShaderBytecode), NULL, &i_RectanglePixelShader), "Rectangle pixel shader creation failed");
	}
	{
		D3D11_INPUT_ELEMENT_DESC m_RectangleInputLayoutDesc[2];
		FLX_ZMEM(&m_RectangleInputLayoutDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);
		m_RectangleInputLayoutDesc[0].SemanticName = "POSITION";
		m_RectangleInputLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		m_RectangleInputLayoutDesc[1].SemanticName = "TEXCOORD";
		m_RectangleInputLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		FLX_SMART_CHECK_HRESULT(i_Device->CreateInputLayout(m_RectangleInputLayoutDesc, 2, g_D3D11VertexShaderBytecode, sizeof(g_D3D11VertexShaderBytecode), &i_RectangleInputLayout), "Rectangle input layout creation failed");
	}
	return true;
}
void falx::D3D11GraphicsDevice::BeginFrame()
{
	i_Context->ClearRenderTargetView(i_DxgiRepView, (const float32*)&m_ClearColor.x);
	i_Context->OMSetBlendState(i_BlendState, NULL, 0xffffffff);
}

void falx::D3D11GraphicsDevice::EndFrame()
{
	i_Context->OMSetBlendState(i_BlendState, NULL, 0xffffffff);
	i_Context->OMSetRenderTargets(1, &i_DxgiRepView, NULL);
	i_Context->VSSetShader(i_RectangleVertexShader, NULL, 0);
	i_Context->PSSetShader(i_RectanglePixelShader, NULL, 0);
	i_Context->OMSetDepthStencilState(i_DssDisabled, 0);
	i_Context->IASetIndexBuffer(i_RectangleIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	UINT m_Stride = sizeof(float32) * 5;
	UINT m_Offset = 0;
	i_Context->IASetVertexBuffers(0, 1, &i_RectangleVertexBuffer, &m_Stride, &m_Offset);
	for (ID3D11ShaderResourceView* m_Gbuffer : i_Gbuffers) {
		i_Context->PSSetShaderResources(0, 1, &m_Gbuffer);
		i_Context->DrawIndexed(6, 0, 0);
	}
	FLX_SMART_CHECK_HRESULT(i_SwapChain->Present(0, 0), "DXGI swapchain presentation failed");
}


void falx::D3D11GraphicsDevice::Dismiss()
{
	i_RectangleInputLayout->Release();
	i_RectanglePixelShader->Release();
	i_RectangleVertexShader->Release();
	i_DssDisabled->Release();
	i_DssEnabled->Release();
	i_RectangleIndexBuffer->Release();
	i_RectangleVertexBuffer->Release();
	i_BlendState->Release();
	i_DxgiRepView->Release();
	i_SwapChain->Release();
	i_Factory->Release();
	i_Adapter->Release();
	i_DxgiDevice->Release();
	i_Context->Release();
	i_Device->Release();
}


#endif // FLX_TRY_D3D11

