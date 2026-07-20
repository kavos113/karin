import vertex;
import push_constants;
import draw;

[shader("fragment")]
float4 frag_main(
    VertexOutput input)
{
    float4 outColor = push.f.color;
    float blurRadius = push.f.patternParams.x;

    float d = saturate(sdf(input.localPos, push.f.shapeType, push.f.shapeParams) / blurRadius);

    float alpha = exp(-4.0 * d * d);
    alpha = max((alpha - 0.018) / (1.0 - 0.018), 0.0);

    return float4(outColor.xyz, alpha);
}
