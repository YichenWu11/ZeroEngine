#include "../common.hlsl"

#include "../effect/pixelate.hlsl"
#include "../effect/edge_detect.hlsl"
#include "../effect/blur.hlsl"

struct VSInput {
    float3 position  : POSITION;
    float2 tex_coord : TEXCOORD;
};

struct PSInput {
    float4 position   : SV_POSITION;
    float2 tex_coord  : TEXCOORD0;
    float2 screen_pos : TEXCOORD1;
};

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

    // return Blur(input.tex_coord * u_TilingFactor, u_TexIndex) * u_ModulateColor;
    return ret_color;
}
