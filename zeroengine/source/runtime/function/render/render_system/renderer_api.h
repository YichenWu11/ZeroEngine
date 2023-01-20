#pragma once

namespace Zero {
    class RendererAPI {
    public:
        enum class API {
            NONE = 0,
            DX12
        };

    public:
        void clear();
        void setClearColor(const DirectX::SimpleMath::Vector4& color);

        static API getAPI() { return s_api; }

    private:
        static API s_api;
    };
} // namespace Zero