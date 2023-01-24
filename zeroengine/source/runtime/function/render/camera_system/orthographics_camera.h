#pragma once

namespace Zero {
    class OrthographicsCamera {
    public:
        OrthographicsCamera(float left, float right, float bottom, float top);

        void setPosition(const DirectX::SimpleMath::Vector3& position) {
            m_position = position;
            recalculateViewMatrix();
        }
        const DirectX::SimpleMath::Vector3& getPosition() const { return m_position; }

        void setRotation(float rotation) {
            m_rotation = rotation;
            recalculateViewMatrix();
        }

        void setProjection(float left, float right, float bottom, float top);

        float getRotation() const { return m_rotation; }

        const DirectX::SimpleMath::Matrix& getProjectionMatrix() const { return m_projection_matrix; }
        const DirectX::SimpleMath::Matrix& getViewMatrix() const { return m_view_matrix; }
        const DirectX::SimpleMath::Matrix& getViewProjectionMatrix() const { return m_view_proj_matrix; }

    private:
        void recalculateViewMatrix();

    private:
        DirectX::SimpleMath::Matrix m_projection_matrix{DirectX::SimpleMath::Matrix::Identity};
        DirectX::SimpleMath::Matrix m_view_matrix{DirectX::SimpleMath::Matrix::Identity};
        DirectX::SimpleMath::Matrix m_view_proj_matrix{DirectX::SimpleMath::Matrix::Identity};

        DirectX::SimpleMath::Vector3 m_position{0.0f, 0.0f, 0.0f};
        float                        m_rotation = 0.0f;
    };
} // namespace Zero