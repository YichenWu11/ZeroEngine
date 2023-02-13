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
            m_ViewportWidth  = width;
            m_ViewportHeight = height;
            updateProjection();
        }

        DirectX::SimpleMath::Matrix getView() const { return mView; }
        DirectX::SimpleMath::Matrix getViewProjection() const { return mView * m_projection; }

        DirectX::SimpleMath::Vector3 getPosition() const { return mPosition; }
        void                         setPosition(const DirectX::XMFLOAT3& v);

        DirectX::SimpleMath::Vector3 getRight() const { return mRight; }
        DirectX::SimpleMath::Vector3 getUp() const { return mUp; }
        DirectX::SimpleMath::Vector3 getLook() const { return mLook; }

        float getNearZ() const { return mNearZ; }
        float getFarZ() const { return mFarZ; }
        float getAspect() const { return mAspect; }
        float getFovY() const { return mFovY; }
        float getFovX() const;

        float getNearWindowWidth() const { return mAspect * mNearWindowHeight; }
        float getNearWindowHeight() const { return mNearWindowHeight; }
        float getFarWindowWidth() const { return mAspect * mFarWindowHeight; }
        float getFarWindowHeight() const { return mFarWindowHeight; }

    private:
        void updateProjection();
        void updateView();

        bool onMouseScroll(MouseScrolledEvent& event);

        // Set frustum.
        void setLens(float fovY, float aspect, float zn, float zf);

        void lookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
        void lookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

        // Strafe/Walk the camera a distance d.
        void strafe(float d);
        void walk(float d);

        // Rotate the camera.
        void pitch(float angle);
        void rotateY(float angle);

    private:
        // Camera coordinate system with coordinates relative to world space.
        DirectX::XMFLOAT3 mPosition = {0.0f, 0.0f, -2.0f};
        DirectX::XMFLOAT3 mRight    = {1.0f, 0.0f, 0.0f};
        DirectX::XMFLOAT3 mUp       = {0.0f, 1.0f, 0.0f};
        DirectX::XMFLOAT3 mLook     = {0.0f, 0.0f, 1.0f};

        // Cache frustum properties.
        float mNearZ            = 0.1f;
        float mFarZ             = 1000.0f;
        float mAspect           = 1.778f;
        float mFovY             = 45.0f;
        float mNearWindowHeight = 0.0f;
        float mFarWindowHeight  = 0.0f;

        bool mViewDirty = true;

        // Cache View/Proj matrices.
        DirectX::XMFLOAT4X4 mView = Chen::CDX12::Math::MathHelper::Identity4x4();

        float                        m_ViewportWidth = 1280, m_ViewportHeight = 720;
        DirectX::SimpleMath::Vector2 m_InitialMousePosition = {0.0f, 0.0f};
    };
} // namespace Zero