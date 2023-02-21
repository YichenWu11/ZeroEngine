#include "runtime/function/pool/mesh_pool.h"
#include "runtime/function/render/render_system/buffer.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    static VertexBufferLayout               layout;
    static std::vector<rtti::Struct const*> structs;

    void MeshPool::buildBasicMesh() {
        structs.emplace_back(&layout);

        DXRawDevicePtr device = GET_RENDER_CONTEXT().getGraphicsDevice();

        VertexData2D vertices_square[] = {
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
            {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
        };

        uint32_t indices_square[] = {0, 3, 1, 3, 2, 1};

        ThrowIfFailed(
            device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_cmdAllocator.GetAddressOf())));
        ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
        ThrowIfFailed(m_commandList->Close());
        ThrowIfFailed(m_cmdAllocator->Reset());
        ThrowIfFailed(m_commandList->Reset(m_cmdAllocator.Get(), nullptr));

        auto square_mesh = Zero::CreateRef<Mesh>(
            device,
            structs,
            array_count(vertices_square) * sizeof(VertexData2D) / layout.structSize,
            array_count(indices_square));

        auto vert_buffer_square = VertexBuffer::create(
            reinterpret_cast<float*>(vertices_square),
            array_count(vertices_square) * sizeof(VertexData2D));

        auto indi_buffer_square = IndexBuffer::create(
            indices_square,
            array_count(indices_square) * sizeof(uint32_t));

        vert_buffer_square->bind(m_commandList.Get(), square_mesh.get());
        indi_buffer_square->bind(m_commandList.Get(), square_mesh.get());

        ThrowIfFailed(m_commandList->Close());

        ID3D12CommandList* ppM_commandLists[] = {m_commandList.Get()};
        GET_RENDER_CONTEXT().getCommandQueue()->ExecuteCommandLists(array_count(ppM_commandLists), ppM_commandLists);
        GET_RENDER_CONTEXT().flush();

        m_mesh_pool["square"] = square_mesh;
    }

    void MeshPool::registerMesh(std::string_view mesh_name,
                                VertexData2D*    vertices,
                                uint32_t         vertices_count,
                                uint32_t*        indices,
                                uint32_t         indices_count) {
        if (m_mesh_pool.contains(std::string(mesh_name))) {
            LOG_WARN("mesh with this name ({}) has already exsited!", mesh_name);
            return;
        }

        DXRawDevicePtr device = GET_RENDER_CONTEXT().getGraphicsDevice();

        ThrowIfFailed(m_cmdAllocator->Reset());
        ThrowIfFailed(m_commandList->Reset(m_cmdAllocator.Get(), nullptr));

        auto mesh = Zero::CreateRef<Mesh>(
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

        vert_buffer_square->bind(m_commandList.Get(), mesh.get());
        indi_buffer_square->bind(m_commandList.Get(), mesh.get());

        ThrowIfFailed(m_commandList->Close());

        ID3D12CommandList* ppM_commandLists[] = {m_commandList.Get()};
        GET_RENDER_CONTEXT().getCommandQueue()->ExecuteCommandLists(array_count(ppM_commandLists), ppM_commandLists);
        GET_RENDER_CONTEXT().flush();

        m_mesh_pool[std::string(mesh_name)] = mesh;

        LOG_INFO("register mesh named {0} success!", mesh_name);
    }

    void MeshPool::removeMesh(std::string_view mesh_name) {
        if (m_mesh_pool.contains(std::string(mesh_name)))
            m_mesh_pool.erase(std::string(mesh_name));
        else
            LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
    }

    void MeshPool::delayDisposeMesh(std::string_view mesh_name, FrameResource* frameres) {
        if (!m_mesh_pool.contains(std::string(mesh_name)))
            LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        m_mesh_pool[std::string(mesh_name)]->DelayDispose(frameres);
    }

    Zero::Ref<Mesh> MeshPool::getMesh(std::string_view mesh_name) {
        if (m_mesh_pool.contains(std::string(mesh_name)))
            return m_mesh_pool[std::string(mesh_name)];

        LOG_WARN("mesh with this name ({}) does not exsit!", mesh_name);
        return nullptr;
    }

    std::string MeshPool::getMeshName(const Zero::Ref<Mesh>& target) {
        for (auto& [name, mesh] : m_mesh_pool) {
            if (target == mesh)
                return name;
        }
        LOG_WARN("this mesh does not exsit in m_mesh_pool!");
        return {};
    }
} // namespace Zero