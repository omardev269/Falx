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
			
		}
		if (aSamplerMode == SAMPLER_MODE_NEAREST) {
			
		}
		if (aSamplerMode >= SAMPLER_MODE_ANISOTROPIC_2) {
			m_SamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
			
		}
	}
	return true;
}

void falx::D3D11Material::Dismiss()
{

}
