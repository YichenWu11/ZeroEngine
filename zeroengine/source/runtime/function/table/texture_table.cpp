#include <CDX12/DescriptorHeapMngr.h>
#include <DDSTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <WICTextureLoader.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;

namespace Zero {
    TextureTable::~TextureTable() {
        if (!m_tex_alloc.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Free(std::move(m_tex_alloc));
    }

    void TextureTable::bindRenderContext(RenderContext* context) {
        m_render_context = context;
        m_tex_alloc      = DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Allocate(168);
    }

    void TextureTable::registerTex(
        const TextureInitInfo& info,
        TexFileFormat          file_format) {
        ZE_ASSERT(m_render_context && "Bind the device first(TextureTable)!");

        if (m_textures.contains(info.name)) {
            LOG_WARN("The texture with this name({0}) has exsited!", info.name);
            return;
        }

        ID3D12Device* device = m_render_context->getGraphicsDevice();
        auto          tex    = std::make_unique<Texture>(
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

        m_textures[info.name] = std::move(tex);

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = m_textures[info.name]->GetColorSrvDesc(0);
        device->CreateShaderResourceView(
            m_textures[info.name]->GetResource(),
            &desc,
            m_tex_alloc.GetCpuHandle(m_textures.size() - 1));

        auto uploadResourcesFinished = resourceUpload.End(m_render_context->getCommandQueue());
        uploadResourcesFinished.wait();
    }
} // namespace Zero