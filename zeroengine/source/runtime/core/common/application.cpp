#include <imgui.h>

#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/render_system/renderer_2d.h"
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

        m_window = IWindowSystem::create();
        m_window->setEventCallback(ZE_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();

        m_ImGuiLayer = new ImGuiLayer(m_window->getNativeWindowHandle());
        pushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        Renderer::shutdown();
        LOG_INFO("zeroengine shutdown");
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

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
            if (e.m_handled)
                break;
            (*it)->onEvent(e);
        }
    }

    void Application::run() {
        while (m_running) {
            float    time     = ImGui::GetTime();
            TimeStep timestep = time - m_lastframe_time;
            m_lastframe_time  = time;

            if (!m_minimized) {
                m_ImGuiLayer->begin();
                for (Layer* layer : m_layerStack)
                    layer->onImGuiRender();
                m_ImGuiLayer->end();

                for (Layer* layer : m_layerStack)
                    layer->onUpdate(timestep);
            }

            m_window->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        m_running = false;
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& e) {
        if (e.getWidth() == 0 || e.getHeight() == 0) {
            m_minimized = true;
            return false;
        }

        m_minimized = false;
        Renderer::onWindowResize(e.getWidth(), e.getHeight());
        return false;
    }
} // namespace Zero
