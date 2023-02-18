#pragma once

#include "runtime/function/render/camera_system/camera.h"

namespace Zero {
    class SceneCamera : public Camera {
    public:
        enum class ProjectionType { Perspective  = 0,
                                    Orthographic = 1 };

    public:
        SceneCamera(const DirectX::SimpleMath::Matrix& _projection = DirectX::SimpleMath::Matrix::Identity);
        ~SceneCamera() = default;

        void setOrthographic(float size, float near_clip, float far_clip);
        void setPerspective(float v_fov, float near_clip, float far_clip);

        void setViewportSize(uint32_t width, uint32_t height);

        // perspective
        float getPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
        void  setPerspectiveVerticalFOV(float verticalFov) {
             m_perspectiveFOV = verticalFov;
             recalculateProjection();
        }
        float getPerspectiveNearClip() const { return m_perspectiveNear; }
        void  setPerspectiveNearClip(float near_clip) {
             m_perspectiveNear = near_clip;
             recalculateProjection();
        }
        float getPerspectiveFarClip() const { return m_perspectiveFar; }
        void  setPerspectiveFarClip(float far_clip) {
             m_perspectiveFar = far_clip;
             recalculateProjection();
        }

        // orthographic
        float getOrthographicSize() const { return m_orthographicSize; }
        void  setOrthographicSize(float size) {
             m_orthographicSize = size;
             recalculateProjection();
        }
        float getOrthographicNearClip() const { return m_orthographicNear; }
        void  setOrthographicNearClip(float near_clip) {
             m_orthographicNear = near_clip;
             recalculateProjection();
        }
        float getOrthographicFarClip() const { return m_orthographicFar; }
        void  setOrthographicFarClip(float far_clip) {
             m_orthographicFar = far_clip;
             recalculateProjection();
        }

        ProjectionType getProjectionType() const { return m_projectionType; }
        void           setProjectionType(ProjectionType type) {
                      m_projectionType = type;
                      recalculateProjection();
        }

    private:
        void recalculateProjection();

    private:
        ProjectionType m_projectionType = ProjectionType::Orthographic;

        // perspective
        float m_perspectiveFOV  = DirectX::XMConvertToRadians(45.0f);
        float m_perspectiveNear = 0.01f;
        float m_perspectiveFar  = 1000.0f;

        // orthographic
        float m_orthographicSize = 10.0f;
        float m_orthographicNear = -1.0f;
        float m_orthographicFar  = 1.0f;

        float m_aspectRatio = 16.0f / 9.0f;
    };
} // namespace Zero