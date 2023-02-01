#pragma once

#include "runtime/function/render/camera_system//camera.h"

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
            return Matrix::CreateRotationZ(DirectX::XMConvertToRadians(-rotation.z))
                   * Matrix::CreateScale(scale)
                   * Matrix::CreateTranslation(translation);
        }
    };

    struct SpriteRendererComponent {
        Color    color{1.0f, 1.0f, 1.0f, 1.0f};
        uint32_t tex_index{0};
        float    tiling_factor{1.0f};

        SpriteRendererComponent()                               = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Color& _color, uint32_t _tex_index = 0, float _tiling_factor = 1.0f) :
            color(_color), tex_index(_tex_index), tiling_factor(_tiling_factor) {}
    };

    struct CameraComponent {
        Camera camera;
        bool   is_current = true;

        CameraComponent()                       = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const Matrix& _projection) :
            camera(_projection) {}
    };
} // namespace Zero