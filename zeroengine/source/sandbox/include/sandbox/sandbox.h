#pragma once

#include "sandbox/example_layer.h"
#include "sandbox/sandbox_2d.h"

class Sandbox : public Zero::Application {
public:
    Sandbox() {
        // pushLayer(new ExampleLayer());
        pushLayer(new Sandbox2D());
    }

    ~Sandbox() {
    }
};

Zero::Application* createApplication() {
    Zero::RendererAPI::setEditorModeEnabled(false);
    return new Sandbox();
}
