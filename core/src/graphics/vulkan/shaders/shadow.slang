import vertex;
import push_constants;

[shader("fragment")]
float4 frag_main(
    VertexOutput input)
{
    float4 outColor = push.f.color;
    float blurRadius = push.f.shapeParams.x;
    float4 rect = push.f.patternParams;

    float top = rect.x;
    float bottom = rect.y;
    float left = rect.z;
    float right = rect.w;

    // if need no blur, distance > blurRadius
    // bound: distance = 0
    float distance = min(
        min(input.pixelPos.x - left, right - input.pixelPos.x),
        min(input.pixelPos.y - top, bottom - input.pixelPos.y));

    return outColor * (distance / blurRadius);
}
