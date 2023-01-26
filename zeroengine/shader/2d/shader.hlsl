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
cbuffer _ModelMatrix : register(b0, space1) { float4x4 u_LocalToWorldMatrix; };
cbuffer _Modulate : register(b1) { float4 u_ModulateColor; }
cbuffer _TexVariables : register(b1, space1) {
  uint u_TexIndex;
  uint u_TilingFactor;
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
  return TextureMap[u_TexIndex].Sample(u_samAnisotropicWrap, input.tex_coord) *
         u_ModulateColor;
}
