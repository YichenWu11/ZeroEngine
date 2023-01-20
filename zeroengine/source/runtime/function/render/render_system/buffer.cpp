#include <CDX12/Metalib.h>
#include <CDX12/Resource/Mesh.h>
#include <CDX12/Resource/UploadBuffer.h>

#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/renderer.h"

using namespace Chen::CDX12;
using namespace DirectX;

namespace Zero {
    static VertexBufferLayout buffer_layout;

    /*
        VertexBuffer
    */

    VertexBuffer::VertexBuffer(ID3D12Device* device, XMFLOAT3* vertices, uint32_t size) {
        size_t             VERTEX_COUNT = size / sizeof(XMFLOAT3);
        std::vector<vbyte> vertex_data(buffer_layout.structSize * VERTEX_COUNT);
        vbyte*             vertex_dataPtr = vertex_data.data();
        for (size_t i = 0; i < VERTEX_COUNT; ++i) {
            XMFLOAT3 vert                              = vertices[i];
            buffer_layout.position.Get(vertex_dataPtr) = vert;
            XMFLOAT4 color(
                vert.x + 0.5f,
                vert.y + 0.5f,
                vert.z + 0.5f,
                1);
            buffer_layout.color.Get(vertex_dataPtr) = color;
            vertex_dataPtr += buffer_layout.structSize;
        }

        m_vertex_upload = new UploadBuffer(
            device,
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

    VertexBuffer* VertexBuffer::create(ID3D12Device* device, XMFLOAT3* vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::NONE:
                ZE_ASSERT(false && "RendererAPI::NONE is not currently supported!");
            case RendererAPI::API::DX12:
                return new VertexBuffer(device, vertices, size);
        }

        ZE_ASSERT(false && "Unknown RendererAPI");
        return nullptr;
    }

    /*
        IndexBuffer
    */

    IndexBuffer::IndexBuffer(ID3D12Device* device, uint32_t* indices, uint32_t size) :
        m_index_count(size / 4) {
        m_index_upload = new UploadBuffer(
            device,
            size);
        m_index_upload->CopyData(0, {reinterpret_cast<vbyte const*>(indices), size});
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

    IndexBuffer* IndexBuffer::create(ID3D12Device* device, uint32_t* indices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::NONE:
                ZE_ASSERT(false && "RendererAPI::NONE is not currently supported!");
            case RendererAPI::API::DX12:
                return new IndexBuffer(device, indices, size);
        }

        ZE_ASSERT(false && "Unknown RendererAPI");
        return nullptr;
    }
} // namespace Zero