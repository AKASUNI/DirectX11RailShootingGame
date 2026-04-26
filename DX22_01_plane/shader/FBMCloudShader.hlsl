// 定数バッファ（CPU側の cbCloudData と同じ順番・同じサイズ）
cbuffer cbCloud : register(b0)
{
    float2 WindOffset;
    float CloudHeight;
    float Coverage;

    float Sharpness;
    float FadeRadius;
    float2 pad;

    float3 CloudColor;
    float pad2;

    float3 ShadowColor;
    float pad3;

    float3 CameraPos;
    float pad4;

    float4x4 WorldViewProj;
};

// ======================================
// 頂点シェーダー
// ======================================
struct VSInput
{
    float3 pos : POSITION; // 板ポリの頂点座標（ローカル空間）
};

struct VSOutput
{
    float4 clipPos : SV_POSITION; // クリップ空間座標（必須）
    float3 worldPos : TEXCOORD0; // ワールド座標（PS側でUV計算に使う）
};

VSOutput VS_Cloud(VSInput input)
{
    VSOutput output;
    output.clipPos = mul(float4(input.pos, 1.0), WorldViewProj);
    output.worldPos = input.pos + float3(CameraPos.x, CloudHeight, CameraPos.z); // ← デバッグ版ではなくこちら
    return output;
}

// ======================================
// ノイズ関数（テクスチャ不要）
// ======================================
float hash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5);
}

float valueNoise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    float2 u = f * f * (3.0 - 2.0 * f); // smoothstep

    return lerp(
        lerp(hash(i), hash(i + float2(1, 0)), u.x),
        lerp(hash(i + float2(0, 1)), hash(i + float2(1, 1)), u.x),
        u.y
    );
}

float fbm(float2 uv)
{
    float value = 0.0;
    float amplitude = 0.5;
    [unroll]
    for (int i = 0; i < 5; i++)
    {
        value += valueNoise(uv) * amplitude;
        uv *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

// ======================================
// ピクセルシェーダー
// ======================================
float4 PS_Cloud(VSOutput input) : SV_Target
{
    float2 uv = input.worldPos.xz / 2.0 + WindOffset;
    float f = fbm(uv);
    float threshold = 1.0 - Coverage;
    float density = saturate((f - threshold) * Sharpness);
    float3 col = lerp(ShadowColor, CloudColor, density);

    return float4(col, density); // f → density に戻す
}