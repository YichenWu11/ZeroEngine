#pragma once

#include "runtime/core/base/layer_stack.h"
#include "runtime/core/util/time_step.h"

#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/render/window_system/window_system.h"
#include "runtime/function/scripting/lua_interpreter.h"

#include "runtime/resource/config_manager/config_manager.h"
#include "runtime/resource/resource_manager.h"

namespace Zero {
    class Application {
    public:
        Application(const std::filesystem::path& game_root_path = {});
        virtual ~Application();

        void run();
        void close();
        void preLoadResources();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* layer);

        WindowSystem*    getWindow() { return m_window.get(); }
        ResourceManager* getResourceMngr() { return m_resource_manager.get(); }
        ConfigManager*   getConfigMngr() { return m_config_manager.get(); }
        ImGuiLayer*      getImGuiLayer() { return m_ImGuiLayer; }
        LuaInterpreter*  getLuaInterpreter() { return m_lua_interpreter.get(); }

        static Application& get() { return *s_instance; }

    private:
        bool onWindowClose(WindowCloseEvent& e);
        bool onWindowResize(WindowResizeEvent& e);

    private:
        Scope<WindowSystem>    m_window;
        Scope<ResourceManager> m_resource_manager;
        Scope<ConfigManager>   m_config_manager;
        Scope<LuaInterpreter>  m_lua_interpreter;
        ImGuiLayer*            m_ImGuiLayer;

        LayerStack m_layerStack;
        Timer      m_timer;

        bool m_running   = true;
        bool m_minimized = false;

    private:
        static Application* s_instance;
    };

    Application* createApp();
} // namespace Zero
