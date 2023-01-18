#pragma once

#include "runtime/function/event/event.h"

namespace Zero {
    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) :
            m_mouse_x(x), m_mouse_y(y) {}

        inline float getX() const { return m_mouse_x; }
        inline float getY() const { return m_mouse_y; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_mouse_x, m_mouse_y;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset) :
            m_x_offset(xOffset), m_y_offset(yOffset) {}

        inline float getXOffset() const { return m_x_offset; }
        inline float getYOffset() const { return m_y_offset; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_x_offset, m_y_offset;
    };

    class MouseButtonEvent : public Event {
    public:
        inline int getMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(int button) :
            m_button(button) {}

        int m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) :
            MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) :
            MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
} // namespace Zero
