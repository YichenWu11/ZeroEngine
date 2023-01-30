#include "sandbox/sandbox_2d.h"

// Sandbox2Dcpp11

Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f) {}

void Sandbox2D::onAttach() {
    GET_TEXTURE_TABLE().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png");
    GET_TEXTURE_TABLE().registerTex(
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul.png");
    GET_TEXTURE_TABLE().registerTex(
        "SpriteSheet",
        std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/game/texture/sprite_sheet.png");

    m_texture_stair = Zero::SubTexture2D::createFromCoords(
        GET_TEXTURE_TABLE().getTextureFromName("SpriteSheet"),
        {7, 6},
        {128.0f, 128.0f});

    m_texture_bush = Zero::SubTexture2D::createFromCoords(
        GET_TEXTURE_TABLE().getTextureFromName("SpriteSheet"),
        {2, 3},
        {256.0f, 128.0f});

    m_camera_controller.setZoomLevel(1.0f);
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
    // {
    //     Zero::RenderCommand::setClearColor(clear_color);
    //     Zero::RenderCommand::clear();
    // }

    {
        ZE_PROFILE_SCOPE("Renderer2D::Render");
        Zero::Renderer2D::beginScene(m_camera_controller.getCamera());

        Zero::Renderer2D::drawQuad(
            {-0.8f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

        Zero::Renderer2D::drawQuad(
            position,
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

        Zero::Renderer2D::drawQuad(
            {0.8f, 0.0f},
            {1.0f, 1.0f},
            0.0f,
            {1.0f, 1.0f, 1.0f, 1.0f},
            GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

        // Zero::Renderer2D::drawCellQuad(
        //     {0.0f, 1.0f, 0.3f},
        //     {1.0f, 1.0f},
        //     0.0f,
        //     m_texture_stair);

        // Zero::Renderer2D::drawCellQuad(
        //     {0.0f, -1.0f, 0.2f},
        //     {2.0f, 1.0f},
        //     0.0f,
        //     m_texture_bush);

        Zero::Renderer2D::drawQuad(
            {0.0f, 0.0f, 0.2f},
            {10.0f, 10.0f},
            0.0f,
            {0.8f, 0.8f, 0.8f, 1.0f},
            GET_TEXTURE_TABLE().getTexIndexFromName("asoul"), 5.0f);

        for (float y = -5.0f; y < 5.0f; y += 0.5f) {
            for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                Zero::Renderer2D::drawQuad(
                    {x, y, 0.1f},
                    {0.45f, 0.45f},
                    0.0f,
                    {(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.8f});
            }
        }

        Zero::Renderer2D::endScene();
    }
}

void Sandbox2D::onImGuiRender() {
    ZE_PROFILE_FUNCTION();

    static auto tex_alloc = GET_TEXTURE_TABLE().getTexAllocation();

    // Note: Switch this to true to enable dockspace
    static bool dockingEnabled = true;
    if (dockingEnabled) {
        static bool               dockspaceOpen             = true;
        static bool               opt_fullscreen_persistant = true;
        bool                      opt_fullscreen            = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags           = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Exit")) {
                    Zero::Application::get().close();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("ASOUL");
        ImGui::Image(
            ImTextureID(tex_alloc->GetGpuHandle(2).ptr),
            ImVec2(190, 190));
        ImGui::SameLine();
        ImGui::DragFloat2("POSITION", reinterpret_cast<float*>(&position), 0.05f);
        ImGui::End();

        ImGui::Begin("IMAGE");
        ImGui::Image(
            Zero::Renderer::getOffScreenID(),
            ImVec2(1280.0f, 720.0f));
        ImGui::End();

        // ImGui::Begin("IMAGE");
        // ImGui::Image(ImTextureID(tex_alloc->GetGpuHandle(1).ptr), ImVec2(1280.0f, 720.0f));
        // ImGui::End();

        ImGui::ShowDemoWindow();

        ImGui::End();
    }
    else {
    }

    ZE_PROFILE_RENDER();
}

void Sandbox2D::onEvent(Zero::Event& event) {
    m_camera_controller.onEvent(event);
}
