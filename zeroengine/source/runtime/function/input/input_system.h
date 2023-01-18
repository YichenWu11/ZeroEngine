#pragma once

namespace Zero {
    class InputSystem {
    public:
        static bool isKeyPressed(int keycode);

        static bool                    isMouseButtonPressed(int button);
        static std::pair<float, float> getMousePosition();
        static float                   getMouseX();
        static float                   getMouseY();

    private:
        static std::unique_ptr<InputSystem> s_instance;
    };
} // namespace Zero