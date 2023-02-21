#pragma once

#include "runtime/zero.h"

class ExampleLayer : public Zero::Layer {
public:
    ExampleLayer() :
        Layer("Example"), m_camera_controller(1280.0f / 720.0f) {
    }

    void onUpdate(Zero::TimeStep timestep) override {
        // update
        m_camera_controller.onUpdate(timestep);

        // render
        Zero::RenderCommand::setClearColor(clear_color);
        Zero::RenderCommand::clear();

        Zero::Renderer::beginScene(m_camera_controller.getCamera());

        Zero::Renderer::submit(GET_MESH_POOL().getMesh("square").get());
        Zero::Renderer::submit(GET_MESH_POOL().getMesh("square").get());
        Zero::Renderer::submit(GET_MESH_POOL().getMesh("square").get());

        Zero::Renderer::endScene();
    }

    void onImGuiRender() override {
        {
            ImGui::Begin("DEBUG");
            ImGui::ColorEdit4("CLEAR_COLOR", reinterpret_cast<float*>(&clear_color));
            ImGui::End();
        }

        {
            ImGui::Begin("IMAGE");
            ImGui::Image(GET_TEXTURE_POOL().getImTextureID(0), ImVec2(190, 190));
            ImGui::End();
        }
    }

    void onEvent(Zero::Event& event) override {
        m_camera_controller.onEvent(event);

        Zero::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Zero::KeyPressedEvent>(ZE_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));
    }

    bool onKeyPressedEvent(Zero::KeyPressedEvent& event) {
        return false;
    }

private:
    Zero::OrthographicsCameraController m_camera_controller;

    Color clear_color{0.2f, 0.2f, 0.2f, 1.0f};
};