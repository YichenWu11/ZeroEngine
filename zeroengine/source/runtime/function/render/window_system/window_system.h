#pragma once

#include "runtime/function/event/event.h"

namespace Zero {
    class RenderContext;

    struct WindowCreateInfo {
        int         width{1280};
        int         height{720};
        std::string title{"ZeroEngine"};
        bool        is_fullscreen{false};
    };

    class WindowSystem {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        WindowSystem(const WindowCreateInfo& create_info);
        virtual ~WindowSystem();

        void onUpdate();

        int getWidth() const { return m_data.width; }
        int getHeight() const { return m_data.height; }

        HWND getNativeWindowHandle() { return m_window; }

        // window attributes
        void setEventCallback(const EventCallbackFn& callback) { m_data.event_callback = callback; }
        void setVSync(bool enabled);
        bool isVSync() const;

    private:
        virtual void init(const WindowCreateInfo& create_info);
        virtual void shutdown();

    private:
        // hwnd
        HWND m_window;

        struct WindowData {
            int         width;
            int         height;
            std::string title;
            bool        v_sync;

            EventCallbackFn event_callback;
        };

        static WindowData m_data;

    public:
        static LRESULT CALLBACK
        msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    };

} // namespace Zero
