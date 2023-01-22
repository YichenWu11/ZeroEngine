#include <backends/imgui_impl_win32.h>

#include "runtime/function/event/application_event.h"
#include "runtime/function/event/key_event.h"
#include "runtime/function/event/mouse_event.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/window_system/i_window_system.h"
#include "runtime/function/render/window_system/window_system.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Zero {

#define EVENT_CALLBACK(e) \
    if (m_data.event_callback) m_data.event_callback(e);

    WindowSystem::WindowData WindowSystem::m_data;
    POINT                    WindowSystem::m_last_mouse_pos;

    LRESULT CALLBACK
    MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return true;

        return WindowSystem::msgProc(hwnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK
    WindowSystem::msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
            // WM_SIZE is sent when the user resizes the window.
            case WM_SIZE: {
                // Save the new client area dimensions.
                m_data.width  = LOWORD(lParam);
                m_data.height = HIWORD(lParam);

                WindowResizeEvent event(m_data.width, m_data.height);
                EVENT_CALLBACK(event)
                return 0;
            }

            // WM_CLOSE is sent when the user close the window
            case WM_CLOSE: {
                WindowCloseEvent event;
                EVENT_CALLBACK(event)
                return 0;
            }

            // KEY ACTION
            case WM_KEYDOWN: {
                int             repeat_count = int32_t(lParam) & 0xFF;
                KeyPressedEvent event(wParam, repeat_count);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_KEYUP: {
                KeyReleasedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_CHAR: {
                KeyTypedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }

            // MOUSE BUTTON
            case WM_LBUTTONDOWN: {
                MouseButtonPressedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_MBUTTONDOWN: {
                MouseButtonPressedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_RBUTTONDOWN: {
                MouseButtonPressedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_LBUTTONUP: {
                MouseButtonReleasedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_MBUTTONUP: {
                MouseButtonReleasedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }
            case WM_RBUTTONUP: {
                MouseButtonReleasedEvent event(wParam);
                EVENT_CALLBACK(event)
                return 0;
            }

            // MOUSE MOVE
            case WM_MOUSEMOVE: {
                // should be offset
                MouseMovedEvent event(
                    static_cast<float>(GET_X_LPARAM(lParam)),
                    static_cast<float>(GET_Y_LPARAM(lParam)));

                m_last_mouse_pos.x = GET_X_LPARAM(lParam);
                m_last_mouse_pos.y = GET_Y_LPARAM(lParam);

                EVENT_CALLBACK(event)
                return 0;
            }

            // SCROLL
            case WM_MOUSEWHEEL: {
                MouseScrolledEvent event(
                    static_cast<float>(GET_X_LPARAM(lParam)),
                    static_cast<float>(GET_Y_LPARAM(lParam)));
                EVENT_CALLBACK(event)
                return 0;
            }

            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            }
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // *************************************************************************************************
    // *************************************************************************************************
    // *************************************************************************************************

    IWindowSystem* IWindowSystem::create(const WindowCreateInfo& create_info) {
        return new WindowSystem(create_info);
    }

    WindowSystem::WindowSystem(const WindowCreateInfo& create_info) {
        init(create_info);
    }

    WindowSystem::~WindowSystem() {
        delete m_context;
        shutdown();
    }

    void WindowSystem::init(const WindowCreateInfo& create_info) {
        m_data.title  = create_info.title;
        m_data.width  = create_info.width;
        m_data.height = create_info.height;

        LOG_INFO("creating window {0} ({1}, {2})", create_info.title, create_info.width, create_info.height);

        WNDCLASSEXW wc = {
            sizeof(wc),
            CS_CLASSDC,
            MainWndProc,
            0L,
            0L,
            GetModuleHandle(NULL), NULL, NULL, (HBRUSH)GetStockObject(NULL_BRUSH), NULL, L"MainWnd", NULL};

        if (!::RegisterClassExW(&wc))
            LOG_CRITICAL("RegisterClass Failed.");

        RECT rc;
        SetRect(&rc, 0, 0, m_data.width, m_data.height);
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

        if (create_info.is_fullscreen)
            m_window = CreateWindow(
                wc.lpszClassName,
                L"Zero Engine",
                WS_POPUPWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZE | WS_SIZEBOX,
                300, 200,
                m_data.width, m_data.height,
                NULL, NULL, wc.hInstance, NULL);
        else
            m_window = CreateWindow(
                wc.lpszClassName,
                L"Zero Engine",
                WS_POPUPWINDOW | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX,
                300, 200,
                m_data.width, m_data.height,
                NULL, NULL, wc.hInstance, NULL);

        if (!m_window)
            LOG_CRITICAL("CreateWindow Failed.");

        m_context = new RenderContext(m_window);
        m_context->init(m_data.width, m_data.height);

        if (create_info.is_fullscreen)
            ShowWindow(m_window, SW_SHOWMAXIMIZED);
        else
            ShowWindow(m_window, SW_SHOWDEFAULT);
        UpdateWindow(m_window);

        setVSync(true);
    }

    void WindowSystem::shutdown() {
        ::DestroyWindow(m_window);
        ::UnregisterClassW(L"MainWnd", GetModuleHandle(NULL));
    }

    void WindowSystem::onUpdate() {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        m_context->swapBuffer();
    }

    void WindowSystem::onResize(int width, int height) {
        m_context->onResize(width, height);
    }

    void WindowSystem::setVSync(bool enabled) {
        if (enabled) {
            // do something to enable vertical sync
        }

        m_data.v_sync = enabled;
    }

    bool WindowSystem::isVSync() const {
        return m_data.v_sync;
    }

} // namespace Zero