#include "runtime/function/input/input_system.h"

namespace Zero {
    std::unique_ptr<InputSystem> InputSystem::s_instance = std::make_unique<InputSystem>();

    bool InputSystem::isKeyPressed(int keycode) {
        return GetAsyncKeyState(keycode) & 0x0001;
    }

    bool InputSystem::isMouseButtonPressed(int button) {
        return GetAsyncKeyState(button) & 0x0001;
    }

    std::pair<float, float> InputSystem::getMousePosition() {
        POINT pos;
        bool  success = GetCursorPos(&pos);
        ZE_ASSERT(success && "GetCursorPos failed!");
        ScreenToClient(FindWindow(L"MainWnd", NULL), &pos);
        return {(float)pos.x, (float)pos.y};
    }

    float InputSystem::getMouseX() {
        auto [x, y] = getMousePosition();
        return x;
    }

    float InputSystem::getMouseY() {
        auto [x, y] = getMousePosition();
        return y;
    }
} // namespace Zero
