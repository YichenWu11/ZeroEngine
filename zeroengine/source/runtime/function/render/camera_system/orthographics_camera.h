#pragma once

namespace Zero {
    class OrthographicsCamera {
    public:
        OrthographicsCamera(float left, float right, float bottom, float top);

        void setPosition(const Vector3& position) {
            m_position = position;
            recalculateViewMatrix();
        }
        const Vector3& getPosition() const { return m_position; }

        void setRotation(float rotation) {
            m_rotation = rotation;
            recalculateViewMatrix();
        }

        void setProjection(float left, float right, float bottom, float top);

        float getRotation() const { return m_rotation; }

        const Matrix& getProjectionMatrix() const { return m_projection_matrix; }
        const Matrix& getViewMatrix() const { return m_view_matrix; }
        const Matrix& getViewProjectionMatrix() const { return m_view_proj_matrix; }

    private:
        void recalculateViewMatrix();

    private:
        Matrix m_projection_matrix{Matrix::Identity};
        Matrix m_view_matrix{Matrix::Identity};
        Matrix m_view_proj_matrix{Matrix::Identity};

        Vector3 m_position{0.0f, 0.0f, 0.0f};
        float   m_rotation = 0.0f;
    };
} // namespace Zero