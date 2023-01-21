#include <CDX12/DescriptorHeapMngr.h>
#include <CDX12/Shader/PSOManager.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    DXGI_FORMAT RenderContext::s_colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT RenderContext::s_depthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    RenderContext::RenderContext(HWND window_handle) :
        m_window_handle(window_handle) {
        ZE_ASSERT(window_handle && "window handle passed to dx12_context is nullptr!");
    }

    RenderContext::~RenderContext() {
        flushCommandQueue();

        m_frameResourceMngr->CleanUp();

        if (!m_rtvCpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetRTVCpuDH()->Free(std::move(m_rtvCpuDH));
        if (!m_dsvCpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetDSVCpuDH()->Free(std::move(m_dsvCpuDH));
        if (!m_csuCpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUCpuDH()->Free(std::move(m_csuCpuDH));
        if (!m_csuGpuDH.IsNull())
            DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Free(std::move(m_csuGpuDH));

#if defined(DEBUG) || defined(_DEBUG)
            // debug_device->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
#endif
    }

    void RenderContext::init(int width, int height) {
        m_viewport    = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
        m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

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

            m_rtvCpuDH = DescriptorHeapMngr::GetInstance().GetRTVCpuDH()->Allocate(10);
            m_dsvCpuDH = DescriptorHeapMngr::GetInstance().GetDSVCpuDH()->Allocate(10);
            m_csuGpuDH = DescriptorHeapMngr::GetInstance().GetCSUGpuDH()->Allocate(10);
        }

        // Create frame resources.
        {
            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvCpuDH.GetCpuHandle());
            CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvCpuDH.GetCpuHandle());

            // Create a RTV for each frame.
            for (uint32_t n = 0; n < s_frame_count; ++n) {
                m_renderTargets[n] = std::unique_ptr<Texture>(new Texture(m_device.Get(), m_swapChain.Get(), n));
                m_depthTargets[n]  = std::unique_ptr<Texture>(
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

            m_frameResourceMngr = std::make_unique<FrameResourceMngr>(s_frame_count, m_device.Get());
        }

        {
            psoManager = std::unique_ptr<PSOManager>(new PSOManager(m_device.Get()));

            std::vector<std::pair<std::string, Shader::Property>> properties;
            properties.emplace_back(
                "_Global",
                Shader::Property(
                    ShaderVariableType::ConstantBuffer,
                    0,
                    0,
                    0));

            colorShader = std::unique_ptr<BasicShader>(
                new BasicShader(
                    properties,
                    m_device.Get()));

            ComPtr<ID3DBlob> vertexShader;
            ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
            // Enable better shader debugging with the graphics debugging tools.
            uint32_t compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
            uint32_t compileFlags = 0;
#endif
            auto         shader_path = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "zeroengine/shader/shader.hlsl";
            std::wstring path        = AnsiToWString(shader_path.string());

            ThrowIfFailed(D3DCompileFromFile(path.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
            ThrowIfFailed(D3DCompileFromFile(path.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
            colorShader->vsShader            = std::move(vertexShader);
            colorShader->psShader            = std::move(pixelShader);
            colorShader->rasterizerState     = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            colorShader->blendState          = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            auto&& depthStencilState         = colorShader->depthStencilState;
            depthStencilState.DepthEnable    = true;
            depthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;
            depthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
            depthStencilState.StencilEnable  = false;
        }

        {
            ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
            m_fenceValue = 0;
            flushCommandQueue();
        }
    }

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

    void RenderContext::onResize(int width, int height) {
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
            m_renderTargets[n] = std::unique_ptr<Texture>(new Texture(m_device.Get(), m_swapChain.Get(), n));
            m_depthTargets[n]  = std::unique_ptr<Texture>(
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

        ThrowIfFailed(commandList->Close());
        m_commandQueue.Execute(commandList.Get());

        flushCommandQueue();
    }

    void RenderContext::swapBuffer() {
        m_backBufferIndex = (m_backBufferIndex + 1) % s_frame_count;
    }

    void RenderContext::beginRender() {
        m_frameResourceMngr->BeginFrame();
    }

    void RenderContext::endRender() {
        populateCommandList(*(m_frameResourceMngr->GetCurrentFrameResource()), m_backBufferIndex);
        m_frameResourceMngr->Execute(m_commandQueue.Get());

        ImGuiIO& io = ImGui::GetIO();
        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault(
                NULL,
                (void*)(m_frameResourceMngr->GetCurrentFrameResource()->GetCmdList().Get()));
        }

        ThrowIfFailed(m_swapChain->Present(0, 0));
        m_frameResourceMngr->EndFrame(m_commandQueue.Get());
    }

    void RenderContext::populateCommandList(FrameResource& frameRes, uint frameIndex) {
        auto     cmdListHandle = frameRes.Command();
        GCmdList cmdList(cmdListHandle.CmdList());

        // Set necessary state.
        m_stateTracker.RecordState(m_renderTargets[frameIndex].get(), D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_stateTracker.RecordState(m_depthTargets[frameIndex].get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
        m_stateTracker.UpdateState(cmdList.Get());

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvCpuDH.GetCpuHandle(0), frameIndex, m_rtvCpuDH.GetDescriptorSize());
        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvCpuDH.GetCpuHandle(0), frameIndex, m_dsvCpuDH.GetDescriptorSize());
        frameRes.SetRenderTarget(
            m_renderTargets[frameIndex].get(),
            &rtvHandle,
            &dsvHandle);
        frameRes.ClearRTV(rtvHandle);
        frameRes.ClearDSV(dsvHandle);

        Matrix viewProjMatrix = Matrix::Identity;

        auto constBuffer = frameRes.AllocateConstBuffer({reinterpret_cast<uint8_t const*>(&viewProjMatrix), sizeof(viewProjMatrix)});

        bindProperties.clear();
        bindProperties.emplace_back("_Global", constBuffer);

        for (auto& mesh : m_draw_list) {
            frameRes.DrawMesh(
                colorShader.get(),
                psoManager.get(),
                mesh,
                s_colorFormat,
                s_depthFormat,
                bindProperties);
        }

        cmdList->SetDescriptorHeaps(1, get_rvalue_ptr(m_csuGpuDH.GetDescriptorHeap()));
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());

        m_stateTracker.RestoreState(cmdList.Get());

        // clear mesh
        m_draw_list.clear();
    }

} // namespace Zero