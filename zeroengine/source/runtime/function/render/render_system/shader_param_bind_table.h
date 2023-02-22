#pragma once

/*
    Binding Rule:
        - For BufferView, bind the actual data
        - For DescriptorAllocView, bind the pointer and the offset

*/

#include <CDX12/DescripitorHeap/DescriptorHeapAllocation.h>
#include <CDX12/Shader/BasicShader.h>
#include <CDX12/Util/BindProperty.h>

#include "runtime/core/util/singleton.h"

#define GET_SHADER_BIND_TABLE() ::Zero::ShaderParamBindTable::getInstance()

namespace Zero {
    enum class ShaderUsage : uint8_t {
        BASIC = 0,
        COMPUTE
    };

    class ShaderParamBindTable : public Singleton<ShaderParamBindTable> {
    public:
        using TParamBindTable  = std::map<std::string, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>>;
        using TShaderBindTable = std::map<Chen::CDX12::Shader*, TParamBindTable>;
        using TShaderTable     = std::map<std::string, Ref<Chen::CDX12::Shader>>;

    public:
        void registerShader(
            std::string_view                                                       shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            std::span<D3D12_STATIC_SAMPLER_DESC>                                   samplers = Chen::CDX12::GlobalSamplers::GetSamplers(),
            ShaderUsage                                                            usage    = ShaderUsage::BASIC);
        void registerShader(
            std::string_view                                                       shader_name,
            std::span<std::pair<std::string, Chen::CDX12::Shader::Property> const> properties,
            ComPtr<ID3D12RootSignature>&&                                          rootSig,
            ShaderUsage                                                            usage = ShaderUsage::BASIC);

        void removeShader(std::string_view shader_name);

        void bindParam(Chen::CDX12::Shader*, std::string_view, std::variant<std::pair<Chen::CDX12::DescriptorHeapAllocation const*, uint32_t>, std::span<const uint8_t>>);

        Chen::CDX12::Shader* getShader(std::string_view);
        TParamBindTable&     getShaderPropTable(Chen::CDX12::Shader*);

    private:
        TShaderTable     m_shader_table;
        TShaderBindTable m_shader_bind_table;
    };
} // namespace Zero