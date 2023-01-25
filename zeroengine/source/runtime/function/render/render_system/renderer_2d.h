#pragma once

#include "runtime/function/render/camera_system/orthographics_camera.h"

namespace Zero {
    class RenderContext;

    class Renderer2D {
    public:
        static void init(RenderContext* context);
        static void shutdown();

        static void beginScene(const OrthographicsCamera& camera);
        static void endScene();

        static void drawQuad(
            const DirectX::SimpleMath::Vector2& position,
            const DirectX::SimpleMath::Vector2& size,
            const DirectX::SimpleMath::Color&   color,
            int32_t                             tex_index = -1);
        static void drawQuad(
            const DirectX::SimpleMath::Vector3& position,
            const DirectX::SimpleMath::Vector2& size,
            const DirectX::SimpleMath::Color&   color,
            int32_t                             tex_index = -1);

    private:
        static RenderContext* s_render_context;
    };
} // namespace Zero