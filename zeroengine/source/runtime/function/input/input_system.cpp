#include "runtime/function/input/input_system.h"

namespace Zero {
    bool InputSystem::isKeyPressed(int keycode) {
        return GetAsyncKeyState(keycode) & 0x8000;
    }

    bool InputSystem::isMouseButtonPressed(int button) {
        return GetAsyncKeyState(button) & 0x8000;
    }

    Vector2 InputSystem::getMousePosition() {
        POINT pos;
        bool  success = GetCursorPos(&pos);
        ASSERT(success, "GetCursorPos failed!");
        ScreenToClient(FindWindow(L"MainWnd", NULL), &pos);
        return {(float)pos.x, (float)pos.y};
    }

    float InputSystem::getMouseX() {
        return getMousePosition().x;
    }

    float InputSystem::getMouseY() {
        return getMousePosition().y;
    }
} // namespace Zero
