#pragma once

#include <CDX12/Resource/Mesh.h>

#include "runtime/core/util/singleton.h"

namespace Chen::CDX12 {
    class FrameResource;
}

#define GET_MESH_POOL() ::Zero::MeshPool::getInstance()

namespace Zero {
    struct VertexData2D;

    class MeshPool : public Singleton<MeshPool> {
    public:
        void init() { buildBasicMesh(); }

        void registerMesh(
            std::string_view mesh_name,
            VertexData2D*    vertices,
            uint32_t         vertices_count,
            uint32_t*        indices,
            uint32_t         indices_count);

        void removeMesh(std::string_view mesh_name);

        void delayDisposeMesh(std::string_view mesh_name, Chen::CDX12::FrameResource* frameres);

        Ref<Chen::CDX12::Mesh> getMesh(std::string_view mesh_name);
        std::string            getMeshName(const Ref<Chen::CDX12::Mesh>&);

        bool isMeshExist(std::string_view name) const { return m_mesh_pool.contains(std::string(name)); }

    private:
        void buildBasicMesh();

    private:
        std::map<std::string, Ref<Chen::CDX12::Mesh>> m_mesh_pool;

        ComPtr<ID3D12CommandAllocator>    m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
    };
} // namespace Zero