#pragma once

#include <entt/entt.hpp>

#include "runtime/core/util/time_step.h"

namespace Zero {
    class Entity;

    class Scene {
        friend class Entity;

    public:
        Scene();
        ~Scene();

        Entity createEntity(const std::string& name = std::string{});

        void onUpdate(TimeStep timestep);
        void onViewportResize(uint32_t width, uint32_t height);

    private:
        entt::registry m_registry;
        uint32_t       m_viewport_width{0};
        uint32_t       m_viewport_height{0};
    };
} // namespace Zero