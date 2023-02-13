#pragma once

#include <CDX12/Math/MathHelper.h>

#include "runtime/core/util/time_step.h"
#include "runtime/function/event/event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/render/camera_system/camera.h"

namespace Zero {
    class EditorCamera : public Camera {
    public:
        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void onUpdate(TimeStep timestep);
        void onEvent(Event& event);

        void setViewportSize(float width, float height) {
            m_viewportWidth  = width;
            m_viewportHeight = height;
            updateProjection();
        }

        void setPosition(const DirectX::XMFLOAT3& v) {
            m_position  = v;
            m_viewDirty = true;
        }

        DirectX::SimpleMath::Matrix getView() const { return m_view; }
        DirectX::SimpleMath::Matrix getViewProjection() const { return m_view * m_projection; }

        DirectX::SimpleMath::Vector3 getPosition() const { return m_position; }

        DirectX::SimpleMath::Vector3 getRight() const { return m_right; }
        DirectX::SimpleMath::Vector3 getUp() const { return m_up; }
        DirectX::SimpleMath::Vector3 getLook() const { return m_look; }

        float getNearZ() const { return m_nearZ; }
        float getFarZ() const { return m_farZ; }
        float getAspect() const { return m_aspect; }
        float getFovY() const { return m_fovY; }
        float getFovX() const { return 2.0f * atan(0.5f * getNearWindowWidth() / m_nearZ); }

        float getNearWindowWidth() const { return m_aspect * m_nearWindowHeight; }
        float getNearWindowHeight() const { return m_nearWindowHeight; }
        float getFarWindowWidth() const { return m_aspect * m_farWindowHeight; }
        float getFarWindowHeight() const { return m_farWindowHeight; }

    private:
        void setViewDirty() { m_viewDirty = true; }
        void setLens(float fovY, float aspect, float zn, float zf);

        void strafe(float d);
        void walk(float d);

        void pitch(float angle);
        void rotateY(float angle);

        void updateProjection();
        void updateView();

        bool onMouseScroll(MouseScrolledEvent& event);

    private:
        DirectX::XMFLOAT3 m_position = {0.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 m_right    = {1.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 m_up       = {0.0f, 1.0f, 0.0f};
        DirectX::XMFLOAT3 m_look     = {0.0f, 0.0f, 1.0f};

        float m_nearZ            = 0.1f;
        float m_farZ             = 1000.0f;
        float m_aspect           = 1.778f;
        float m_fovY             = 45.0f;
        float m_nearWindowHeight = 0.0f;
        float m_farWindowHeight  = 0.0f;

        float m_zoom_level = 1.0f; // for ortho

        bool m_viewDirty = true;

        DirectX::XMFLOAT4X4 m_view = Chen::CDX12::Math::MathHelper::Identity4x4();

        float                        m_viewportWidth = 1280, m_viewportHeight = 720;
        DirectX::SimpleMath::Vector2 m_initialMousePosition = {0.0f, 0.0f};
    };
} // namespace Zero