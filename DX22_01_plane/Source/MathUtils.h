#pragma once
#include <DirectXMath.h>
using namespace DirectX;

// ワールド行列を生成する（純粋な計算 → テスト可能）
XMMATRIX BuildWorldMatrix(
    const XMFLOAT3& pos,
    const XMFLOAT3& rot,
    const XMFLOAT3& scale
);

// ビュー行列を生成する
XMMATRIX BuildViewMatrix(
    const XMVECTOR& camPos,
    const XMVECTOR& target,
    const XMVECTOR& up
);

// プロジェクション行列を生成する
XMMATRIX BuildProjectionMatrix(
    float fovDegrees,
    float aspectRatio,
    float nearZ,
    float farZ
);