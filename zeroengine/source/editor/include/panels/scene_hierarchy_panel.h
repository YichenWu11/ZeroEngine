#pragma once

#include "runtime/function/scene/entity.h"
#include "runtime/function/scene/scene.h"

namespace Zero {
    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);
        ~SceneHierarchyPanel() = default;

        void setContext(const Ref<Scene>& context);

        void onImGuiRender();

    private:
        void drawEntityNode(Entity entity);
        void drawComponents(Entity entity);

    private:
        Ref<Scene> m_context;
        Entity     m_selected_entity;
    };
} // namespace Zero