#include "runtime/function/render/camera_system/orthographics_camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Zero {
    // XMMatrixOrthographicLH
    OrthographicsCamera::OrthographicsCamera(float left, float right, float bottom, float top) :
        m_projection_matrix(XMMatrixOrthographicLH(right - left, top - bottom, -1.0f, 1.0f)) {
        m_view_proj_matrix = m_view_matrix * m_projection_matrix;
    }

    void OrthographicsCamera::recalculateViewMatrix() {
        Matrix transform = Matrix::CreateTranslation(m_position);
        Matrix rotation  = Matrix::CreateRotationZ(XMConvertToRadians(m_rotation));

        m_view_matrix      = (transform * rotation).Invert();
        m_view_proj_matrix = m_view_matrix * m_projection_matrix;
    }

    void OrthographicsCamera::setProjection(float left, float right, float bottom, float top) {
        m_projection_matrix = XMMatrixOrthographicLH(right - left, top - bottom, -1.0f, 1.0f);
        m_view_proj_matrix  = m_view_matrix * m_projection_matrix;
    }
} // namespace Zero