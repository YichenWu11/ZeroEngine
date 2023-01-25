#pragma once

#include "runtime/zero.h"

using namespace DirectX::SimpleMath;

class Sandbox2D : public Zero::Layer {
public:
    Sandbox2D() :
        Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f) {}
    virtual ~Sandbox2D() = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Zero::TimeStep timestep) override;
    void onImGuiRender() override;
    void onEvent(Zero::Event& event) override;

private:
    Zero::OrthographicsCameraController m_camera_controller;

    Color clear_color{0.2f, 0.2f, 0.2f, 1.0f};
};