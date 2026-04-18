// ピクセルシェーダーへの入力（VSの出力）
struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};


// ピクセルシェーダー
float4 PS(PSInput input) : SV_TARGET
{
    return input.color;
}
