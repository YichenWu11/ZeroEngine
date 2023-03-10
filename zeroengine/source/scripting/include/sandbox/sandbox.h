#pragma once

#include "sandbox/example_layer.h"

class Sandbox : public Zero::Application {
public:
    Sandbox() {
        pushLayer(new ExampleLayer());
    }

    ~Sandbox() {
    }
};

Zero::Application* createApp() {
    return new Sandbox();
}
