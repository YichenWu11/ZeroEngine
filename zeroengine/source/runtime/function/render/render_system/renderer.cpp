#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"

using namespace Chen::CDX12;

namespace Zero {
    Renderer::SceneData* Renderer::s_scene_data = new Renderer::SceneData();

    void Renderer::init() {
        Renderer2D::init();
    }

    void Renderer::shutdown() {
        RenderContext::getInstance().shutdown();
        Renderer2D::shutdown();
    }

    void Renderer::onWindowResize(int width, int height) {
        RenderContext::getInstance().onResize(width, height);
    }

    void Renderer::resizeFrameBuffer(int width, int height) {
        RenderContext::getInstance().resizeFrameBuffer(width, height);
    }

    void Renderer::beginScene(const OrthographicsCamera& camera) {
        // takes all the scene settings(camera, lights, environment etc)
        s_scene_data->view_projection_matrix = (camera.getViewProjectionMatrix()).Transpose();

        BasicShader* shader =
            static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("transparent"));

        GET_SHADER_BIND_TABLE().bindParam(
            shader,
            "_PassConstant",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&s_scene_data->view_projection_matrix),
                sizeof(s_scene_data->view_projection_matrix)});

        GET_SHADER_BIND_TABLE().bindParam(
            shader,
            "TextureMap",
            std::make_pair(&RenderContext::getInstance().getTexAlloc(), 0));
    }

    void Renderer::endScene() {
        RenderContext::getInstance().onRender();
    }

    void Renderer::submit(Mesh* mesh, const Matrix& transfrom) {
    }

    ImTextureID Renderer::getOffScreenID() {
        return RenderContext::getInstance().getOffScreenID();
    }

    FrameBufferConfiguration Renderer::getFrameBufferConfig() {
        return RenderContext::getInstance().getFrameBufferConfig();
    }
} // namespace Zero