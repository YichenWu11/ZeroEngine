#pragma once

#include <entt/entt.hpp>

#include "runtime/core/util/time_step.h"

namespace Zero {
    class Entity;
    class SceneHierarchyPanel;
    class SceneSerializer;

    class Scene {
        friend class Entity;
        friend class SceneHierarchyPanel;
        friend class SceneSerializer;

    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name = std::string{});
        void   destroyEntity(Entity entity);

        void onUpdate(TimeStep timestep);
        void onViewportResize(uint32_t width, uint32_t height);

    private:
        template <typename T>
        void onComponentAdded(Entity entity, T& component);

    private:
        entt::registry m_registry;
        uint32_t       m_viewport_width{0};
        uint32_t       m_viewport_height{0};
    };
} // namespace Zero