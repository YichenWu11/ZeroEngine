#pragma once

#include <CDX12/Material/Texture.h>

namespace Zero {
    class FrameBuffer;

    struct FrameBufferConfiguration {
        uint32_t                    width;
        uint32_t                    height;
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_cpu_handle;
        D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle;
        DXGI_FORMAT                 format{DXGI_FORMAT_R8G8B8A8_UNORM};
        uint32_t                    samples{1};
        bool                        swapchain_target{false};
    };

    class FrameBuffer {
    public:
        FrameBuffer(const FrameBufferConfiguration& config);
        ~FrameBuffer();

        FrameBufferConfiguration getConfiguration() const { return m_config; }
        Chen::CDX12::Texture*    getInnerTexture() { return m_inner_texture.get(); }

        void onResize(const FrameBufferConfiguration& config);

    private:
        FrameBufferConfiguration          m_config;
        Zero::Scope<Chen::CDX12::Texture> m_inner_texture;
    };
} // namespace Zero