#pragma once

/*
    Main Pass in 2d rendering. (including the ui_pass)
*/

#include "runtime/function/render/render_system/render_pass.h"

namespace Chen::CDX12 {
    class UploadBuffer;
} // namespace Chen::CDX12

using Microsoft::WRL::ComPtr;

namespace Zero {
    // TODO: ObjectConstant Here
    struct ObjectConstant2D {
        Matrix   transform;
        Color    modulate;
        uint32_t tex_index{0};
        int32_t  entity_id{-1};
        uint32_t pad0{0};
        uint32_t pad1{0};
        float    tiling_factor{1.0f};
    };

    class MainCameraPass2D final : public RenderPass {
    public:
        MainCameraPass2D();
        ~MainCameraPass2D();

        void preLoadResource() override;
        void delayDisposeResource(Chen::CDX12::FrameResource&) override;
        void drawPass(Chen::CDX12::FrameResource&, uint32_t frameIndex, bool offscreen = true) override;
        void drawPassIndirect(Chen::CDX12::FrameResource& frameRes, uint32_t frameIndex, bool offscreen = true) override;

    private:
        void preZSortPass();

    private:
        Chen::CDX12::UploadBuffer* m_indirectDrawBuffer[3] = {nullptr};
    };
} // namespace Zero