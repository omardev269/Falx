// BismIllahIRRahmaanIRRaheem
/* model loader */

#ifndef LOADER_H_FLX
#define LOADER_H_FLX
#include <graphics/graphicsdef.h>
#include <vector>
#include <graphics/scene.h>

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
	bit LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene, std::vector<IndividualClump>& oClumps, std::vector<Brush>& oBrushes);
	bit LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene);
	bit LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene, ObjectID& oObjectID);
	bit LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene, float3 aPosition, float3 aRotation);
}
#endif // !LOADER_H_FLX