#include "editor_layer.h"

namespace Zero {
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"), m_camera_controller(1280.0f / 720.0f) {}

    void EditorLayer::onAttach() {
        GET_TEXTURE_TABLE().registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png");
        GET_TEXTURE_TABLE().registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul.png");
        GET_TEXTURE_TABLE().registerTex(
            "SpriteSheet",
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/game/texture/sprite_sheet.png");

        m_texture_stair = SubTexture2D::createFromCoords(
            GET_TEXTURE_TABLE().getTextureFromName("SpriteSheet"),
            {7, 6},
            {128.0f, 128.0f});

        m_texture_bush = SubTexture2D::createFromCoords(
            GET_TEXTURE_TABLE().getTextureFromName("SpriteSheet"),
            {2, 3},
            {256.0f, 128.0f});

        m_camera_controller.setZoomLevel(1.0f);
    }

    void EditorLayer::onDetach() {
    }

    void EditorLayer::onUpdate(TimeStep timestep) {
        // update
        {
            ZE_PROFILE_SCOPE("CameraController::onUpdate");
            if (m_viewport_focused)
                m_camera_controller.onUpdate(timestep);
        }

        // render
        // {
        //     RenderCommand::setClearColor(clear_color);
        //     RenderCommand::clear();
        // }

        {
            ZE_PROFILE_SCOPE("Renderer2D::Render");

            // Renderer2D::drawCellQuad(
            //     {0.0f, 1.0f, 0.3f},
            //     {1.0f, 1.0f},
            //     0.0f,
            //     m_texture_stair);

            // Renderer2D::drawCellQuad(
            //     {0.0f, -1.0f, 0.2f},
            //     {2.0f, 1.0f},
            //     0.0f,
            //     m_texture_bush);

            Renderer2D::beginScene(m_camera_controller.getCamera());

            Renderer2D::drawQuad(
                {-0.8f, 0.0f},
                {1.0f, 1.0f},
                0.0f,
                {1.0f, 1.0f, 1.0f, 1.0f},
                GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

            Renderer2D::drawQuad(
                position,
                {1.0f, 1.0f},
                0.0f,
                {1.0f, 1.0f, 1.0f, 1.0f},
                GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

            Renderer2D::drawQuad(
                {0.8f, 0.0f},
                {1.0f, 1.0f},
                0.0f,
                {1.0f, 1.0f, 1.0f, 1.0f},
                GET_TEXTURE_TABLE().getTexIndexFromName("bella"));

            Renderer2D::drawQuad(
                {0.0f, 0.0f, 0.2f},
                {10.0f, 10.0f},
                0.0f,
                {0.8f, 0.8f, 0.8f, 1.0f},
                GET_TEXTURE_TABLE().getTexIndexFromName("asoul"), 5.0f);

            for (float y = -5.0f; y < 5.0f; y += 0.5f) {
                for (float x = -5.0f; x < 5.0f; x += 0.5f) {
                    Renderer2D::drawQuad(
                        {x, y, 0.1f},
                        {0.45f, 0.45f},
                        0.0f,
                        {(x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.8f});
                }
            }

            Renderer2D::endScene();
        }
    }

    void EditorLayer::onImGuiRender() {
        ZE_PROFILE_FUNCTION();

        if (m_dockspace_enable) {
            static auto tex_alloc = GET_TEXTURE_TABLE().getTexAllocation();

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
                        Application::get().close();
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

            // Viewer
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("VIEWER");
            m_viewport_focused = ImGui::IsWindowFocused();
            m_viewport_hovered = ImGui::IsWindowHovered();
            Application::get().getImGuiLayer()->blockEvents(!m_viewport_focused || !m_viewport_hovered);

            ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
            if (m_viewport_size != *((Vector2*)&viewport_panel_size)) {
                Renderer::resizeFrameBuffer((uint32_t)viewport_panel_size.x, (uint32_t)viewport_panel_size.y);
                m_viewport_size = Vector2{viewport_panel_size.x, viewport_panel_size.y};
                m_camera_controller.onResize(viewport_panel_size.x, viewport_panel_size.y);
            }

            ImGui::Image(
                Renderer::getOffScreenID(),
                ImVec2{m_viewport_size.x, m_viewport_size.y});
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::ShowDemoWindow();

            ZE_PROFILE_RENDER();

            ImGui::End();
        }
    }

    void EditorLayer::onEvent(Event& event) {
        m_camera_controller.onEvent(event);
    }

} // namespace Zero