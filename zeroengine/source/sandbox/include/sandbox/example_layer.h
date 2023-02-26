#pragma once

#include "runtime/zero.h"

class ExampleLayer : public Zero::Layer {
public:
    ExampleLayer() :
        Layer("Example") {
    }

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Zero::TimeStep timestep) override;
    void onImGuiRender() override;
    void onEvent(Zero::Event& event) override;
    bool onWindowResizeEvent(Zero::WindowResizeEvent& event);

private:
    Zero::World m_world;
};