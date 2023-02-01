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

    private:
        entt::registry m_registry;
    };
} // namespace Zero