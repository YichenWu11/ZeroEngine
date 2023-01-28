#pragma once

#include <CDX12/Material/Texture.h>

namespace Zero {
    class SubTexture2D {
    public:
        SubTexture2D(const Ref<Chen::CDX12::Texture>&    texture,
                     const DirectX::SimpleMath::Vector2& min,
                     const DirectX::SimpleMath::Vector2& max);

        const Ref<Chen::CDX12::Texture>     getTexture() const { return m_texture; }
        const DirectX::SimpleMath::Vector2* getTexCoords() const { return m_tex_coords; }

        static Ref<SubTexture2D> createFromCoords(
            const Ref<Chen::CDX12::Texture>&,
            const DirectX::SimpleMath::Vector2& coords,
            const DirectX::SimpleMath::Vector2& cell_size,
            const DirectX::SimpleMath::Vector2& sprite_size = {1.0f, 1.0f});

    private:
        Ref<Chen::CDX12::Texture>    m_texture;
        DirectX::SimpleMath::Vector2 m_tex_coords[4];
    };
} // namespace Zero