#pragma once

#include "runtime/function/render/render_system/render_command.h"

namespace Chen::CDX12 {
    class Mesh;
}

namespace Zero {
    class RenderContext;

    class Renderer {
    public:
        static void bindRenderContext(RenderContext* context) { m_render_context = context; }

        static void beginScene();
        static void endScene();

        static void submit(Chen::CDX12::Mesh*);

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        static RenderContext* m_render_context;
    };
} // namespace Zero