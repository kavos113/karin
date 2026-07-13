import draw;
import vertex;
import push_constants;

// image: image, gradient: gradientLut
Texture2D tex;
SamplerState samp;

[shader("fragment")]
float4 frag_main(
    VertexOutput input,
    uniform PushConstants push)
{
    float4 outColor;

    float sdis = sdf(input.uv, push.shapeType, push.shapeParams);

    if (sdis > 0.0)
    {
        discard;
    }

    switch (push.patternType)
    {
    case 0: // solid
        outColor = push.color;
        break;

    case 1: // linear gradient
    {
        float t = linear_gradient_t(push, input.pixelPos);
        outColor = tex.Sample(samp, float2(t, 0.5));
        break;
    }

    case 2: // radial gradient
    {
        float t = radial_gradient_t(push, input.pixelPos);
        if (t >= 0.0)
        {
            outColor = tex.Sample(samp, float2(t, 0.5));
        }
        else
        {
            discard;
        }
        break;
    }

    case 3: // image
        outColor = image_color(push, input.pixelPos, tex, samp, input.uv);
        break;

    default:
        discard;
    }

    outColor.a *= push.patternParams.x;

    return outColor;
}
