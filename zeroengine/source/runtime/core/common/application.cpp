#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/core/log/log_system.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/window_system/window_system.h"

using namespace Chen::CDX12;

namespace Zero {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        ZE_ASSERT(!s_instance && "Application already exists!");
        s_instance = this;

        m_window = std::unique_ptr<IWindowSystem>(IWindowSystem::create());
        m_window->setEventCallback(ZE_BIND_EVENT_FN(Application::onEvent));

        m_ImGuiLayer = new ImGuiLayer(
            static_cast<WindowSystem*>(m_window.get())->getRenderContext(),
            m_window->getNativeWindowHandle());
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* layer) {
        m_layerStack.pushOverlay(layer);
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(ZE_BIND_EVENT_FN(Application::onWindowClose));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.m_handled)
                break;
        }
    }

    void Application::run() {
        RenderContext* render_context = static_cast<WindowSystem*>(m_window.get())->getRenderContext();
        ID3D12Device*  device         = render_context->getGraphicsDevice();
        Renderer::bindRenderContext(render_context);

        // *******************************************************************************************
        // create mesh for test
        VertexBufferLayout                 layout;
        std::vector<::rtti::Struct const*> structs;
        structs.emplace_back(&layout);

        static DirectX::XMFLOAT3 vertices[] = {
            DirectX::XMFLOAT3(-0.5, -0.5, 0.5),
            DirectX::XMFLOAT3(0, 0.5, 0.5),
            DirectX::XMFLOAT3(0.5, -0.5, 0.5)};

        static uint                       indices[] = {0, 1, 2};
        ComPtr<ID3D12CommandAllocator>    cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
        ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
        ThrowIfFailed(commandList->Close());
        ThrowIfFailed(cmdAllocator->Reset());
        ThrowIfFailed(commandList->Reset(cmdAllocator.Get(), nullptr));

        std::unique_ptr<Mesh> triangle_mesh = std::make_unique<Mesh>(
            render_context->getGraphicsDevice(),
            structs,
            array_count(vertices),
            array_count(indices));

        VertexBuffer* vert_buffer = VertexBuffer::create(render_context->getGraphicsDevice(),
                                                         vertices,
                                                         array_count(vertices) * sizeof(DirectX::XMFLOAT3));

        IndexBuffer* indi_buffer = IndexBuffer::create(render_context->getGraphicsDevice(),
                                                       indices,
                                                       array_count(indices) * sizeof(uint32_t));

        vert_buffer->bind(commandList.Get(), triangle_mesh.get());
        indi_buffer->bind(commandList.Get(), triangle_mesh.get());

        ThrowIfFailed(commandList->Close());

        //// Execute CommandList
        ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
        render_context->getCommandQueue()->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);

        {
            render_context->getCommandQueue()->Signal(render_context->getFence(), 1);
            if (render_context->getFence()->GetCompletedValue() < 1) {
                LPCWSTR falseValue  = 0;
                HANDLE  eventHandle = CreateEventEx(nullptr, falseValue, false, EVENT_ALL_ACCESS);
                ThrowIfFailed(render_context->getFence()->SetEventOnCompletion(1, eventHandle));
                WaitForSingleObject(eventHandle, INFINITE);
                CloseHandle(eventHandle);
            }
        }
        // create mesh for test
        // *******************************************************************************************

        while (m_running) {
            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            DirectX::SimpleMath::Color clear_color = {0.1f, 0.1f, 0.1f, 1.0f};

            RenderCommand::setClearColor(clear_color);
            RenderCommand::clear();

            m_ImGuiLayer->begin();
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_ImGuiLayer->end();

            Renderer::beginScene();
            Renderer::submit(triangle_mesh.get());
            Renderer::endScene();

            m_window->onUpdate();
        }

        delete vert_buffer;
        delete indi_buffer;
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        m_running = false;
        return true;
    }
} // namespace Zero
