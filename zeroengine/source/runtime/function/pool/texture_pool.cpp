#include <CDX12/DescriptorHeapMngr.h>
#include <DDSTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <WICTextureLoader.h>

#include "runtime/function/pool/texture_pool.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    uint32_t TexturePool::s_invalid_index = -1;

    TexturePool::~TexturePool() {
        if (!m_tex_alloc.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Free(std::move(m_tex_alloc));
    }

    void TexturePool::init() {
        m_tex_alloc = DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Allocate(168);
        registerTex(
            std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/white.png"); // default texture
    }

    void TexturePool::registerTex(const std::filesystem::path& tex_path, TexFileFormat file_format) {
        if (!std::filesystem::exists(tex_path)) {
            LOG_ERROR("The file with this path ({0}) does not exsit!(in registerTex)", tex_path.string());
            return;
        }

        registerTex(tex_path.stem().string(), tex_path, file_format);
    }

    void TexturePool::registerTex(const std::string& name, const std::filesystem::path& tex_path, TexFileFormat file_format) {
        if (!std::filesystem::exists(tex_path)) {
            LOG_ERROR("The file with this path ({0}) does not exsit!(in registerTex)", tex_path.string());
            return;
        }

        TextureBuildInfo build_info{AnsiToWString(tex_path.string()).c_str(), name};
        registerTex(build_info, file_format);
    }

    void TexturePool::registerTex(
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
        m_texture_table[info.name] = tex;

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = m_texture_table[info.name]->GetColorSrvDesc(0);
        device->CreateShaderResourceView(
            m_texture_table[info.name]->GetResource(),
            &desc,
            m_tex_alloc.GetCpuHandle(m_texture_table.size() - 1));

        auto uploadResourcesFinished = resourceUpload.End(GET_RENDER_CONTEXT().getCommandQueue());
        uploadResourcesFinished.wait();

        LOG_INFO("register texture named {0} success!", info.name);
    }

    Zero::Ref<Texture> TexturePool::getTextureFromName(const std::string& tex_name) {
        if (!m_texture_table.contains(tex_name)) {
            LOG_WARN("The texture with this name({0}) dose not exsit!", tex_name);
            return {};
        }

        return m_texture_table[tex_name];
    }

    uint32_t TexturePool::getTexIndexFromName(const std::string& tex_name) {
        if (!m_texture_table.contains(tex_name)) {
            LOG_WARN("The texture with this name({0}) does not exsit!", tex_name);
            return s_invalid_index;
        }

        return m_texname2index[tex_name];
    }

    uint32_t TexturePool::getTexIndex(const Ref<Chen::CDX12::Texture>& target) {
        for (auto& [name, texture] : m_texture_table) {
            if (texture == target)
                return getTexIndexFromName(name);
        }
        LOG_WARN("The texture does not exsit!(getTexIndex)");
        return s_invalid_index;
    }

    std::string TexturePool::getTextureName(const Ref<Chen::CDX12::Texture>& target) {
        for (auto& [name, texture] : m_texture_table) {
            if (texture == target)
                return name;
        }
        LOG_WARN("The texture does not exsit!(getTextureName)");
        return {};
    }
} // namespace Zero