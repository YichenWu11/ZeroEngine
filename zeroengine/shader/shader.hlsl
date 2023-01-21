struct PSInput {
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

cbuffer _Global : register(b0) { float4x4 _CameraWorldToViewMatrix; };

// SamplerState gsamPointWrap : register(s0);
// SamplerState gsamPointClamp : register(s1);
// SamplerState gsamLinearWrap : register(s2);
// SamplerState gsamLinearClamp : register(s3);
// SamplerState gsamAnisotropicWrap : register(s4);
// SamplerState gsamAnisotropicClamp : register(s5);
// SamplerComparisonState gsamShadow : register(s6);

PSInput VSMain(float3 position : POSITION, float4 color : COLOR) {
  PSInput result;

  result.position = mul(_CameraWorldToViewMatrix, float4(position, 1));
  result.color = color;

  return result;
}

float4 PSMain(PSInput input) : SV_TARGET { return input.color; }
