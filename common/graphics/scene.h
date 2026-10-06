// BismIllahIRRahmaanIRRaheem
/* graphics scene header */
#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H
#include <graphics/device.h>
namespace falx
{
	struct ObjectID {
		ulargeint ID;
	};
	struct ClumpID {
		ulargeint FatherID;
		ulargeint ID;
	};
	struct CLUMP_METADATA {
		ObjectID Parent;
		matrix4x4 Transform;
		ulargeint StartVertexBufferBytes;
		ulargeint StartIndexBufferBytes;
		ulargeint StartIndex;
		ulargeint NumVertexBufferBytes;
		ulargeint NumIndexBufferBytes;
		uint16 NumVertices;
		uint32 NumIndices;
	};
	struct IGraphicsScene {
		virtual bit Create(IGraphicsDevice* aiGraphicsDevice) = 0;
		virtual ObjectID CreateObject(ulargeint aVertexBufferBytes, ulargeint aIndexBufferBytes, uint32 aStride) = 0;
		virtual ClumpID CreateClump(CLUMP_METADATA aMetadata, float32* apVertexStartData, uint16* apIndexStartData) = 0;
		virtual void UpdateClump(ClumpID aClump, matrix4x4 aTransform) = 0;
		virtual void UpdateClumpNumVertices(ClumpID aClump, uint16 aNumVertices) = 0;
		virtual void UpdateClumpNumIndices(ClumpID aClump, uint32 aNumIndices) = 0;
		virtual void UpdateClump(ClumpID aClump, float32* apVertexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void UpdateClump(ClumpID aClump, uint16* apIndexData, ulargeint aCurrentAttemptedAllocationSize) = 0;
		virtual void DismissClump(ClumpID& aClump) = 0;
		virtual void DismissObject(ObjectID aObject) = 0;
		virtual void AlbedoRender(ClumpID aClump) = 0;
		virtual void AlbedoRender(ObjectID aObject) = 0;
		virtual void AlbedoRender() = 0;
		virtual void Dismiss() = 0;
	};
	void CreateGraphicsScene(IGraphicsScene*& oiGraphicsScene, IGraphicsDevice* aiGraphicsDevice);
}
#endif // !GRAPHICS_SCENE_H