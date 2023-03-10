#include "runtime/function/scripting/binder/lua_entity_binder.h"
#include "runtime/function/scene/components.h"
#include "runtime/function/scene/entity.h"

namespace Zero {
    void LuaEntityBinder::bindEntity(sol::state& p_luaState) {
        p_luaState.new_usertype<Entity>(
            "Entity",

            /* Methods */
            "GetName", &Entity::getName,
            "SetName", &Entity::setName,
            "GetParentScene", &Entity::getParentScene,

            /* Components Getters */
            "GetTransform", &Entity::getComponent<TransformComponent>,
            "GetSprite", &Entity::getComponent<SpriteComponent>,
            "GetTile", &Entity::getComponent<TileComponent>,
            "GetCamera", &Entity::getComponent<CameraComponent>,
            "GetRigidbody2D", &Entity::getComponent<Rigidbody2DComponent>,

            /* Lua Script relatives */
            "GetLuaScript", [](Entity& p_this) -> sol::table { return (p_this.hasComponent<LuaScriptComponent>()) ? p_this.getComponent<LuaScriptComponent>().object : sol::nil; },

            /* Components Creators */
            "AddTransform", &Entity::addComponent<TransformComponent>,
            "AddSprite", &Entity::addComponent<SpriteComponent>,
            "AddTile", &Entity::addComponent<TileComponent>,
            "AddCamera", &Entity::addComponent<CameraComponent>,
            "AddRigidbody2D", &Entity::addComponent<Rigidbody2DComponent>,

            /* Components Destructors */
            "RemoveTransform", &Entity::removeComponent<TransformComponent>,
            "RemoveSprite", &Entity::removeComponent<SpriteComponent>,
            "RemoveTile", &Entity::removeComponent<TileComponent>,
            "RemoveCamera", &Entity::removeComponent<CameraComponent>,
            "RemoveRigidbody2D", &Entity::removeComponent<Rigidbody2DComponent>);
    }
} // namespace Zero