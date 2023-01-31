struct VSInput {
    float3 position : POSITION;
    float2 tex_coord : TEXCOORD;
};

struct PSInput {
    float4 position : SV_POSITION;
    float2 tex_coord : TEXCOORD0;
    float2 screen_pos : TEXCOORD1;
};

cbuffer _ViewProjMatrix : register(b0) { float4x4 u_CameraWorldToViewMatrix; };

cbuffer _ObjectConstant : register(b1) {
    float4x4 u_LocalToWorldMatrix;
    float4 u_ModulateColor;
    uint u_TexIndex;
    uint pad0;
    uint pad1;
    uint pad2;
    float u_TilingFactor;
}

Texture2D TextureMap[168] : register(t0);

SamplerState u_samPointWrap : register(s0);
SamplerState u_samPointClamp : register(s1);
SamplerState u_samLinearWrap : register(s2);
SamplerState u_samLinearClamp : register(s3);
SamplerState u_samAnisotropicWrap : register(s4);
SamplerState u_samAnisotropicClamp : register(s5);
SamplerComparisonState u_samShadow : register(s6);

PSInput VSMain(VSInput input) {
    PSInput result;

    float4 posW = mul(float4(input.position, 1.0f), u_LocalToWorldMatrix);
    result.position = mul(posW, u_CameraWorldToViewMatrix);

    result.tex_coord = input.tex_coord;
    result.screen_pos = result.position.xy;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET {
    float dist = 1.0f - distance(input.screen_pos * 0.8f, float2(0.0f, 0.0f));
    dist = clamp(dist, 0.0f, 1.0f);
    dist = sqrt(dist);

    float4 ret_color =
        TextureMap[u_TexIndex].Sample(u_samAnisotropicWrap, input.tex_coord * u_TilingFactor) *
        u_ModulateColor;

#ifdef EDITOR_MODE
    // editor
    if (ret_color.a == 0.0f)
      return float4(input.tex_coord, 1.0f, 0.5f);
#endif

    return ret_color;
}
