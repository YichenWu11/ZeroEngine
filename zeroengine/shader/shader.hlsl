struct VSInput {
  float3 position : POSITION;
  float2 tex_coord : TEXCOORD;
};

struct PSInput {
  float4 position : SV_POSITION;
  float2 tex_coord : TEXCOORD;
};

cbuffer _ViewProjMatrix : register(b0) { float4x4 CameraWorldToViewMatrix; };
cbuffer _ModelMatrix : register(b0, space1) { float4x4 LocalToWorldMatrix; };
cbuffer _Modulate : register(b1) { float4 ModulateColor; }

Texture2D TextureMap[50] : register(t0);

SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);
SamplerComparisonState gsamShadow : register(s6);

PSInput VSMain(VSInput input) {
  PSInput result;

  float4 posW = mul(float4(input.position, 1.0f), LocalToWorldMatrix);
  result.position = mul(posW, CameraWorldToViewMatrix);
  result.tex_coord = input.tex_coord;

  return result;
}

float4 PSMain(PSInput input) : SV_TARGET {
  float2 tex_coord = float2(input.tex_coord.x, 1.0f - input.tex_coord.y);
  return TextureMap[0].Sample(gsamAnisotropicWrap, tex_coord);
}
