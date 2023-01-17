#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/core/log/log_system.h"

namespace Zero {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() {
        m_window = std::unique_ptr<IWindowSystem>(IWindowSystem::create());
        m_window->setEventCallback(BIND_EVENT_FN(onEvent));
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
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));

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
