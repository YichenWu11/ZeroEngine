#include "runtime/function/scene/scene_camera.h"

using namespace DirectX::SimpleMath;

namespace Zero {
    SceneCamera::SceneCamera(const Matrix& _projection) :
        Camera(_projection) {
        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float near_clip, float far_clip) {
        m_projectionType   = ProjectionType::Orthographic;
        m_orthographicSize = size;
        m_orthographicNear = near_clip;
        m_orthographicFar  = far_clip;
        recalculateProjection();
    }

    void SceneCamera::setPerspective(float v_fov, float near_clip, float far_clip) {
        m_projectionType  = ProjectionType::Perspective;
        m_perspectiveFOV  = v_fov;
        m_perspectiveNear = near_clip;
        m_perspectiveFar  = far_clip;
        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        m_aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        if (m_projectionType == ProjectionType::Perspective) {
            if (DirectX::XMScalarNearEqual(m_perspectiveNear, m_perspectiveFar, 0.00001f))
                m_perspectiveFar = m_perspectiveNear + 0.1f;

            m_perspectiveNear = std::max(m_perspectiveNear, 0.1f);
            m_perspectiveFar  = std::max(m_perspectiveFar, 0.1f);

            m_projection = DirectX::XMMatrixPerspectiveFovLH(
                m_perspectiveFOV,
                m_aspectRatio,
                m_perspectiveNear,
                m_perspectiveFar);
        }
        else {
            if (DirectX::XMScalarNearEqual(m_orthographicNear, m_orthographicFar, 0.00001f))
                m_orthographicFar = m_orthographicNear + 0.1f;

            m_orthographicSize = std::max(m_orthographicSize, 0.1f);

            m_projection = DirectX::XMMatrixOrthographicLH(
                m_orthographicSize * m_aspectRatio,
                m_orthographicSize,
                m_orthographicNear,
                m_orthographicFar);
        }
    }

} // namespace Zero