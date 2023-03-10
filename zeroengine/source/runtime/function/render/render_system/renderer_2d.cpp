#include <CDX12/Resource/Mesh.h>

#include "runtime/core/base/application.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer_2d.h"
#include "runtime/function/render/render_system/renderer_api.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"

using namespace Chen::CDX12;
using namespace DirectX;

namespace Zero {
    struct Renderer2DData {};

    static Renderer2DData s_Data;

    void Renderer2D::init() {
    }

    void Renderer2D::shutdown() {
    }

    void Renderer2D::beginScene(const Camera& camera, const Matrix& cam_transform) {
        beginScene(cam_transform.Invert() * camera.getProjection());
    }

    void Renderer2D::beginScene(const EditorCamera& camera) {
        beginScene(camera.getViewProjection());
    }

    void Renderer2D::beginScene(const OrthographicsCamera& camera) {
        beginScene(camera.getViewProjectionMatrix());
    }

    void Renderer2D::beginScene(const Matrix& view_proj) {
        // takes all the scene settings(camera, lights, environment etc)
        BasicShader* shader =
            static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("transparent"));

        static Matrix view_proj_matrix;
        view_proj_matrix = view_proj.Transpose();

        GET_SHADER_BIND_TABLE().bindParam(
            shader,
            "_PassConstant",
            std::span<const uint8_t>{
                reinterpret_cast<uint8_t const*>(&view_proj_matrix),
                sizeof(view_proj_matrix)});

        GET_SHADER_BIND_TABLE().bindParam(
            shader,
            "TextureMap",
            std::make_pair(&RenderContext::getInstance().getTexAlloc(), 0));
    }

    void Renderer2D::endScene() {
        RenderContext::getInstance().onRender();
    }

    void Renderer2D::drawQuad(
        const Vector2& position,
        const Vector2& size,
        float          rotation,
        const Color&   color,
        uint32_t       tex_index,
        float          tiling_factor) {
        drawQuad({position.x, position.y, 0.0f}, size, rotation, color, tex_index);
    }

    void Renderer2D::drawQuad(
        const Vector3& position,
        const Vector2& size,
        float          rotation,
        const Color&   color,
        uint32_t       tex_index,
        float          tiling_factor) {
        Matrix transform = Matrix::CreateRotationZ(XMConvertToRadians(-rotation))
                           * Matrix::CreateScale(size.x, size.y, 1.0f)
                           * Matrix::CreateTranslation(position);

        drawQuad(transform, color, tex_index, tiling_factor);
    }

    void Renderer2D::drawQuad(
        const Matrix& transform,
        const Color&  color,
        uint32_t      tex_index,
        float         tiling_factor,
        int           entity_id) {
        static auto square_mesh =
            Application::get().getResourceMngr()->get<ResourceType::Mesh>("square");

        ASSERT(square_mesh, "the square mesh retrieve failure for unknown error(drawQuad)!");

        RenderContext::getInstance().submit(square_mesh->getMesh(), transform, color, tex_index, tiling_factor, entity_id);
    }

    void Renderer2D::drawSprite(
        const Matrix&          transform,
        const SpriteComponent& src,
        int                    entity_id) {
        drawQuad(transform, src.color, src.tex_index, src.tiling_factor, entity_id);
    }

    void Renderer2D::drawTile(
        const Matrix&        transform,
        const TileComponent& tc,
        int                  entity_id) {
        Tile tile(tc.tile_sheet,
                  Vector2{float(tc.coord_x), float(tc.coord_y)},
                  Vector2{float(tc.size_x), float(tc.size_y)});

        RenderContext::getInstance().submit(
            Application::get().getResourceMngr()->get<ResourceType::Mesh>(tile.getTileName())->getMesh(),
            transform,
            tc.color,
            Application::get().getResourceMngr()->index<ResourceType::Texture>(tile.getTileSheet()->getName()),
            1.0f,
            entity_id);
    }
} // namespace Zero