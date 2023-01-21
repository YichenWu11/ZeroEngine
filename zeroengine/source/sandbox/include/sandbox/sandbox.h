#pragma once

#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"
#include "runtime/function/gui/imgui_layer.h"
#include "runtime/function/input/input_system.h"

namespace Zero {
    class ExampleLayer : public Layer {
    public:
        ExampleLayer() :
            Layer("Example") {
        }

        void onUpdate() override {
            // if (InputSystem::isMouseButtonPressed(VK_LBUTTON))
            //     LOG_TRACE("lbutton is pressed");
            // if (InputSystem::isMouseButtonPressed(VK_RBUTTON))
            //     LOG_TRACE("rbutton is pressed");
            // if (InputSystem::isKeyPressed('A'))
            //     LOG_TRACE("A is pressed");
        }

        void onEvent(Event& event) override {
            // LOG_TRACE("{0}", event.toString());
        }
    };

    class Sandbox : public Application {
    public:
        Sandbox() {
            pushLayer(new ExampleLayer());
            // pushOverlay(new ImGuiLayer());
        }

        ~Sandbox() {
        }
    };

    Application* createApplication() {
        return new Sandbox();
    }
} // namespace Zero