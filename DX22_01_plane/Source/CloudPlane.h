#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct cbCloudData
{
    DirectX::XMFLOAT2   WindOffset;
    float               CloudHeight;
    float               Coverage;
    float               Sharpness;
    float               FadeRadius;
    float               pad[2];
    DirectX::XMFLOAT3   CloudColor;
    float               pad2;
    DirectX::XMFLOAT3   ShadowColor;
    float               pad3;
    DirectX::XMFLOAT3   CameraPos;
    float               pad4;
    DirectX::XMFLOAT4X4 WorldViewProj;
};

class CloudPlane
{
public:
    void Init(ID3D11Device* device);
    void Update(float deltaTime, const DirectX::XMFLOAT3& cameraPos);
    void Draw(ID3D11DeviceContext* ctx,
        const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& proj);

private:
    ComPtr<ID3D11Buffer>        m_vb;
    ComPtr<ID3D11Buffer>        m_ib;
    ComPtr<ID3D11Buffer>        m_cb;
    ComPtr<ID3D11VertexShader>  m_vs;
    ComPtr<ID3D11PixelShader>   m_ps;
    ComPtr<ID3D11InputLayout>   m_layout;
    ComPtr<ID3D11BlendState>    m_blendState;
    cbCloudData                 m_cbData = {};

    DirectX::XMFLOAT2 m_windDir = { 1.0f, 0.3f };
    float             m_windSpeed = 0.003f;
    //float m_time = 0.0f; // Å© í«â¡
};