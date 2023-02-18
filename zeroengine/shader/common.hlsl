#ifndef COMMON
#define COMMON

cbuffer _PassConstant : register(b0) { float4x4 u_CameraWorldToViewMatrix; };

cbuffer _ObjectConstant : register(b1) {
    float4x4 u_LocalToWorldMatrix;
    float4 u_ModulateColor;
    uint u_TexIndex;
    int  u_entity_id;
    uint pad0;
    uint pad1;
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

#endif