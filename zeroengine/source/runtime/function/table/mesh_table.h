#pragma once

#include <CDX12/Resource/Mesh.h>

namespace Zero {
    class MeshTable {
    public:
        static MeshTable& getInstance() {
            static MeshTable instance;
            return instance;
        }

        void init() {}

        void buildBasicMesh();

        void registerMesh(const std::string&, float*, uint32_t*);

        Zero::Ref<Chen::CDX12::Mesh> getMesh(const std::string&);

    private:
        MeshTable()  = default;
        ~MeshTable() = default;

        std::unordered_map<std::string, Zero::Ref<Chen::CDX12::Mesh>> m_mesh_table;
    };
} // namespace Zero