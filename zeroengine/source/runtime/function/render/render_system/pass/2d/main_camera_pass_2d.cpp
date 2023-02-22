#include <CDX12/FrameResource.h>
#include <CDX12/Resource/Mesh.h>
#include <CDX12/Shader/BasicShader.h>
#include <CDX12/Shader/PSOManager.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "runtime/function/pool/mesh_pool.h"
#include "runtime/function/pool/texture_pool.h"
#include "runtime/function/render/render_system/pass/2d/main_camera_pass_2d.h"
#include "runtime/function/render/render_system/render_context.h"
#include "runtime/function/render/render_system/renderer_api.h"
#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/resource/config_manager/config_manager.h"

using namespace Chen::CDX12;

namespace Zero {
    MainCameraPass2D::MainCameraPass2D() {
        LOG_INFO("-- MainCameraPass2D register success.");
    }

    MainCameraPass2D::~MainCameraPass2D() {
        RenderContext& render_context = GET_RENDER_CONTEXT();
        for (int i = 0; i < render_context.s_frame_count; ++i) {
            if (m_indirectDrawBuffer[i] != nullptr) {
                delete m_indirectDrawBuffer[i];
                m_indirectDrawBuffer[i] = nullptr;
            }
        }
    }

    void MainCameraPass2D::preLoadResource() {
        auto         shader_path = GET_CONFIG_MNGR().getRootFolder() / "zeroengine/shader/2d/shader.hlsl";
        std::wstring path        = AnsiToWString(shader_path.string());

        std::vector<std::pair<std::string, Shader::Property>> properties;
        properties.emplace_back(
            "_PassConstant",
            Shader::Property(
                ShaderVariableType::ConstantBuffer,
                0,
                0,
                0));
        properties.emplace_back(
            "_ObjectConstant",
            Shader::Property(
                ShaderVariableType::ConstantBuffer,
                0,
                1,
                0));
        properties.emplace_back(
            "TextureMap",
            Shader::Property(
                ShaderVariableType::SRVDescriptorHeap,
                0,
                0,
                168));

        GET_SHADER_BIND_TABLE().registerShader("common", properties);
        BasicShader* shader =
            static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("common"));

        shader->SetVsShader(path.c_str());
        shader->SetPsShader(path.c_str());
        shader->rasterizerState          = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        shader->blendState               = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        auto&& depthStencilState         = shader->depthStencilState;
        depthStencilState.DepthEnable    = true;
        depthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;
        depthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depthStencilState.StencilEnable  = false;

        GET_SHADER_BIND_TABLE().registerShader("transparent", properties);
        BasicShader* trans_shader =
            static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("transparent"));
        trans_shader->SetVsShader(path.c_str());

        if (RendererAPI::isEditorMode()) {
            const D3D_SHADER_MACRO shader_macro[] = {
                "EDITOR_MODE", "1",
                NULL, NULL};

            trans_shader->SetPsShader(path.c_str(), shader_macro);
        }
        else {
            trans_shader->SetPsShader(path.c_str());
        }
        trans_shader->rasterizerState          = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        trans_shader->rasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        // trans_shader->rasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;

        trans_shader->blendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
        transparencyBlendDesc.BlendEnable   = true;
        transparencyBlendDesc.LogicOpEnable = false;

        transparencyBlendDesc.SrcBlend  = D3D12_BLEND_SRC_ALPHA;
        transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        transparencyBlendDesc.BlendOp   = D3D12_BLEND_OP_ADD;

        transparencyBlendDesc.SrcBlendAlpha  = D3D12_BLEND_ONE;
        transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
        transparencyBlendDesc.BlendOpAlpha   = D3D12_BLEND_OP_ADD;

        transparencyBlendDesc.LogicOp               = D3D12_LOGIC_OP_NOOP;
        transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        trans_shader->blendState.RenderTarget[0]      = transparencyBlendDesc;
        trans_shader->depthStencilState.DepthFunc     = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        trans_shader->depthStencilState.StencilEnable = true;
    }

    void MainCameraPass2D::delayDisposeResource(FrameResource&) {
        // do nothing
    }

    void MainCameraPass2D::preZSortPass() {
        RenderContext& render_context = GET_RENDER_CONTEXT();

        // NOTE: trick here, get the z-index from the _34 of the transform matrix
        std::sort(render_context.m_draw_2d_list.begin(), render_context.m_draw_2d_list.end(),
                  [](std::tuple<Ref<Mesh>, ObjectConstant2D> a, std::tuple<Ref<Mesh>, ObjectConstant2D> b) {
            return std::get<1>(a).transform._34 > std::get<1>(b).transform._34;
        });
    }

    // **************************************************************************************************

    /*
        direct draw
            - if `offscreen`, render to m_framebuffer; else render to m_renderTarget
            - and render imgui_data together
    */
    void MainCameraPass2D::drawPass(FrameResource& frameRes, uint32_t frameIndex, bool offscreen) {
        preZSortPass();

        RenderContext& render_context = GET_RENDER_CONTEXT();

        if (offscreen) {
            if (offscreen) {
                render_context.m_stateTracker.RecordState(
                    render_context.m_frameBuffers[frameIndex]->getInnerTexture(),
                    D3D12_RESOURCE_STATE_RENDER_TARGET);
            }
            else {
                render_context.m_stateTracker.RecordState(
                    render_context.m_renderTargets[frameIndex].get(),
                    D3D12_RESOURCE_STATE_RENDER_TARGET);
            }
            render_context.m_stateTracker.RecordState(
                render_context.m_depthTargets[frameIndex].get(),
                D3D12_RESOURCE_STATE_DEPTH_WRITE);

            render_context.m_stateTracker.UpdateState(render_context.m_currframe_cmdlist.Get());

            CD3DX12_CPU_DESCRIPTOR_HANDLE off_rtvHandle(
                render_context.m_rtvCpuDH.GetCpuHandle(3),
                frameIndex,
                render_context.m_rtvCpuDH.GetDescriptorSize());

            CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
                render_context.m_dsvCpuDH.GetCpuHandle(0),
                frameIndex,
                render_context.m_dsvCpuDH.GetDescriptorSize());

            CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
                render_context.m_rtvCpuDH.GetCpuHandle(0),
                frameIndex,
                render_context.m_rtvCpuDH.GetDescriptorSize());

            if (offscreen) {
                frameRes.SetRenderTarget(
                    render_context.m_frameBuffers[frameIndex]->getInnerTexture(),
                    &off_rtvHandle,
                    &dsvHandle);
                frameRes.ClearRTV(off_rtvHandle);
            }
            else {
                frameRes.SetRenderTarget(
                    render_context.m_renderTargets[frameIndex].get(),
                    &rtvHandle,
                    &dsvHandle);
                frameRes.ClearRTV(rtvHandle);
            }

            frameRes.ClearDSV(dsvHandle);

            render_context.m_currframe_cmdlist->SetDescriptorHeaps(
                1,
                get_rvalue_ptr(GET_TEXTURE_POOL().getTexAllocation()->GetDescriptorHeap()));

            BasicShader* shader =
                static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("transparent"));

            auto& prop_table = GET_SHADER_BIND_TABLE().getShaderPropTable(shader);

            // draw call
            for (auto& [mesh, obj_constant] : render_context.m_draw_2d_list) {
                // bind object-varying constants
                GET_SHADER_BIND_TABLE()
                    .bindParam(
                        shader,
                        "_ObjectConstant",
                        std::span<const uint8_t>{
                            reinterpret_cast<uint8_t const*>(&obj_constant),
                            sizeof(obj_constant)});

                render_context.m_bindProperties.clear();

                // bind the constants which does not differ among different objects
                for (auto& prop : prop_table) {
                    auto name = prop.first;
                    std::visit(overloaded{
                                   [&](std::span<const uint8_t> data) {
                        auto buffer = frameRes.AllocateConstBuffer(data);
                        render_context.m_bindProperties.emplace_back(name, buffer);
                                   },
                                   [&](std::pair<DescriptorHeapAllocation const*, uint32_t> data) {
                        render_context.m_bindProperties.emplace_back(name, DescriptorHeapAllocView(data.first, data.second));
                    }},
                        prop.second);
                }

                frameRes.DrawMesh(
                    shader,
                    render_context.m_psoManager.get(),
                    mesh.get(),
                    render_context.s_colorFormat,
                    render_context.s_depthFormat,
                    render_context.m_bindProperties);
            }

            if (!offscreen) {
                render_context.m_currframe_cmdlist->SetDescriptorHeaps(
                    1,
                    get_rvalue_ptr(GET_TEXTURE_POOL().getTexAllocation()->GetDescriptorHeap()));

                ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), render_context.m_currframe_cmdlist.Get());
            }

            render_context.m_stateTracker.RestoreState(render_context.m_currframe_cmdlist.Get());
        }
    }

    /*
        multi-indirect draw
            - if `offscreen`, render to m_framebuffer; else render to m_renderTarget
            - and render imgui_data together
    */
    void MainCameraPass2D::drawPassIndirect(FrameResource& frameRes, uint32_t frameIndex, bool offscreen) {
        preZSortPass();

        RenderContext& render_context = GET_RENDER_CONTEXT();

        if (offscreen) {
            render_context.m_stateTracker.RecordState(
                render_context.m_frameBuffers[frameIndex]->getInnerTexture(),
                D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
        else {
            render_context.m_stateTracker.RecordState(
                render_context.m_renderTargets[frameIndex].get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
        render_context.m_stateTracker.RecordState(
            render_context.m_depthTargets[frameIndex].get(),
            D3D12_RESOURCE_STATE_DEPTH_WRITE);

        render_context.m_stateTracker.UpdateState(render_context.m_currframe_cmdlist.Get());

        CD3DX12_CPU_DESCRIPTOR_HANDLE off_rtvHandle(
            render_context.m_rtvCpuDH.GetCpuHandle(3),
            frameIndex,
            render_context.m_rtvCpuDH.GetDescriptorSize());

        CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
            render_context.m_dsvCpuDH.GetCpuHandle(0),
            frameIndex,
            render_context.m_dsvCpuDH.GetDescriptorSize());

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
            render_context.m_rtvCpuDH.GetCpuHandle(0),
            frameIndex,
            render_context.m_rtvCpuDH.GetDescriptorSize());

        if (offscreen) {
            frameRes.SetRenderTarget(
                render_context.m_frameBuffers[frameIndex]->getInnerTexture(),
                &off_rtvHandle,
                &dsvHandle);
            frameRes.ClearRTV(off_rtvHandle);
        }
        else {
            frameRes.SetRenderTarget(
                render_context.m_renderTargets[frameIndex].get(),
                &rtvHandle,
                &dsvHandle);
            frameRes.ClearRTV(rtvHandle);
        }

        frameRes.ClearDSV(dsvHandle);

        render_context.m_currframe_cmdlist->SetDescriptorHeaps(
            1,
            get_rvalue_ptr(GET_TEXTURE_POOL().getTexAllocation()->GetDescriptorHeap()));

        BasicShader* shader =
            static_cast<BasicShader*>(GET_SHADER_BIND_TABLE().getShader("transparent"));

        auto& prop_table = GET_SHADER_BIND_TABLE().getShaderPropTable(shader);

        render_context.m_bindProperties.clear();

        // bind the constants which does not differ among different objects
        for (auto& prop : prop_table) {
            auto name = prop.first;
            std::visit(overloaded{
                           [&](std::span<const uint8_t> data) {
                auto buffer = frameRes.AllocateConstBuffer(data);
                render_context.m_bindProperties.emplace_back(name, buffer);
                           },
                           [&](std::pair<DescriptorHeapAllocation const*, uint32_t> data) {
                render_context.m_bindProperties.emplace_back(name, DescriptorHeapAllocView(data.first, data.second));
            }},
                prop.second);
        }

        // delete last frame uploadbuffer
        auto last_frame_index = (frameIndex + render_context.s_frame_count - 1) % render_context.s_frame_count;
        if (m_indirectDrawBuffer[last_frame_index] != nullptr) {
            delete m_indirectDrawBuffer[last_frame_index];
            m_indirectDrawBuffer[last_frame_index] = nullptr;
        }

        if (!render_context.m_draw_2d_list.empty()) {
            auto mesh_count = render_context.m_draw_2d_list.size();

            m_indirectDrawBuffer[frameIndex] = new UploadBuffer(
                render_context.getGraphicsDevice(),
                mesh_count * sizeof(IndirectDrawCommand));

            m_indirectDrawBuffer[frameIndex]->DelayDispose(&frameRes);

            int  cnt                    = 0;
            auto indirectDrawBufferData = std::vector<IndirectDrawCommand>(mesh_count);
            auto obj_constant_array     = std::vector<ObjectConstant2D>(mesh_count);

            // draw call
            for (auto& [mesh, obj_constant] : render_context.m_draw_2d_list) {
                // NOTE: assign
                obj_constant_array[cnt].transform     = obj_constant.transform;
                obj_constant_array[cnt].modulate      = obj_constant.modulate;
                obj_constant_array[cnt].tex_index     = obj_constant.tex_index;
                obj_constant_array[cnt].tiling_factor = obj_constant.tiling_factor;
                obj_constant_array[cnt].entity_id     = obj_constant.entity_id;

                auto obj_cbuffer = frameRes.AllocateConstBuffer(
                    std::span<const uint8_t>{
                        reinterpret_cast<uint8_t const*>(&obj_constant_array[cnt]),
                        sizeof(obj_constant_array[cnt])});

                // NOTE: cnt + 1
                indirectDrawBufferData[cnt] = (frameRes.getIndirectArguments(mesh.get(), obj_cbuffer.buffer->GetAddress(), cnt + 1, sizeof(ObjectConstant2D)));

                m_indirectDrawBuffer[frameIndex]->CopyData(cnt * sizeof(IndirectDrawCommand),
                                                           {reinterpret_cast<vbyte const*>(&(indirectDrawBufferData[cnt])),
                                                            sizeof(IndirectDrawCommand)});

                cnt++;
            }

            // indirect draw call
            frameRes.DrawMeshIndirect(
                shader,
                render_context.m_psoManager.get(),
                (std::get<0>(render_context.m_draw_2d_list[0]))->Layout(),
                render_context.s_colorFormat,
                render_context.s_depthFormat,
                render_context.m_bindProperties,
                m_indirectDrawBuffer[frameIndex],
                mesh_count,
                render_context.m_command_signature.Get());
        }

        if (!offscreen) {
            render_context.m_currframe_cmdlist->SetDescriptorHeaps(
                1,
                get_rvalue_ptr(GET_TEXTURE_POOL().getTexAllocation()->GetDescriptorHeap()));

            ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), render_context.m_currframe_cmdlist.Get());
        }

        render_context.m_stateTracker.RestoreState(render_context.m_currframe_cmdlist.Get());
    }
} // namespace Zero