#include "runtime/function/table/mesh_table.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    void MeshTable::buildBasicMesh() {
        ID3D12Device* device = RenderContext::getInstance().getGraphicsDevice();

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
            1.0f,
            0.5f,
            -0.5f,
            0.0f,
            1.0f,
            1.0f,
            -0.5f,
            0.5f,
            0.0f,
            0.0f,
            0.0f,
            0.5f,
            0.5f,
            0.0f,
            1.0f,
            0.0f,
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

        auto triangle_mesh = Zero::CreateRef<Mesh>(
            device,
            structs,
            array_count(vertices_tri) * sizeof(float) / layout.structSize,
            array_count(indices_tri));

        auto vert_buffer_tri = VertexBuffer::create(
            vertices_tri,
            array_count(vertices_tri) * sizeof(float));

        auto indi_buffer_tri = IndexBuffer::create(
            indices_tri,
            array_count(indices_tri) * sizeof(uint32_t));

        vert_buffer_tri->bind(commandList.Get(), triangle_mesh.get());
        indi_buffer_tri->bind(commandList.Get(), triangle_mesh.get());

        auto square_mesh = Zero::CreateRef<Mesh>(
            device,
            structs,
            array_count(vertices_square) * sizeof(float) / layout.structSize,
            array_count(indices_square));

        auto vert_buffer_square = VertexBuffer::create(
            vertices_square,
            array_count(vertices_square) * sizeof(float));

        auto indi_buffer_square = IndexBuffer::create(
            indices_square,
            array_count(indices_square) * sizeof(uint32_t));

        vert_buffer_square->bind(commandList.Get(), square_mesh.get());
        indi_buffer_square->bind(commandList.Get(), square_mesh.get());

        ThrowIfFailed(commandList->Close());

        ID3D12CommandList* ppCommandLists[] = {commandList.Get()};
        RenderContext::getInstance().getCommandQueue()->ExecuteCommandLists(array_count(ppCommandLists), ppCommandLists);
        RenderContext::getInstance().flush();

        m_mesh_table["triangle"] = std::move(triangle_mesh);
        m_mesh_table["square"]   = std::move(square_mesh);
    }

    void MeshTable::registerMesh(const std::string& mesh_name, float* vertices, uint32_t* indices) {
    }

    void MeshTable::removeMesh(const std::string& mesh_name) {
        if (m_mesh_table.contains(mesh_name))
            m_mesh_table.erase(mesh_name);
        else
            LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
    }

    void MeshTable::delayDisposeMesh(const std::string& mesh_name, Chen::CDX12::FrameResource* frameres) {
        if (!m_mesh_table.contains(mesh_name))
            LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        m_mesh_table[mesh_name]->DelayDispose(frameres);
    }

    Zero::Ref<Chen::CDX12::Mesh> MeshTable::getMesh(const std::string& mesh_name) {
        if (m_mesh_table.contains(mesh_name))
            return m_mesh_table[mesh_name];
        LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        return nullptr;
    }

} // namespace Zero