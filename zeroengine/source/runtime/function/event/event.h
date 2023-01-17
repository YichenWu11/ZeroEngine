#pragma once

#include "runtime/core/common/core.h"

namespace Zero {

    // Events here are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType {
        None = 0,
        /* window */
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        /* app */
        AppTick,
        AppUpdate,
        AppRender,
        /* key */
        KeyPressed,
        KeyReleased,
        /* mouse button */
        MouseButtonPressed,
        MouseButtonReleased,
        /* mouse */
        MouseMoved,
        MouseScrolled
    };

    // filter
    enum EventCategory {
        None                     = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_CATEGORY(category) \
    virtual int getCategoryFlags() const override { return category; }

    class Event {
        friend class EventDispatcher;

    public:
        virtual EventType   getEventType() const     = 0;
        virtual const char* getName() const          = 0;
        virtual int         getCategoryFlags() const = 0;
        virtual std::string toString() const { return getName(); }

        inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; }

        bool m_handled = false;
    };

    class EventDispatcher {
        template <typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event) :
            m_event(event) {}

        template <typename T>
        bool Dispatch(EventFn<T> func) {
            if (m_event.getEventType() == T::getStaticType()) {
                m_event.m_handled = func(*(T*)&m_event);
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }

} // namespace Zero
