struct PSInput {
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

cbuffer _Global : register(b0) { float4x4 _CameraWorldToViewMatrix; };

PSInput VSMain(float3 position : POSITION, float4 color : COLOR) {
  PSInput result;

  result.position = mul(_CameraWorldToViewMatrix, float4(position, 1));
  result.color = color;

  return result;
}

float4 PSMain(PSInput input) : SV_TARGET { return input.color; }
