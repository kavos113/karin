implementing draw;

public float sdf(
    float2 pos,
    uint shapeType,
    float4 shapeParams)
{
    float sdis;

    if (shapeType == 1) // Ellipse(normalized -> circle)
    {
        float2 r = shapeParams.xy;
        sdis = (length(pos / r) - 1.0) * length(r);
    }
    else if (shapeType == 2) // Rounded Rectangle
    {
        float2 r = shapeParams.zw;
        float2 s = (abs(pos) - shapeParams.xy) / r;
        float2 t = s + 1.0;
        sdis = lerp(max(s.x, s.y), length(t) - 1.0, step(0.0, min(t.x, t.y))) * length(r);
    }
    else if (shapeType == 3) // Rectangle
    {
        float2 s = abs(pos) - shapeParams.xy;
        sdis = length(max(s, 0.0)) + min(max(s.x, s.y), 0.0);
    }
    else
    {
        sdis = -1.0;
    }

    return sdis;
}
