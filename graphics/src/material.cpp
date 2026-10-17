// BismIllahIRRahmaanIRRaheem
/* material loader implementation */

#include <graphics/material.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11material.h>
#endif // FLX_TRY_D3D11

void falx::CreateMaterial(IMaterial*& aoMaterial, MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice)
{
    FLX_SMART_CHECK(aiGraphicsDevice != NULL, "Invalid graphics device for material creation");
#ifdef FLX_TRY_D3D11
    aoMaterial = new D3D11Material;
    if (aoMaterial->Create(aDesc, aSamplerMode, aiGraphicsDevice)) return;
    delete aoMaterial;
    aoMaterial = NULL;
#else
    aoMaterial = NULL;
    return;
#endif // FLX_TRY_D3D11
}
