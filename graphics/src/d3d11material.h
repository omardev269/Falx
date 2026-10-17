// BismIllahIRRahmaanIRRaheem
/* d3d11 material implementation class declaration */

#ifndef D3D11MATERIAL_FLX_H
#define	D3D11MATERIAL_FLX_H
#include <graphics/material.h>
#include <d3d11device.h>
namespace falx {
	class D3D11Material : public IMaterial {
		D3D11GraphicsDevice* i_GraphicsDevice;
		ID3D11Texture2D* i_Texture;
		ID3D11ShaderResourceView* i_TextureView;
		ID3D11SamplerState* i_SamplerState;
	public:
		bit Create(MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice) override;
		bit Reset(MATERIAL_DESC& aDesc) override;
		bit Reset(SamplerMode aSamplerMode) override;
		void SoftChangeBitmap(uint8* apBitmapData) override;
		void Dismiss() override;
	};
}
#endif // !D3D11MATERIAL_FLX_H