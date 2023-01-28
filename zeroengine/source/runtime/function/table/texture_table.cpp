#include <CDX12/DescriptorHeapMngr.h>
#include <DDSTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <WICTextureLoader.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;

namespace Zero {
    uint32_t TextureTable::s_invalid_index = -1;

    TextureTable::~TextureTable() {
        if (!m_tex_alloc.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Free(std::move(m_tex_alloc));
    }

    void TextureTable::init() {
        m_tex_alloc = DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Allocate(168);
    }

    void TextureTable::registerTex(const std::string& name, const std::filesystem::path& tex_path, TexFileFormat file_format) {
        if (!std::filesystem::exists(tex_path)) {
            LOG_ERROR("The file with this path ({0}) does not exsit!(in registerTex)", tex_path.string());
            return;
        }

        TextureBuildInfo init_info{AnsiToWString(tex_path.string()).c_str(), name};
        registerTex(init_info, file_format);
    }

    void TextureTable::registerTex(const std::filesystem::path& tex_path, TexFileFormat file_format) {
        if (!std::filesystem::exists(tex_path)) {
            LOG_ERROR("The file with this path ({0}) does not exsit!(in registerTex)", tex_path.string());
            return;
        }

        TextureBuildInfo init_info{AnsiToWString(tex_path.string()).c_str(), tex_path.stem().string()};
        registerTex(init_info, file_format);
    }

    void TextureTable::registerTex(
        const TextureBuildInfo& info,
        TexFileFormat           file_format) {
        if (m_texture_table.contains(info.name)) {
            LOG_WARN("The texture with this name({0}) has exsited!", info.name);
            return;
        }

        ID3D12Device* device = GET_RENDER_CONTEXT().getGraphicsDevice();
        auto          tex    = Zero::CreateRef<Texture>(
            device,
            info.width,
            info.height,
            info.format,
            info.dimension,
            info.depth,
            info.mip,
            info.usage,
            info.resourceState);

        DirectX::ResourceUploadBatch resourceUpload(device);

        resourceUpload.Begin();

        switch (file_format) {
            case TexFileFormat::DDS: {
                ThrowIfFailed(DirectX::CreateDDSTextureFromFile(
                    device,
                    resourceUpload,
                    info.path.c_str(),
                    tex->ReleaseAndGetAddress()));

                break;
            }
            case TexFileFormat::WIC: {
                ThrowIfFailed(DirectX::CreateWICTextureFromFile(
                    device,
                    resourceUpload,
                    info.path.c_str(),
                    tex->ReleaseAndGetAddress()));

                break;
            }
            default:
                break;
        }

        m_texname2index[info.name] = m_texture_table.size();
        m_texture_table[info.name] = std::move(tex);

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = m_texture_table[info.name]->GetColorSrvDesc(0);
        device->CreateShaderResourceView(
            m_texture_table[info.name]->GetResource(),
            &desc,
            m_tex_alloc.GetCpuHandle(m_texture_table.size() - 1));

        auto uploadResourcesFinished = resourceUpload.End(GET_RENDER_CONTEXT().getCommandQueue());
        uploadResourcesFinished.wait();

        LOG_INFO("register Mesh named {0} success!", info.name);
    }

    Zero::Ref<Texture> TextureTable::getTextureFromName(const std::string& tex_name) {
        if (!m_texture_table.contains(tex_name)) {
            LOG_WARN("The texture with this name({0}) dose not exsit!", tex_name);
            return {};
        }

        return m_texture_table[tex_name];
    }

    uint32_t TextureTable::getTexIndexFromName(const std::string& tex_name) {
        if (!m_texture_table.contains(tex_name)) {
            LOG_WARN("The texture with this name({0}) dose not exsit!", tex_name);
            return s_invalid_index;
        }

        return m_texname2index[tex_name];
    }

    uint32_t TextureTable::getTexIndex(const Ref<Chen::CDX12::Texture>& target) {
        for (auto& [name, texture] : m_texture_table) {
            if (texture == target)
                return getTexIndexFromName(name);
        }
        return s_invalid_index;
    }
} // namespace Zero