#pragma once

#include "runtime/zero.h"

#include "panels/content_browser_panel.h"
#include "panels/scene_hierarchy_panel.h"

using namespace DirectX::SimpleMath;

namespace Zero {
    class EditorLayer : public Layer {
    public:
        EditorLayer();
        ~EditorLayer() = default;

        void onAttach() override;
        void onDetach() override;
        void onUpdate(TimeStep timestep) override;
        void onImGuiRender() override;
        void onEvent(Event& event) override;

    private:
        bool onKeyPressed(KeyPressedEvent& e);

        void newScene();
        void openScene();
        void saveSceneAs();

        void onScenePlay();
        void onSceneStop();

    private:
        Vector2 m_viewport_size;
        Vector2 m_viewport_bounds[2];

        bool m_viewport_focused{false};
        bool m_viewport_hovered{false};
        bool m_dockspace_enable{true};

        Ref<SubTexture2D> m_texture_stair;
        Ref<SubTexture2D> m_texture_bush;

        Ref<Scene> m_active_scene;

        SceneHierarchyPanel m_scene_hierarchy_panel;
        ContentBrowserPanel m_content_browser_panel;

        EditorCamera m_editor_camera;

        Entity m_hovered_entity;

        int m_GizmoType{7};

        enum class SceneMode {
            Editor = 0,
            Game   = 1
        };

        SceneMode m_sceneMode = SceneMode::Editor;
    };
} // namespace Zero