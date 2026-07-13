implementing draw;

public float sdf(
    float2 uv,
    uint shapeType,
    float2 shapeParams)
{
    float sdis;

    if (shapeType == 1) // Ellipse(normalized -> circle)
    {
        sdis = length(uv) - 1.0;
    }
    else if (shapeType == 2) // Rounded Rectangle
    {
        float2 cornerPos = abs(uv) - float2(1.0, 1.0) + shapeParams;
        sdis = min(cornerPos.x, cornerPos.y) < 1.0
                   ? -1.0
                   : length(cornerPos / shapeParams) - 1.0;
    }
    else
    {
        sdis = -1.0;
    }

    return sdis;
}
