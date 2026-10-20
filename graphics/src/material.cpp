// BismIllahIRRahmaanIRRaheem
/* material loader implementation */

#include <graphics/material.h>
#include <runtime/rtdebug.h>
#ifdef FLX_TRY_D3D11
#include <d3d11material.h>
#endif // FLX_TRY_D3D11

namespace falx {
    class NullMaterial : public IMaterial {
        uint2 m_BitmapSize = {};
    public:
        bit Create(MATERIAL_DESC& aDesc, SamplerMode, IGraphicsDevice*) override { m_BitmapSize = aDesc.BitmapSize; return true; }
        bit Reset(MATERIAL_DESC& aDesc) override { m_BitmapSize = aDesc.BitmapSize; return true; }
        bit Reset(SamplerMode) override { return true; }
        void SoftChangeBitmap(uint8*) override {}
        void GetBitmapSize(uint2& oBitmapSize) override { oBitmapSize = m_BitmapSize; }
        void Dismiss() override {}
    };
}

void falx::CreateMaterial(IMaterial*& aoMaterial, MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice)
{
    FLX_SMART_CHECK(aiGraphicsDevice != NULL, "Invalid graphics device for material creation");
    if (!falx::g_GraphicsAPIAvailable) {
        aoMaterial = new falx::NullMaterial;
        aoMaterial->Create(aDesc, aSamplerMode, aiGraphicsDevice);
        return;
    }
    if (!falx::g_GraphicsDeviceAvailable) {
        aoMaterial = NULL;
        return;
    }
#ifdef FLX_TRY_D3D11
    aoMaterial = new D3D11Material;
    if (aoMaterial->Create(aDesc, aSamplerMode, aiGraphicsDevice)) return;
    delete aoMaterial;
    aoMaterial = nullptr;
#else
    aoMaterial = nullptr;
    return;
#endif // FLX_TRY_D3D11
}
