#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Material/Texture.h>

namespace Zero {
    class RenderContext;

    struct TextureInitInfo {
        std::wstring                       path;
        std::string                        name;
        Chen::CDX12::TextureDimension      dimension{Chen::CDX12::TextureDimension::Tex2D};
        uint32_t                           width{100};
        uint32_t                           height{100};
        DXGI_FORMAT                        format{DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM_SRGB};
        uint32_t                           depth{1};
        uint32_t                           mip{1};
        Chen::CDX12::Texture::TextureUsage usage{Chen::CDX12::Texture::TextureUsage::GenericColor};
        D3D12_RESOURCE_STATES              resourceState{D3D12_RESOURCE_STATE_COMMON};
    };

    class TextureTable {
    public:
        enum class TexFileFormat : uint8_t { WIC = 0,
                                             DDS };

    public:
        static TextureTable& getInstance() {
            static TextureTable instance;
            return instance;
        }

        void bindRenderContext(RenderContext* context);

        void registerTex(const std::filesystem::path&, TexFileFormat = TexFileFormat::WIC);
        void registerTex(const TextureInitInfo&, TexFileFormat = TexFileFormat::WIC);

        uint32_t getTexIndexFromName(const std::string&);

        Chen::CDX12::DescriptorHeapAllocation* getTexAllocation() { return &m_tex_alloc; }

    private:
        TextureTable() = default;
        ~TextureTable();

    private:
        RenderContext*                        m_render_context{nullptr};
        Chen::CDX12::DescriptorHeapAllocation m_tex_alloc;

        std::map<std::string, uint32_t>                  m_texname2index;
        std::map<std::string, Ref<Chen::CDX12::Texture>> m_texture_table;
    };

} // namespace Zero