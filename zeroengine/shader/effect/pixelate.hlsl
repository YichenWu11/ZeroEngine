#include "../common.hlsl"

float4 Pixelate(float2 uv, uint tex_index) {
    uint2 var = uint2(uv.x * 100, uv.y * 100);
    uv = float2((float)var.x / 100, (float)var.y / 100);

    return TextureMap[tex_index].Sample(u_samAnisotropicWrap, uv);
}