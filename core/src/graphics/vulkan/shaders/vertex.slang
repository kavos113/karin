module vertex;

import push_constants;

public struct VertexOutput
{
    public float4 pos : SV_Position;
    public float2 pixelPos;
    public float2 uv;
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
    VertexInput input)
{
    VertexOutput output;

    output.pos = mul(mul(float4(input.pos, 0.0, 1.0), push.v.model), matrices.proj);
    output.uv = input.uv;
    output.pixelPos = mul(push.v.model, float4(input.pos, 0.0, 1.0)).xy;

    return output;
}
