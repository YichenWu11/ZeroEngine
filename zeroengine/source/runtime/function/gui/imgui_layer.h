#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <imgui.h>

#include "runtime/core/common/layer.h"
#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"

namespace Zero {
    class RenderContext;

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(RenderContext*, HWND);
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;

        void begin();
        void end();

    private:
        void setZeroImGuiStyle();

    private:
        RenderContext* m_context;
        HWND           m_handle;
    };
} // namespace Zero
