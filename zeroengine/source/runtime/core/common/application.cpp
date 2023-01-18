#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/core/log/log_system.h"

namespace Zero {
    Application* Application::s_instance = nullptr;

    Application::Application() {
        ZE_ASSERT(!s_instance && "Application already exists!");
        s_instance = this;

        m_window = std::unique_ptr<IWindowSystem>(IWindowSystem::create());
        m_window->setEventCallback(ZE_BIND_EVENT_FN(Application::onEvent));
    }

    Application::~Application() {
    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
        layer->onAttach();
    }

    void Application::pushOverlay(Layer* layer) {
        m_layerStack.pushOverlay(layer);
        layer->onAttach();
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
        while (m_running) {
            for (Layer* layer : m_layerStack)
                layer->onUpdate();

            m_window->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent& e) {
        m_running = false;
        return true;
    }
} // namespace Zero
