#pragma once

/*
    Binding Rule:
        - For BufferView, bind the actual data
        - For DescriptorAllocView, bind the pointer and the offset

*/

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Shader/BasicShader.h>
#include <CDX12/Util/BindProperty.h>

#define GET_SHADER_BIND_TABLE() ::Zero::ShaderParamBindTable::getInstance()

namespace Zero {
    enum class ShaderUsage : uint8_t {
        BASIC = 0,
        COMPUTE
    };

    class ShaderParamBindTable {
    public:
        using TParamBindTable  = std::map<std::string, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>>;
        using TShaderBindTable = std::map<Chen::CDX12::Shader*, TParamBindTable>;
        using TShaderTable     = std::map<std::string, Zero::Ref<Chen::CDX12::Shader>>;

    public:
        static ShaderParamBindTable& getInstance() {
            static ShaderParamBindTable instance;
            return instance;
        }

        ShaderParamBindTable(const ShaderParamBindTable&)            = delete;
        ShaderParamBindTable& operator=(const ShaderParamBindTable&) = delete;

        void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            std::span<D3D12_STATIC_SAMPLER_DESC>                                   samplers = Chen::CDX12::GlobalSamplers::GetSamplers(),
            ShaderUsage                                                            usage    = ShaderUsage::BASIC);
        void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            ComPtr<ID3D12RootSignature>&&                                          rootSig,
            ShaderUsage                                                            usage = ShaderUsage::BASIC);

        void removeShader(const std::string& shader_name);

        void bindParam(Chen::CDX12::Shader*, const std::string&, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>);

        Chen::CDX12::Shader* getShader(const std::string&);
        TParamBindTable&     getShaderPropTable(Chen::CDX12::Shader*);

    private:
        ShaderParamBindTable()  = default;
        ~ShaderParamBindTable() = default;

    private:
        TShaderTable     m_shader_table;
        TShaderBindTable m_shader_bind_table;
    };
} // namespace Zero