// BismIllahIRRahmaanIRRaheem
/* d3d11 core pipeline declarations */

#ifndef FLX_D3D11COREPIPELINE_H
#define FLX_D3D11COREPIPELINE_H
#include <graphics/corepipeline.h>
#include <d3d11device.h>
namespace falx {
	class ID3D11GraphicsPipeline : public IGraphicsPipeline {
	protected:
		D3D11GraphicsDevice* i_Device;
		ID3D11ShaderResourceView* i_Texture;
		ID3D11SamplerState* i_Sampler;
		matrix4x4 m_Transform;
		// -
		virtual void ReactionForTransformChange() = 0;
	public:
		virtual void Render(std::function<void()> ahDraw) = 0;
		FLX_INLINEFUNC void SetTransform(const matrix4x4& aTransform) {
			m_Transform = aTransform;
			ReactionForTransformChange();
		}
		FLX_INLINEFUNC void SetRenderTexture(ID3D11ShaderResourceView* aiTexture) {
			i_Texture = aiTexture;
		}
		FLX_INLINEFUNC void SetRenderSampler(ID3D11SamplerState* aiSampler) {
			i_Sampler = aiSampler;
		}
	};
	void CreateAlbedoGraphicsPipeline(IGraphicsPipeline*& oiPipeline, IGraphicsDevice* aiRegistoire);
}
#endif // !FLX_D3D11COREPIPELINE_H