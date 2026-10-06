// BismIllahIRRahmaanIRRaheem
/* graphics scene loader implementation */

#include <graphics/scene.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11scene.h>
#endif // FLX_TRY_D3D11

void falx::CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice)
{
    FLX_SMART_CHECK(aiGraphicsDevice != NULL, "Invalid graphics device for scene creation");
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
