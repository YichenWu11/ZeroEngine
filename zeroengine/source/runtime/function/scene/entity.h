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
            if (hasComponent<T>()) {
                LOG_WARN("Entity already has this component!");
                return getComponent<T>();
            }

            T& component = m_parent_scene->m_registry.emplace<T>(m_entity_handle, std::forward<Args>(args)...);
            m_parent_scene->onComponentAdded<T>(*this, component);
            return component;
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
        operator entt::entity() const { return m_entity_handle; }
        operator uint32_t() const { return (uint32_t)m_entity_handle; }

        bool operator==(const Entity& other) const {
            return m_entity_handle == other.m_entity_handle && m_parent_scene == other.m_parent_scene;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    private:
        entt::entity m_entity_handle{entt::null};
        Scene*       m_parent_scene;
    };
} // namespace Zero