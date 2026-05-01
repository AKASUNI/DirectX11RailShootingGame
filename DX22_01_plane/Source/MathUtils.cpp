#include "MathUtils.h"

XMMATRIX BuildWorldMatrix(
    const XMFLOAT3& pos,
    const XMFLOAT3& rot,
    const XMFLOAT3& scale)
{
    XMMATRIX mTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);
    XMMATRIX mRotX = XMMatrixRotationX(rot.x);
    XMMATRIX mRotY = XMMatrixRotationY(rot.y);
    XMMATRIX mRotZ = XMMatrixRotationZ(rot.z);
    XMMATRIX mScale = XMMatrixScaling(scale.x, scale.y, scale.z);

    return XMMatrixTranspose(mScale * mRotX * mRotY * mRotZ * mTrans);
}

XMMATRIX BuildViewMatrix(
    const XMVECTOR& camPos,
    const XMVECTOR& target,
    const XMVECTOR& up)
{
    return XMMatrixTranspose(XMMatrixLookAtLH(camPos, target, up));
}

XMMATRIX BuildProjectionMatrix(
    float fovDegrees,
    float aspectRatio,
    float nearZ,
    float farZ)
{
    return XMMatrixTranspose(
        XMMatrixPerspectiveFovLH(
            XMConvertToRadians(fovDegrees),
            aspectRatio,
            nearZ, farZ
        )
    );
}