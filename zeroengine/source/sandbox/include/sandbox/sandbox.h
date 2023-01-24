#pragma once

#include "runtime/zero.h"

using namespace DirectX::SimpleMath;

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

        Zero::Renderer::submit(Zero::MeshTable::getInstance().getMesh("square"), tri_trans_0);
        Zero::Renderer::submit(Zero::MeshTable::getInstance().getMesh("square"), {0.0f, 0.0f, 0.0f});
        Zero::Renderer::submit(Zero::MeshTable::getInstance().getMesh("square"), {0.8f, 0.3f, 0.0f});

        Zero::Renderer::endScene();
    }

    void onImGuiRender() override {
        {
            ImGui::Begin("DEBUG");

            ImGui::DragFloat("TRI0_X", &(tri_trans_0.x), 0.01f,
                             -2.0f, 2.0f, "%.2f");
            ImGui::DragFloat("TRI0_Y", &(tri_trans_0.y), 0.01f,
                             -2.0f, 2.0f, "%.2f");

            ImGui::ColorEdit4("CLEAR_COLOR", reinterpret_cast<float*>(&clear_color));

            ImGui::End();
        }

        {
            ImGui::Begin("IMAGE");

            static auto tex_alloc = Zero::TextureTable::getInstance().getTexAllocation();

            ImGui::Image(ImTextureID(tex_alloc->GetGpuHandle(0).ptr), ImVec2(190, 190));

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

    Vector3 tri_trans_0{-0.8f, -0.3f, 0.0f};
    Color   clear_color{0.2f, 0.2f, 0.2f, 1.0f};
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
