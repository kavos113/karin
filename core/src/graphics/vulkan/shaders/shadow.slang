import vertex;
import push_constants;
import draw;

[shader("fragment")]
float4 frag_main(
    VertexOutput input)
{
    float4 outColor = push.f.color;
    float blurRadius = push.f.patternParams.x;

    float dis = sdf(input.localPos, push.f.shapeType, push.f.shapeParams);

    return outColor * (1 - saturate(dis / blurRadius));
}
