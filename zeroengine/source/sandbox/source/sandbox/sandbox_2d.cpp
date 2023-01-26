#include "sandbox/sandbox_2d.h"

// Sandbox2D

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
    Zero::TextureTable::getInstance().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png");
    Zero::TextureTable::getInstance().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul.png");
}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(Zero::TimeStep timestep) {
    // update
    {
        ZE_PROFILE_SCOPE("CameraController::onUpdate");
        m_camera_controller.onUpdate(timestep);
    }

    // render
    {
        Zero::RenderCommand::setClearColor(clear_color);
        Zero::RenderCommand::clear();
    }

    {
        ZE_PROFILE_SCOPE("Render");
        Zero::Renderer2D::beginScene(m_camera_controller.getCamera());

        Zero::Renderer2D::drawQuad(
            {-0.8f, -0.3f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            Zero::TextureTable::getInstance().getTexIndexFromName("bella"));
        Zero::Renderer2D::drawQuad(
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            Zero::TextureTable::getInstance().getTexIndexFromName("bella"));
        Zero::Renderer2D::drawQuad(
            {0.8f, 0.3f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            Zero::TextureTable::getInstance().getTexIndexFromName("bella"));

        Zero::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() {
    static auto tex_alloc = Zero::TextureTable::getInstance().getTexAllocation();

    // {
    //     ImGui::Begin("DEBUG");
    //     ImGui::ColorEdit4("CLEAR_COLOR", reinterpret_cast<float*>(&clear_color));
    //     ImGui::End();
    // }

    {
        ImGui::Begin("IMAGE");
        ImGui::Image(ImTextureID(tex_alloc->GetGpuHandle(2).ptr), ImVec2(190, 190));
        ImGui::End();
    }

    // ZE_PROFILE_RENDER();
}

void Sandbox2D::onEvent(Zero::Event& event) {
    m_camera_controller.onEvent(event);
}
