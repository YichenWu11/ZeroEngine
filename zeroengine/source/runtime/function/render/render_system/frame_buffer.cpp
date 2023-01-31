#include "runtime/function/render/render_system/frame_buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    FrameBuffer::FrameBuffer(const FrameBufferConfiguration& config) :
        m_config(config) {
        onResize(config);
    }

    FrameBuffer::~FrameBuffer() {
    }

    void FrameBuffer::onResize(const FrameBufferConfiguration& config) {
        ID3D12Device* device = GET_RENDER_CONTEXT().getGraphicsDevice();

        m_inner_texture.reset();

        m_config        = config;
        m_inner_texture = Zero::CreateScope<Texture>(
            device,
            config.width,
            config.height,
            config.format,
            TextureDimension::Tex2D,
            1,
            1,
            Texture::TextureUsage::RenderTarget,
            D3D12_RESOURCE_STATE_COMMON);

        device->CreateRenderTargetView(m_inner_texture->GetResource(), nullptr, config.rtv_cpu_handle);

        D3D12_SHADER_RESOURCE_VIEW_DESC desc = m_inner_texture->GetColorSrvDesc(0);
        device->CreateShaderResourceView(
            m_inner_texture->GetResource(),
            &desc,
            config.srv_cpu_handle);
    }
} // namespace Zero