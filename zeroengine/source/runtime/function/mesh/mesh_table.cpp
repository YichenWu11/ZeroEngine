#include <CDX12/Resource/Mesh.h>

#include "runtime/function/mesh/mesh_table.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

// TODO: fix

namespace Zero {
    std::unordered_map<std::string, std::unique_ptr<Chen::CDX12::Mesh>> MeshTable::s_mesh_table;

    void MeshTable::registerMesh(RenderContext* render_context, const std::string& mesh_name) {
        ID3D12Device* device = render_context->getGraphicsDevice();

        static VertexBufferLayout                  layout;
        static std::vector<::rtti::Struct const*> structs;
        structs.emplace_back(&layout);

        DirectX::XMFLOAT3 vertices[] = {
            DirectX::XMFLOAT3(-0.5, -0.5, 0.5),
            DirectX::XMFLOAT3(0, 0.5, 0.5),
            DirectX::XMFLOAT3(0.5, -0.5, 0.5)};

        uint                       indices[] = {0, 1, 2};
        ComPtr<ID3D12CommandAllocator>    cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
        ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
        ThrowIfFailed(commandList->Close());
        ThrowIfFailed(cmdAllocator->Reset());
        ThrowIfFailed(commandList->Reset(cmdAllocator.Get(), nullptr));

        auto triangle_mesh = std::make_unique<Mesh>(
            device,
            structs,
            array_count(vertices),
            array_count(indices));

        VertexBuffer* vert_buffer = VertexBuffer::create(device,
                                                         vertices,
                                                         array_count(vertices) * sizeof(DirectX::XMFLOAT3));

        IndexBuffer* indi_buffer = IndexBuffer::create(device,
                                                       indices,
                                                       array_count(indices) * sizeof(uint32_t));

        vert_buffer->bind(commandList.Get(), triangle_mesh.get());
        indi_buffer->bind(commandList.Get(), triangle_mesh.get());

        ThrowIfFailed(commandList->Close());

        ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
        render_context->getCommandQueue()->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
        render_context->flush();

        s_mesh_table[mesh_name] = std::move(triangle_mesh);

        delete vert_buffer;
        delete indi_buffer;
    }

    Mesh* MeshTable::getMesh(const std::string& mesh_name) {
        if (s_mesh_table.contains(mesh_name))
            return s_mesh_table[mesh_name].get();
        LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        return nullptr;
    }

} // namespace Zero