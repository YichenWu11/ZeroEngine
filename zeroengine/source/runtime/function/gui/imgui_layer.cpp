#include <CDX12/DescriptorHeapMngr.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>

#include "runtime/core/common/application.h"
#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_system/render_context.h"

namespace Zero {
    ImGuiLayer::ImGuiLayer(HWND handle) :
        Layer("ImGuiLayer"),
        m_handle(handle) {
    }

    ImGuiLayer::~ImGuiLayer() {
    }

    void ImGuiLayer::onAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        // ImGui::StyleColorsLight();
        setZeroImGuiStyle();

        auto font_path = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/font/ZeroEngineFont.ttf";
        io.Fonts->AddFontFromFileTTF(font_path.string().c_str(), 20.0f);

        ImGuiInitInfo init_info = RenderContext::getInstance().getImGuiInitInfo();

        ImGui_ImplWin32_Init(m_handle);

        ImGui_ImplDX12_Init(RenderContext::getInstance().getGraphicsDevice(),
                            3,
                            DXGI_FORMAT_R8G8B8A8_UNORM,
                            init_info.descriptor_heap,
                            init_info.cpu_handle,
                            init_info.gpu_handle);
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onEvent(Event& e) {
        ImGuiIO& io = ImGui::GetIO();
        e.m_handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
        e.m_handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    }

    void ImGuiLayer::begin() {
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end() {
        ImGuiIO&     io  = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize   = ImVec2(app.getWindow().getWidth(), app.getWindow().getHeight());

        ImGui::Render();
    }

    void ImGuiLayer::setZeroImGuiStyle() {
        auto& colors              = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        colors[ImGuiCol_Header]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg]        = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive]  = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered]         = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive]          = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.156f, 0.25f, 1.0f};
        colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.156f, 0.25f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.156f, 0.25f, 1.0f};
    }
} // namespace Zero
