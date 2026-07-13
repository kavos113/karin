module vertex;

import push_constants;

public struct VertexOutput
{
    float4 pos : SV_Position;
    float2 pixelPos;
    float2 uv;
};

struct UniformBuffer
{
    float4x4 proj;
};
ConstantBuffer<UniformBuffer> matrices;

public struct VertexInput
{
    float2 pos;
    float2 uv;
};

[shader("vertex")]
public VertexOutput vert_main(
    VertexInput input, 
    uniform TransformMatrix push
)
{
    VertexOutput output;

    output.pos = mul(matrices.proj, mul(push.model, float4(input.pos, 0.0, 1.0)));
    output.uv = input.uv;
    output.pixelPos = mul(push.model, float4(input.pos, 0.0, 1.0)).xy;

    return output;
}