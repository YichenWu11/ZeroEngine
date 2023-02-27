#include "runtime/resource/mesh.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    static VertexBufferLayout               layout;
    static std::vector<rtti::Struct const*> structs;

    Resource<ResourceType::Mesh>::Resource(std::string_view name) :
        IResource(name) {
    }

    Resource<ResourceType::Mesh>::Resource(std::string_view name,
                                           VertexData2D*    vertices,
                                           uint32_t         vertices_count,
                                           uint32_t*        indices,
                                           uint32_t         indices_count) :
        IResource(name) {
        structs.clear();
        structs.emplace_back(&layout);

        ComPtr<ID3D12CommandAllocator>    m_cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        DXRawDevicePtr                    device = RenderContext::getInstance().getGraphicsDevice();

        ThrowIfFailed(
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.GetAddressOf())));
        ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
        ThrowIfFailed(m_commandList->Close());
        ThrowIfFailed(m_cmdAllocator->Reset());
        ThrowIfFailed(m_commandList->Reset(m_cmdAllocator.Get(), nullptr));

        m_mesh = Zero::CreateScope<Mesh>(
            device,
            structs,
            vertices_count * sizeof(VertexData2D) / layout.structSize,
            indices_count);

        auto vert_buffer_square = VertexBuffer::create(
            reinterpret_cast<float*>(vertices),
            vertices_count * sizeof(VertexData2D));

        auto indi_buffer_square = IndexBuffer::create(
            indices,
            indices_count * sizeof(uint32_t));

        vert_buffer_square->bind(m_commandList.Get(), m_mesh.get());
        indi_buffer_square->bind(m_commandList.Get(), m_mesh.get());

        ThrowIfFailed(m_commandList->Close());

        ID3D12CommandList* ppM_commandLists[] = {m_commandList.Get()};
        RenderContext::getInstance().getCommandQueue()->ExecuteCommandLists(array_count(ppM_commandLists), ppM_commandLists);
        RenderContext::getInstance().flush();

        LOG_INFO("register mesh named {0} success!", name);
    }

} // namespace Zero
