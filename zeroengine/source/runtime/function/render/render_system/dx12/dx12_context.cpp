#include "runtime/function/render/render_system/dx12/dx12_context.h"

namespace Zero {
    DX12Context::DX12Context(HWND window_handle) :
        m_window_handle(window_handle) {
        ZE_ASSERT(window_handle && "window handle passed to dx12_context is nullptr!");
    }

    void DX12Context::init() {
        // TODO: init the dx12_context
    }

    void DX12Context::swapBuffer() {
        // TODO: do the buffer swaping here
    }
} // namespace Zero