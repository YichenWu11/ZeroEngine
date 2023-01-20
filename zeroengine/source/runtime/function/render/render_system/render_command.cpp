#include "runtime/function/render/render_system/render_command.h"

namespace Zero {
    // FIXME: memory leak
    RendererAPI* RenderCommand::s_RendererApi = new RendererAPI();
} // namespace Zero