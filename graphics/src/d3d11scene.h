// BismIllahIRRahmaanIRRaheem
/* d3d11 graphics scene impl decl header */
#ifndef D3D11SCENE_FLX_H
#define D3D11SCENE_FLX_H
#include <graphics/scene.h>
#ifdef FLX_TRY_D3D11
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11device.h>
namespace falx
{
    struct ClearClumpD3D11 {
		CLUMP_METADATA Metadata;
    };
    struct D3D11Object {
		ID3D11Buffer* i_VertexBuffer;
		ID3D11Buffer* i_IndexBuffer;
		ulargeint m_VertexBufferBytes;
		ulargeint m_IndexBufferBytes;
		std::vector<ClearClumpD3D11> m_Clumps;
    };
	class D3D11GraphicsScene : public IGraphicsScene {
		ID3D11VertexShader* i_AlbedoVertexShader;
		ID3D11PixelShader* i_AlbedoPixelShader;
		ID3D11Buffer* i_AlbedoConstantBuffer;
		ID3D11InputLayout* i_InputLayout;
		D3D11GraphicsDevice* i_GraphicsDevice;
		std::vector<D3D11Object*> m_Objects;
		ID3D11Texture2D* i_WhiteTexture;
		ID3D11ShaderResourceView* i_WhiteTextureSRV;
	public:
        bit Create(IGraphicsDevice* aiGraphicsDevice) override;
        ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) override;
        ClumpID CreateClump(CLUMP_METADATA aMetadata, float32* apVertexStartData, uint16* apIndexStartData) override;
        void UpdateClump(ClumpID aClump, matrix4x4 aTransform) override;
        void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) override;
        void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) override;
        void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) override;
        void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) override;
        void DismissClump(ClumpID& aClump) override;
        void DismissObject(ObjectID aObject) override;
        void AlbedoRender(ClumpID aClump) override;
        void AlbedoRender(ObjectID aObject) override;
        void AlbedoRender() override;
        void Dismiss() override;
    };
}
#endif // FLX_TRY_D3D11
#endif // !D3D11SCENE_FLX_H
