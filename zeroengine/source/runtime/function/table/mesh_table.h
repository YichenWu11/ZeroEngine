#pragma once

#include <CDX12/Resource/Mesh.h>

namespace Chen::CDX12 {
    class FrameResource;
}

#define GET_MESH_TABLE() ::Zero::MeshTable::getInstance()

namespace Zero {
    struct VertexData2D;

    class MeshTable {
    public:
        static MeshTable& getInstance() {
            static MeshTable instance;
            return instance;
        }

        void init() { buildBasicMesh(); }

        void buildBasicMesh();

        void registerMesh(
            const std::string&,
            VertexData2D* vertices,
            uint32_t      vertices_count,
            uint32_t*     indices,
            uint32_t      indices_count);

        void removeMesh(const std::string&);

        void disposeTempMeshes();
        void delayDisposeMesh(const std::string&, Chen::CDX12::FrameResource* frameres);

        Zero::Ref<Chen::CDX12::Mesh> getMesh(const std::string&);
        std::string                  getMeshName(const Zero::Ref<Chen::CDX12::Mesh>&);
        std::string                  getMeshName(Chen::CDX12::Mesh*);

        bool isMeshExsit(const std::string& name) { return m_mesh_table.contains(name); }

    private:
        MeshTable()  = default;
        ~MeshTable() = default;

    private:
        std::unordered_map<std::string, Zero::Ref<Chen::CDX12::Mesh>> m_mesh_table;

        ComPtr<ID3D12CommandAllocator>    m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
    };
} // namespace Zero