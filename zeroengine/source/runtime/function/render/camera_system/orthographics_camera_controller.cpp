#include "runtime/function/render/camera_system/orthographics_camera_controller.h"
#include "runtime/function/input/input_system.h"

namespace Zero {
    OrthographicsCameraController::OrthographicsCameraController(float aspect_ratio, bool rotation_enabled) :
        m_aspect_ratio(aspect_ratio),
        m_bounds{-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level},
        m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
        m_rotation_enabled(rotation_enabled) {}

    void OrthographicsCameraController::onUpdate(TimeStep timestep) {
        if (InputSystem::isKeyPressed('A'))
            m_camera_position.x -= m_camera_move_speed * timestep;
        else if (InputSystem::isKeyPressed('D'))
            m_camera_position.x += m_camera_move_speed * timestep;

        if (InputSystem::isKeyPressed('S'))
            m_camera_position.y -= m_camera_move_speed * timestep;
        else if (InputSystem::isKeyPressed('W'))
            m_camera_position.y += m_camera_move_speed * timestep;

        if (m_rotation_enabled) {
            if (InputSystem::isKeyPressed('Q'))
                m_camera_rotation += m_camera_rotate_speed * timestep;
            else if (InputSystem::isKeyPressed('E'))
                m_camera_rotation -= m_camera_rotate_speed * timestep;
            m_camera.setRotation(m_camera_rotation);
        }

        m_camera.setPosition(m_camera_position);
        m_camera_move_speed = m_zoom_level;
    }

    void OrthographicsCameraController::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicsCameraController::onMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicsCameraController::onWindowResize));
    }

    void OrthographicsCameraController::onResize(float width, float height) {
        m_aspect_ratio = width / height;
        calculateView();
    }

    void OrthographicsCameraController::calculateView() {
        m_bounds = {-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level, m_zoom_level};
        m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
    }

    bool OrthographicsCameraController::onMouseScrolled(MouseScrolledEvent& e) {
        m_zoom_level -= e.getZOffset() * 0.001f;
        m_zoom_level = std::max(m_zoom_level, 0.05f);
        calculateView();
        return false;
    }

    bool OrthographicsCameraController::onWindowResize(WindowResizeEvent& e) {
        onResize((float)e.getWidth(), (float)e.getHeight());
        return false;
    }

} // namespace Zero