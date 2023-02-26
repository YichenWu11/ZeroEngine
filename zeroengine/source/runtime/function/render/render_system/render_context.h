#pragma once

#include "runtime/function/render/render_system/dx.h"

#include "runtime/core/util/singleton.h"
#include "runtime/function/render/render_system/frame_buffer.h"
#include "runtime/function/render/render_system/pass/2d/main_camera_pass_2d.h"
#include "runtime/function/render/render_system/pass/ui_pass.h"

using Chen::CDX12::DescriptorHeapAllocation;
using Chen::CDX12::Mesh;
using Chen::CDX12::Texture;

#define GET_RENDER_CONTEXT() ::Zero::RenderContext::getInstance()

namespace Zero {
    struct ImGuiInitInfo {
        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle;
        D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle;
        DXRawDescHeapPtr            descriptor_heap;
    };

    class RenderContext : public Singleton<RenderContext> {
        friend class MainCameraPass2D;
        friend class UIPass;

    public:
        void init(HWND window_handle, int width, int height);
        void shutdown();
        void swapBuffer();

        void registerRenderPass();

        void onRender();

        void submit(
            Mesh*         mesh,
            const Matrix& trans,
            const Color&  color,
            uint32_t      tex_index,
            float         tiling_factor,
            int           entity_id) {
            ObjectConstant2D obj_constant;
            obj_constant.transform     = trans.Transpose();
            obj_constant.modulate      = color;
            obj_constant.tex_index     = tex_index;
            obj_constant.tiling_factor = tiling_factor;
            obj_constant.entity_id     = entity_id;
            m_draw_2d_list.emplace_back(std::make_tuple(mesh, obj_constant));
        }

        void flush() { flushCommandQueue(); }
        void onResize(uint32_t width, uint32_t height);
        void resizeFrameBuffer(int width, int height);

        void setVsync(bool vsync) { m_is_vsync_enable = vsync; }

        Chen::CDX12::FrameResource& getCurrFrameResource() { return *m_frameResourceMngr->GetCurrentFrameResource(); }
        FrameBufferConfiguration    getFrameBufferConfig() { return m_frameBuffers[0]->getConfiguration(); }
        DXRawDevicePtr              getGraphicsDevice() { return m_device.Get(); }
        DXRawCmdQueuePtr            getCommandQueue() { return m_commandQueue.Get(); }
        ImGuiInitInfo               getImGuiInitInfo() {
                          return {m_csuGpuDH.GetCpuHandle(), m_csuGpuDH.GetGpuHandle(), m_csuGpuDH.GetDescriptorHeap()};
        }
        ImTextureID               getOffScreenID() { return ImTextureID(m_csuGpuDH.GetGpuHandle(m_backBufferIndex + 1).ptr); }
        DescriptorHeapAllocation& getTexAlloc() { return m_tex_alloc; }

    private:
        void drawRenderPasses(Chen::CDX12::FrameResource& frameRes, uint frameIndex);
        void flushCommandQueue();

    private:
        static const uint32_t s_frame_count = 3; // triple buffering by default
        static DXGI_FORMAT    s_colorFormat;
        static DXGI_FORMAT    s_depthFormat;

    private:
        HWND m_window_handle;
        bool m_is_vsync_enable{false};

        DXDevice  m_device;
        DXFactory m_dxgiFactory;

        DXSwapChain        m_swapChain;
        DXCommandQueue     m_commandQueue;
        DXCommandSignature m_command_signature;
        DXCommandList      m_currframe_cmdlist;

        DXViewPort m_viewport;
        DXRect     m_scissorRect;

        // render_target, depth_target, frame_buffer
        Scope<Chen::CDX12::Texture> m_renderTargets[s_frame_count];
        Scope<Chen::CDX12::Texture> m_depthTargets[s_frame_count];
        Scope<FrameBuffer>          m_frameBuffers[s_frame_count];

        Scope<DXPSOMngr>            m_psoManager;
        Scope<DXFrameResMngr>       m_frameResourceMngr; // frameResourceMngr
        std::vector<DXBindProperty> m_bindProperties;
        DXResStateTracker           m_stateTracker; // stateTracker

        // DescriptorHeapAllocation
        DescriptorHeapAllocation m_rtvCpuDH;
        DescriptorHeapAllocation m_dsvCpuDH;
        DescriptorHeapAllocation m_csuGpuDH;

        DescriptorHeapAllocation m_tex_alloc;

        // Synchronization objects
        uint32_t m_backBufferIndex;
        DXFence  m_fence;
        uint64_t m_fenceValue;

        // Initial Total Descriptor_Num
        uint32_t numCpuRTV         = 168;
        uint32_t numCpuDSV         = 168;
        uint32_t numCpuCSU         = 648;
        uint32_t numGpuCSU_static  = 648;
        uint32_t numGpuCSU_dynamic = 648;

        // debug
        DXDebugDevice debug_device;

        // draw_list
        std::vector<std::tuple<Mesh*, ObjectConstant2D>> m_draw_2d_list;

        // RenderPass
        std::vector<Scope<RenderPass>> m_render_passes;
    };
} // namespace Zero