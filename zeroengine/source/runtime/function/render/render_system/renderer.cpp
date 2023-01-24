#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    RenderContext*       Renderer::s_render_context = nullptr;
    Renderer::SceneData* Renderer::s_scene_data     = new Renderer::SceneData();

    void Renderer::init(RenderContext* context) {
        s_render_context = context;
        MeshTable::getInstance().bindRenderContext(context);
        TextureTable::getInstance().bindRenderContext(context);
    }

    void Renderer::onWindowResize(int width, int height) {
        ZE_ASSERT(s_render_context && "bind the render_context first!");

        s_render_context->onResize(width, height);
    }

    void Renderer::beginScene(OrthographicsCamera& camera) {
        ZE_ASSERT(s_render_context && "bind the render_context first!");

        // takes all the scene settings(camera, lights, environment etc)
        s_scene_data->view_projection_matrix = (camera.getViewProjectionMatrix()).Transpose();

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
            static_cast<BasicShader*>(ShaderParamBindTable::getInstance().getShader("transparent"));

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "_ViewProjMatrix",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&s_scene_data->view_projection_matrix),
                sizeof(s_scene_data->view_projection_matrix)});

        auto tex_alloc = TextureTable::getInstance().getTexAllocation();

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "TextureMap",
            std::make_pair(tex_alloc, 0));

        s_render_context->submit(mesh, trans);
    }
} // namespace Zero