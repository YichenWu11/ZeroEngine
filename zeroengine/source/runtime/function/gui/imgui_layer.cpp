#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "runtime/core/common/application.h"
#include "runtime/function/gui/imgui_layer.h"

// NOTE: this layer dosen't work so far

namespace Zero {
    ImGuiLayer::ImGuiLayer() :
        Layer("ImGuiLayer") {
    }
    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::onAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // docking enable
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.KeyMap[ImGuiKey_Tab]        = VK_TAB;
        io.KeyMap[ImGuiKey_LeftArrow]  = VK_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow]    = VK_UP;
        io.KeyMap[ImGuiKey_DownArrow]  = VK_DOWN;
        io.KeyMap[ImGuiKey_PageUp]     = VK_PRIOR;
        io.KeyMap[ImGuiKey_PageDown]   = VK_NEXT;
        io.KeyMap[ImGuiKey_Home]       = VK_HOME;
        io.KeyMap[ImGuiKey_End]        = VK_END;
        io.KeyMap[ImGuiKey_Insert]     = VK_INSERT;
        io.KeyMap[ImGuiKey_Delete]     = VK_DELETE;
        io.KeyMap[ImGuiKey_Backspace]  = VK_BACK;
        io.KeyMap[ImGuiKey_Space]      = VK_SPACE;
        io.KeyMap[ImGuiKey_Enter]      = VK_RETURN;
        io.KeyMap[ImGuiKey_Escape]     = VK_ESCAPE;
        io.KeyMap[ImGuiKey_A]          = GetAsyncKeyState('A');
        io.KeyMap[ImGuiKey_C]          = GetAsyncKeyState('C');
        io.KeyMap[ImGuiKey_V]          = GetAsyncKeyState('V');
        io.KeyMap[ImGuiKey_X]          = GetAsyncKeyState('X');
        io.KeyMap[ImGuiKey_Y]          = GetAsyncKeyState('Y');
        io.KeyMap[ImGuiKey_Z]          = GetAsyncKeyState('Z');

        ImGui_ImplWin32_Init(GetModuleHandle(NULL));

        // TODO: init imgui_impldx12
        // ImGui_ImplDX12_Init(device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, heap, cpuHandle,
        //                     gpuHandle);
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onUpdate() {
        ImGuiIO&     io  = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize   = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        // TODO: set the delat_time
        // io.DeltaTime = m_time;

        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();

        // TODO: impl in render_backend
        // ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
    }

    void ImGuiLayer::onEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseButtonPressedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onKeyPressedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onKeyTypedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onKeyReleasedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(ZE_BIND_EVENT_FN(ImGuiLayer::onWindowResizeEvent));
    }

    bool ImGuiLayer::onMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
        ImGuiIO& io                      = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
        ImGuiIO& io                      = ImGui::GetIO();
        io.MouseDown[e.getMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::onMouseMovedEvent(MouseMovedEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.getX(), e.getY());

        return false;
    }

    bool ImGuiLayer::onMouseScrolledEvent(MouseScrolledEvent& e) {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.getXOffset();
        io.MouseWheel += e.getYOffset();

        return false;
    }

    bool ImGuiLayer::onKeyPressedEvent(KeyPressedEvent& e) {
        ImGuiIO& io                 = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = true;

        io.KeyCtrl  = io.KeysDown[VK_LCONTROL] || io.KeysDown[VK_RCONTROL];
        io.KeyShift = io.KeysDown[VK_LSHIFT] || io.KeysDown[VK_RSHIFT];
        io.KeyAlt   = io.KeysDown[VK_LMENU] || io.KeysDown[VK_RMENU];
        io.KeySuper = io.KeysDown[VK_LWIN] || io.KeysDown[VK_RWIN];
        return false;
    }

    bool ImGuiLayer::onKeyReleasedEvent(KeyReleasedEvent& e) {
        ImGuiIO& io                 = ImGui::GetIO();
        io.KeysDown[e.getKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::onKeyTypedEvent(KeyTypedEvent& e) {
        ImGuiIO& io      = ImGui::GetIO();
        int      keycode = e.getKeyCode();
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiLayer::onWindowResizeEvent(WindowResizeEvent& e) {
        ImGuiIO& io                = ImGui::GetIO();
        io.DisplaySize             = ImVec2(e.getWidth(), e.getHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        // TODO: set viewport

        return false;
    }

} // namespace Zero
