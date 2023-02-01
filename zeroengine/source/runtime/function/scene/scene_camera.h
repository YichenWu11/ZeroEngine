#pragma once

#include "runtime/function/render/camera_system/camera.h"

namespace Zero {
    class SceneCamera : public Camera {
    public:
        SceneCamera(const DirectX::SimpleMath::Matrix& _projection = DirectX::SimpleMath::Matrix::Identity);
        ~SceneCamera() = default;

        void setOrthographic(float size, float near_clip, float far_clip);
        void setViewportSize(uint32_t width, uint32_t height);

        float getOrthographicSize() const { return m_orthographicSize; }
        void  setOrthographicSize(float size) {
             m_orthographicSize = size;
             recalculateProjection();
        }

    private:
        void recalculateProjection();

    private:
        float m_orthographicSize = 10.0f;
        float m_orthographicNear = -1.0f;
        float m_orthographicFar  = 1.0f;

        float m_aspectRatio = 16.0f / 9.0f;
    };
} // namespace Zero