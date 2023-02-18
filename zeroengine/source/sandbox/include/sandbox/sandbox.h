#pragma once

#include "sandbox/example_layer.h"
#include "sandbox/sandbox_2d.h"

using namespace DirectX::SimpleMath;

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
