#include "runtime/function/scene/entity.h"

namespace Zero {
    Entity::Entity(entt::entity entity_handle, Scene* scene) :
        m_entity_handle(entity_handle), m_parent_scene(scene) {
    }
} // namespace Zero