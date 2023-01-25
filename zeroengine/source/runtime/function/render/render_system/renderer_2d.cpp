#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    struct Renderer2DStorage {};

    RenderContext* Renderer2D::s_render_context = nullptr;

    void Renderer2D::init(RenderContext* context) {
        ZE_ASSERT(context && "render_context can not be nullptr");
        s_render_context = context;
        MeshTable::getInstance().bindRenderContext(context);
        TextureTable::getInstance().bindRenderContext(context);
    }

    void Renderer2D::shutdown() {
        ZE_ASSERT(s_render_context && "init the renderer2d first!");
    }

    void Renderer2D::beginScene(const OrthographicsCamera& camera) {
        ZE_ASSERT(s_render_context && "init the renderer2d first!");

        // takes all the scene settings(camera, lights, environment etc)
        BasicShader* shader =
            static_cast<BasicShader*>(ShaderParamBindTable::getInstance().getShader("transparent"));

        static Matrix view_proj_matrix;
        view_proj_matrix = camera.getViewProjectionMatrix().Transpose();

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "_ViewProjMatrix",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&view_proj_matrix),
                sizeof(view_proj_matrix)});

        auto tex_alloc = TextureTable::getInstance().getTexAllocation();

        ShaderParamBindTable::getInstance().bindParam(
            shader,
            "TextureMap",
            std::make_pair(tex_alloc, 0));
    }

    void Renderer2D::endScene() {
        ZE_ASSERT(s_render_context && "init the renderer2d first!");

        s_render_context->beginRender();
        s_render_context->endRender();
    }

    void Renderer2D::drawQuad(
        const DirectX::SimpleMath::Vector2& position,
        const DirectX::SimpleMath::Vector2& size,
        const DirectX::SimpleMath::Color&   color,
        int32_t                             tex_index) {
        ZE_ASSERT(s_render_context && "init the renderer2d first!");

        drawQuad({position.x, position.y, 0.0f}, size, color, tex_index);
    }

    void Renderer2D::drawQuad(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector2& size,
        const DirectX::SimpleMath::Color&   color,
        int32_t                             tex_index) {
        ZE_ASSERT(s_render_context && "init the renderer2d first!");

        Matrix transform = Matrix::CreateScale(size.x, size.y, 1.0f) * Matrix::CreateTranslation(position);

        static Chen::CDX12::Mesh* mesh = MeshTable::getInstance().getMesh("square");

        ZE_ASSERT(mesh && "the square mesh retrieve failure for unknown error(drawQuad)!");

        s_render_context->submit(mesh, transform, color, tex_index);
    }
} // namespace Zero