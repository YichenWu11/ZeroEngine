#pragma once

#include <CDX12/Resource/Texture.h>

namespace Zero {
    class SubTexture2D {
    public:
        SubTexture2D(const Ref<Chen::CDX12::Texture>& texture,
                     const Vector2&                   min,
                     const Vector2&                   max);

        const Ref<Chen::CDX12::Texture> getTexture() const { return m_texture; }
        const Vector2*                  getTexCoords() const { return m_tex_coords; }

        std::string constructSubTexName() const;

        static Ref<SubTexture2D> createFromCoords(
            const Ref<Chen::CDX12::Texture>&,
            const Vector2& coords,
            const Vector2& cell_size,
            const Vector2& sprite_size = {1.0f, 1.0f});

    private:
        Ref<Chen::CDX12::Texture> m_texture;
        Vector2                   m_tex_coords[4];
        std::string               m_sub_name;

    private:
        static uint32_t s_count;
    };
} // namespace Zero