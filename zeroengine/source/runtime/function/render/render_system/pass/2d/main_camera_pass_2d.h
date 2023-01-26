#pragma once

/*
    Main Pass in 2d rendering. (including the ui_pass)
*/

#include "runtime/function/render/render_system/render_pass.h"

namespace Chen::CDX12 {
    class Mesh;
}

namespace Zero {
    class MainCameraPass2D final : public RenderPass {
    public:
        MainCameraPass2D();
        ~MainCameraPass2D();

        void preLoadResource() override;
        void delayDisposeResource(Chen::CDX12::FrameResource&) override;
        void drawPass(Chen::CDX12::FrameResource&, uint32_t frameIndex) override;

    private:
        // std::vector<std::tuple<Chen::CDX12::Mesh*, DirectX::SimpleMath::Matrix, DirectX::SimpleMath::Color, int32_t>> m_draw_2d_list;
    };
} // namespace Zero