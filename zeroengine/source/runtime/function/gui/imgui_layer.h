#pragma once

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <imgui.h>

#include "runtime/core/base/layer.h"
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

        void blockEvents(bool block) { m_block_events = block; }

    private:
        void setZeroImGuiStyle();

    private:
        HWND m_handle;

        bool m_block_events = true;
    };
} // namespace Zero
