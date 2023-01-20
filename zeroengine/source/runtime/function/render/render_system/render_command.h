#pragma once

#include "runtime/function/render/render_system/renderer_api.h"

namespace Zero {
    class RenderCommand {
    public:
        static void clear() {
            s_RendererApi->clear();
        }

        static void setClearColor(const DirectX::SimpleMath::Color& color) {
            s_RendererApi->setClearColor(color);
        }

    private:
        static RendererAPI* s_RendererApi;
    };
} // namespace Zero