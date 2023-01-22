#pragma once

#include <imgui.h>

#include "runtime/zero.h"

using namespace DirectX::SimpleMath;

class ExampleLayer : public Zero::Layer {
public:
    ExampleLayer() :
        Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f) {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        if (Zero::InputSystem::isKeyPressed(VK_LEFT))
            m_camera_position.x -= m_camera_move_speed * timestep;
        else if (Zero::InputSystem::isKeyPressed(VK_RIGHT))
            m_camera_position.x += m_camera_move_speed * timestep;

        if (Zero::InputSystem::isKeyPressed(VK_DOWN))
            m_camera_position.y -= m_camera_move_speed * timestep;
        else if (Zero::InputSystem::isKeyPressed(VK_UP))
            m_camera_position.y += m_camera_move_speed * timestep;

        if (Zero::InputSystem::isKeyPressed('A'))
            m_camera_rotation += m_camera_rotate_speed * timestep;
        else if (Zero::InputSystem::isKeyPressed('D'))
            m_camera_rotation -= m_camera_rotate_speed * timestep;

        Zero::RenderCommand::setClearColor({0.2f, 0.2f, 0.2f, 1.0f});
        Zero::RenderCommand::clear();

        m_camera.setPosition(m_camera_position);
        m_camera.setRotation(m_camera_rotation);

        Zero::Renderer::beginScene(m_camera);

        Zero::Renderer::submit(Zero::MeshTable::getMesh("triangle"), tri_trans_0);
        Zero::Renderer::submit(Zero::MeshTable::getMesh("triangle"), {0.0f, 0.0f, 0.0f});
        Zero::Renderer::submit(Zero::MeshTable::getMesh("triangle"), {0.8f, 0.3f, 0.0f});

        Zero::Renderer::endScene();
    }

    void onImGuiRender() override {
        ImGui::Begin("DEBUG");

        ImGui::DragFloat("TRI0_X", &(tri_trans_0.x), 0.01f,
                         -2.0f, 2.0f, "%.2f");
        ImGui::DragFloat("TRI0_Y", &(tri_trans_0.y), 0.01f,
                         -2.0f, 2.0f, "%.2f");

        ImGui::End();
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

    // per second
    float m_camera_move_speed   = 4.0f;
    float m_camera_rotate_speed = 50.0f;

    Vector3 tri_trans_0{-0.8f, -0.3f, 0.0f};
    Vector4 tri_color_0{1.0f, 1.0f, 1.0f, 1.0f};
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
