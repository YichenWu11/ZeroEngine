#pragma once

#include <imgui.h>

#include "runtime/zero.h"

using namespace DirectX::SimpleMath;

class ExampleLayer : public Zero::Layer {
public:
    ExampleLayer() :
        Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f) {
    }

    void onUpdate() override {
        if (Zero::InputSystem::isKeyPressed(VK_LEFT))
            m_camera_position.x -= m_camera_move_speed;
        else if (Zero::InputSystem::isKeyPressed(VK_RIGHT))
            m_camera_position.x += m_camera_move_speed;

        if (Zero::InputSystem::isKeyPressed(VK_DOWN))
            m_camera_position.y -= m_camera_move_speed;
        else if (Zero::InputSystem::isKeyPressed(VK_UP))
            m_camera_position.y += m_camera_move_speed;

        // FIXME: ortho camera rotation error
        if (Zero::InputSystem::isKeyPressed('A'))
            m_camera_rotation += m_camera_rotate_speed;
        else if (Zero::InputSystem::isKeyPressed('D'))
            m_camera_rotation -= m_camera_rotate_speed;

        Zero::RenderCommand::setClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        Zero::RenderCommand::clear();

        m_camera.setPosition(m_camera_position);
        m_camera.setRotation(m_camera_rotation);

        Zero::Renderer::beginScene(m_camera);
        Zero::Renderer::submit(Zero::MeshTable::getMesh("triangle"));
        Zero::Renderer::endScene();
    }

    void onImGuiRender() override {
    }

    void onEvent(Zero::Event& event) override {
        Zero::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Zero::KeyPressedEvent>(ZE_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));
    }

    bool onKeyPressedEvent(Zero::KeyPressedEvent& event) {
        return false;
    }

private:
    Zero::OrthographicsCamera m_camera;

    Vector3 m_camera_position;
    float   m_camera_rotation = 0.0f;

    float m_camera_move_speed   = 0.02f;
    float m_camera_rotate_speed = 1.0f;
};

class Sandbox : public Zero::Application {
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox() {
    }
};

Zero::Application* createApplication() {
    return new Sandbox();
}
