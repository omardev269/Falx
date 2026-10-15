// BismIllahIRRahmaanIRRaheem
/* d3d11 albedo pipeline impl declaration */

#ifndef FLX_D3D11ALBEDOPIPELINE_H
#define FLX_D3D11ALBEDOPIPELINE_H
#include <d3d11corepipeline.h>
namespace falx {
	class D3D11AlbedoPipeline : public ID3D11GraphicsPipeline {
		ID3D11VertexShader* i_VertexShader;
		ID3D11PixelShader* i_PixelShader;
		ID3D11InputLayout* i_InputLayout;
		ID3D11Buffer* i_TransformBuffer;
		ID3D11Texture2D* i_RenderTexture;
		ID3D11ShaderResourceView* i_RenderTextureView;
		ID3D11RenderTargetView* i_RenderTargetView;
		void ReactionForTransformChange() override;
	public:
		bit Create(IGraphicsDevice* aiRegistoire) override;
		void Render(std::function<void()> ahDraw) override;
		void PushToGraphicsDevice() override;
		void Dismiss() override;
	};
}
#endif // !FLX_D3D11ALBEDOPIPELINE_H