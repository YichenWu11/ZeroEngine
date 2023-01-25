#pragma once

#include <CDX12/Resource/Mesh.h>

namespace Zero {
    class RenderContext;

    class MeshTable {
    public:
        static MeshTable& getInstance() {
            static MeshTable instance;
            return instance;
        }

        void bindRenderContext(RenderContext* context) { m_render_context = context; }

        void buildBasicMesh();

        void registerMesh(const std::string&, float*, uint32_t*);

        Zero::Ref<Chen::CDX12::Mesh> getMesh(const std::string&);

    private:
        MeshTable()  = default;
        ~MeshTable() = default;

        RenderContext*                                                m_render_context;
        std::unordered_map<std::string, Zero::Ref<Chen::CDX12::Mesh>> m_mesh_table;
    };
} // namespace Zero