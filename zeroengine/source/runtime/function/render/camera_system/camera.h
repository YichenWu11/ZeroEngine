#pragma once

namespace Zero {
    class Camera {
    public:
        Camera(const DirectX::SimpleMath::Matrix& projection = DirectX::SimpleMath::Matrix::Identity) :
            m_projection(projection) {}

        const DirectX::SimpleMath::Matrix& getProjection() const { return m_projection; }

    private:
        DirectX::SimpleMath::Matrix m_projection;
    };
} // namespace Zero