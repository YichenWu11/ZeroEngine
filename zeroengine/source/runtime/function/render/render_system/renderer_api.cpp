#include <CDX12/Material/Texture.h>

#include "runtime/function/render/render_system/renderer_api.h"

namespace Zero {
    RendererAPI::API       RendererAPI::s_api                         = RendererAPI::API::DX12;
    RendererAPI::Dimension RendererAPI::s_dimension                   = RendererAPI::Dimension::TWO_D;
    bool                   RendererAPI::s_multi_indirect_draw_enabled = true;
    bool                   RendererAPI::is_editor_mode                = true;

    void RendererAPI::clear() {
    }

    void RendererAPI::setClearColor(const DirectX::SimpleMath::Vector4& color) {
        Chen::CDX12::Texture::setClearColor(color);
    }
} // namespace Zero