// カメラの定数バッファ追加
cbuffer CameraBuffer : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
};

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
    float4 pos = float4(input.pos, 1.f);
    
    // ワールド、ビュー、プロジェクションの順に変換
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    output.pos = pos;
    output.color = input.color;
    return output;
}