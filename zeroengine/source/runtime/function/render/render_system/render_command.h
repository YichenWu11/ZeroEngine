#pragma once

#include <CDX12/Resource/Texture.h>

namespace Zero {
    class RenderCommand {
    public:
        static void clear() {
        }

        static void setClearColor(const Color& color) {
            Chen::CDX12::Texture::setClearColor(color);
        }
    };
} // namespace Zero