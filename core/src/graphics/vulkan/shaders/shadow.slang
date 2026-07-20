import vertex;
import push_constants;
import draw;

[shader("fragment")]
float4 frag_main(
    VertexOutput input)
{
    float4 outColor = push.f.color;
    float blurRadius = push.f.patternParams.x;

    float d = sdf(input.localPos, push.f.shapeType, push.f.shapeParams);

    // norm dist. by erf
    float x = d / (1.4142136 * blurRadius * 0.5);
    float alpha = 0.5 - 0.5 * sign(x) * sqrt(1.0 - exp(-1.2732395 * x * x));

    return float4(outColor.xyz, alpha);
}
