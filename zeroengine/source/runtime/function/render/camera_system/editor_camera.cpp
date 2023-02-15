#include <CDX12/Math/Quaternion.h>

#include "runtime/function/input/input_system.h"
#include "runtime/function/render/camera_system/editor_camera.h"
#include "runtime/function/render/render_system/renderer_api.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace Chen::CDX12;

namespace Zero {
    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip) :
        Camera(XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), aspectRatio, nearClip, farClip)) {
        setLens(fov, aspectRatio, nearClip, farClip);
        updateView();
    }

    void EditorCamera::onUpdate(TimeStep ts) {
        const Vector2& mouse{InputSystem::getMouseX(), InputSystem::getMouseY()};
        Vector2        delta   = (mouse - m_initialMousePosition) * 0.003f;
        m_initialMousePosition = mouse;

        if (RendererAPI::is2D()) {
            // 2d
            if (InputSystem::isMouseButtonPressed(VK_MBUTTON)) {
                m_position.x += m_zoom_level * -delta.x;
                m_position.y += m_zoom_level * delta.y;
            }

            setViewDirty();
        }
        else {
            // 3d
            if (InputSystem::isMouseButtonPressed(VK_RBUTTON)) {
                if (InputSystem::isKeyPressed('W'))
                    walk(5.0f * ts);

                if (InputSystem::isKeyPressed('S'))
                    walk(-5.0f * ts);

                if (InputSystem::isKeyPressed('A'))
                    strafe(-5.0f * ts);

                if (InputSystem::isKeyPressed('D'))
                    strafe(5.0f * ts);

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
        if (RendererAPI::is2D()) {
            m_zoom_level -= e.getZOffset() * 0.001f;
            m_zoom_level = std::max(m_zoom_level, 0.05f);
            updateProjection();
        }

        return false;
    }

    void EditorCamera::setLens(float fovY, float aspect, float zn, float zf) {
        // cache properties
        m_fovY   = fovY;
        m_aspect = aspect;
        m_nearZ  = zn;
        m_farZ   = zf;

        m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
        m_farWindowHeight  = 2.0f * m_farZ * tanf(0.5f * m_fovY);

        updateProjection();
    }

    void EditorCamera::strafe(float d) {
        // m_position += d*m_right
        XMVECTOR s = XMVectorReplicate(d);
        XMVECTOR r = XMLoadFloat3(&m_right);
        XMVECTOR p = XMLoadFloat3(&m_position);
        XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, r, p));

        m_viewDirty = true;
    }

    void EditorCamera::walk(float d) {
        // m_position += d*m_look
        XMVECTOR s = XMVectorReplicate(d);
        XMVECTOR l = XMLoadFloat3(&m_look);
        XMVECTOR p = XMLoadFloat3(&m_position);
        XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));

        m_viewDirty = true;
    }

    void EditorCamera::pitch(float angle) {
        // Rotate up and look vector about the right vector.

        XMMATRIX R = XMMatrixRotationQuaternion(Math::Quaternion(XMLoadFloat3(&m_right), angle));

        XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
        XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));

        m_viewDirty = true;
    }

    void EditorCamera::rotateY(float angle) {
        // Rotate the basis vectors about the world y-axis.

        XMMATRIX R = XMMatrixRotationQuaternion(Math::Quaternion(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), angle));

        XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
        XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
        XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));

        m_viewDirty = true;
    }

    void EditorCamera::updateProjection() {
        m_aspect = m_viewportWidth / m_viewportHeight;
        if (RendererAPI::is2D())
            m_projection = XMMatrixOrthographicLH(2.0f * m_zoom_level * m_aspect, 2.0f * m_zoom_level, -1.0f, 1.0f);
        else
            m_projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fovY), m_aspect, m_nearZ, m_farZ);
    }

    void EditorCamera::updateView() {
        if (m_viewDirty) {
            XMVECTOR R = XMLoadFloat3(&m_right);
            XMVECTOR U = XMLoadFloat3(&m_up);
            XMVECTOR L = XMLoadFloat3(&m_look);
            XMVECTOR P = XMLoadFloat3(&m_position);

            // Keep camera's axes orthogonal to each other and of unit length.
            L = XMVector3Normalize(L);
            U = XMVector3Normalize(XMVector3Cross(L, R));

            // U, L already ortho-normal, so no need to normalize cross product.
            R = XMVector3Cross(U, L);

            // Fill in the view matrix entries.
            float x = -XMVectorGetX(XMVector3Dot(P, R));
            float y = -XMVectorGetX(XMVector3Dot(P, U));
            float z = -XMVectorGetX(XMVector3Dot(P, L));

            XMStoreFloat3(&m_right, R);
            XMStoreFloat3(&m_up, U);
            XMStoreFloat3(&m_look, L);

            m_view(0, 0) = m_right.x;
            m_view(1, 0) = m_right.y;
            m_view(2, 0) = m_right.z;
            m_view(3, 0) = x;

            m_view(0, 1) = m_up.x;
            m_view(1, 1) = m_up.y;
            m_view(2, 1) = m_up.z;
            m_view(3, 1) = y;

            m_view(0, 2) = m_look.x;
            m_view(1, 2) = m_look.y;
            m_view(2, 2) = m_look.z;
            m_view(3, 2) = z;

            m_view(0, 3) = 0.0f;
            m_view(1, 3) = 0.0f;
            m_view(2, 3) = 0.0f;
            m_view(3, 3) = 1.0f;

            m_viewDirty = false;
        }
    }
} // namespace Zero