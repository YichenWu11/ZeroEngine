#pragma once

#include <entt/entt.hpp>

#include "runtime/core/util/time_step.h"
#include "runtime/function/render/camera_system/editor_camera.h"

class b2World;

namespace Zero {
    class Entity;
    class SceneHierarchyPanel;
    class SceneSerializer;

    class Scene {
        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;

    public:
        Scene() = default;
        ~Scene();

        Entity createEntity(const std::string& name = std::string{});
        void   destroyEntity(Entity entity);

        void onRuntimeStart();
        void onRuntimeStop();

        void onUpdateRuntime(TimeStep timestep);
        void onUpdateEditor(TimeStep timestep, EditorCamera& camera);

        void onViewportResize(uint32_t width, uint32_t height);

        Entity getPrimaryCameraEntity();

    private:
        template <typename T>
        void onComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_registry;
        uint32_t       m_viewport_width{0};
        uint32_t       m_viewport_height{0};

        b2World* m_physics_world{nullptr};
    };
} // namespace Zero