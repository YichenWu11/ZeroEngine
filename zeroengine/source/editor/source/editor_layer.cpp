#include <ImGuizmo.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/scene/scene_serializer.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "editor_layer.h"

namespace Zero {
    EditorLayer::EditorLayer() :
        Layer("EditorLayer") {}

    void EditorLayer::onAttach() {
        ZE_PROFILE_FUNCTION();

        m_active_scene  = CreateRef<Scene>();
        m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        m_scene_hierarchy_panel.setContext(m_active_scene);
    }

    void EditorLayer::onDetach() {
        ZE_PROFILE_FUNCTION();
    }

    void EditorLayer::onUpdate(TimeStep timestep) {
        ZE_PROFILE_FUNCTION();

        // resize
        {
            auto config = Renderer::getFrameBufferConfig();
            if (m_viewport_size.x > 0.0f
                && m_viewport_size.y > 0.0f
                && ((config.width != m_viewport_size.x) || (config.height != m_viewport_size.y))) {
                Renderer::resizeFrameBuffer((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);

                m_editor_camera.setViewportSize(m_viewport_size.x, m_viewport_size.y);
                m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            }
        }

        // render
        {
            RenderCommand::setClearColor({0.2f, 0.2f, 0.2f, 1.0f});
            RenderCommand::clear();
        }

        {
            ZE_PROFILE_SCOPE("Renderer2D::RenderScene");

            // update scene

            switch (m_sceneMode) {
                case SceneMode::Editor: {
                    if (m_viewport_hovered)
                        m_editor_camera.onUpdate(timestep);

                    m_active_scene->onUpdateEditor(timestep, m_editor_camera);
                    break;
                }
                case SceneMode::Game: {
                    m_active_scene->onUpdateRuntime(timestep);
                    break;
                }
            }

            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewport_bounds[0].x;
            my -= m_viewport_bounds[0].y;
            Vector2 viewport_size = m_viewport_bounds[1] - m_viewport_bounds[0];
            my                    = viewport_size.y - my;
            int mouse_x           = (int)mx;
            int mouse_y           = (int)my;

            if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < (int)viewport_size.x && mouse_y < (int)viewport_size.y) {
                // int pixelData    = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
                // m_hovered_entity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
            }
        }
    }

    void EditorLayer::onImGuiRender() {
        ZE_PROFILE_FUNCTION();

        if (m_dockspace_enable) {
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
            ImGuiIO&    io          = ImGui::GetIO();
            ImGuiStyle& style       = ImGui::GetStyle();
            float       minWinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x   = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            style.WindowMinSize.x = minWinSizeX;

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Menu")) {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                        newScene();

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                        openScene();

                    if (ImGui::MenuItem("Save As...", "Ctrl+S"))
                        saveSceneAs();

                    if (ImGui::MenuItem("Exit"))
                        Application::get().close();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // ************************************************************************************************
            m_scene_hierarchy_panel.onImGuiRender();
            m_content_browser_panel.onImGuiRender();

            {
                // Viewer
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
                ImGui::Begin("VIEWER", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar);

                auto viewport_offset = ImGui::GetCursorPos(); // Includes tab bar

                if (ImGui::BeginMenuBar()) {
                    // ImVec4 check_button_color = ImVec4(93.0f / 255.0f, 10.0f / 255.0f, 66.0f / 255.0f, 1.00f);
                    ImVec4 check_button_color = ImVec4{0.365f, 0.04f, 0.26f, 1.0f};

                    {
                        ImGui::Indent(10.f);

                        if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE) {
                            ImGui::PushID("Trans");
                            ImGui::PushStyleColor(ImGuiCol_Button,
                                                  ImVec4(check_button_color.x, check_button_color.y, check_button_color.z, 0.40f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, check_button_color);
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, check_button_color);

                            ImGui::Button("Trans");

                            ImGui::PopStyleColor(3);
                            ImGui::PopID();
                        }
                        else {
                            if (ImGui::Button("Trans") && !ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                        }
                        ImGui::Unindent();
                    }

                    ImGui::SameLine();

                    {
                        if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
                            ImGui::PushID("Rotate");
                            ImGui::PushStyleColor(ImGuiCol_Button,
                                                  ImVec4(check_button_color.x, check_button_color.y, check_button_color.z, 0.40f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, check_button_color);
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, check_button_color);

                            ImGui::Button("Rotate");

                            ImGui::PopStyleColor(3);
                            ImGui::PopID();
                        }
                        else {
                            if (ImGui::Button("Rotate") && !ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                        }
                    }

                    ImGui::SameLine();

                    {
                        if (m_GizmoType == ImGuizmo::OPERATION::SCALE) {
                            ImGui::PushID("Scale");
                            ImGui::PushStyleColor(ImGuiCol_Button,
                                                  ImVec4(check_button_color.x, check_button_color.y, check_button_color.z, 0.40f));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, check_button_color);
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, check_button_color);

                            ImGui::Button("Scale");

                            ImGui::PopStyleColor(3);
                            ImGui::PopID();
                        }
                        else {
                            if (ImGui::Button("Scale") && !ImGuizmo::IsUsing()) m_GizmoType = ImGuizmo::OPERATION::SCALE;
                        }
                    }

                    ImGui::SameLine();

                    float indent_scale = Application::get().getWindow()->getWidth() / 1280.f;

                    {
                        ImGui::Indent(ImGui::GetWindowSize().x - 110.0f);

                        if (m_sceneMode == SceneMode::Editor) {
                            ImGui::PushID("Editor Mode");
                            if (ImGui::Button("Editor Mode")) {
                                onScenePlay();
                            }
                            ImGui::PopID();
                        }
                        else if (m_sceneMode == SceneMode::Game) {
                            ImGui::PushID("Game Mode");
                            if (ImGui::Button("Game Mode")) {
                                onSceneStop();
                            }
                            ImGui::PopID();
                        }

                        ImGui::Unindent();
                    }

                    ImGui::EndMenuBar();
                }

                m_viewport_focused = ImGui::IsWindowFocused();
                m_viewport_hovered = ImGui::IsWindowHovered();
                Application::get().getImGuiLayer()->blockEvents(!m_viewport_focused && !m_viewport_hovered);

                ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
                if (m_viewport_size != *((Vector2*)&viewport_panel_size)) {
                    m_viewport_size = Vector2{viewport_panel_size.x, viewport_panel_size.y};
                }

                ImGui::Image(
                    Renderer::getOffScreenID(),
                    ImVec2{m_viewport_size.x, m_viewport_size.y});

                auto   window_size = ImGui::GetWindowSize();
                ImVec2 min_bound   = ImGui::GetWindowPos();
                min_bound.x += viewport_offset.x;
                min_bound.y += viewport_offset.y;

                ImVec2 max_bound     = {min_bound.x + window_size.x, min_bound.y + window_size.y};
                m_viewport_bounds[0] = Vector2{min_bound.x, min_bound.y};
                m_viewport_bounds[1] = Vector2{max_bound.x, max_bound.y};

                // NOTE: Gizmos
                Entity selected_entity = m_scene_hierarchy_panel.getSelectedEntity();
                if (selected_entity && m_GizmoType != -1 && m_sceneMode == SceneMode::Editor) {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    float window_width  = (float)ImGui::GetWindowWidth();
                    float window_height = (float)ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

                    // Editor camera
                    const Matrix& camera_projection = m_editor_camera.getProjection();
                    Matrix        camera_view       = m_editor_camera.getView();

                    // Entity transform
                    auto&  tc        = selected_entity.getComponent<TransformComponent>();
                    Matrix transform = tc.getTransform();

                    // Snapping
                    bool  snap       = InputSystem::isKeyPressed(VK_LCONTROL);
                    float snap_value = 0.5f; // Snap to 0.5m for translation/scale
                    // Snap to 45 degrees for rotation
                    if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                        snap_value = 45.0f;

                    float snap_values[3] = {snap_value, snap_value, snap_value};

                    ImGuizmo::Manipulate(reinterpret_cast<const float*>(&camera_view),
                                         reinterpret_cast<const float*>(&camera_projection),
                                         (ImGuizmo::OPERATION)m_GizmoType,
                                         ImGuizmo::LOCAL,
                                         reinterpret_cast<float*>(&transform),
                                         nullptr,
                                         snap ? snap_values : nullptr);

                    if (ImGuizmo::IsUsing()) {
                        Vector3    translation, scale;
                        Quaternion q_rotation;

                        auto helper = [](Vector3&& vec3) {
                            if (vec3.x == -0.0f) vec3.x = 0.0f;
                            if (vec3.y == -0.0f) vec3.x = 0.0f;
                            if (vec3.z == -0.0f) vec3.x = 0.0f;
                            return vec3;
                        };

                        transform.Decompose(scale, q_rotation, translation);

                        tc.translation = translation;
                        tc.rotation    = helper(q_rotation.ToEuler());
                        tc.scale       = scale;
                    }
                }

                ImGui::End();
                ImGui::PopStyleVar();
            }

            ZE_PROFILE_RENDER();

            // ************************************************************************************************

            ImGui::End();
        }
    }

    void EditorLayer::onEvent(Event& event) {
        if (m_sceneMode == SceneMode::Editor) {
            m_editor_camera.onEvent(event);
        }

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(ZE_BIND_EVENT_FN(EditorLayer::onKeyPressed));
    }

    bool EditorLayer::onKeyPressed(KeyPressedEvent& e) {
        bool control = InputSystem::isKeyPressed(VK_LCONTROL) || InputSystem::isKeyPressed(VK_RCONTROL);
        bool shift   = InputSystem::isKeyPressed(VK_LSHIFT) || InputSystem::isKeyPressed(VK_RSHIFT);

        switch (e.getKeyCode()) {
            case 'N': {
                if (control) newScene();
                break;
            }
            case 'O': {
                if (control) openScene();
                break;
            }
            case 'S': {
                if (control) saveSceneAs();
                break;
            }
        }

        return false;
    }

    void EditorLayer::newScene() {
        m_active_scene = CreateRef<Scene>();
        m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_scene_hierarchy_panel.setContext(m_active_scene);
    }

    void EditorLayer::openScene() {
        auto open_path = std::filesystem::path(
            GET_FILE_DIALOG().openFile("Scene (*.json)\0*.json\0"));

        if (!open_path.empty()) {
            m_active_scene = CreateRef<Scene>();
            m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            m_scene_hierarchy_panel.setContext(m_active_scene);

            SceneSerializer serializer(m_active_scene);
            serializer.deserialize(open_path);
        }
    }

    void EditorLayer::saveSceneAs() {
        auto save_path = std::filesystem::path(
            GET_FILE_DIALOG().saveFile("Scene (*.json)\0*.json\0"));

        if (!save_path.empty()) {
            SceneSerializer serializer(m_active_scene);
            serializer.serialize(save_path);
        }
    }

    void EditorLayer::onScenePlay() {
        m_active_scene->onRuntimeStart();
        m_sceneMode = SceneMode::Game;
    }
    void EditorLayer::onSceneStop() {
        m_active_scene->onRuntimeStop();
        m_sceneMode = SceneMode::Editor;
    }

} // namespace Zero