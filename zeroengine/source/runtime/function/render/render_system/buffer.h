#pragma once

/*
    vertex_buffer and index_buffer here is not the real book of linear memory in GPU end.
    It is just a media layer to pass vertex and index data to mesh.
*/

#include <CDX12/Util/ReflactableStruct.h>

namespace Chen::CDX12 {
    class UploadBuffer;
    class Mesh;
} // namespace Chen::CDX12

namespace Zero {
    // struct VertexBufferLayout : public Chen::CDX12::rtti::Struct {
    //     Chen::CDX12::rtti::Var<DirectX::XMFLOAT3> position  = "POSITION";
    //     Chen::CDX12::rtti::Var<DirectX::XMFLOAT3> normal    = "NORMAL";
    //     Chen::CDX12::rtti::Var<DirectX::XMFLOAT3> tex_coord = "TEXCOORD";
    //     Chen::CDX12::rtti::Var<DirectX::XMFLOAT4> tangent   = "TANGENT";
    // };

    struct VertexBufferLayout : public Chen::CDX12::rtti::Struct {
        Chen::CDX12::rtti::Var<DirectX::XMFLOAT3> position  = "POSITION";
        Chen::CDX12::rtti::Var<DirectX::XMFLOAT2> tex_coord = "TEXCOORD";
    };

    class VertexBuffer {
    public:
        VertexBuffer(ID3D12Device* device, float* vertices, uint32_t size);
        ~VertexBuffer();

        void bind(ID3D12GraphicsCommandList* cmdlist, Chen::CDX12::Mesh*) const;
        void unbind() const;

        static Zero::Ref<VertexBuffer> create(ID3D12Device* device, float* vertices, uint32_t size);

    private:
        Chen::CDX12::UploadBuffer* m_vertex_upload;
    };

    class IndexBuffer {
    public:
        IndexBuffer(ID3D12Device* device, uint32_t* indices, uint32_t size);
        ~IndexBuffer();

        void     bind(ID3D12GraphicsCommandList* cmdlist, Chen::CDX12::Mesh*) const;
        void     unbind() const;
        uint32_t getCount() const { return m_index_count; }

        static Zero::Ref<IndexBuffer> create(ID3D12Device* device, uint32_t* indices, uint32_t size);

    private:
        uint32_t                   m_index_count;
        Chen::CDX12::UploadBuffer* m_index_upload;
    };
} // namespace Zero