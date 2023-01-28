#include "runtime/function/render/render_system/subtexture_2d.h"
#include "runtime/function/table/texture_table.h"

using namespace Chen::CDX12;
using namespace DirectX::SimpleMath;

namespace Zero {
    SubTexture2D::SubTexture2D(const Ref<Chen::CDX12::Texture>&    texture,
                               const DirectX::SimpleMath::Vector2& min,
                               const DirectX::SimpleMath::Vector2& max) :
        m_texture(texture) {
        m_tex_coords[0] = Vector2{min.x, min.y};
        m_tex_coords[1] = Vector2{max.x, min.y};
        m_tex_coords[2] = Vector2{max.x, max.y};
        m_tex_coords[3] = Vector2{min.x, max.y};

        m_sub_name =
            GET_TEXTURE_TABLE().getTextureName(m_texture) + std::to_string(min.x + min.y + max.x + max.y);
    }

    Ref<SubTexture2D> SubTexture2D::createFromCoords(
        const Ref<Chen::CDX12::Texture>&    texture,
        const DirectX::SimpleMath::Vector2& coords,
        const DirectX::SimpleMath::Vector2& cell_size,
        const DirectX::SimpleMath::Vector2& sprite_size) {
        Vector2 min = {
            (coords.x * cell_size.x) / texture->GetWidth(),
            (coords.y * cell_size.y) / texture->GetHeight()};

        Vector2 max = {
            ((coords.x + sprite_size.x) * cell_size.x) / texture->GetWidth(),
            ((coords.y + sprite_size.y) * cell_size.y) / texture->GetHeight()};

        return CreateRef<SubTexture2D>(texture, min, max);
    }

    std::string SubTexture2D::constructSubTexName() const {
        return m_sub_name;
    }
} // namespace Zero