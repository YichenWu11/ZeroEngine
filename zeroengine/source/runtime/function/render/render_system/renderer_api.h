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

        static bool is2D() { return s_dimension == Dimension::TWO_D; }
        static bool is3D() { return s_dimension == Dimension::THREE_D; }

        static void set2D() { s_dimension = Dimension::TWO_D; }
        static void set3D() { s_dimension = Dimension::THREE_D; }

        static bool isMultiIndirectDrawEnable() { return s_multi_indirect_draw_enabled; }
        static void setMutilIndirectDrawEnabled(bool enabled) { s_multi_indirect_draw_enabled = enabled; }

        static bool isEditorMode() { return s_is_editor_mode; }
        static void setEditorModeEnabled(bool enabled) { s_is_editor_mode = enabled; }

    private:
        static API       s_api;
        static Dimension s_dimension;
        static bool      s_multi_indirect_draw_enabled;
        static bool      s_is_editor_mode;
    };
} // namespace Zero