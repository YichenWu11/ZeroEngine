#pragma once

#include <sol/sol.hpp>

#include "runtime/core/util/time_step.h"
#include "runtime/function/physics/collider2d.h"
#include "runtime/function/scene/scene_camera.h"
#include "runtime/function/scene/scriptable_entity.h"
#include "runtime/resource/resource/tile_sheet.h"

namespace Zero {
    struct NameComponent {
        std::string name;

        NameComponent()                     = default;
        NameComponent(const NameComponent&) = default;
        NameComponent(const std::string& _name) :
            name(_name) {}
    };

    // 2D
    struct TransformComponent {
        Vector3 translation{0.0f, 0.0f, 0.0f};
        Vector3 rotation{0.0f, 0.0f, 0.0f};
        Vector3 scale{1.0f, 1.0f, 1.0f};

        TransformComponent()                          = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const Vector3& _translation) :
            translation(_translation) {}

        Matrix getTransform() const {
            return Matrix::CreateScale(scale)
                   * Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(rotation))
                   * Matrix::CreateTranslation(translation);
        }
    };

    struct SpriteComponent {
        Color    color{1.0f, 1.0f, 1.0f, 1.0f};
        uint32_t tex_index{0};
        float    tiling_factor{1.0f};

        SpriteComponent()                       = default;
        SpriteComponent(const SpriteComponent&) = default;
        SpriteComponent(const Color& _color, uint32_t _tex_index = 0, float _tiling_factor = 1.0f) :
            color(_color), tex_index(_tex_index), tiling_factor(_tiling_factor) {}
    };

    struct TileComponent {
        Resource<ResourceType::TileSheet>* tile_sheet{nullptr};
        uint32_t                           coord_x{0};
        uint32_t                           coord_y{0};
        uint32_t                           size_x{1};
        uint32_t                           size_y{1};

        Color color{1.0f, 1.0f, 1.0f, 1.0f};

        TileComponent()                     = default;
        TileComponent(const TileComponent&) = default;
        TileComponent(std::string_view tile_sheet);
    };

    struct CameraComponent {
        SceneCamera camera;
        bool        is_current{true};
        bool        is_fixed_aspectRatio{false};
        bool        is_fixed_rotation{true};

        CameraComponent()                       = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const Matrix& _projection) :
            camera(_projection) {}
    };

    struct NativeScriptComponent {
        ScriptableEntity* instance = nullptr;

        ScriptableEntity* (*instantiateScript)(){nullptr};
        void (*destroyScript)(NativeScriptComponent*){nullptr};

        template <typename T>
        void bind() {
            instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            destroyScript     = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
        }
    };

    struct LuaScriptComponent {
        sol::table            object{sol::nil};
        std::filesystem::path script_path;
        Entity                parent_entity;

        LuaScriptComponent()                          = default;
        LuaScriptComponent(const LuaScriptComponent&) = default;
        LuaScriptComponent(const std::filesystem::path& path) :
            script_path(path) {}

        bool registerToLuaContext(sol::state& p_luaState) {
            auto result = p_luaState.safe_script_file(script_path.string(), &sol::script_pass_on_error);

            if (!result.valid()) {
                sol::error err = result;
                LOG_ERROR(err.what());
                return false;
            }
            else {
                if (result.return_count() == 1 && result[0].is<sol::table>()) {
                    object           = result[0];
                    object["parent"] = parent_entity;
                    return true;
                }
                else {
                    LOG_ERROR("{} missing return expression", script_path.string());
                    return false;
                }
            }
        }

        void unregisterFromLuaContext() { object = sol::nil; }

        template <typename... Args>
        void luaCall(const std::string& p_functionName, Args&&... p_args) {
            if (object.valid()) {
                if (object[p_functionName].valid()) {
                    sol::protected_function pfr       = object[p_functionName];
                    auto                    pfrResult = pfr.call(object, std::forward<Args>(p_args)...);
                    if (!pfrResult.valid()) {
                        sol::error err = pfrResult;
                        LOG_ERROR(err.what());
                    }
                }
            }
        }
    };

    // physics
    struct Rigidbody2DComponent {
        enum class BodyType { Static = 0,
                              Dynamic,
                              Kinematic };

        BodyType   type{BodyType::Static};
        Collider2D collider;
        // Shape and type is never mutable.

        bool is_fixed_rotation{false};
        bool is_mutable{true};
        bool show_box{true};

        Rigidbody2DComponent() = default;

        // Storage for runtime
        void* runtime_body{nullptr};

        Vector2 linear_velocity{0.0f, 0.0f};
        float   angular_velocity{0.0f};

        Vector2 old_linear_velocity{0.0f, 0.0f};
        float   old_angular_velocity{0.0f};

        Vector2 old_translation{0.0f, 0.0f};
        float   old_rotation{0.0f};
    };

} // namespace Zero