#include <CDX12/DescriptorHeapMngr.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "runtime/core/common/application.h"
#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_system/render_context.h"

namespace Zero {
    ImGuiLayer::ImGuiLayer(RenderContext* context, HWND handle) :
        Layer("ImGuiLayer"),
        m_context(context),

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

        // ImGui::StyleColorsDark();
        // ImGui::StyleColorsClassic();
        ImGui::StyleColorsLight();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        auto font_path = std::filesystem::path(ZERO_XSTR(ZE_ROOT_DIR)) / "asset/font/ZeroEngineFont.ttf";
        io.Fonts->AddFontFromFileTTF(font_path.string().c_str(), 20.0f);

        ImGuiInitInfo init_info = m_context->getImGuiInitInfo();

        ImGui_ImplWin32_Init(m_handle);

        ImGui_ImplDX12_Init(m_context->getGraphicsDevice(),
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

    void ImGuiLayer::onImGuiRender() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
} // namespace Zero
