// BismIllahIRRahmaanIRRaheem
/* material header */

#ifndef FLX_MATERIAL_H
#define FLX_MATERIAL_H
#include <graphics/device.h>

namespace falx {
	// IMaterial is a heavy object (data-wise) and can change due to me wanting PBR - that is why it needs a descriptor

	struct MATERIAL_DESC {
		uint8* BitmapData;
		uint2 BitmapSize;
	};
	
	enum SamplerMode : uint8 {
		SAMPLER_MODE_NEAREST, // pixelated
		SAMPLER_MODE_LINEAR, // lerped
		SAMPLER_MODE_ANISOTROPIC_2, // anisotropic filtering (may not be supported in some cases)
		SAMPLER_MODE_ANISOTROPIC_4, // anisotropic filtering (may not be supported in some cases)
		SAMPLER_MODE_ANISOTROPIC_8, // anisotropic filtering (may not be supported in some cases)
		SAMPLER_MODE_ANISOTROPIC_16 // anisotropic filtering (may not be supported in some cases)
	};

	struct IMaterial {
		virtual bit Create(MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice) = 0;
		virtual bit Reset(MATERIAL_DESC& aDesc) = 0;
		virtual bit Reset(SamplerMode aSamplerMode) = 0;
		virtual void SoftChangeBitmap(uint8* apBitmapData) = 0;
		virtual void Dismiss() = 0;
	};

	void CreateMaterial(IMaterial*& aoMaterial, MATERIAL_DESC& aDesc, SamplerMode aSamplerMode, IGraphicsDevice* aiGraphicsDevice);
}
#endif // !FLX_MATERIAL_H