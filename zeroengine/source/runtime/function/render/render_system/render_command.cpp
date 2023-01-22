#include "runtime/function/render/render_system/render_command.h"

namespace Zero {
    Zero::Scope<RendererAPI> RenderCommand::s_RendererApi = std::make_unique<RendererAPI>();
} // namespace Zero