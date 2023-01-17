#pragma once

#include "runtime/function/event/event.h"

namespace Zero {
    struct WindowCreateInfo {
        int         width{1280};
        int         height{720};
        std::string title{"ZeroEngine"};
        bool        is_fullscreen{false};
    };

    class IWindowSystem {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~IWindowSystem() = default;

        virtual void onUpdate() = 0;

        virtual int getWidth() const  = 0;
        virtual int getHeight() const = 0;

        // window attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled)                            = 0;
        virtual bool isVSync() const                                   = 0;

        static IWindowSystem* create(const WindowCreateInfo& create_info = WindowCreateInfo());
    };

} // namespace Zero
