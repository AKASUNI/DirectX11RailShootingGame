#include "GolfBall.h"
#include "Collision.h"

#include "Game.h"
#include "Ground.h"

#include "Pole.h"

using namespace std;
using namespace DirectX::SimpleMath;

//=======================================
//初期化処理
//=======================================
void GolfBall::Init()
{
	// メッシュ読み込み
	StaticMesh staticmesh;

	//3Dモデルデータ
	//std::u8string modelFile = u8"assets/model/golfBall/golf_ball.mtl";
	std::u8string modelFile = u8"assets/model/golfBall/golf_ball.obj";

	//テクスチャディレクトリ
	std::string texDirectory = "assets/model/golfBall";

	//Meshを読み込む
	std::string tmpStr1(reinterpret_cast<const char*>(modelFile.c_str()), modelFile.size());
	
	staticmesh.Load(tmpStr1, texDirectory);

	m_MeshRenderer.Init(staticmesh);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	// サブセット情報取得
	m_subsets = staticmesh.GetSubsets();

	// テクスチャ情報取得
	m_Textures = staticmesh.GetTextures();

	// マテリアル情報取得	
	std::vector<MATERIAL> materials = staticmesh.GetMaterials();

	// マテリアル数分ループ
	for (int i = 0; i < materials.size(); i++)
	{
		// マテリアルオブジェクト生成
		std::unique_ptr<Material> m = std::make_unique<Material>();

		// マテリアル情報をセット
		m->Create(materials[i]);

		// マテリアルオブジェクトを配列に追加
		m_Materials.push_back(std::move(m));
	}

	//モデルによってスケールを調整
	m_Scale.x = 1.f;
	m_Scale.y = 1.f;
	m_Scale.z = 1.f;

	//速度を最初に与える
	m_Velocity.x = 0.8f;
}

//=======================================
//更新処理
//=======================================
void GolfBall::Update()
{
	if (m_State != 0)  return; 

	Vector3 oldPos = m_Position;

	//速度が0に近づいたら停止
	if (m_Velocity.LengthSquared() < 0.03f)
	{
		m_StopCount++;
		//m_Velocity = Vector3(0.f, 0.f, 0.f);
	}
	else
	{
		m_StopCount = 0;

		//減速度
		float decelerationPower = 0.01f;

		//逆ベクトル（正規化）
		Vector3 deceleration = -m_Velocity;
		deceleration.Normalize();

		//加速度を計算
		m_Acceleration = deceleration * decelerationPower;

		m_Velocity += m_Acceleration;
	}

	if (m_StopCount > 10)
	{
		m_Velocity = Vector3(0.f, 0.f, 0.f);
		m_State = 1;	//静止
	}

	//重力
	const float gravity = 0.1f;
	m_Velocity.y -= gravity;

	m_Position += m_Velocity;

	float radius = 0.5f;

	//Groundの頂点データを取得
	vector<Ground*> grounds = Game::GetInstance()->GetObjects<Ground>();
	vector<VERTEX_3D> vertices;
	for (auto& g : grounds)
	{
		vector<VERTEX_3D> vecs = g->GetVertices();
		for (auto& v : vecs)
		{
			vertices.emplace_back(v);
		}
	}

	float moveDistance = 9999;
	Vector3 contactPoint;
	Vector3 normal;

	//地面との当たり判定
	for (int i = 0; i < vertices.size(); i += 3)
	{
		Collision::Polygon collisionPolyygon = {
			vertices[i + 0].position,
			vertices[i + 1].position,
			vertices[i + 2].position,
		};

		Vector3 cp;
		Collision::Segment collisionSegment = { oldPos, m_Position };
		Collision::Sphere collisionSphere = { m_Position, radius };

		//線分とポリゴンの判定
		if (Collision::CheckHit(collisionSegment, collisionPolyygon, cp))
		{
			float md = 0;
			Vector3 np = Collision::moveSphere(collisionSegment, radius, collisionPolyygon, cp, md);
			if (moveDistance > md)
			{
				moveDistance = md;
				m_Position = np;
				contactPoint = cp;
				normal = Collision::GetNormal(collisionPolyygon);
			}
		}
		else if (Collision::CheckHit(collisionSphere, collisionPolyygon, cp))
		{
			//球体とポリゴンの判定
			Vector3 np = Collision::moveSphere(collisionSphere, collisionPolyygon, cp);
			float md = (np - oldPos).Length();
			if (moveDistance > md)
			{
				moveDistance = md;
				m_Position = np;
				contactPoint = cp;
				normal = Collision::GetNormal(collisionPolyygon);
			}
		}
	}

	if (moveDistance != 9999)
	{
		//MessageBoxA(NULL, "接触", "確認", MB_OK);

		//ボールの速度ベクトルの法線方向と接戦成分を分解する
		float velocityNormal = Collision::Dot(m_Velocity, normal);
		//法線方向
		Vector3 v1 = velocityNormal * normal;
		//接戦方向
		Vector3 v2 = m_Velocity - v1;

		//反射ベクトル
		const float restitution = 1.f;
		const float friction = 0.5f;
		Vector3 reflectedVelocity = v2 * friction - v1 * restitution;

		//ボールの速度更新
		m_Velocity = reflectedVelocity;
	}

	//下に落ちたらリスポーン
	if (m_Position.y < -100)
	{
		//リスポーン座標
		m_Position = Vector3(0.f, 50.f, 0.f);
		m_Velocity = Vector3(0.f, 0.f, 0.f);
	}

	//ポール位置を取得
	vector<Pole*> pole = Game::GetInstance()->GetObjects<Pole>();
	if (pole.size() > 0)
	{
		Vector3 polePos = pole[0]->GetPosition();
		Collision::Sphere ballCollision = { m_Position, radius };
		Collision::Sphere poleCollision = { polePos, 0.5f };

		//カップイン
		if (Collision::CheckHit(ballCollision, poleCollision))
		{
			m_State = 2;
		}
	}
}

//=======================================
//描画処理
//=======================================
void GolfBall::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera();

	// カメラ追従
	cam->SetTarget(m_Position);

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	m_Shader.SetGPU();

	// インデックスバッファ・頂点バッファをセット
	m_MeshRenderer.BeforeDraw();

	//マテリアル数分ループ 
	for (int i = 0; i < m_subsets.size(); i++)
	{
		// マテリアルをセット(サブセット情報の中にあるマテリアルインデックスを使用)
		m_Materials[m_subsets[i].MaterialIdx]->SetGPU();

		if (m_Materials[m_subsets[i].MaterialIdx]->isTextureEnable())
		{
			m_Textures[m_subsets[i].MaterialIdx]->SetGPU();
		}

		m_MeshRenderer.DrawSubset(
			m_subsets[i].IndexNum,		// 描画するインデックス数
			m_subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
			m_subsets[i].VertexBase);	// 頂点バッファの最初から使用
	}
}

//=======================================
//終了処理
//=======================================
void GolfBall::Uninit()
{

}


//void GolfBall::SetGround(Ground* ground)
//{
//	m_Ground = ground;
//}

void GolfBall::Move(const DirectX::SimpleMath::Vector2& moveDir)
{
	m_Position += (Vector3)moveDir;
}


