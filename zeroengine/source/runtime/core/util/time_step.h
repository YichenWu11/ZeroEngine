#pragma once

namespace Zero {
    class TimeStep {
    public:
        TimeStep(float time = 0.0f) :
            m_time(time) {
        }

        operator float() const { return m_time; }

        float getSeconds() const { return m_time; }
        float getMillSeconds() const { return m_time * 1000.0f; }

    private:
        float m_time = 0.0f;
    };
} // namespace Zero