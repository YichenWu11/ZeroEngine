#include "runtime/function/scene/scene_camera.h"

using namespace DirectX::SimpleMath;

namespace Zero {
    SceneCamera::SceneCamera(const Matrix& _projection) :
        Camera(_projection) {
        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float near_clip, float far_clip) {
        m_orthographicSize = size;
        m_orthographicNear = near_clip;
        m_orthographicFar  = far_clip;
        recalculateProjection();
    }

    void SceneCamera::setViewportSize(uint32_t width, uint32_t height) {
        m_aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection() {
        m_projection = Matrix::CreateOrthographic(
            m_orthographicSize * m_aspectRatio,
            m_orthographicSize,
            m_orthographicNear,
            m_orthographicFar);
    }

} // namespace Zero