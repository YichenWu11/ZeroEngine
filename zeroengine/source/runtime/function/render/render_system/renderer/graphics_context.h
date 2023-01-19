#pragma once

namespace Zero {
    class GraphicsContext {
    public:
        virtual void init()       = 0;
        virtual void swapBuffer() = 0;
    };
} // namespace Zero