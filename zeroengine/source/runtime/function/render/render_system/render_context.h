#pragma once

#include <CDX12/CmdQueue.h>
#include <CDX12/DescripitorHeap/CPUDescriptorHeap.h>
#include <CDX12/DescripitorHeap/DescriptorHeapWrapper.h>
#include <CDX12/DescripitorHeap/GPUDescriptorHeap.h>
#include <CDX12/Device.h>
#include <CDX12/FrameResourceMngr.h>
#include <CDX12/Resource/ResourceStateTracker.h>
#include <CDX12/Util/BindProperty.h>

using Microsoft::WRL::ComPtr;

namespace Chen::CDX12 {
    class PSOManager;
}

namespace Zero {
    class RenderContext {
    public:
        RenderContext(HWND window_handle);
        ~RenderContext();

        void init(int width, int height);
        void swapBuffer();

        void beginRender();
        void endRender();

        void submitMesh(Chen::CDX12::Mesh* mesh) { m_draw_list.push_back(mesh); }

        ID3D12Device*              getGraphicsDevice() { return m_device.Get(); }
        ID3D12GraphicsCommandList* getCommandList() { return m_frameResourceMngr->GetCurrentFrameResource()->GetCmdList().Get(); }
        ID3D12CommandQueue*        getCommandQueue() { return m_commandQueue.Get(); }
        ID3D12Fence*               getFence() { return m_fence.Get(); }

        D3D12_CPU_DESCRIPTOR_HANDLE getImGuiInitCPUHandle() { return m_csuGpuDH.GetCpuHandle(); }
        D3D12_GPU_DESCRIPTOR_HANDLE getImGuiInitGPUHandle() { return m_csuGpuDH.GetGpuHandle(); }
        ID3D12DescriptorHeap*       getImGuiDH() { return m_csuGpuDH.GetDescriptorHeap(); }

    private:
        void populateCommandList(Chen::CDX12::FrameResource& frameRes, uint frameIndex);

    private:
        HWND               m_window_handle;
        static DXGI_FORMAT s_colorFormat;
        static DXGI_FORMAT s_depthFormat;

        static const uint32_t s_frame_count = 3; // triple buffering by default

        // Pipeline objects.
        CD3DX12_VIEWPORT        m_viewport;
        CD3DX12_RECT            m_scissorRect;
        ComPtr<IDXGISwapChain3> m_swapChain;

        Chen::CDX12::Device                   m_device;
        Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;

        Chen::CDX12::CmdQueue m_commandQueue;

        std::unique_ptr<Chen::CDX12::PSOManager>  psoManager;
        std::unique_ptr<Chen::CDX12::BasicShader> colorShader;

        std::vector<Chen::CDX12::BindProperty> bindProperties;

        std::unique_ptr<Chen::CDX12::Texture> m_renderTargets[s_frame_count];
        std::unique_ptr<Chen::CDX12::Texture> m_depthTargets[s_frame_count];

        // DescriptorHeap
        Chen::CDX12::DescriptorHeapAllocation m_rtvCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_dsvCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_csuCpuDH;
        Chen::CDX12::DescriptorHeapAllocation m_csuGpuDH;

        std::unique_ptr<Chen::CDX12::FrameResourceMngr> m_frameResourceMngr;

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

        std::vector<Chen::CDX12::Mesh*> m_draw_list;

        // debug
        ComPtr<ID3D12DebugDevice> debug_device;
    };
} // namespace Zero