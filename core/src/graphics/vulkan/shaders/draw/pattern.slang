implementing draw;

import push_constants;

public float linear_gradient_t(PushConstants push, float2 pixelPos)
{
    float2 startPoint = push.color.xy;
    float2 endPoint = push.color.zw;
    float2 direction = endPoint - startPoint;
    float2 pixel = pixelPos - startPoint;

    float lengthSq = dot(direction, direction);

    float t;
    if (lengthSq < 0.0001)
    {
        t = 0.0; // startPoint and endPoint are the same
    }
    else
    {
        t = dot(pixel, direction) / lengthSq;
    }

    return t;
}

public float radial_gradient_t(PushConstants push, float2 pixelPos)
{
    float2 center = push.color.xy;
    float2 offset = push.color.zw;
    float2 radius = push.patternParams.yz;

    float2 d = (pixelPos - center - offset) / radius;
    float2 o = -offset / radius;

    float a = dot(o, o) - 1.0;
    float b = -2.0 * dot(o, d);
    float c = dot(d, d);

    float t = -1.0;

    if (abs(a) < 0.0001)
    {
        if (abs(b) < 0.0001)
        {
            t = 0.0;
        }
        else
        {
            t = -c / b;
        }
    }
    else
    {
        float dis = b * b - 4.0 * a * c;
        if (dis < 0.0)
        {
            discard; // No intersection
        }

        t = (-b - sqrt(dis)) / (2.0 * a);
    }

    return t;
}

public float4 image_color(
    PushConstants push,
    float2 pixelPos,
    Texture2D image,
    SamplerState s,
    float2 uv)
{
    float4 color;

    float uvMode = push.patternParams.w;
    if (uvMode == 0.0)
    {
        color = image.Sample(s, uv);
    }
    else
    {
        float2 offset = push.color.xy;
        float2 scale = push.color.zw;
        float2 size = push.patternParams.yz;

        float2 windowUv = (pixelPos + offset) / (size * scale);
        color = image.Sample(s, windowUv);
    }

    return color;
}
