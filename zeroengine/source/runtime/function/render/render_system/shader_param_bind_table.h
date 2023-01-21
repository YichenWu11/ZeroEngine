#pragma once

/*
    Binding Rule:
        - For BufferView, bind the actual data
        - For DescriptorAllocView, bind the pointer

*/

#include <CDX12/Shader/BasicShader.h>
#include <CDX12/Util/BindProperty.h>

namespace Chen::CDX12 {
    class DescriptorHeapAllocation;
}

namespace Zero {
    // uint8_t const*

    class ShaderParamBindTable {
    public:
        using ParamBindTable  = std::unordered_map<std::string, std::variant<Chen::CDX12::DescriptorHeapAllocation const*, std::span<const uint8_t>>>;
        using ShaderBindTable = std::unordered_map<Chen::CDX12::Shader*, ParamBindTable>;
        using ShaderTable     = std::unordered_map<std::string, std::unique_ptr<Chen::CDX12::Shader>>;

    public:
        static void bindDevice(ID3D12Device*);

        static void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            std::span<D3D12_STATIC_SAMPLER_DESC>                                   samplers = Chen::CDX12::GlobalSamplers::GetSamplers());

        static void registerShader(
            const std::string&                                                     shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            ComPtr<ID3D12RootSignature>&&                                          rootSig);

        static void bindParam(Chen::CDX12::Shader*, const std::string&, std::variant<Chen::CDX12::DescriptorHeapAllocation const*, std::span<const uint8_t>>);

        static Chen::CDX12::Shader* getShader(const std::string&);

        static ParamBindTable& getShaderPropTable(Chen::CDX12::Shader*);

    private:
        static ID3D12Device* s_device;

        static ShaderTable     s_shader_table;
        static ShaderBindTable s_shader_bind_table;
    };
} // namespace Zero