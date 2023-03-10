#include <imgui.h>

#include "runtime/core/base/application.h"
#include "runtime/core/base/layer.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"
#include "runtime/function/render/window_system/window_system.h"

using namespace Chen::CDX12;

namespace Zero {
    Application* Application::s_instance = nullptr;

    Application::Application(const std::filesystem::path& game_root_path) {
        ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        Zero::LogSystem::init();
        LOG_INFO("zero start");

        m_config_manager   = CreateScope<ConfigManager>(game_root_path);
        m_window           = CreateScope<WindowSystem>(WindowCreateInfo{});
        m_resource_manager = CreateScope<ResourceManager>();
        m_lua_interpreter  = CreateScope<LuaInterpreter>();

        m_window->setEventCallback(BIND_EVENT_FN(Application::onEvent));

        preLoadResources();
    }

    Application::~Application() {
        Renderer::shutdown();
        LOG_INFO("zero shutdown");
    }

    void Application::preLoadResources() {
        m_resource_manager->add<ResourceType::Texture>(
            m_config_manager->getAssetFolder() / "desc/tex/white_desc.json");

        VertexData2D vertices_square[] = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
        };
        uint32_t indices_square[] = {0, 3, 1, 3, 2, 1};

        VertexBufferLayout layout;

        m_resource_manager->add<ResourceType::Mesh>(
            "square",
            vertices_square,
            array_count(vertices_square) * sizeof(VertexData2D) / layout.structSize,
            indices_square,
            array_count(indices_square));

        m_ImGuiLayer = new ImGuiLayer(m_window->getNativeWindowHandle());
        pushOverlay(m_ImGuiLayer);
    }

    void Application::pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
    }

    void Application::pushOverlay(Layer* layer) {
        m_layerStack.pushOverlay(layer);
    }

    void Application::onEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResize));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
            if (e.m_handled)
                break;
            (*it)->onEvent(e);
        }
    }

    void Application::run() {
        m_timer.Reset();

        while (m_running) {
            m_timer.Tick();
            m_resource_manager->tick();

            TimeStep timestep = m_timer.DeltaTime();

            static float elapsed_time = 0.0f;
            elapsed_time += timestep;

            // frame_rate
            if (elapsed_time > 1.0f) {
                std::wstring window_text =
                    L"Zero Engine - " + std::to_wstring((int)ImGui::GetIO().Framerate) + L" fps";
                SetWindowText(FindWindow(L"MainWnd", NULL), window_text.c_str());
                elapsed_time -= 1.0f;
            }

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

    void Application::close() {
        m_running = false;
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
