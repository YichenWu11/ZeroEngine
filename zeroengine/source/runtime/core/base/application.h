#pragma once

#include "runtime/core/base/layer_stack.h"
#include "runtime/core/util/time_step.h"
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
        void close();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        IWindowSystem& getWindow() { return *m_window; }
        ImGuiLayer*    getImGuiLayer() { return m_ImGuiLayer; }

        static Application& get() { return *s_instance; }

    private:
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

        Zero::Scope<IWindowSystem> m_window;
        ImGuiLayer*                m_ImGuiLayer;
        bool                       m_running   = true;
        bool                       m_minimized = false;
        LayerStack                 m_layerStack;
        float                      m_lastframe_time = 0.0f;

    private:
        static Application* s_instance;
    };

    // TODO: to be defined in client
    Application* createApplication();
} // namespace Zero