// BismIllahIRRahmaanIRRaheem
/* D3D11 render target inhclass decl*/

#ifndef D3D11_RENDER_TARGET_H
#define D3D11_RENDER_TARGET_H
#include <graphics/rendertarget.h>
#ifdef FLX_TRY_D3D11
#include <d3d11device.h>
namespace falx {

	class D3D11RenderTarget : public IRenderTarget
	{
		D3D11GraphicsDevice* p_Parent;
		ID3D11RenderTargetView* i_RenderTargetView;
		ID3D11Texture2D* i_PrimaryTexture;
		ID3D11Texture2D* i_Stager; // NULL when unavailable
		int2 m_TextureSize;
	public:
		bit Create(IGraphicsDevice* aiParentDevice, int2 aTextureSize) override;
		void ChangePixels(std::vector<changeablePixel> aChangeablePixels) override;
		void GetPixels(color*& oColors) override;
		bit EnableCPURead() override;
		void DisableCPURead() override;
		void Resize(int2 aNewSize) override;
		FLX_INLINEFUNC ID3D11RenderTargetView* GetNativeRenderTargetView() const { return i_RenderTargetView; };
		FLX_INLINEFUNC ID3D11Texture2D* GetPrimaryTexture() const { return i_PrimaryTexture; };
		FLX_INLINEFUNC ID3D11Texture2D* GetStagerTexture() const { return i_Stager; };
		void Dismiss() override;
	};

}
#endif // FLX_TRY_D3D11
#endif // !D3D11_RENDER_TARGET_H