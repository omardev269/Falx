// BismIllahIRRahmaanIRRaheem
/* d3d11 graphics scene impl decl header */
#ifndef D3D11SCENE_FLX_H
#define D3D11SCENE_FLX_H
#include <graphics/scene.h>
#ifdef FLX_TRY_D3D11
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>
namespace falx
{
    class D3D11GraphicsScene : public IGraphicsScene {
    public:
        bit Create(IGraphicsDevice* aiGraphicsDevice) override;
        ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes) override;
        ClumpID CreateClump(ulargeint aObjectCount, float32* apVertexStartData, uint32* apIndexStartData) override;
        void UpdateClump(ClumpID aClump, matrix4x4 aTransform) override;
        void UpdateClump(ClumpID aClump, uint16 aNumVertices) override;
        void UpdateClump(ClumpID aClump, uint32 aNumIndices) override;
        void UpdateClump(ClumpID aClump, float32* apVertexData) override;
        void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) override;
        void DismissClump(ClumpID aClump) override;
        void DismissObject(ObjectID aObject) override;
        void AlbedoRender(ClumpID aClump) override;
        void AlbedoRender(ObjectID aObject) override;
        void AlbedoRender() override;
        void Dismiss() override;
    };
}
#endif // FLX_TRY_D3D11
#endif // !D3D11SCENE_FLX_H
