#include "runtime/function/render/render_system/shader_param_bind_table.h"
#include "runtime/function/render/render_system/render_context.h"

using namespace Chen::CDX12;

namespace Zero {
    Shader* ShaderParamBindTable::getShader(std::string_view name) {
        if (m_shader_table.contains(std::string(name)))
            return m_shader_table[std::string(name)].get();
        LOG_WARN("The shader with this name ({}) does not exist!", name);
        return nullptr;
    }

    void ShaderParamBindTable::registerShader(
        std::string_view                                          shader_name,
        std::span<std::pair<std::string, Shader::Property> const> properties,
        std::span<D3D12_STATIC_SAMPLER_DESC>                      samplers,
        ShaderUsage                                               usage) {
        if (m_shader_table.contains(std::string(shader_name))) {
            LOG_WARN("The shader with this name ({}) has existed!", shader_name);
            return;
        }

        ASSERT(RenderContext::getInstance().getGraphicsDevice(), "registerShader before creating device!");

        switch (usage) {
            case ShaderUsage::BASIC: {
                auto shader                              = Zero::CreateRef<BasicShader>(properties, RenderContext::getInstance().getGraphicsDevice(), samplers);
                m_shader_bind_table[shader.get()]        = TParamBindTable();
                m_shader_table[std::string(shader_name)] = std::move(shader);
                break;
            }
            case ShaderUsage::COMPUTE:
                break;
        }
    }

    void ShaderParamBindTable::registerShader(
        std::string_view                                          shader_name,
        std::span<std::pair<std::string, Shader::Property> const> properties,
        ComPtr<ID3D12RootSignature>&&                             rootSig,
        ShaderUsage                                               usage) {
        if (m_shader_table.contains(std::string(shader_name))) {
            LOG_WARN("The shader with this name ({}) has existed!", shader_name);
            return;
        }

        switch (usage) {
            case ShaderUsage::BASIC: {
                auto shader                              = Zero::CreateRef<BasicShader>(properties, std::move(rootSig));
                m_shader_bind_table[shader.get()]        = TParamBindTable();
                m_shader_table[std::string(shader_name)] = std::move(shader);
                break;
            }
            case ShaderUsage::COMPUTE:
                break;
        }
    }

    void ShaderParamBindTable::removeShader(std::string_view shader_name) {
        if (!m_shader_table.contains(std::string(shader_name))) {
            LOG_WARN("The shader with this name ({}) does not exist!", shader_name);
            return;
        }

        m_shader_bind_table.erase(m_shader_table[std::string(shader_name)].get());
        m_shader_table.erase(std::string(shader_name));

        LOG_INFO("The shader with this name ({}) has been removed!", shader_name);
    }

    void ShaderParamBindTable::bindParam(Shader* shader, std::string_view prop_name, std::variant<std::pair<DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>> data) {
        if (!m_shader_bind_table.contains(shader)) {
            LOG_WARN("The shader with does not exist!");
            return;
        }

        auto& prop_table = m_shader_bind_table[shader];

        std::visit([&](auto&& arg) { prop_table[std::string(prop_name)] = arg; }, data);
    }

    ShaderParamBindTable::TParamBindTable& ShaderParamBindTable::getShaderPropTable(Shader* shader) {
        if (!m_shader_bind_table.contains(shader)) {
            LOG_WARN("This shader does not exist in the ShaderBindTable!");
        }

        return m_shader_bind_table[shader];
    }
} // namespace Zero