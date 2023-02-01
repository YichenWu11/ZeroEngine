#pragma once

namespace Zero {
    class Camera {
    public:
        Camera(const DirectX::SimpleMath::Matrix& projection = DirectX::SimpleMath::Matrix::Identity) :
            m_projection(projection) {}

        virtual ~Camera() = default;

        const DirectX::SimpleMath::Matrix& getProjection() const { return m_projection; }

    protected:
        DirectX::SimpleMath::Matrix m_projection;
    };
} // namespace Zero