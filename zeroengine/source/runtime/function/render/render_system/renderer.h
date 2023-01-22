#pragma once

#include "runtime/function/render/camera_system/orthographics_camera.h"
#include "runtime/function/render/render_system/render_command.h"

namespace Chen::CDX12 {
    class Mesh;
}

namespace Zero {
    class RenderContext;

    class Renderer {
    public:
        static void bindRenderContext(RenderContext* context) { s_render_context = context; }

        static void beginScene(OrthographicsCamera& camera);
        static void endScene();

        static void submit(Chen::CDX12::Mesh*, const DirectX::SimpleMath::Vector3& = {0.0f, 0.0f, 0.0f});

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        struct SceneData {
            DirectX::SimpleMath::Matrix view_projection_matrix;
            DirectX::SimpleMath::Matrix model_matrix;
        };

    private:
        static RenderContext* s_render_context;
        static SceneData*     s_scene_data;
    };
} // namespace Zero