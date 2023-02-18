#include <CDX12/Metalib.h>
#include <CDX12/Resource/Mesh.h>
#include <CDX12/Resource/UploadBuffer.h>

#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"

using namespace Chen::CDX12;
using namespace DirectX;

namespace Zero {
    /*
        VertexBuffer
    */

    VertexBuffer::VertexBuffer(float* vertices, uint32_t byte_size) {
        static VertexBufferLayout buffer_layout;

        size_t             VERTEX_COUNT = byte_size / sizeof(float);
        std::vector<vbyte> vertex_data(byte_size);
        vbyte*             vertex_dataPtr = vertex_data.data();
        for (size_t i = 0; i < VERTEX_COUNT; i += buffer_layout.structSize / sizeof(float)) {
            // position
            XMFLOAT3 position{vertices[i], vertices[i + 1], vertices[i + 2]};
            buffer_layout.position.Get(vertex_dataPtr) = position;
            // tex_coord
            XMFLOAT2 tex_coord(vertices[i + 3], vertices[i + 4]);
            buffer_layout.tex_coord.Get(vertex_dataPtr) = tex_coord;

            vertex_dataPtr += buffer_layout.structSize;
        }

        m_vertex_upload = new UploadBuffer(
            GET_RENDER_CONTEXT().getGraphicsDevice(),
            vertex_data.size());
        m_vertex_upload->CopyData(0, vertex_data);
    }

    VertexBuffer::~VertexBuffer() {
        delete m_vertex_upload;
    }

    void VertexBuffer::bind(ID3D12GraphicsCommandList* cmdlist, Mesh* mesh) const {
        cmdlist->CopyBufferRegion(
            mesh->VertexBuffers()[0].GetResource(),
            0,
            m_vertex_upload->GetResource(),
            0,
            m_vertex_upload->GetByteSize());
    }

    void VertexBuffer::unbind() const {
    }

    Zero::Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t byte_size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::NONE:
                ZE_ASSERT(false, "RendererAPI::NONE is not currently supported!");
            case RendererAPI::API::DX12:
                return Zero::CreateRef<VertexBuffer>(vertices, byte_size);
        }

        ZE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    /*
        IndexBuffer
    */

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t byte_size) :
        m_index_count(byte_size / 4) {
        m_index_upload = new UploadBuffer(
            GET_RENDER_CONTEXT().getGraphicsDevice(),
            byte_size);
        m_index_upload->CopyData(0, {reinterpret_cast<vbyte const*>(indices), byte_size});
    }

    IndexBuffer::~IndexBuffer() {
        delete m_index_upload;
    }

    void IndexBuffer::bind(ID3D12GraphicsCommandList* cmdlist, Mesh* mesh) const {
        cmdlist->CopyBufferRegion(
            mesh->IndexBuffer().GetResource(),
            0,
            m_index_upload->GetResource(),
            0,
            m_index_upload->GetByteSize());
    }

    void IndexBuffer::unbind() const {
    }

    Zero::Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t byte_size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::NONE:
                ZE_ASSERT(false, "RendererAPI::NONE is not currently supported!");
            case RendererAPI::API::DX12:
                return Zero::CreateRef<IndexBuffer>(indices, byte_size);
        }

        ZE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
} // namespace Zero