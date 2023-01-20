#pragma once

#include "runtime/core/common/layer_stack.h"
#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/render/window_system/window_system.h"

namespace Zero {
    class Application {
    public:
        Application();
        virtual ~Application();

        void run();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        IWindowSystem& getWindow() { return *m_window; }

        static Application& get() { return *s_instance; }

    private:
        bool onWindowClose(WindowCloseEvent& e);

        std::unique_ptr<IWindowSystem> m_window;
        ImGuiLayer*                    m_ImGuiLayer;
        bool                           m_running = true;
        LayerStack                     m_layerStack;

    private:
        static Application* s_instance;
    };

    // TODO: to be defined in client
    Application* createApplication();
} // namespace Zero