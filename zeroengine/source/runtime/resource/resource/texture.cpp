#include <CDX12/DXUtil.h>
#include <DDSTextureLoader.h>
#include <ResourceUploadBatch.h>
#include <WICTextureLoader.h>

#include "runtime/core/base/application.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/resource/resource/texture.h"

using namespace Chen::CDX12;

namespace Zero {
    uint32_t Resource<ResourceType::Texture>::s_tex_count = 0;

    Resource<ResourceType::Texture>::ResLoader::res_type Resource<ResourceType::Texture>::ResLoader::operator()(const std::filesystem::path& path) const {
        rapidjson::Document doc;
        JsonSerialzer::deserialze(doc, path);
        auto        res_path = (doc.HasMember("is_engine_internel") && doc["is_engine_internel"].GetBool()) ?
                                   Application::get().getConfigMngr()->getAssetFolder() / doc["path"].GetString() :
                                   Application::get().getConfigMngr()->getGameAssetFolder() / doc["path"].GetString();
        TextureDesc desc{
            res_path.string(),
            (doc["is_wip"].GetBool()) ? TexFileFormat::WIC : TexFileFormat::DDS};

        return CreateScope<Resource<ResourceType::Texture>>(
            (doc.HasMember("name")) ? doc["name"].GetString() : res_path.stem().string(),
            desc);
    }

    Resource<ResourceType::Texture>::ResLoader::res_type Resource<ResourceType::Texture>::ResLoader::operator()(std::string_view name, const std::filesystem::path& path) const {
        rapidjson::Document doc;
        JsonSerialzer::deserialze(doc, path);
        auto        res_path = (std::filesystem::path(ZE_XSTR(ROOT_DIR)) / "asset" / doc["path"].GetString());
        TextureDesc desc{
            res_path.string(),
            (doc["is_wip"].GetBool()) ? TexFileFormat::WIC : TexFileFormat::DDS};

        return CreateScope<Resource<ResourceType::Texture>>(name, desc);
    }

    Resource<ResourceType::Texture>::Resource(std::string_view name) :
        IResource(name) {
    }

    Resource<ResourceType::Texture>::Resource(std::string_view name, const TextureDesc& tex_desc) :
        IResource(name) {
        m_is_valid = true;

        DXRawDevicePtr device = RenderContext::getInstance().getGraphicsDevice();
        m_texture             = CreateScope<Texture>(
            device,
            tex_desc.width,
            tex_desc.height,
            tex_desc.format,
            tex_desc.dimension,
            tex_desc.depth,
            tex_desc.mip,
            tex_desc.usage,
            tex_desc.resourceState);

        DirectX::ResourceUploadBatch resourceUpload(device);

        resourceUpload.Begin();

        switch (tex_desc.file_format) {
            case TexFileFormat::DDS: {
                ThrowIfFailed(DirectX::CreateDDSTextureFromFile(
                    device,
                    resourceUpload,
                    AnsiToWString(tex_desc.path).c_str(),
                    m_texture->ReleaseAndGetAddress()));

                break;
            }
            case TexFileFormat::WIC: {
                ThrowIfFailed(DirectX::CreateWICTextureFromFile(
                    device,
                    resourceUpload,
                    AnsiToWString(tex_desc.path).c_str(),
                    m_texture->ReleaseAndGetAddress()));

                break;
            }
            default:
                break;
        }

        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = m_texture->GetColorSrvDesc(0);
        device->CreateShaderResourceView(
            m_texture->GetResource(),
            &srv_desc,
            RenderContext::getInstance().getTexAlloc().GetCpuHandle(s_tex_count));

        auto uploadResourcesFinished = resourceUpload.End(RenderContext::getInstance().getCommandQueue());
        uploadResourcesFinished.wait();

        LOG_INFO("register texture named {0} success!", name);
        s_tex_count++;
    }

} // namespace Zero
