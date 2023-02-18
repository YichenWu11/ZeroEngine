#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Material/Texture.h>

#include "runtime/core/util/singleton.h"

#define GET_TEXTURE_POOL() ::Zero::TexturePool::getInstance()

namespace Zero {
    struct TextureBuildInfo {
        std::wstring                       path;
        std::string                        name;
        Chen::CDX12::TextureDimension      dimension{Chen::CDX12::TextureDimension::Tex2D};
        uint32_t                           width{100};
        uint32_t                           height{100};
        DXGI_FORMAT                        format{DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM};
        uint32_t                           depth{1};
        uint32_t                           mip{1};
        Chen::CDX12::Texture::TextureUsage usage{Chen::CDX12::Texture::TextureUsage::GenericColor};
        D3D12_RESOURCE_STATES              resourceState{D3D12_RESOURCE_STATE_COMMON};
    };

    class TexturePool : public Singleton<TexturePool> {
        friend class Singleton<TexturePool>;

    public:
        enum class TexFileFormat : uint8_t { WIC = 0,
                                             DDS };

        enum class TexType : uint8_t {
            COMMON = 0,
            ICON   = 1
        };

    public:
        void
        init();

        void registerTex(const std::filesystem::path& path, TexFileFormat = TexFileFormat::WIC);
        void registerTex(std::string_view tex_name, const std::filesystem::path& path, TexFileFormat = TexFileFormat::WIC);
        void registerTex(const TextureBuildInfo& build_info, TexFileFormat = TexFileFormat::WIC);

        Zero::Ref<Chen::CDX12::Texture> getTextureFromName(std::string_view);
        uint32_t                        getTexIndexFromName(std::string_view);
        uint32_t                        getTexIndex(const Ref<Chen::CDX12::Texture>&);
        std::string                     getTextureName(const Ref<Chen::CDX12::Texture>&);

        Chen::CDX12::DescriptorHeapAllocation* getTexAllocation() { return &m_tex_alloc; }

        ImTextureID getImTextureID(uint32_t tex_idx) { return ImTextureID(m_tex_alloc.GetGpuHandle(tex_idx).ptr); }

    private:
        ~TexturePool();

    private:
        Chen::CDX12::DescriptorHeapAllocation m_tex_alloc;

        std::map<std::string, uint32_t>                  m_texname2index;
        std::map<std::string, Ref<Chen::CDX12::Texture>> m_texture_pool;

    private:
        static uint32_t s_invalid_index;
    };

} // namespace Zero