#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Zero {
    struct Renderer2DData {};

    static Renderer2DData s_Data;

    void Renderer2D::init() {
    }

    void Renderer2D::shutdown() {
    }

    void Renderer2D::beginScene(const OrthographicsCamera& camera) {
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
        GET_RENDER_CONTEXT().beginRender();
        GET_RENDER_CONTEXT().endRender();
    }

    void Renderer2D::drawQuad(
        const DirectX::SimpleMath::Vector2& position,
        const DirectX::SimpleMath::Vector2& size,
        float                               rotation,
        const DirectX::SimpleMath::Color&   color,
        uint32_t                            tex_index,
        float                               tiling_factor) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, color, tex_index);
    }

    void Renderer2D::drawQuad(
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector2& size,
        float                               rotation,
        const DirectX::SimpleMath::Color&   color,
        uint32_t                            tex_index,
        float                               tiling_factor) {
        Matrix transform = Matrix::CreateRotationZ(XMConvertToRadians(-rotation))
                           * Matrix::CreateScale(size.x, size.y, 1.0f)
                           * Matrix::CreateTranslation(position);

        static Zero::Ref<Mesh> mesh = MeshTable::getInstance().getMesh("square");

        ZE_ASSERT(mesh && "the square mesh retrieve failure for unknown error(drawQuad)!");

        GET_RENDER_CONTEXT().submit(mesh.get(), transform, color, tex_index, tiling_factor);
    }
} // namespace Zero