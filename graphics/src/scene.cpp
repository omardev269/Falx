// BismIllahIRRahmaanIRRaheem
/* graphics scene loader implementation */

#include <graphics/scene.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11scene.h>
#endif // FLX_TRY_D3D11

namespace falx {
    class NullGraphicsScene : public IGraphicsScene {
    public:
        bit Create(IGraphicsDevice*) override { return true; }
        ObjectID CreateObject(ulargeint, ulargeint, uint32) override { return { 0 }; }
        ClumpID CreateClump(CLUMP_METADATA, float32*, uint16*) override { return { 0, 0 }; }
        void UpdateClump(ClumpID, matrix4x4) override {}
        void UpdateClump(ClumpID, IMaterial*) override {}
        void UpdateClumpNumVertices(ClumpID, uint16) override {}
        void UpdateClumpNumIndices(ClumpID, uint32) override {}
        void UpdateClump(ClumpID, float32*, ulargeint) override {}
        void UpdateClump(ClumpID, uint16*, ulargeint) override {}
        void DismissClump(ClumpID& aClump) override { aClump = { 0, 0 }; }
        void DismissObject(ObjectID) override {}
		void AlbedoRenderExcept(ObjectID, matrix4x4) override {}
        void AlbedoRender(ClumpID, matrix4x4) override {}
        void AlbedoRender(ObjectID, matrix4x4) override {}
        void AlbedoRender(matrix4x4) override {}
        void Dismiss() override {}
		IGraphicsDevice* GetParent() override { return NULL; }
    };
}
void falx::CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice)
{
    FLX_SMART_CHECK(aiGraphicsDevice != NULL, "Invalid graphics device for scene creation");
    if (!falx::g_GraphicsAPIAvailable) {
        oiGraphicsScene = new falx::NullGraphicsScene;
        oiGraphicsScene->Create(aiGraphicsDevice);
        return;
    }
    if (!falx::g_GraphicsDeviceAvailable) {
        oiGraphicsScene = NULL;
        return;
    }
#ifdef FLX_TRY_D3D11
    oiGraphicsScene = new D3D11GraphicsScene;
    if (oiGraphicsScene->Create(aiGraphicsDevice)) return;
    delete oiGraphicsScene;
    oiGraphicsScene = NULL;
#else
    oiGraphicsScene = NULL;
    return;
#endif // FLX_TRY_D3D11
}
