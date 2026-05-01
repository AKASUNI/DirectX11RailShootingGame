#include <gtest/gtest.h>
#include "MathUtils.h"

float M(const XMMATRIX& m, int row, int col)
{
    XMFLOAT4X4 f;
    XMStoreFloat4x4(&f, m);
    return f.m[row][col];
}

class WorldMatrixTest : public ::testing::Test
{
protected:
    XMFLOAT3 zeroPos = { 0,0,0 };
    XMFLOAT3 zeroRot = { 0,0,0 };
    XMFLOAT3 unitScale = { 1,1,1 };
};

TEST_F(WorldMatrixTest, IdentityWhenNothingApplied)
{
    XMMATRIX result = BuildWorldMatrix(zeroPos, zeroRot, unitScale);
    EXPECT_NEAR(M(result, 0, 0), 1.f, 1e-5f);
    EXPECT_NEAR(M(result, 1, 1), 1.f, 1e-5f);
    EXPECT_NEAR(M(result, 2, 2), 1.f, 1e-5f);
    EXPECT_NEAR(M(result, 3, 3), 1.f, 1e-5f);
}

TEST_F(WorldMatrixTest, TranslationIsApplied)
{
    XMFLOAT3 pos = { 1.f, 2.f, 3.f };
    XMMATRIX result = BuildWorldMatrix(pos, zeroRot, unitScale);
    EXPECT_NEAR(M(result, 0, 3), 1.f, 1e-5f);
    EXPECT_NEAR(M(result, 1, 3), 2.f, 1e-5f);
    EXPECT_NEAR(M(result, 2, 3), 3.f, 1e-5f);
}

TEST_F(WorldMatrixTest, ScaleIsApplied)
{
    XMFLOAT3 scale = { 2.f, 3.f, 4.f };
    XMMATRIX result = BuildWorldMatrix(zeroPos, zeroRot, scale);
    EXPECT_NEAR(M(result, 0, 0), 2.f, 1e-5f);
    EXPECT_NEAR(M(result, 1, 1), 3.f, 1e-5f);
    EXPECT_NEAR(M(result, 2, 2), 4.f, 1e-5f);
}

TEST_F(WorldMatrixTest, RotationZ90Degrees)
{
    XMFLOAT3 rot = { 0, 0, XM_PIDIV2 };
    XMMATRIX result = BuildWorldMatrix(zeroPos, rot, unitScale);
    EXPECT_NEAR(M(result, 0, 0), 0.f, 1e-5f);
    EXPECT_NEAR(M(result, 0, 1), -1.f, 1e-5f); // Å© 1.f Ç -1.f Ç…ïœçX
}

TEST(ProjectionMatrixTest, FovAndAspectAreReflected)
{
    XMMATRIX proj = BuildProjectionMatrix(45.f, 800.f / 600.f, 0.1f, 100.f);
    float expected = 1.f / (tanf(XMConvertToRadians(45.f) / 2.f) * (800.f / 600.f));
    EXPECT_NEAR(M(proj, 0, 0), expected, 1e-4f);
}

TEST(ProjectionMatrixTest, NearFarPlaneRange)
{
    XMMATRIX proj = BuildProjectionMatrix(45.f, 1.f, 0.1f, 100.f);
    float expected = 100.f / (100.f - 0.1f);
    EXPECT_NEAR(M(proj, 2, 2), expected, 1e-4f);
}