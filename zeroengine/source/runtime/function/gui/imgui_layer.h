#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <imgui.h>

#include "runtime/core/common/layer.h"
#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"

namespace Zero {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer(HWND);
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onEvent(Event& e) override;

        void begin();
        void end();

    private:
        void setZeroImGuiStyle();

    private:
        HWND m_handle;
    };
} // namespace Zero
