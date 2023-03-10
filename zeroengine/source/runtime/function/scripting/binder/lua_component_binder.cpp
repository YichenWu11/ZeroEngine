#include "runtime/function/scripting/binder/lua_component_binder.h"
#include "runtime/function/scene/components.h"

namespace Zero {
    void LuaComponentBinder::bindComponent(sol::state& p_luaState) {
        p_luaState.new_usertype<TransformComponent>(
            "TransformComponent",

            /* Variables */
            "translation", &TransformComponent::translation,
            "rotation", &TransformComponent::rotation,
            "scale", &TransformComponent::scale);

        p_luaState.new_usertype<SpriteComponent>(
            "SpriteComponent",

            /* Variables */
            "color", &SpriteComponent::color,
            "tex_index", &SpriteComponent::tex_index,
            "tiling_factor", &SpriteComponent::tiling_factor);

        p_luaState.new_usertype<TileComponent>(
            "TileComponent",

            /* Variables */
            "coord_x", &TileComponent::coord_x,
            "coord_y", &TileComponent::coord_y,
            "size_x", &TileComponent::size_x,
            "size_y", &TileComponent::size_y);

        p_luaState.new_usertype<CameraComponent>(
            "CameraComponent",

            /* Variables */
            "is_current", &CameraComponent::is_current,
            "is_fixed_aspectRatio", &CameraComponent::is_fixed_aspectRatio,
            "is_fixed_rotation", &CameraComponent::is_fixed_rotation,

            /* Methods */
            "SetOrthographicSize", [](CameraComponent& cc, float size) { cc.camera.setOrthographicSize(size); });

        p_luaState.new_enum<Rigidbody2DComponent::BodyType>(
            "BodyType",
            {{"Static", Rigidbody2DComponent::BodyType::Static},
             {"Dynamic", Rigidbody2DComponent::BodyType::Dynamic},
             {"Kinematic", Rigidbody2DComponent::BodyType::Kinematic}});

        p_luaState.new_usertype<Rigidbody2DComponent>(
            "Rigidbody2DComponent",

            /* Variables */
            "type", &Rigidbody2DComponent::type,
            "is_fixed_rotation", &Rigidbody2DComponent::is_fixed_rotation,
            "is_mutable", &Rigidbody2DComponent::is_mutable,
            "show_box", &Rigidbody2DComponent::show_box,
            "linear_velocity", &Rigidbody2DComponent::linear_velocity,
            "angular_velocity", &Rigidbody2DComponent::angular_velocity,
            "old_linear_velocity", &Rigidbody2DComponent::old_linear_velocity,
            "old_angular_velocity", &Rigidbody2DComponent::old_angular_velocity,
            "old_translation", &Rigidbody2DComponent::old_translation,
            "old_rotation", &Rigidbody2DComponent::old_rotation,

            /* Methods */
            "CreateBox", [](Rigidbody2DComponent& r2c, float w, float h) { r2c.collider.createBox(w, h); });
    }
} // namespace Zero