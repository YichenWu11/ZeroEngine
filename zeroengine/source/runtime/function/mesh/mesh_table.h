#pragma once

namespace Chen::CDX12 {
    class Mesh;
}

namespace Zero {
    class RenderContext;

    class MeshTable {
    public:
        static void registerMesh(RenderContext*, const std::string&);

        static Chen::CDX12::Mesh* getMesh(const std::string&);

    private:
        static std::unordered_map<std::string, std::unique_ptr<Chen::CDX12::Mesh>> s_mesh_table;
    };
} // namespace Zero