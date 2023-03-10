#pragma once

#include "runtime/function/scene/entity.h"

namespace Zero {
    class ScriptableEntity {
        friend class Scene;

    public:
        virtual ~ScriptableEntity() {}

        template <typename T>
        T& getComponent() {
            return m_entity.getComponent<T>();
        }

    protected:
        virtual void onCreate() {}
        virtual void onDestroy() {}
        virtual void onUpdate(TimeStep ts) {}

        // physics messages:
        virtual void onCollisionEnter(Entity entity) {}
        virtual void onCollisionExit(Entity entity) {}

    private:
        Entity m_entity;
    };
} // namespace Zero