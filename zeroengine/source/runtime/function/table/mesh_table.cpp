#include "runtime/function/table/mesh_table.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    void MeshTable::buildBasicMesh() {
        ZE_ASSERT(m_render_context && "Bind the render_context first (MeshTable)!");

        ID3D12Device* device = m_render_context->getGraphicsDevice();

        static VertexBufferLayout               layout;
        static std::vector<rtti::Struct const*> structs;
        structs.emplace_back(&layout);

        float vertices_tri[] = {
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            0.1f,
            0.0f,
            0.5f,
            0.0f,
            1.0f,
            1.0f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
        };

        uint32_t indices_tri[] = {0, 1, 2};

        float vertices_square[] = {
            -0.5f,
            -0.5f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            0.0f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            1.0f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            1.0f,
        };

        uint32_t indices_square[] = {0, 2, 1, 2, 3, 1};

        ComPtr<ID3D12CommandAllocator>    cmdAllocator;
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(cmdAllocator.GetAddressOf())));
        ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
        ThrowIfFailed(commandList->Close());
        ThrowIfFailed(cmdAllocator->Reset());
        ThrowIfFailed(commandList->Reset(cmdAllocator.Get(), nullptr));

        auto triangle_mesh = std::make_shared<Mesh>(
            device,
            structs,
            array_count(vertices_tri) * sizeof(float) / layout.structSize,
            array_count(indices_tri));

        VertexBuffer* vert_buffer_tri = VertexBuffer::create(device,
                                                             vertices_tri,
                                                             array_count(vertices_tri) * sizeof(float));

        IndexBuffer* indi_buffer_tri = IndexBuffer::create(device,
                                                           indices_tri,
                                                           array_count(indices_tri) * sizeof(uint32_t));

        vert_buffer_tri->bind(commandList.Get(), triangle_mesh.get());
        indi_buffer_tri->bind(commandList.Get(), triangle_mesh.get());

        auto square_mesh = std::make_shared<Mesh>(
            device,
            structs,
            array_count(vertices_square) * sizeof(float) / layout.structSize,
            array_count(indices_square));

        VertexBuffer* vert_buffer_square = VertexBuffer::create(device,
                                                                vertices_square,
                                                                array_count(vertices_square) * sizeof(float));

        IndexBuffer* indi_buffer_square = IndexBuffer::create(device,
                                                              indices_square,
                                                              array_count(indices_square) * sizeof(uint32_t));

        vert_buffer_square->bind(commandList.Get(), square_mesh.get());
        indi_buffer_square->bind(commandList.Get(), square_mesh.get());

        ThrowIfFailed(commandList->Close());

        ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
        m_render_context->getCommandQueue()->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
        m_render_context->flush();

        m_mesh_table["triangle"] = std::move(triangle_mesh);
        m_mesh_table["square"]   = std::move(square_mesh);

        delete vert_buffer_tri;
        delete indi_buffer_tri;
        delete vert_buffer_square;
        delete indi_buffer_square;
    }

    void MeshTable::registerMesh(const std::string& mesh_name, float* vertices, uint32_t* indices) {
        ZE_ASSERT(m_render_context && "Bind the render_context first (MeshTable)!");
    }

    Mesh* MeshTable::getMesh(const std::string& mesh_name) {
        ZE_ASSERT(m_render_context && "Bind the render_context first (MeshTable)!");

        if (m_mesh_table.contains(mesh_name))
            return m_mesh_table[mesh_name].get();
        LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        return nullptr;
    }

} // namespace Zero