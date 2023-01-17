#pragma once

#include "runtime/core/common/layer_stack.h"
#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/render/window_system/i_window_system.h"

namespace Zero {
    class Application {
    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

    private:
        bool onWindowClose(WindowCloseEvent& e);

        std::unique_ptr<IWindowSystem> m_window;
        bool                           m_running = true;
        LayerStack                     m_layerStack;
    };

    // TODO: to be defined
    Application* createApplication();
} // namespace Zero