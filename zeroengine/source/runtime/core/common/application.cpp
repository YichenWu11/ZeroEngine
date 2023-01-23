#include <imgui.h>

#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/window_system/window_system.h"
#include "runtime/function/table/mesh_table.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        ZE_ASSERT(!s_instance && "Application already exists!");
        s_instance = this;

        m_window = Zero::Scope<IWindowSystem>(IWindowSystem::create());
        m_window->setEventCallback(ZE_BIND_EVENT_FN(Application::onEvent));

        RenderContext* render_context = static_cast<WindowSystem*>(m_window.get())->getRenderContext();

        Renderer::init(render_context);

        m_ImGuiLayer = new ImGuiLayer(
            render_context,
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
        dispatcher.Dispatch<WindowResizeEvent>(ZE_BIND_EVENT_FN(Application::onWindowResize));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
            (*--it)->onEvent(e);
            if (e.m_handled)
                break;
        }
    }

    void Application::run() {
        MeshTable::getInstance().buildBasicMesh();

        auto asoul_path = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/texture/common/bella.png";

        TextureInitInfo init_info{AnsiToWString(asoul_path.string()).c_str(), "BELLA"};
        TextureTable::getInstance().registerTex(init_info);

        while (m_running) {
            float    time     = ImGui::GetTime();
            TimeStep timestep = time - m_lastframe_time;
            m_lastframe_time  = time;

            m_ImGuiLayer->begin();
            for (Layer* layer : m_layerStack)
                layer->onImGuiRender();
            m_ImGuiLayer->end();

            for (Layer* layer : m_layerStack)
                layer->onUpdate(timestep);

            m_window->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        m_running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& e) {
        m_window->onResize(e.getWidth(), e.getHeight());
        return true;
    }
} // namespace Zero
