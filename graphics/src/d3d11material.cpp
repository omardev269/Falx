// BismIllahIRRahmaanIRRaheem
/* d3d11 material implementation */

#include <d3d11material.h>
#include <rtd3d11debug.h>

bit falx::D3D11Material::Create(MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice)
{
	
	{
		// -TRIVIALISM
		FLX_ZMEM(this, sizeof(*this));
		i_GraphicsDevice = (D3D11GraphicsDevice*)aiGraphicsDevice;
		m_BitmapSize = aDesc.BitmapSize;
	}
	{
		D3D11_SAMPLER_DESC m_SamplerDesc;
		FLX_ZMEM(&m_SamplerDesc, sizeof(m_SamplerDesc));
		if ((i_GraphicsDevice->m_FeatureLevel <= D3D_FEATURE_LEVEL_9_3) || i_GraphicsDevice->m_Warp) {
			m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		}
		else {
			m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
			m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		if (aSamplerMode == SAMPLER_MODE_LINEAR) {
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		}
		if (aSamplerMode == SAMPLER_MODE_NEAREST) {
			m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
			m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		}
		if (aSamplerMode >= SAMPLER_MODE_ANISOTROPIC_2) {
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			if (i_GraphicsDevice->m_FeatureLevel <= D3D_FEATURE_LEVEL_9_1) {
				m_SamplerDesc.MaxAnisotropy = 2;
			}
			else {
				m_SamplerDesc.MaxAnisotropy = (uint32)aSamplerMode;
			}
		}
		m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateSamplerState(&m_SamplerDesc, &i_SamplerState), "Failed to create sampler state");
	}
	{
		D3D11_TEXTURE2D_DESC m_TextureDesc;
		FLX_ZMEM(&m_TextureDesc, sizeof(m_TextureDesc));
		m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_TextureDesc.Width = aDesc.BitmapSize.x;
		m_TextureDesc.Height = aDesc.BitmapSize.y;
		m_TextureDesc.MipLevels = 1;
		m_TextureDesc.ArraySize = 1;
		m_TextureDesc.SampleDesc.Count = 1;
		m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SUBRESOURCE_DATA m_SubresourceData;
		FLX_ZMEM(&m_SubresourceData, sizeof(m_SubresourceData));
		m_SubresourceData.pSysMem = aDesc.BitmapData;
		FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateTexture2D(&m_TextureDesc, &m_SubresourceData, &i_Texture), "Failed to create texture");
	}
	{
		FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateShaderResourceView(i_Texture, NULL, &i_TextureView), "Failed to create shader resource view");
	}
	return true;
}

bit falx::D3D11Material::Reset(MATERIAL_DESC& aDesc)
{
	D3D11_TEXTURE2D_DESC m_TextureDesc;
	FLX_ZMEM(&m_TextureDesc, sizeof(m_TextureDesc));
	m_TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDesc.Width = aDesc.BitmapSize.x;
	m_TextureDesc.Height = aDesc.BitmapSize.y;
	m_TextureDesc.MipLevels = 1;
	m_TextureDesc.ArraySize = 1;
	m_TextureDesc.SampleDesc.Count = 1;
	m_TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	D3D11_SUBRESOURCE_DATA m_SubresourceData;
	FLX_ZMEM(&m_SubresourceData, sizeof(m_SubresourceData));
	m_SubresourceData.pSysMem = aDesc.BitmapData;
	i_TextureView->Release();
	i_Texture->Release();
	FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateTexture2D(&m_TextureDesc, &m_SubresourceData, &i_Texture), "Failed to create texture");
	FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateShaderResourceView(i_Texture, NULL, &i_TextureView), "Failed to create shader resource view");
	return true;
}

bit falx::D3D11Material::Reset(SamplerMode aSamplerMode)
{
	D3D11_SAMPLER_DESC m_SamplerDesc;
	FLX_ZMEM(&m_SamplerDesc, sizeof(m_SamplerDesc));
	if ((i_GraphicsDevice->m_FeatureLevel <= D3D_FEATURE_LEVEL_9_3) || i_GraphicsDevice->m_Warp) {
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	}
	else {
		m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	}
	if (aSamplerMode == SAMPLER_MODE_LINEAR) {
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	}
	if (aSamplerMode == SAMPLER_MODE_NEAREST) {
		m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	}
	if (aSamplerMode >= SAMPLER_MODE_ANISOTROPIC_2) {
		m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		if (i_GraphicsDevice->m_FeatureLevel <= D3D_FEATURE_LEVEL_9_1) {
			m_SamplerDesc.MaxAnisotropy = 2;
		}
		else {
			m_SamplerDesc.MaxAnisotropy = (uint32)aSamplerMode;
		}
	}
	m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	i_SamplerState->Release();
	FLX_SMART_CHECK_HRESULT(i_GraphicsDevice->i_Device->CreateSamplerState(&m_SamplerDesc, &i_SamplerState), "Failed to create sampler state");
	return true;
}

void falx::D3D11Material::SoftChangeBitmap(uint8* apBitmapData)
{
	D3D11_BOX m_CoverBox;
	FLX_ZMEM(&m_CoverBox, sizeof(m_CoverBox));
	m_CoverBox.bottom = m_BitmapSize.y;
	m_CoverBox.right = m_BitmapSize.x;
	m_CoverBox.back = 1;
	i_GraphicsDevice->i_Context->UpdateSubresource(i_Texture, 0, &m_CoverBox, apBitmapData, 4 * i_GraphicsDevice->m_Viewport.Width, 0);
}

void falx::D3D11Material::Dismiss()
{
	i_TextureView->Release();
	i_Texture->Release();
	i_SamplerState->Release();
}
