#include "runtime/function/input/input_system.h"

namespace Zero {
    Zero::Scope<InputSystem> InputSystem::s_instance = Zero::CreateScope<InputSystem>();

    bool InputSystem::isKeyPressed(int keycode) {
        return GetAsyncKeyState(keycode) & 0x8000;
    }

    bool InputSystem::isMouseButtonPressed(int button) {
        return GetAsyncKeyState(button) & 0x8000;
    }

    DirectX::SimpleMath::Vector2 InputSystem::getMousePosition() {
        POINT pos;
        bool  success = GetCursorPos(&pos);
        ZE_ASSERT(success && "GetCursorPos failed!");
        ScreenToClient(FindWindow(L"MainWnd", NULL), &pos);
        return {(float)pos.x, (float)pos.y};
    }

    float InputSystem::getMouseX() {
        auto pos = getMousePosition();
        return pos.x;
    }

    float InputSystem::getMouseY() {
        auto pos = getMousePosition();
        return pos.y;
    }
} // namespace Zero
