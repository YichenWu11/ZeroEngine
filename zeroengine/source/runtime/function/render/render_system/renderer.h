#pragma once

#include <entt/entt.hpp>
#include <imgui.h>

#include "runtime/function/render/camera_system/orthographics_camera.h"
#include "runtime/function/render/render_system/render_command.h"

namespace Chen::CDX12 {
    class Mesh;
}

namespace Zero {
    class Renderer {
    public:
        static void init();
        static void shutdown();

        static void onWindowResize(int, int);
        static void resizeFrameBuffer(int, int);

        static void beginScene(const OrthographicsCamera& camera);
        static void endScene();

        static void submit(Chen::CDX12::Mesh*, const DirectX::SimpleMath::Matrix& = DirectX::SimpleMath::Matrix::Identity);

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
        static ImTextureID      getOffScreenID();

    private:
        struct SceneData {
            DirectX::SimpleMath::Matrix view_projection_matrix;
            DirectX::SimpleMath::Matrix model_matrix;
        };

    private:
        static SceneData* s_scene_data;
    };
} // namespace Zero