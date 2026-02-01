Texture2D<float4> tex : register(t0);
SamplerState sample : register(s0);

struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

Output vs(float4 position : POSITION, float2 uv : TEXCOORD0)
{
    Output output;
    output.pos = position;
    output.uv = uv;
    return output;
}

float4 ps(Output input) : SV_TARGET
{
    float4 textureColor = tex.Sample(sample, input.uv);
    float4 finalColor = textureColor;
    
    return finalColor;
}