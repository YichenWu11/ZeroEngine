#include <CDX12/Resource/Mesh.h>

#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer.h"

using namespace Chen::CDX12;

namespace Zero {
    RenderContext* Renderer::m_render_context = nullptr;

    void Renderer::beginScene() {
        // takes all the scene settings(camera, lights, environment etc)
        m_render_context->beginRender();
    }

    void Renderer::endScene() {
        m_render_context->endRender();
    }

    void Renderer::submit(Mesh* mesh) {
        m_render_context->submitMesh(mesh);
    }
} // namespace Zero