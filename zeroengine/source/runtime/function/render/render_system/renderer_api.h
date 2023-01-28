#pragma once

namespace Zero {
    class RendererAPI {
    public:
        enum class API {
            NONE = 0,
            DX12
        };

        enum class Dimension {
            TWO_D = 0,
            THREE_D
        };

    public:
        void clear();
        void setClearColor(const DirectX::SimpleMath::Vector4& color);

        static API       getAPI() { return s_api; }
        static Dimension getRenderDimension() { return s_dimension; }
        static bool      isMultiIndirectDrawEnable() { return s_multi_indirect_draw_enabled; }
        static void      setMutilIndirectDrawEnabled(bool enabled) { s_multi_indirect_draw_enabled = enabled; }

    private:
        static API       s_api;
        static Dimension s_dimension;
        static bool      s_multi_indirect_draw_enabled;
    };
} // namespace Zero