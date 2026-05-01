#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "MathUtils.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


using namespace DirectX;

//ウィンドウサイズ
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const wchar_t* WINDOW_TITLE = L"DX11 Triangle";

// 頂点構造体
struct Vertex
{
    XMFLOAT3 pos;   // 位置 (x, y, z)
    XMFLOAT4 color; // 色  (r, g, b, a)
};

// カメラ構造体
struct CameraBuffer
{
    XMMATRIX world;
    XMMATRIX view;
    XMMATRIX projection;
};


//グローバル変数
ID3D11Device* g_pDevice = nullptr;
ID3D11DeviceContext* g_pContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_pRenderTarget = nullptr;
ID3D11VertexShader* g_pVertexShader = nullptr;
ID3D11PixelShader* g_pPixelShader = nullptr;
ID3D11InputLayout* g_pInputLayout = nullptr;
ID3D11Buffer* g_pVertexBuffer = nullptr;
ID3D11Buffer* g_pCameraBuffer = nullptr;


// カメラ設定
const XMVECTOR CAM_POS = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);   // カメラ位置
const XMVECTOR CAM_TARGET = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f); // 注視点
const XMVECTOR CAM_UP = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);     // 上方向
XMFLOAT3 g_pos = { 0.0f, 0.0f, 0.0f };  // 位置
XMFLOAT3 g_rot = { 0.0f, 0.0f, 0.0f };  // 回転（ラジアン）
XMFLOAT3 g_scale = { 1.0f, 1.0f, 1.0f };  // スケール


// シェーダーのディレクトリパス
const wchar_t* SHADER_PATH_V = L"Shader\\VertexShader.hlsl";
const wchar_t* SHADER_PATH_P = L"Shader\\PixelShader.hlsl";


// 前方宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HRESULT InitDX(HWND hWnd);
void    InitGeometry();
void    Render();
void    Cleanup();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    // ウィンドウクラス登録
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"DX11TriangleClass";
    RegisterClassEx(&wc);

    // ウィンドウ生成
    RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hWnd = CreateWindow(
        wc.lpszClassName, WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) return -1;

    // DirectX 初期化
    if (FAILED(InitDX(hWnd)))
    {
        MessageBox(nullptr, L"DirectX初期化失敗", L"Error", MB_OK);
        return -1;
    }

    // 三角形の頂点データ初期化
    InitGeometry();

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // メインループ
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
        }
    }

    Cleanup();
    return (int)msg.wParam;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// DirectX 初期化
HRESULT InitDX(HWND hWnd)
{
    HRESULT hr = S_OK;

    // スワップチェーン設定
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = WINDOW_WIDTH;
    sd.BufferDesc.Height = WINDOW_HEIGHT;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &sd, &g_pSwapChain,
        &g_pDevice, &featureLevel, &g_pContext);
    if (FAILED(hr)) return hr;

    // レンダーターゲット生成
    ID3D11Texture2D* pBackBuffer = nullptr;
    g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    hr = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTarget);
    pBackBuffer->Release();
    if (FAILED(hr)) return hr;

    g_pContext->OMSetRenderTargets(1, &g_pRenderTarget, nullptr);

    // ビューポート設定
    D3D11_VIEWPORT vp = {};
    vp.Width = (float)WINDOW_WIDTH;
    vp.Height = (float)WINDOW_HEIGHT;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    g_pContext->RSSetViewports(1, &vp);

    // シェーダーコンパイル＆生成
    ID3DBlob* pVSBlob = nullptr;
    ID3DBlob* pPSBlob = nullptr;
    ID3DBlob* pErrBlob = nullptr;

    // 頂点シェーダー
    hr = D3DCompileFromFile(SHADER_PATH_V, nullptr, nullptr,
        "VS", "vs_4_0", D3DCOMPILE_DEBUG, 0, &pVSBlob, &pErrBlob);
    if (FAILED(hr))
    {
        if (pErrBlob) { OutputDebugStringA((char*)pErrBlob->GetBufferPointer()); pErrBlob->Release(); }
        return hr;
    }

    // ピクセルシェーダー
    hr = D3DCompileFromFile(SHADER_PATH_P, nullptr, nullptr,
        "PS", "ps_4_0", D3DCOMPILE_DEBUG, 0, &pPSBlob, &pErrBlob);
    if (FAILED(hr))
    {
        if (pErrBlob) { OutputDebugStringA((char*)pErrBlob->GetBufferPointer()); pErrBlob->Release(); }
        return hr;
    }

    g_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    g_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);


    // カメラ定数バッファ生成
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(CameraBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    g_pDevice->CreateBuffer(&cbd, nullptr, &g_pCameraBuffer);



    // 入力レイアウト定義
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    g_pDevice->CreateInputLayout(layout, 2,
        pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pInputLayout);

    pVSBlob->Release();
    pPSBlob->Release();

    return S_OK;
}

// 三角形の頂点バッファ生成
void InitGeometry()
{
    // クリップ空間座標 (-1〜1) で直接指定
    Vertex vertices[] =
    {
        { XMFLOAT3(0.0f,  0.5f, 0.0f), XMFLOAT4(1,0,0,1) },  // 上　：赤
        { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0,1,0,1) },  // 右下：緑
        { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(0,0,1,1) }, // 左下：青
    };

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(vertices);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    g_pDevice->CreateBuffer(&bd, &initData, &g_pVertexBuffer);
}

// 描画
void Render()
{
    float clearColor[4] = { 0.1f, 0.1f, 0.3f, 1.0f };
    g_pContext->ClearRenderTargetView(g_pRenderTarget, clearColor);

    g_rot.z += 0.05f;

    D3D11_MAPPED_SUBRESOURCE mapped;
    g_pContext->Map(g_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    CameraBuffer* cb = (CameraBuffer*)mapped.pData;

    // ★ 分離した関数を使う
    cb->world = BuildWorldMatrix(g_pos, g_rot, g_scale);
    cb->view = BuildViewMatrix(CAM_POS, CAM_TARGET, CAM_UP);
    cb->projection = BuildProjectionMatrix(
        45.f,
        (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT,
        0.1f, 100.f
    );

    g_pContext->Unmap(g_pCameraBuffer, 0);

    // カメラの定数バッファをスロット0にセット
    g_pContext->VSSetConstantBuffers(0, 1, &g_pCameraBuffer);


    // シェーダーセット
    g_pContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pContext->IASetInputLayout(g_pInputLayout);

    // 頂点バッファセット
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // プリミティブトポロジー（三角形リスト）
    g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 描画（頂点3つ）
    g_pContext->Draw(3, 0);

    // 画面更新
    g_pSwapChain->Present(1, 0);
}

// 後処理
void Cleanup()
{
    if (g_pContext)      g_pContext->ClearState();
    if (g_pVertexBuffer) g_pVertexBuffer->Release();
    if (g_pInputLayout)  g_pInputLayout->Release();
    if (g_pVertexShader) g_pVertexShader->Release();
    if (g_pPixelShader)  g_pPixelShader->Release();
    if (g_pRenderTarget) g_pRenderTarget->Release();
    if (g_pSwapChain)    g_pSwapChain->Release();
    if (g_pContext)      g_pContext->Release();
    if (g_pDevice)       g_pDevice->Release();
    if (g_pCameraBuffer)  g_pCameraBuffer->Release();
}