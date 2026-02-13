float4 vs(float4 pos : POSITION) : SV_POSITION
{
    return pos;
}

float4 ps(float4 pos : SVPOSITION) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}