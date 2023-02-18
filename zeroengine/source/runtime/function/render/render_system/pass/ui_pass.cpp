#include <backends/imgui_impl_dx12.h>

#include "runtime/function/render/render_system/pass/ui_pass.h"
#include "runtime/function/render/render_system/render_context.h"

namespace Zero {
    UIPass::UIPass() {
        LOG_INFO("-- UIPass register success.");
    }

    UIPass::~UIPass() {
    }

    void UIPass::preLoadResource() {
    }

    void UIPass::delayDisposeResource(Chen::CDX12::FrameResource&) {
    }

    void UIPass::drawPass(Chen::CDX12::FrameResource& frameRes, uint32_t frameIndex, bool offscreen) {
        RenderContext& render_context = GET_RENDER_CONTEXT();

        if (offscreen) {
            render_context.m_stateTracker.RecordState(
                render_context.m_renderTargets[frameIndex].get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET);
            render_context.m_stateTracker.RecordState(
                render_context.m_depthTargets[frameIndex].get(),
                D3D12_RESOURCE_STATE_DEPTH_WRITE);

            render_context.m_stateTracker.UpdateState(render_context.m_currframe_cmdlist.Get());

            CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
                render_context.m_dsvCpuDH.GetCpuHandle(0),
                frameIndex,
                render_context.m_dsvCpuDH.GetDescriptorSize());

            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                render_context.m_rtvCpuDH.GetCpuHandle(0),
                frameIndex,
                render_context.m_rtvCpuDH.GetDescriptorSize());

            // ui_pass
            frameRes.SetRenderTarget(
                render_context.m_renderTargets[frameIndex].get(),
                &rtvHandle,
                &dsvHandle);
            frameRes.ClearRTV(rtvHandle);
            frameRes.ClearDSV(dsvHandle);

            ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), render_context.m_currframe_cmdlist.Get());

            render_context.m_stateTracker.RestoreState(render_context.m_currframe_cmdlist.Get());
        }
        else {
            // do nothing
        }
    }

    void UIPass::drawPassIndirect(Chen::CDX12::FrameResource& frameRes, uint32_t frameIndex, bool offscreen) {
        drawPass(frameRes, frameIndex, offscreen);
    }
} // namespace Zero