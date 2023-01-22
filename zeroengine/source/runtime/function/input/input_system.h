#pragma once

namespace Zero {
    class InputSystem {
    public:
        static bool isKeyPressed(int keycode);

        static bool                         isMouseButtonPressed(int button);
        static DirectX::SimpleMath::Vector2 getMousePosition();
        static float                        getMouseX();
        static float                        getMouseY();

    private:
        static Zero::Scope<InputSystem> s_instance;
    };
} // namespace Zero