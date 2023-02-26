#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Resource/Texture.h>

#include "runtime/resource/resource.h"

namespace Zero {
    enum class TexFileFormat : uint8_t { WIC = 0,
                                         DDS };

    struct TextureDesc {
        std::string                        path;
        TexFileFormat                      file_format{TexFileFormat::WIC};
        Chen::CDX12::TextureDimension      dimension{Chen::CDX12::TextureDimension::Tex2D};
        uint32_t                           width{100};
        uint32_t                           height{100};
        DXGI_FORMAT                        format{DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM};
        uint32_t                           depth{1};
        uint32_t                           mip{1};
        Chen::CDX12::Texture::TextureUsage usage{Chen::CDX12::Texture::TextureUsage::GenericColor};
        D3D12_RESOURCE_STATES              resourceState{D3D12_RESOURCE_STATE_COMMON};
    };

    template <>
    class Resource<ResourceType::Texture> final : public IResource {
    public:
        struct ResLoader final {
            using res_type = Scope<Resource<ResourceType::Texture>>;

            res_type operator()(const std::filesystem::path& path) const {
                TextureDesc desc{path.string()};
                return CreateScope<Resource<ResourceType::Texture>>(path.stem().string(), desc);
            }

            res_type operator()(std::string_view name, const std::filesystem::path& path) const {
                TextureDesc desc{path.string()};
                return CreateScope<Resource<ResourceType::Texture>>(name, desc);
            }
        };

    public:
        Resource(std::string_view name);
        Resource(std::string_view name, const TextureDesc& tex_desc);

        bool validate() const { return m_is_valid; }

        Chen::CDX12::Texture* getTexture() { return m_texture.get(); }

    private:
        bool                        m_is_valid = false;
        Scope<Chen::CDX12::Texture> m_texture;

    private:
        static uint32_t s_tex_count;
    };
} // namespace Zero
