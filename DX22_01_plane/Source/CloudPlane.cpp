#include "CloudPlane.h"
#include <stdio.h>

void CloudPlane::Init(ID3D11Device* device)
{
    struct Vertex { DirectX::XMFLOAT3 pos; };
    float s = 20.f;
    Vertex verts[] = {
        { { -s, 0, -s } }, { {  s, 0, -s } },
        { {  s, 0,  s } }, { { -s, 0,  s } },
    };
    UINT indices[] = { 0,1,2, 0,2,3 };

    // 頂点バッファ
    D3D11_BUFFER_DESC vbd = {};
    vbd.ByteWidth = sizeof(verts);          // ✅ 追加
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vd = { verts };
    device->CreateBuffer(&vbd, &vd, &m_vb);

    // インデックスバッファ
    D3D11_BUFFER_DESC ibd = {};
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // ✅ 修正
    D3D11_SUBRESOURCE_DATA id = { indices };
    device->CreateBuffer(&ibd, &id, &m_ib);

    // 定数バッファ
    D3D11_BUFFER_DESC cbd = {};
    cbd.ByteWidth = sizeof(cbCloudData);
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    device->CreateBuffer(&cbd, nullptr, &m_cb);

    // ブレンドステート
    D3D11_BLEND_DESC bd = {};
    bd.RenderTarget[0].BlendEnable = TRUE;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;     // ✅ 修正
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // ✅ 追加
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    device->CreateBlendState(&bd, &m_blendState);

    // 初期パラメータ
    m_cbData.CloudHeight = 1.0f;
    m_cbData.Coverage = 0.7f;  // 0.1 → 0.7 （大きいほど雲が多い）
    m_cbData.Sharpness = 4.0f;
    m_cbData.FadeRadius = 18.0f; // 1.0 → 10.0 に変更
    m_cbData.CloudColor = { 1.0f, 1.0f, 1.0f };
    m_cbData.ShadowColor = { 0.5f, 0.55f, 0.65f };

    // シェーダーコンパイル
    ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;

    HRESULT hr = D3DCompileFromFile(
        L"Shader\\FBMCloudShader.hlsl", nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "VS_Cloud", "vs_5_0", D3DCOMPILE_DEBUG, 0,
        &vsBlob, &errBlob);

    if (FAILED(hr))
    {
        if (errBlob)
        {
            // OutputDebugString だけだと見逃しやすいのでMessageBoxに変更
            MessageBoxA(nullptr,
                (char*)errBlob->GetBufferPointer(),
                "VS Compile Error", MB_OK);
        }
        else
        {
            MessageBoxA(nullptr, "errBlob が null（ファイルが空の可能性）",
                "VS Compile Error", MB_OK);
        }
        return;
    }

    hr = D3DCompileFromFile(
        L"Shader\\FBMCloudShader.hlsl", nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "PS_Cloud", "ps_5_0", D3DCOMPILE_DEBUG, 0,
        &psBlob, &errBlob);
    if (FAILED(hr)) {
        if (errBlob) OutputDebugStringA((char*)errBlob->GetBufferPointer());
        char buf[64]; sprintf_s(buf, "PS hr=0x%08X\n", hr);
        OutputDebugStringA(buf);
        return;
    }

    device->CreateVertexShader(
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        nullptr, &m_vs);
    device->CreatePixelShader(
        psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
        nullptr, &m_ps);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
          0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    device->CreateInputLayout(
        layout, 1,
        vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
        &m_layout);
}

void CloudPlane::Update(float deltaTime, const DirectX::XMFLOAT3& cameraPos)
{
   // m_time += deltaTime; // ← 追加

    // 色を時間で変化させる（RGBが順番に変わる）
    m_cbData.WindOffset.x += m_windDir.x * m_windSpeed * deltaTime;
    m_cbData.WindOffset.y += m_windDir.y * m_windSpeed * deltaTime;
    m_cbData.CameraPos = cameraPos;
}


void CloudPlane::Draw(ID3D11DeviceContext* ctx,
    const DirectX::XMMATRIX& view,
    const DirectX::XMMATRIX& proj)
{
    DirectX::XMMATRIX world = DirectX::XMMatrixTranslation(
        m_cbData.CameraPos.x, m_cbData.CloudHeight, m_cbData.CameraPos.z);
    DirectX::XMMATRIX wvp = world * view * proj;
    DirectX::XMStoreFloat4x4(&m_cbData.WorldViewProj,
        DirectX::XMMatrixTranspose(wvp));

    D3D11_MAPPED_SUBRESOURCE mapped;
    ctx->Map(m_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    memcpy(mapped.pData, &m_cbData, sizeof(cbCloudData));
    ctx->Unmap(m_cb.Get(), 0);

    float blendFactor[4] = {};
    ctx->OMSetBlendState(m_blendState.Get(), blendFactor, 0xFFFFFFFF);

    UINT stride = sizeof(DirectX::XMFLOAT3), offset = 0;
    ctx->IASetInputLayout(m_layout.Get());              // ✅ 追加
    ctx->IASetVertexBuffers(0, 1, m_vb.GetAddressOf(), &stride, &offset);
    ctx->IASetIndexBuffer(m_ib.Get(), DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->VSSetShader(m_vs.Get(), nullptr, 0);
    ctx->PSSetShader(m_ps.Get(), nullptr, 0);
    ctx->VSSetConstantBuffers(0, 1, m_cb.GetAddressOf()); // ✅ 追加
    ctx->PSSetConstantBuffers(0, 1, m_cb.GetAddressOf());
    ctx->DrawIndexed(6, 0, 0);

    ctx->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);
}