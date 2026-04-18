// 頂点シェーダーへの入力
struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
};


struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

// 頂点シェーダー
PSInput VS(VSInput input)
{
    PSInput output;
    // クリップ空間をそのまま使う（座標変換なし）
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    return output;
}