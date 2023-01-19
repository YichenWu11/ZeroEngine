#pragma once

#include "runtime/function/render/render_system/renderer/graphics_context.h"

namespace Zero {
    class DX12Context : public GraphicsContext {
    public:
        DX12Context(HWND window_handle);

        void init() override;
        void swapBuffer() override;

    private:
        HWND m_window_handle;
    };
} // namespace Zero