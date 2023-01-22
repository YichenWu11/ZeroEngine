struct PSInput {
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

cbuffer _ViewProjMatrix : register(b0) { float4x4 CameraWorldToViewMatrix; };
cbuffer _ModelMatrix : register(b0, space1) { float4x4 LocalToWorldMatrix; };
cbuffer _Modulate : register(b1) { float4 ModulateColor; }

// SamplerState gsamPointWrap : register(s0);
// SamplerState gsamPointClamp : register(s1);
// SamplerState gsamLinearWrap : register(s2);
// SamplerState gsamLinearClamp : register(s3);
// SamplerState gsamAnisotropicWrap : register(s4);
// SamplerState gsamAnisotropicClamp : register(s5);
// SamplerComparisonState gsamShadow : register(s6);

PSInput VSMain(float3 position : POSITION, float4 color : COLOR) {
  PSInput result;

  float4 posW = mul(float4(position, 1.0f), LocalToWorldMatrix);
  result.position = mul(posW, CameraWorldToViewMatrix);
  result.color = color * ModulateColor;

  return result;
}

float4 PSMain(PSInput input) : SV_TARGET { return input.color; }
