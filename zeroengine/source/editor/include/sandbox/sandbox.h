#pragma once

#include "runtime/core/common/application.h"
#include "runtime/core/common/layer.h"

namespace Zero {
    class ExampleLayer : public Layer {
    public:
        ExampleLayer() :
            Layer("Example") {
        }

        void onUpdate() override {
            LOG_INFO("ExampleLayer::Update");
        }

        void onEvent(Event& event) override {
            LOG_TRACE("{0}", event.toString());
        }
    };

    class Sandbox : public Application {
    public:
        Sandbox() {
            pushLayer(new ExampleLayer());
        }

        ~Sandbox() {
        }
    };

    Application* createApplication() {
        return new Sandbox();
    }
} // namespace Zero