// BismIllahIRRahmaanIRRaheem
/* model loader */

#ifndef LOADER_H_FLX
#define LOADER_H_FLX
#include <graphics/graphicsdef.h>
#include <vector>

namespace falx {
	struct IndividualClump {
		float32* m_ModelVerts;
		uint16 m_NumVerts;
		uint16* m_ModelIndices;
		uint32 m_NumIndices;
	};
	struct Brush {
		color* m_Bitmap;
		uint16 m_Width;
		uint16 m_Height;
		std::vector<uint32> m_ClumpAddresses;
	};
	void LoadModel(const char* aPath, std::vector<IndividualClump>& oClumps, std::vector<Brush>& oBrushes);
}
#endif // !LOADER_H_FLX