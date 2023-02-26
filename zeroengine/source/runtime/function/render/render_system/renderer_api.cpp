#include "runtime/function/render/render_system/renderer_api.h"

namespace Zero {
    RendererAPI::API       RendererAPI::s_api                         = RendererAPI::API::DX12;
    RendererAPI::Dimension RendererAPI::s_dimension                   = RendererAPI::Dimension::TWO_D;
    bool                   RendererAPI::s_multi_indirect_draw_enabled = true;
    bool                   RendererAPI::s_is_editor_mode              = false;
} // namespace Zero