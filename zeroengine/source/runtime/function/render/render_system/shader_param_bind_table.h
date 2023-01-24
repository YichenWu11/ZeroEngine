#pragma once

/*
    Binding Rule:
        - For BufferView, bind the actual data
        - For DescriptorAllocView, bind the pointer and the offset

*/

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Shader/BasicShader.h>
#include <CDX12/Util/BindProperty.h>

namespace Zero {
    enum class ShaderUsage : uint8_t {
        BASIC = 0,
        COMPUTE
    };

    class ShaderParamBindTable {
    public:
        using ParamBindTable  = std::unordered_map<std::string, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>>;
        using ShaderBindTable = std::unordered_map<Chen::CDX12::Shader*, ParamBindTable>;
        using ShaderTable     = std::unordered_map<std::string, Zero::Ref<Chen::CDX12::Shader>>;

    public:
        static ShaderParamBindTable& getInstance() {
            static ShaderParamBindTable instance;
            return instance;
        }

        void bindDevice(ID3D12Device*);

        void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            std::span<D3D12_STATIC_SAMPLER_DESC>                                   samplers = Chen::CDX12::GlobalSamplers::GetSamplers());

        void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            ComPtr<ID3D12RootSignature>&&                                          rootSig);

        void removeShader(const std::string& shader_name);

        void bindParam(Chen::CDX12::Shader*, const std::string&, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>);

        Chen::CDX12::Shader* getShader(const std::string&);

        ParamBindTable& getShaderPropTable(Chen::CDX12::Shader*);

    private:
        ID3D12Device* m_device;

        ShaderTable     m_shader_table;
        ShaderBindTable m_shader_bind_table;
    };
} // namespace Zero