#pragma once

#include "runtime/zero.h"

using namespace DirectX::SimpleMath;

namespace Zero {
    class EditorLayer : public Zero::Layer {
    public:
        EditorLayer();
        ~EditorLayer() = default;

        void onAttach() override;
        void onDetach() override;
        void onUpdate(Zero::TimeStep timestep) override;
        void onImGuiRender() override;
        void onEvent(Zero::Event& event) override;

    private:
        Zero::OrthographicsCameraController m_camera_controller;

        Vector2 m_viewport_size;

        bool m_dockspace_enable = true;

        Zero::Ref<Zero::SubTexture2D> m_texture_stair;
        Zero::Ref<Zero::SubTexture2D> m_texture_bush;

        Vector2 position{0.0f, 0.0f};
    };
} // namespace Zero