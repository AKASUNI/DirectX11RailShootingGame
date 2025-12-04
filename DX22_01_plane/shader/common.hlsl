
struct VS_IN
{
    float4 pos : POSITION0;
    float4 nrm : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
};

struct LIGHT
{
    bool Enable; //有効状態
    bool3 Dummy;
    float4 Direction; //向き
    float4 Diffuse; //強さ、色
    float4 Ambient; //環境光の強さ、色
};

// 0番目の定数バッファにワールド行列
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
// 1番目の定数バッファにビュー行列
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
// 2番目の定数バッファにプロジェクション行列
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}


// 3番目の定数バッファに光源情報
cbuffer LightBuffer : register(b3)
{
    LIGHT Light;
}

struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float  Shininess;
    bool   TextureEnable;
    bool2  Dummy;
};

cbuffer MaterialBuffer : register(b4)
{
    MATERIAL material;
}

//UV座標変換行列
cbuffer TextureBuffer : register(b5)
{
    matrix matrixTex;
}
