#pragma once

#include <sstream>

#include "runtime/function/event/event.h"

namespace Zero {
    class KeyEvent : public Event {
    public:
        inline int getKeyCode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(int keycode) :
            m_keycode(keycode) {}

        int m_keycode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode, int repeatCount) :
            KeyEvent(keycode), m_repeat_count(repeatCount) {}

        inline int getRepeatCount() const { return m_repeat_count; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keycode << " (" << m_repeat_count << " repeats)";
            return ss.str();
        }

        static EventType    getStaticType() { return EventType::KeyPressed; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "KeyPressed"; }

    private:
        int m_repeat_count;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keycode) :
            KeyEvent(keycode) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keycode;
            return ss.str();
        }

        static EventType    getStaticType() { return EventType::KeyReleased; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "KeyReleased"; }
    };

} // namespace Zero