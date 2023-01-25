#pragma once

#include "runtime/function/render/window_system/i_window_system.h"

namespace Zero {
    class RenderContext;

    class WindowSystem : public IWindowSystem {
    public:
        WindowSystem(const WindowCreateInfo& create_info);
        virtual ~WindowSystem();

        void onUpdate() override;

        int getWidth() const override { return m_data.width; }
        int getHeight() const override { return m_data.height; }

        HWND getNativeWindowHandle() override { return m_window; }

        RenderContext* getRenderContext() { return m_context.get(); }

        // window attributes
        void setEventCallback(const EventCallbackFn& callback) override { m_data.event_callback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

    private:
        virtual void init(const WindowCreateInfo& create_info);
        virtual void shutdown();

    private:
        // hwnd
        HWND                     m_window;
        Zero::Ref<RenderContext> m_context;

        static POINT m_last_mouse_pos;

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
