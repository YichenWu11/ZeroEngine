#pragma once

#include "runtime/function/render/camera_system/camera.h"
#include "runtime/function/render/camera_system/editor_camera.h"
#include "runtime/function/render/camera_system/orthographics_camera.h"
#include "runtime/function/render/render_system/subtexture_2d.h"
#include "runtime/function/scene/components.h"

namespace Zero {
    class Renderer2D {
    public:
        static void init();
        static void shutdown();

        static void beginScene(const Camera& camera, const Matrix& cam_transform);
        static void beginScene(const EditorCamera& camera);
        static void beginScene(const OrthographicsCamera& camera);
        static void endScene();

        static void drawQuad(
            const Vector2& position,
            const Vector2& size,
            float          rotation,
            const Color&   color         = {1.0f, 1.0f, 1.0f, 1.0f},
            uint32_t       tex_index     = 0,
            float          tiling_factor = 1.0f);

        static void drawQuad(
            const Vector3& position,
            const Vector2& size,
            float          rotation,
            const Color&   color         = {1.0f, 1.0f, 1.0f, 1.0f},
            uint32_t       tex_index     = 0,
            float          tiling_factor = 1.0f);

        static void drawQuad(
            const Matrix& transform,
            const Color&  color         = {1.0f, 1.0f, 1.0f, 1.0f},
            uint32_t      tex_index     = 0,
            float         tiling_factor = 1.0f,
            int           entity_id     = -1);

        static void drawCellQuad(
            const Vector2&           position,
            const Vector2&           size,
            float                    rotation,
            const Ref<SubTexture2D>& sub_texture,
            const Color&             color = {1.0f, 1.0f, 1.0f, 1.0f});

        static void drawCellQuad(
            const Vector3&           position,
            const Vector2&           size,
            float                    rotation,
            const Ref<SubTexture2D>& sub_texture,
            const Color&             color = {1.0f, 1.0f, 1.0f, 1.0f});

        static void drawSprite(
            const Matrix&          transform,
            const SpriteComponent& src,
            int                    entity_id);

    private:
        static void beginScene(const Matrix& view_proj);
    };
} // namespace Zero