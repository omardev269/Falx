// BismIllahIRRahmaanIRRaheem
/* freefly camera header */

#ifndef FREEFLY_FLX_H
#define FREEFLY_FLX_H
#include <graphics/graphicsdef.h>
namespace falx {
	class FreeflyCamera {
		float3 m_Rotation;
		float3 m_Position;
		float3 m_RotationalDelta;
		float3 m_PositionalDelta;
		matrix4x4 m_Mat;
		float32 m_Speed;
	public:
		void Reset();
		void Teleport(float3 aPos);
		void SetRotation(float3 aRot);
		void Move(float3 aDelta);
		void Rotate(float3 aDelta);
		void SetSpeed(float32 aSpeed);
		void Update(float32 aDeltaTime, int32 aFOV, float32 aAspectRatio);
		void GetMatrix(matrix4x4& oMatrix);
	};
}
#endif // !FREEFLY_FLX_H