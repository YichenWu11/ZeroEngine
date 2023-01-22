#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    RenderContext*       Renderer::s_render_context = nullptr;
    Renderer::SceneData* Renderer::s_scene_data     = new Renderer::SceneData();

    void Renderer::beginScene(OrthographicsCamera& camera) {
        ZE_ASSERT(s_render_context && "bind the render_context first!");

        // takes all the scene settings(camera, lights, environment etc)
        s_scene_data->view_projection_matrix = (camera.getViewMatrix() * camera.getProjectionMatrix()).Transpose();

        s_scene_data->model_matrix = (Matrix::CreateTranslation(0.0f, 0.0f, 0.0f)).Transpose();
    }

    void Renderer::endScene() {
        ZE_ASSERT(s_render_context && "bind the render_context first!");

        s_render_context->beginRender();
        s_render_context->endRender();
    }

    void Renderer::submit(Mesh* mesh, const DirectX::SimpleMath::Vector3& trans) {
        ZE_ASSERT(s_render_context && "bind the render_context first!");

        BasicShader* shader =
            static_cast<BasicShader*>(ShaderParamBindTable::getShader("common"));

        ShaderParamBindTable::bindParam(
            shader,
            "_ViewProjMatrix",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&s_scene_data->view_projection_matrix),
                sizeof(s_scene_data->view_projection_matrix)});

        s_render_context->submit(mesh, trans);
    }
} // namespace Zero