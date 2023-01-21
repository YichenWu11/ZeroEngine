#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>

#include "runtime/function/render/render_system/shader_param_bind_table.h"

using namespace Chen::CDX12;

namespace Zero {
    ID3D12Device* ShaderParamBindTable::s_device = nullptr;

    ShaderParamBindTable::ShaderTable     ShaderParamBindTable::s_shader_table;
    ShaderParamBindTable::ShaderBindTable ShaderParamBindTable::s_shader_bind_table;

    void ShaderParamBindTable::bindDevice(ID3D12Device* device) {
        s_device = device;
    }

    Shader* ShaderParamBindTable::getShader(const std::string& name) {
        if (s_shader_table.contains(name))
            return s_shader_table[name].get();
        LOG_WARN("The shader with this name ({}) does not exist!", name);
        return nullptr;
    }

    void ShaderParamBindTable::registerShader(
        const std::string&                                                     shader_name,
        std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
        std::span<D3D12_STATIC_SAMPLER_DESC>                                   samplers) {
        ZE_ASSERT(s_device && "bind the device before registerShader!");

        if (s_shader_table.contains(shader_name)) {
            LOG_WARN("The shader with this name ({}) has existed!", shader_name);
            return;
        }

        auto shader = std::make_unique<BasicShader>(properties, s_device, samplers);

        s_shader_bind_table[shader.get()] = ParamBindTable();
        s_shader_table[shader_name]       = std::move(shader);
    }

    void ShaderParamBindTable::registerShader(
        const std::string&                                                     shader_name,
        std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
        ComPtr<ID3D12RootSignature>&&                                          rootSig) {
        ZE_ASSERT(s_device && "bind the device before registerShader!");

        if (s_shader_table.contains(shader_name)) {
            LOG_WARN("The shader with this name ({}) has existed!", shader_name);
            return;
        }

        auto shader = std::make_unique<BasicShader>(properties, std::move(rootSig));

        s_shader_bind_table[shader.get()] = ParamBindTable();
        s_shader_table[shader_name]       = std::move(shader);
    }

    void ShaderParamBindTable::bindParam(Shader* shader, const std::string& prop_name, std::variant<DescriptorHeapAllocation const*, std::span<const uint8_t>> data) {
        if (!s_shader_bind_table.contains(shader)) {
            LOG_WARN("The shader with does not exist!");
            return;
        }

        auto& prop_table = s_shader_bind_table[shader];

        std::visit([&](auto&& arg) { prop_table[prop_name] = arg; }, data);
    }

    ShaderParamBindTable::ParamBindTable& ShaderParamBindTable::getShaderPropTable(Shader* shader) {
        if (!s_shader_bind_table.contains(shader)) {
            LOG_WARN("The shader with does not exist!");
        }

        return s_shader_bind_table[shader];
    }
} // namespace Zero