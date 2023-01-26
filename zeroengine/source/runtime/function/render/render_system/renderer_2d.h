#pragma once

#include "runtime/function/render/camera_system/orthographics_camera.h"

namespace Zero {
    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const OrthographicsCamera& camera);
        static void endScene();

        static void drawQuad(
            const DirectX::SimpleMath::Vector2& position,
            const DirectX::SimpleMath::Vector2& size,
            float                               rotation,
            const DirectX::SimpleMath::Color&   color         = {1.0f, 1.0f, 1.0f, 1.0f},
            uint32_t                            tex_index     = 0,
            float                               tiling_factor = 1.0f);

        static void drawQuad(
            const DirectX::SimpleMath::Vector3& position,
            const DirectX::SimpleMath::Vector2& size,
            float                               rotation,
            const DirectX::SimpleMath::Color&   color         = {1.0f, 1.0f, 1.0f, 1.0f},
            uint32_t                            tex_index     = 0,
            float                               tiling_factor = 1.0f);
    };
} // namespace Zero