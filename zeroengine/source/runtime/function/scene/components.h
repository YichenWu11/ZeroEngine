#pragma once

#include "runtime/core/util/time_step.h"
#include "runtime/function/scene/scene_camera.h"
#include "runtime/function/scene/scriptable_entity.h"

using namespace DirectX::SimpleMath;

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

    struct CameraComponent {
        SceneCamera camera;
        bool        is_current{true};
        bool        is_fixed_aspectRatio{false};

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

    // physics
    struct Rigidbody2DComponent {
        enum class BodyType { Static = 0,
                              Dynamic,
                              Kinematic };

        BodyType type{BodyType::Static};
        bool     is_fixed_rotation{false};

        // Storage for runtime
        void* runtime_body{nullptr};

        Rigidbody2DComponent()                            = default;
        Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
    };

} // namespace Zero