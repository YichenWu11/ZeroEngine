#pragma once

namespace Chen::CDX12 {
    class FrameResource;
} // namespace Chen::CDX12

namespace Zero {
    struct RenderPassConfiguration {
    };

    class RenderPass {
    public:
        RenderPass()          = default;
        virtual ~RenderPass() = default;

        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;

        virtual void preLoadResource()                                 = 0;
        virtual void delayDisposeResource(Chen::CDX12::FrameResource&) = 0;

        virtual void onResize() {}

        virtual void drawPass(Chen::CDX12::FrameResource&, uint32_t frameIndex, bool offscreen = true)                  = 0;
        virtual void drawPassIndirect(Chen::CDX12::FrameResource& frameRes, uint32_t frameIndex, bool offscreen = true) = 0;
    };
} // namespace Zero