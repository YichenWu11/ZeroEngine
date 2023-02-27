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

        ZE_ASSERT(square_mesh, "the square mesh retrieve failure for unknown error(drawQuad)!");

        RenderContext::getInstance().submit(square_mesh->getMesh(), transform, color, tex_index, tiling_factor, entity_id);
    }

    void Renderer2D::drawCellQuad(
        const Vector2&           position,
        const Vector2&           size,
        float                    rotation,
        const Ref<SubTexture2D>& sub_texture,
        const Color&             color) {
        drawCellQuad({position.x, position.y, 0.0f}, size, rotation, sub_texture, color);
    }

    void Renderer2D::drawCellQuad(
        const Vector3&           position,
        const Vector2&           size,
        float                    rotation,
        const Ref<SubTexture2D>& sub_texture,
        const Color&             color) {
        // if (!GET_MESH_POOL().isMeshExist(sub_texture->constructSubTexName())) {
        //     std::vector<VertexData2D> vertices;
        //     uint32_t                  indices[]  = {0, 3, 1, 3, 2, 1};
        //     auto                      tex_coords = sub_texture->getTexCoords();
        //     vertices.push_back(
        //         VertexData2D{{-0.5f, -0.5f, 0.0f}, {tex_coords[0].x, 1.0f - tex_coords[0].y}});
        //     vertices.push_back(
        //         VertexData2D{{0.5f, -0.5f, 0.0f}, {tex_coords[1].x, 1.0f - tex_coords[1].y}});
        //     vertices.push_back(
        //         VertexData2D{{0.5f, 0.5f, 0.0f}, {tex_coords[2].x, 1.0f - tex_coords[2].y}});
        //     vertices.push_back(
        //         VertexData2D{{-0.5f, 0.5f, 0.0f}, {tex_coords[3].x, 1.0f - tex_coords[3].y}});

        //     GET_MESH_POOL().registerMesh(
        //         sub_texture->constructSubTexName(),
        //         vertices.data(),
        //         vertices.size(),
        //         indices,
        //         6);
        // }

        // Matrix transform = Matrix::CreateRotationZ(XMConvertToRadians(-rotation))
        //                    * Matrix::CreateScale(size.x, size.y, 1.0f)
        //                    * Matrix::CreateTranslation(position);

        // RenderContext::getInstance().submit(
        //     GET_MESH_POOL().getMesh(sub_texture->constructSubTexName()),
        //     transform,
        //     color,
        //     0, /* get the index of texture */
        //     1.0f, -1);
    }

    void Renderer2D::drawSprite(
        const Matrix&          transform,
        const SpriteComponent& src,
        int                    entity_id) {
        drawQuad(transform, src.color, src.tex_index, src.tiling_factor, entity_id);
    }
} // namespace Zero