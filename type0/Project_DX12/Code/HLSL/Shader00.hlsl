Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct Output
{
    float4 svPos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Output vs(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output output;
    output.svPos = pos;
    output.uv = uv;
    return output;
}

float4 ps(Output input) : SV_TARGET
{
    return float4(tex.Sample(smp, input.uv));
    //return float4(1, 1, 0, 1);
}