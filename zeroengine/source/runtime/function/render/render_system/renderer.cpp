#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData();

    void Renderer::init() {
        GET_MESH_TABLE().init();
        GET_TEXTURE_TABLE().init();
        Renderer2D::init();
    }

    void Renderer::shutdown() {
        GET_RENDER_CONTEXT().shutdown();
        Renderer2D::shutdown();
    }

    void Renderer::onWindowResize(int width, int height) {
        GET_RENDER_CONTEXT().onResize(width, height);
    }

    void Renderer::beginScene(const OrthographicsCamera& camera) {
        // takes all the scene settings(camera, lights, environment etc)
        s_scene_data->view_projection_matrix = (camera.getViewProjectionMatrix()).Transpose();

        s_scene_data->model_matrix = (Matrix::CreateTranslation(0.0f, 0.0f, 0.0f)).Transpose();

        BasicShader* shader =
            static_cast<BasicShader*>(ShaderParamBindTable::getInstance().getShader("transparent"));

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "_ViewProjMatrix",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&s_scene_data->view_projection_matrix),
                sizeof(s_scene_data->view_projection_matrix)});

        auto tex_alloc = GET_TEXTURE_TABLE().getTexAllocation();

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "TextureMap",
            std::make_pair(tex_alloc, 0));
    }

    void Renderer::endScene() {
        GET_RENDER_CONTEXT().beginRender();
        GET_RENDER_CONTEXT().endRender();
    }

    void Renderer::submit(Mesh* mesh, const DirectX::SimpleMath::Matrix& transfrom) {
    }
} // namespace Zero