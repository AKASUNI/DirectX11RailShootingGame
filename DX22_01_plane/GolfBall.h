#pragma once

#include "Object.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "utility.h"
#include "Material.h"
//#include "Ground.h"

class GolfBall :public Object
{
private:
	//速度
	DirectX::SimpleMath::Vector3 m_Velocity =
		DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);

	//加速度
	DirectX::SimpleMath::Vector3 m_Acceleration =
		DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);

	// 描画の為の情報（メッシュに関わる情報）
	MeshRenderer m_MeshRenderer; // 頂点バッファ・インデックスバッファ・インデックス数

	// 描画の為の情報（見た目に関わる部分）
	std::vector<std::unique_ptr<Material>> m_Materials;
	std::vector<SUBSET> m_subsets;
	std::vector<std::unique_ptr<Texture>> m_Textures; // テクスチャ

	int m_State = 0; //状態　0物理挙動、 1停止、 2カップイン
	int m_StopCount = 0;	//静止カウント
	

	//Ground* m_Ground;

public:
	void Init();
	void Update();
	void Draw(Camera* cam);
	void Uninit();

	//状態の設定・取得
	void SetState(int s) { m_State = s; }
	int GetState() const { return m_State;}

	//ショット
	void Shot(DirectX::SimpleMath::Vector3 v) { m_Velocity = v; }


	//void SetGround(Ground* ground);

	//デバッグ用
	void Move(const DirectX::SimpleMath::Vector2& moveValue);
};

