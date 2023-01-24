#pragma once

#include "runtime/core/util/time_step.h"
#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/render/camera_system/orthographics_camera.h"

namespace Zero {
    class OrthographicsCameraController {
    public:
        OrthographicsCameraController(float aspect_ratio, bool rotation_enabled = false);

        void onUpdate(TimeStep timestep);
        void onEvent(Event& e);

        OrthographicsCamera&       getCamera() { return m_camera; }
        const OrthographicsCamera& getCamera() const { return m_camera; }

        void  setZoomLevel(float level) { m_zoom_level = level; }
        float getZoomLevel() const { return m_zoom_level; }

    private:
        bool onMouseScrolled(MouseScrolledEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

    private:
        float               m_aspect_ratio;
        float               m_zoom_level = 1.0f;
        OrthographicsCamera m_camera;

        bool m_rotation_enabled;

        DirectX::SimpleMath::Vector3 m_camera_position{0.0f, 0.0f, 0.0f};
        float                        m_camera_rotation = 0.0f;

        // per second
        float m_camera_move_speed   = 5.0f;
        float m_camera_rotate_speed = 180.0f;
    };
} // namespace Zero