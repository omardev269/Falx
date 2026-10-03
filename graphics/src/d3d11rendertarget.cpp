// BismIllahIRRahmaanIRRaheem
/* D3D11 render target inhclass implementation */

#include <d3d11rendertarget.h>

bit falx::D3D11RenderTarget::Create(IGraphicsDevice* aiParentDevice, int2 aTextureSize)
{
	p_Parent = (D3D11GraphicsDevice*)aiParentDevice;
	m_TextureSize = aTextureSize;
	
	D3D11_RENDER_TARGET_VIEW_DESC m_RtvDesc;
	FLX_ZMEM(&m_RtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	m_RtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_RtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	
    return true;
}
