// BismIllahIRRahmaanIRRaheem
/* freefly camera implementation */
// MADE MOSTLY BY AI
#include <graphics/freefly.h>
#include <gb_math.h>
#include <string.h>

namespace falx {
    static const float32 FLX_FREEFLY_NEAR = 0.1f;
    static const float32 FLX_FREEFLY_FAR = 1000.0f;
    static const float32 FLX_FREEFLY_MAXPITCH = 1.553343f; // 89 degrees

    void FreeflyCamera::Reset() {
        memset(this, 0, sizeof(*this));
        gb_mat4_identity((gbMat4*)&m_Mat);
        m_Speed = 1.0f;
    }

    void FreeflyCamera::Teleport(float3 aPos) {
        m_Position = aPos;
    }

    void FreeflyCamera::SetRotation(float3 aRot) {
        m_Rotation = aRot;
    }

    void FreeflyCamera::Move(float3 aDelta) {
        m_PositionalDelta.x += aDelta.x;
        m_PositionalDelta.y += aDelta.y;
        m_PositionalDelta.z += aDelta.z;
    }

    void FreeflyCamera::Rotate(float3 aDelta) {
        m_RotationalDelta.x += aDelta.x;
        m_RotationalDelta.y += aDelta.y;
        m_RotationalDelta.z += aDelta.z;
    }

    void FreeflyCamera::SetSpeed(float32 aSpeed) {
        m_Speed = aSpeed;
    }

    void FreeflyCamera::Update(float32 aDeltaTime, int32 aFOV, float32 aAspectRatio) {
        // rotation: x = pitch, y = yaw (radians)
        m_Rotation.x += m_RotationalDelta.x;
        m_Rotation.y += m_RotationalDelta.y;
        m_Rotation.z += m_RotationalDelta.z;
        m_RotationalDelta = float3{ 0.0f, 0.0f, 0.0f };
        if (m_Rotation.x > FLX_FREEFLY_MAXPITCH) m_Rotation.x = FLX_FREEFLY_MAXPITCH;
        if (m_Rotation.x < -FLX_FREEFLY_MAXPITCH) m_Rotation.x = -FLX_FREEFLY_MAXPITCH;

        float32 cp = gb_cos(m_Rotation.x), sp = gb_sin(m_Rotation.x);
        float32 cy = gb_cos(m_Rotation.y), sy = gb_sin(m_Rotation.y);

        gbVec3 forward = gb_vec3(cp * sy, sp, -cp * cy);
        gbVec3 worldUp = gb_vec3(0.0f, 1.0f, 0.0f);
        gbVec3 right, up;
        gb_vec3_cross(&right, forward, worldUp);
        gb_vec3_norm(&right, right);
        gb_vec3_cross(&up, right, forward);

        // positional delta: x = right, y = up, z = forward
        float32 step = m_Speed * aDeltaTime;
        m_Position.x += (right.x * m_PositionalDelta.x + up.x * m_PositionalDelta.y + forward.x * m_PositionalDelta.z) * step;
        m_Position.y += (right.y * m_PositionalDelta.x + up.y * m_PositionalDelta.y + forward.y * m_PositionalDelta.z) * step;
        m_Position.z += (right.z * m_PositionalDelta.x + up.z * m_PositionalDelta.y + forward.z * m_PositionalDelta.z) * step;
        m_PositionalDelta = float3{ 0.0f, 0.0f, 0.0f };

        gbVec3 eye = gb_vec3(m_Position.x, m_Position.y, m_Position.z);
        gbVec3 centre;
        gb_vec3_add(&centre, eye, forward);

        gbMat4 view, proj;
        gb_mat4_look_at(&view, eye, centre, worldUp);
        gb_mat4_perspective(&proj, gb_to_radians((float)aFOV), aAspectRatio, FLX_FREEFLY_NEAR, FLX_FREEFLY_FAR);
        gb_mat4_mul((gbMat4*)&m_Mat, &proj, &view);
    }
    void FreeflyCamera::GetMatrix(matrix4x4& oMatrix)
    {
		oMatrix = m_Mat;
    }
}
