#include <CDX12/Math/Quaternion.h>

#include "runtime/function/input/input_system.h"
#include "runtime/function/render/camera_system/editor_camera.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace Zero {
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
        Camera(XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspectRatio, nearClip, farClip)) {
        setLens(fov, aspectRatio, nearClip, farClip);
        updateView();
    }

    void EditorCamera::updateProjection() {
        mAspect      = m_ViewportWidth / m_ViewportHeight;
        m_projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(mFovY), mAspect, mNearZ, mFarZ);
    }

    void EditorCamera::updateView() {
        // NOTE: Lock the camera's rotation
        if (mViewDirty) {
            XMVECTOR R = XMLoadFloat3(&mRight);
            XMVECTOR U = XMLoadFloat3(&mUp);
            XMVECTOR L = XMLoadFloat3(&mLook);
            XMVECTOR P = XMLoadFloat3(&mPosition);

            // Keep camera's axes orthogonal to each other and of unit length.
            L = XMVector3Normalize(L);
            U = XMVector3Normalize(XMVector3Cross(L, R));

            // U, L already ortho-normal, so no need to normalize cross product.
            R = XMVector3Cross(U, L);

            // Fill in the view matrix entries.
            float x = -XMVectorGetX(XMVector3Dot(P, R));
            float y = -XMVectorGetX(XMVector3Dot(P, U));
            float z = -XMVectorGetX(XMVector3Dot(P, L));

            XMStoreFloat3(&mRight, R);
            XMStoreFloat3(&mUp, U);
            XMStoreFloat3(&mLook, L);

            mView(0, 0) = mRight.x;
            mView(1, 0) = mRight.y;
            mView(2, 0) = mRight.z;
            mView(3, 0) = x;

            mView(0, 1) = mUp.x;
            mView(1, 1) = mUp.y;
            mView(2, 1) = mUp.z;
            mView(3, 1) = y;

            mView(0, 2) = mLook.x;
            mView(1, 2) = mLook.y;
            mView(2, 2) = mLook.z;
            mView(3, 2) = z;

            mView(0, 3) = 0.0f;
            mView(1, 3) = 0.0f;
            mView(2, 3) = 0.0f;
            mView(3, 3) = 1.0f;

            mViewDirty = false;
        }
    }

    void EditorCamera::onUpdate(TimeStep ts) {
        const Vector2& mouse{InputSystem::getMouseX(), InputSystem::getMouseY()};
        Vector2        delta   = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;

        if (InputSystem::isMouseButtonPressed(VK_RBUTTON)) {
            if (InputSystem::isKeyPressed('W'))
                walk(5.0f * ts);

            if (InputSystem::isKeyPressed('S'))
                walk(-5.0f * ts);

            if (InputSystem::isKeyPressed('A'))
                strafe(-5.0f * ts);

            if (InputSystem::isKeyPressed('D'))
                strafe(5.0f * ts);

            if (InputSystem::isMouseButtonPressed(VK_RBUTTON)) {
                pitch(delta.y);
                rotateY(delta.x);
            }
        }

        updateView();
    }

    void EditorCamera::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(ZE_BIND_EVENT_FN(EditorCamera::onMouseScroll));
    }

    bool EditorCamera::onMouseScroll(MouseScrolledEvent& e) {
        return false;
    }

    void EditorCamera::setPosition(const XMFLOAT3& v) {
        mPosition  = v;
        mViewDirty = true;
    }

    float EditorCamera::getFovX() const {
        float halfWidth = 0.5f * getNearWindowWidth();
        return 2.0f * atan(halfWidth / mNearZ);
    }

    void EditorCamera::setLens(float fovY, float aspect, float zn, float zf) {
        // cache properties
        mFovY   = fovY;
        mAspect = aspect;
        mNearZ  = zn;
        mFarZ   = zf;

        mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
        mFarWindowHeight  = 2.0f * mFarZ * tanf(0.5f * mFovY);

        updateProjection();
    }

    void EditorCamera::lookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp) {
        XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
        XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
        XMVECTOR U = XMVector3Cross(L, R);

        XMStoreFloat3(&mPosition, pos);
        XMStoreFloat3(&mLook, L);
        XMStoreFloat3(&mRight, R);
        XMStoreFloat3(&mUp, U);

        mViewDirty = true;
    }

    void EditorCamera::lookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up) {
        XMVECTOR P = XMLoadFloat3(&pos);
        XMVECTOR T = XMLoadFloat3(&target);
        XMVECTOR U = XMLoadFloat3(&up);

        lookAt(P, T, U);

        mViewDirty = true;
    }

    void EditorCamera::strafe(float d) {
        // mPosition += d*mRight
        XMVECTOR s = XMVectorReplicate(d);
        XMVECTOR r = XMLoadFloat3(&mRight);
        XMVECTOR p = XMLoadFloat3(&mPosition);
        XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));

        mViewDirty = true;
    }

    void EditorCamera::walk(float d) {
        // mPosition += d*mLook
        XMVECTOR s = XMVectorReplicate(d);
        XMVECTOR l = XMLoadFloat3(&mLook);
        XMVECTOR p = XMLoadFloat3(&mPosition);
        XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));

        mViewDirty = true;
    }

    void EditorCamera::pitch(float angle) {
        // Rotate up and look vector about the right vector.

        XMMATRIX R = XMMatrixRotationQuaternion(Chen::CDX12::Math::Quaternion(XMLoadFloat3(&mRight), angle));

        XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
        XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

        mViewDirty = true;
    }

    void EditorCamera::rotateY(float angle) {
        // Rotate the basis vectors about the world y-axis.

        XMMATRIX R = XMMatrixRotationQuaternion(Chen::CDX12::Math::Quaternion(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), angle));

        XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
        XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
        XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

        mViewDirty = true;
    }
} // namespace Zero