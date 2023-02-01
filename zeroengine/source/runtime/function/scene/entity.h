#pragma once

#include <entt/entt.hpp>

#include "runtime/function/scene/scene.h"

namespace Zero {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity entity_handle, Scene* scene);
        Entity(const Entity&) = default;

        template <typename T>
        bool hasComponent() {
            return m_parent_scene->m_registry.all_of<T>(m_entity_handle);
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            ZE_ASSERT(!hasComponent<T>() && "Entity already has this component!");
            return m_parent_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
        }

        template <typename T>
        T& getComponent() {
            ZE_ASSERT(hasComponent<T>() && "Entity dose not have this component!");
            return m_parent_scene->m_registry.get<T>(m_entity_handle);
        }

        template <typename T>
        void removeComponent() {
            ZE_ASSERT(hasComponent<T>() && "Entity does not have this component!");
            m_parent_scene->m_registry.remove<T>(m_entity_handle);
        }

        operator bool() const { return m_entity_handle != entt::null; }

    private:
        entt::entity m_entity_handle{entt::null};
        Scene*       m_parent_scene;
    };
} // namespace Zero