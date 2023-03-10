#pragma once

#include <CDX12/Resource/Mesh.h>

#include "runtime/resource/resource.h"

namespace Zero {
    struct VertexData2D;

    template <>
    class Resource<ResourceType::Mesh> final : public IResource {
    public:
        struct ResLoader final {
            using res_type = Scope<Resource<ResourceType::Mesh>>;

            res_type operator()(std::string_view name) const {
                return CreateScope<Resource<ResourceType::Mesh>>(name);
            }

            res_type operator()(std::string_view name,
                                VertexData2D*    vertices,
                                uint32_t         vertices_count,
                                uint32_t*        indices,
                                uint32_t         indices_count) const {
                return CreateScope<Resource<ResourceType::Mesh>>(name, vertices, vertices_count, indices, indices_count);
            }
        };

    public:
        Resource(std::string_view name);
        Resource(std::string_view name,
                 VertexData2D*    vertices,
                 uint32_t         vertices_count,
                 uint32_t*        indices,
                 uint32_t         indices_count);

        bool validate() const { return m_is_valid; }

        Chen::CDX12::Mesh* getMesh() { return m_mesh.get(); }

    private:
        Scope<Chen::CDX12::Mesh> m_mesh;
    };
} // namespace Zero