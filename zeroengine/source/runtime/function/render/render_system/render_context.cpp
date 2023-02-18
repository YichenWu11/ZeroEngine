#include <CDX12/DescriptorHeapMngr.h>
#include <CDX12/Resource/DescHeapAllocView.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "runtime/function/pool/texture_pool.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer_api.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"

using namespace Chen::CDX12;
using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Zero {
    DXGI_FORMAT RenderContext::s_colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT RenderContext::s_depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    void RenderContext::init(HWND window_handle, int width, int height) {
        ZE_ASSERT(window_handle, "window handle passed to dx12_context is NULL!");

        m_window_handle = window_handle;
        m_viewport      = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
        m_scissorRect   = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

#if defined(DEBUG) || defined(_DEBUG)
        {
            ComPtr<ID3D12Debug> debugController;
            ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
            debugController->EnableDebugLayer();
        }
#endif

        // create dxgi
        ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));
        ThrowIfFailed(m_device.Create(nullptr, D3D_FEATURE_LEVEL_11_0));

#if defined(DEBUG) || defined(_DEBUG)
        m_device->QueryInterface(IID_PPV_ARGS(&debug_device));
#endif

        // Describe and create the command queue.
        D3D12_COMMAND_QUEUE_DESC queueDesc = {};
        queueDesc.Flags                    = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;

        ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_commandQueue.raw.GetAddressOf())));

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.BufferCount           = s_frame_count;
        swapChainDesc.Width                 = width;
        swapChainDesc.Height                = height;
        swapChainDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.SampleDesc.Count      = 1;

        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
            m_commandQueue.Get(),
            m_window_handle,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain));

        ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(m_window_handle, DXGI_MWA_NO_ALT_ENTER));
        ThrowIfFailed(swapChain.As(&m_swapChain));
        m_backBufferIndex = 0;

        // Create descriptor heaps.
        {
            DescriptorHeapMngr::GetInstance().Init(
                m_device.Get(),
                numCpuCSU,
                numCpuRTV,
                numCpuDSV,
                numGpuCSU_static,
                numGpuCSU_dynamic);

            m_rtvCpuDH = DescriptorHeapMngr::GetInstance().GetRTVCpuDH()->Allocate(20);
            m_dsvCpuDH = DescriptorHeapMngr::GetInstance().GetDSVCpuDH()->Allocate(20);
            m_csuGpuDH = DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Allocate(20);
        }

        // Create frame resources.
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvCpuDH.GetCpuHandle());
            CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvCpuDH.GetCpuHandle());

            // Create a RTV for each frame.
            for (uint32_t n = 0; n < s_frame_count; ++n) {
                m_renderTargets[n] = Zero::Scope<Texture>(new Texture(m_device.Get(), m_swapChain.Get(), n));
                m_depthTargets[n]  = Zero::Scope<Texture>(
                    new Texture(
                         m_device.Get(),
                         m_scissorRect.right,
                         m_scissorRect.bottom,
                         DXGI_FORMAT_D24_UNORM_S8_UINT,
                         TextureDimension::Tex2D,
                         1,
                         1,
                         Texture::TextureUsage::DepthStencil,
                         D3D12_RESOURCE_STATE_DEPTH_READ));

                FrameBufferConfiguration fb_config{
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height),
                    m_rtvCpuDH.GetCpuHandle(n + 3),
                    m_csuGpuDH.GetCpuHandle(n + 1),
                    s_colorFormat};
                m_frameBuffers[n] = Zero::CreateScope<FrameBuffer>(fb_config);

                m_device->CreateRenderTargetView(m_renderTargets[n]->GetResource(), nullptr, rtvHandle);
                m_device->CreateDepthStencilView(m_depthTargets[n]->GetResource(), nullptr, dsvHandle);
                rtvHandle.Offset(1, m_rtvCpuDH.GetDescriptorSize());
                dsvHandle.Offset(1, m_dsvCpuDH.GetDescriptorSize());
            }

            m_frameResourceMngr = Zero::CreateScope<FrameResourceMngr>(s_frame_count, m_device.Get());
        }

        {
            m_psoManager = Zero::Scope<PSOManager>(new PSOManager(m_device.Get()));
        }

        {
            registerRenderPass();
        }

        {
            D3D12_COMMAND_SIGNATURE_DESC desc;

            D3D12_INDIRECT_ARGUMENT_DESC ind_desc[4];
            ZeroMemory(ind_desc, 4 * sizeof(D3D12_INDIRECT_ARGUMENT_DESC));
            ind_desc[0].Type                                  = D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
            ind_desc[0].ConstantBufferView.RootParameterIndex = GET_SHADER_BIND_TABLE().getShader("transparent")->GetPropRootSigPos("_ObjectConstant");
            ind_desc[1].Type                                  = D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
            ind_desc[1].VertexBuffer.Slot                     = 0;
            ind_desc[2].Type                                  = D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
            ind_desc[3].Type                                  = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

            desc.ByteStride       = sizeof(IndirectDrawCommand);
            desc.NodeMask         = 0;
            desc.NumArgumentDescs = 4;
            desc.pArgumentDescs   = ind_desc;
            m_device->CreateCommandSignature(
                &desc,
                GET_SHADER_BIND_TABLE().getShader("transparent")->RootSig(),
                IID_PPV_ARGS(&m_command_signature));
        }

        {
            ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
            m_fenceValue = 0;
            flushCommandQueue();
        }

        LOG_INFO("dx12 render_context init success");
    }

    void RenderContext::registerRenderPass() {
        LOG_INFO("-------------------------------");
        LOG_INFO("begin to register render pass: ");

        m_render_passes.push_back(Zero::CreateScope<MainCameraPass2D>());
        m_render_passes.push_back(Zero::CreateScope<UIPass>());

        for (auto&& render_pass : m_render_passes)
            render_pass->preLoadResource();

        LOG_INFO("register render pass success!");
        LOG_INFO("-------------------------------");
    }

    void RenderContext::shutdown() {
        // wait for gpu
        flushCommandQueue();

        m_frameResourceMngr->CleanUp();

        for (int i = 0; i < s_frame_count; ++i) {
            m_frameBuffers[i]->onDestroy();
        }

        if (!m_rtvCpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetRTVCpuDH()->Free(std::move(m_rtvCpuDH));
        if (!m_dsvCpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetDSVCpuDH()->Free(std::move(m_dsvCpuDH));
        if (!m_csuGpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Free(std::move(m_csuGpuDH));

#if defined(DEBUG) || defined(_DEBUG)
            // debug_device->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
#endif
    }

    void RenderContext::swapBuffer() {
        m_backBufferIndex = (m_backBufferIndex + 1) % s_frame_count;
    }

    void RenderContext::onRender() {
        m_frameResourceMngr->BeginFrame();

        {
            auto cmdListHandle  = m_frameResourceMngr->GetCurrentFrameResource()->Command();
            m_currframe_cmdlist = GCmdList(cmdListHandle.CmdList());
            drawRenderPasses(*(m_frameResourceMngr->GetCurrentFrameResource()), m_backBufferIndex);
        }

        m_frameResourceMngr->Execute(m_commandQueue.Get());

        ImGuiIO& io = ImGui::GetIO();
        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault(
                NULL,
                (void*)(m_frameResourceMngr->GetCurrentFrameResource()->GetCmdList().Get()));
        }

        ThrowIfFailed(m_swapChain->Present((m_is_vsync_enable) ? 1 : 0, 0));
        m_frameResourceMngr->EndFrame(m_commandQueue.Get());

        // clear draw_list
        m_draw_2d_list.clear();
    }

    // draw call
    void RenderContext::drawRenderPasses(FrameResource& frameRes, uint frameIndex) {
        for (auto&& render_pass : m_render_passes) {
            if (RendererAPI::isMultiIndirectDrawEnable()) {
                render_pass->drawPassIndirect(frameRes, frameIndex, RendererAPI::isEditorMode());
            }
            else {
                render_pass->drawPass(frameRes, frameIndex, RendererAPI::isEditorMode());
            }
        }
    }

    // *******************************************************************************************************

    void RenderContext::flushCommandQueue() {
        m_fenceValue++;

        ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValue));

        if (m_fence->GetCompletedValue() < m_fenceValue) {
            HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

            ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValue, eventHandle));

            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }
    }

    void RenderContext::onResize(uint32_t width, uint32_t height) {
        flushCommandQueue();

        m_viewport    = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
        m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

        ComPtr<ID3D12CommandAllocator>    cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(
            m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
        ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
        ThrowIfFailed(commandList->Close());
        ThrowIfFailed(cmdAllocator->Reset());
        ThrowIfFailed(commandList->Reset(cmdAllocator.Get(), nullptr));

        for (int i = 0; i < s_frame_count; ++i) {
            m_renderTargets[i].reset();
            m_depthTargets[i].reset();
        }

        ThrowIfFailed(m_swapChain->ResizeBuffers(
            s_frame_count,
            width, height,
            s_colorFormat,
            0));

        // because after `onResize`, the window system will call the `swapBuffer()`
        // before the next draw call
        m_backBufferIndex = -1;

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvCpuDH.GetCpuHandle());
        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvCpuDH.GetCpuHandle());

        for (uint32_t n = 0; n < s_frame_count; ++n) {
            m_renderTargets[n] = Zero::Scope<Texture>(new Texture(m_device.Get(), m_swapChain.Get(), n));
            m_depthTargets[n]  = Zero::Scope<Texture>(
                new Texture(
                     m_device.Get(),
                     m_scissorRect.right,
                     m_scissorRect.bottom,
                     DXGI_FORMAT_D24_UNORM_S8_UINT,
                     TextureDimension::Tex2D,
                     1,
                     1,
                     Texture::TextureUsage::DepthStencil,
                     D3D12_RESOURCE_STATE_DEPTH_READ));

            m_device->CreateRenderTargetView(m_renderTargets[n]->GetResource(), nullptr, rtvHandle);
            m_device->CreateDepthStencilView(m_depthTargets[n]->GetResource(), nullptr, dsvHandle);
            rtvHandle.Offset(1, m_rtvCpuDH.GetDescriptorSize());
            dsvHandle.Offset(1, m_dsvCpuDH.GetDescriptorSize());
        }

        for (auto& shader_pass : m_render_passes)
            shader_pass->onResize();

        ThrowIfFailed(commandList->Close());
        m_commandQueue.Execute(commandList.Get());

        flushCommandQueue();
    }

    void RenderContext::resizeFrameBuffer(int width, int height) {
        flushCommandQueue();

        for (int i = 0; i < s_frame_count; ++i) {
            FrameBufferConfiguration fb_config{
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height),
                m_rtvCpuDH.GetCpuHandle(i + 3),
                m_csuGpuDH.GetCpuHandle(i + 1),
                s_colorFormat};
            m_frameBuffers[i]->onResize(fb_config);
        }
    }
} // namespace Zero