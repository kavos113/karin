import draw;
import vertex;
import push_constants;

// image: image, gradient: gradientLut
struct Texture
{
    Texture2D tex;
    SamplerState samp;
};
ParameterBlock<Texture> tex;

struct GlyphAtlas
{
    Texture2D tex;
    SamplerState samp;
};
ParameterBlock<GlyphAtlas> glyphAtlas;

[shader("fragment")]
float4 frag_main(
    VertexOutput input)
{
    float4 outColor;

    float glyphAlpha = glyphAtlas.tex.Sample(glyphAtlas.samp, input.uv).r;

    if (glyphAlpha < 0.01)
    {
        discard;
    }

    switch (push.f.patternType)
    {
    case 0: // solid
        outColor = push.f.color;
        break;

    case 1: // linear gradient
    {
        float t = linear_gradient_t(push.f, input.pixelPos);
        outColor = tex.tex.Sample(tex.samp, float2(t, 0.5));
        break;
    }

    case 2: // radial gradient
    {
        float t = radial_gradient_t(push.f, input.pixelPos);
        if (t >= 0.0)
        {
            outColor = tex.tex.Sample(tex.samp, float2(t, 0.5));
        }
        else
        {
            discard;
        }
        break;
    }

    case 3: // image
        outColor = image_color(push.f, input.pixelPos, tex.tex, tex.samp, input.uv);
        break;

    default:
        discard;
    }

    outColor.a *= glyphAlpha * push.f.patternParams.x;

    return outColor;
}
