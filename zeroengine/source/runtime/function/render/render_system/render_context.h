#pragma once

/*
    NOTE: core render class
*/

#include <CDX12/CmdQueue.h>
#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Device.h>
#include <CDX12/FrameResourceMngr.h>
#include <CDX12/Resource/ResourceStateTracker.h>
#include <CDX12/Util/BindProperty.h>

using Microsoft::WRL::ComPtr;

namespace Chen::CDX12 {
    class PSOManager;
}

namespace Zero {
    struct ImGuiInitInfo {
        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
        D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle;
        ID3D12DescriptorHeap*       descriptor_heap;
    };

    class RenderContext {
    public:
        static RenderContext& getInstance() {
            static RenderContext instance;
            return instance;
        }

        void init(HWND window_handle, int width, int height);
        void swapBuffer();
        void shutdown();

        void setVsync(bool vsync) { m_is_vsync_enable = vsync; }

        void beginRender();
        void endRender();

        void submit(
            Chen::CDX12::Mesh*                 mesh,
            const DirectX::SimpleMath::Matrix& trans,
            const DirectX::SimpleMath::Color&  color,
            uint32_t                           tex_index,
            float                              tiling_factor) {
            m_draw_2d_list.emplace_back(
                std::make_tuple(mesh, trans, color, tex_index));
        }

        void flush() { flushCommandQueue(); }

        void onResize(int width, int height);

        ID3D12Device*       getGraphicsDevice() { return m_device.Get(); }
        ID3D12CommandQueue* getCommandQueue() { return m_commandQueue.Get(); }

        ImGuiInitInfo getImGuiInitInfo() {
            return {m_csuGpuDH.GetCpuHandle(), m_csuGpuDH.GetGpuHandle(), m_csuGpuDH.GetDescriptorHeap()};
        }

    private:
        RenderContext()  = default;
        ~RenderContext() = default;

        RenderContext(const RenderContext&)            = delete;
        RenderContext& operator=(const RenderContext&) = delete;

        void buildShaders();
        void buildTextures();

        void populateCommandList(Chen::CDX12::FrameResource& frameRes, uint frameIndex);
        void flushCommandQueue();

    private:
        bool m_is_vsync_enable{false};

        HWND                  m_window_handle;
        static DXGI_FORMAT    s_colorFormat;
        static DXGI_FORMAT    s_depthFormat;
        static const uint32_t s_frame_count = 3; // triple buffering by default

        Chen::CDX12::Device m_device;

        Chen::CDX12::CmdQueue m_commandQueue;

        CD3DX12_VIEWPORT m_viewport;
        CD3DX12_RECT     m_scissorRect;

        Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
        ComPtr<IDXGISwapChain3>               m_swapChain;

        Zero::Scope<Chen::CDX12::PSOManager> psoManager;

        std::vector<Chen::CDX12::BindProperty> bindProperties;

        Zero::Scope<Chen::CDX12::Texture> m_renderTargets[s_frame_count];
        Zero::Scope<Chen::CDX12::Texture> m_depthTargets[s_frame_count];

        // DescriptorHeap
        Chen::CDX12::DescriptorHeapAllocation m_rtvCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_dsvCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_csuCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_csuGpuDH;

        Zero::Scope<Chen::CDX12::FrameResourceMngr> m_frameResourceMngr;

        Chen::CDX12::ResourceStateTracker m_stateTracker;

        // Synchronization objects.
        uint32_t            m_backBufferIndex;
        ComPtr<ID3D12Fence> m_fence;
        uint64_t            m_fenceValue;

        // Initial Total Descriptor_Num
        uint32_t numCpuRTV         = 168;
        uint32_t numCpuDSV         = 168;
        uint32_t numCpuCSU         = 648;
        uint32_t numGpuCSU_static  = 648;
        uint32_t numGpuCSU_dynamic = 648;

        std::vector<std::tuple<Chen::CDX12::Mesh*, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Color, int32_t>> m_draw_2d_list;

        // debug
        ComPtr<ID3D12DebugDevice> debug_device;
    };
} // namespace Zero