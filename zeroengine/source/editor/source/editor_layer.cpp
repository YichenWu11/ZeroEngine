#include <ImGuizmo.h>

#include "runtime/function/scene/scene_serializer.h"

#include "editor_layer.h"

namespace Zero {
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"), m_camera_controller(1280.0f / 720.0f) {}

    void EditorLayer::onAttach() {
        ZE_PROFILE_FUNCTION();

        GET_TEXTURE_POOL().registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png");
        GET_TEXTURE_POOL().registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul.png");
        GET_TEXTURE_POOL().registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/asoul_moon.png");
        GET_TEXTURE_POOL().registerTex(
            "SpriteSheet",
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/game/texture/sprite_sheet.png");

        m_texture_stair = SubTexture2D::createFromCoords(
            GET_TEXTURE_POOL().getTextureFromName("SpriteSheet"),
            {7, 6},
            {128.0f, 128.0f});

        m_texture_bush = SubTexture2D::createFromCoords(
            GET_TEXTURE_POOL().getTextureFromName("SpriteSheet"),
            {2, 3},
            {256.0f, 128.0f});

        m_active_scene = CreateRef<Scene>();

        // auto&& green_square = m_active_scene->createEntity("green_square");
        // green_square.addComponent<SpriteComponent>(Color{0.0f, 1.0f, 0.0f, 1.0f});

        // auto&& red_square = m_active_scene->createEntity("red_square");
        // red_square.addComponent<SpriteComponent>(Color{1.0f, 0.0f, 0.0f, 1.0f});

        // auto&& camera_a = m_active_scene->createEntity("cameraA");
        // camera_a.addComponent<CameraComponent>();

        // class CameraController : public ScriptableEntity {
        // public:
        //     void onCreate() override {
        //     }

        //     void onDestroy() override {
        //     }

        //     void onUpdate(TimeStep timestep) override {
        //         auto& transform = getComponent<TransformComponent>().translation;
        //         float speed     = 5.0f;

        //         if (InputSystem::isKeyPressed('A'))
        //             transform.x -= speed * timestep;
        //         if (InputSystem::isKeyPressed('D'))
        //             transform.x += speed * timestep;
        //         if (InputSystem::isKeyPressed('W'))
        //             transform.y += speed * timestep;
        //         if (InputSystem::isKeyPressed('S'))
        //             transform.y -= speed * timestep;
        //     }
        // };

        // camera_a.addComponent<NativeScriptComponent>().bind<CameraController>();

        // auto&& camera_b = m_active_scene->createEntity("cameraB");
        // camera_b.addComponent<CameraComponent>();
        // camera_b.getComponent<CameraComponent>().is_current = false;

        m_scene_hierarchy_panel.setContext(m_active_scene);
    }

    void EditorLayer::onDetach() {
        ZE_PROFILE_FUNCTION();
    }

    void
    EditorLayer::onUpdate(TimeStep timestep) {
        ZE_PROFILE_FUNCTION();

        // resize
        {
            auto config = Renderer::getFrameBufferConfig();
            if (m_viewport_size.x > 0.0f
                && m_viewport_size.y > 0.0f
                && ((config.width != m_viewport_size.x) || (config.height != m_viewport_size.y))) {
                Renderer::resizeFrameBuffer((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
                m_camera_controller.onResize(m_viewport_size.x, m_viewport_size.y);

                m_active_scene->onViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
            }
        }

        // update
        {
            ZE_PROFILE_SCOPE("CameraController::onUpdate");
            if (m_viewport_focused)
                m_camera_controller.onUpdate(timestep);
        }

        // render
        {
            // RenderCommand::setClearColor({0.2f, 0.2f, 0.2f, 1.0f});
            RenderCommand::clear();
        }

        {
            ZE_PROFILE_SCOPE("Renderer2D::RenderScene");

            // update scene
            m_active_scene->onUpdate(timestep);
        }
    }

    void EditorLayer::onImGuiRender() {
        ZE_PROFILE_FUNCTION();

        if (m_dockspace_enable) {
            static auto tex_alloc = GET_TEXTURE_POOL().getTexAllocation();

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
                if (ImGui::BeginMenu("File")) {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

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

            {
                ImGui::Begin("ASOUL");
                ImGui::Image(
                    ImTextureID(tex_alloc->GetGpuHandle(2).ptr),
                    ImVec2(200, 200));
                ImGui::SameLine();
                ImGui::End();
            }

            {
                // Viewer
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
                ImGui::Begin("VIEWER");
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

                // NOTE: Gizmos
                Entity selected_entity = m_scene_hierarchy_panel.getSelectedEntity();
                if (selected_entity && m_GizmoType != -1) {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    float window_width  = (float)ImGui::GetWindowWidth();
                    float window_height = (float)ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);

                    // Camera
                    auto          camera_entity     = m_active_scene->getPrimaryCameraEntity();
                    const auto&   camera            = camera_entity.getComponent<CameraComponent>().camera;
                    const Matrix& camera_projection = camera.getProjection();
                    Matrix        camera_view       = camera_entity.getComponent<TransformComponent>().getTransform().Invert();

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
                        Vector3    translation{tc.translation}, scale{tc.scale};
                        Quaternion q_rotation(Quaternion::CreateFromYawPitchRoll(tc.rotation));

                        transform.Decompose(scale, q_rotation, translation);

                        tc.translation = translation;
                        tc.rotation    = q_rotation.ToEuler();
                        if (tc.rotation.x == -0.0f) tc.rotation.x = 0.0f;
                        if (tc.rotation.y == -0.0f) tc.rotation.x = 0.0f;
                        if (tc.rotation.z == -0.0f) tc.rotation.x = 0.0f;

                        tc.scale = scale;
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
        m_camera_controller.onEvent(event);

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

            // Gizmos
            case 'Q':
                m_GizmoType = -1;
                break;
            case 'T':
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case 'R':
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            case 'E':
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
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

} // namespace Zero