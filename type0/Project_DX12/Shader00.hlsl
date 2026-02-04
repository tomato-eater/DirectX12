struct VSInput
{
    float3 pos : POSITION; // 入力：頂点座標
    float2 uv : COLOR; // 入力：頂点色
};

cbuffer ConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
}

cbuffer ConstantBuffer : register(b1)
{
    matrix world;
    float4 color;
}

struct VSOutput
{
    float3 position : SV_POSITION; // 出力：変換後座標
    float2 uv : COLOR; // 出力：頂点色
};

VSOutput vs(VSInput input)
{
    return input;
}

float4 ps(VSOutput input) : SV_TARGET
{
    return float4(1, 1, 1, 1);
}