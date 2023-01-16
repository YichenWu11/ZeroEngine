#pragma once

#include "runtime/function/event/event.h"

namespace Zero {
    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) :
            m_width(width), m_height(height) {}

        inline unsigned int getWidth() const { return m_width; }
        inline unsigned int getHeight() const { return m_height; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }

        static EventType    getStaticType() { return EventType::WindowResize; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "WindowResize"; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        unsigned int m_width, m_height;
    };

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        static EventType    getStaticType() { return EventType::WindowClose; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "WindowClose"; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppTickEvent : public Event {
    public:
        AppTickEvent() {}

        static EventType    getStaticType() { return EventType::AppTick; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "AppTick"; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : public Event {
    public:
        AppUpdateEvent() {}

        static EventType    getStaticType() { return EventType::AppUpdate; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "AppUpdate"; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : public Event {
    public:
        AppRenderEvent() {}

        static EventType    getStaticType() { return EventType::AppRender; }
        virtual EventType   getEventType() const override { return getStaticType(); }
        virtual const char* getName() const override { return "AppRender"; }

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
} // namespace Zero