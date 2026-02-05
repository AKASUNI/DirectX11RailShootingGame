#include "Stage1Scene.h"
#include "Game.h"
#include "Input.h"

#include "Texture2D.h"
using namespace DirectX::SimpleMath;


// コンストラクタ
Stage1Scene::Stage1Scene()
{
	Init();
}

// デストラクタ
Stage1Scene::~Stage1Scene()
{
	Uninit();
}

// 初期化
void Stage1Scene::Init()
{
	m_Par = 4;
	m_StrokeCount = 0;




	// オブジェクトを作成


	Texture2D* pt1 = Game::GetInstance()->AddObject<Texture2D>();
	pt1->SetTexture("assets/texture/ui_back.png");
	pt1->SetPosition(-475.f, -300.f, 0.f);
	pt1->SetScale(270.f, 75.f, 0.f);
	m_MySceneObjects.emplace_back(pt1);

	Texture2D* pt2 = Game::GetInstance()->AddObject<Texture2D>();
	pt2->SetTexture("assets/texture/ui_string.png");
	pt2->SetPosition(-575.f, -245.f, 0.f);
	pt2->SetScale(60.f, 45.f, 0.f);
	pt2->SetUV(1, 1, 2, 1);
	m_MySceneObjects.emplace_back(pt2);

	Texture2D* pt3 = Game::GetInstance()->AddObject<Texture2D>();
	pt3->SetTexture("assets/texture/ui_string.png");
	pt3->SetPosition(-400.f, -305.f, 0.f);
	pt3->SetScale(105.f, 63.f, 0.f);
	pt3->SetUV(2, 1, 2, 1);
	m_MySceneObjects.emplace_back(pt3);

	Texture2D* pt4 = Game::GetInstance()->AddObject<Texture2D>();
	pt4->SetTexture("assets/texture/number.png");
	pt4->SetPosition(-510.f, -245.f, 0.f);
	pt4->SetScale(65.f, 45.f, 0.f);
	pt4->SetUV((float)(m_Par+1), 1, 10, 1);
	m_MySceneObjects.emplace_back(pt4);

	Texture2D* pt5 = Game::GetInstance()->AddObject<Texture2D>();
	pt5->SetTexture("assets/texture/number.png");
	pt5->SetPosition(-485.f, -300.f, 0.f);
	pt5->SetScale(95.f, 72.f, 0.f);
	pt5->SetUV(2, 1, 10, 1);
	m_MySceneObjects.emplace_back(pt5);

	Texture2D* pt6 = Game::GetInstance()->AddObject<Texture2D>();
	pt6->SetTexture("assets/texture/number.png");
	pt6->SetPosition(-556.f, -300.f, 0.f);
	pt6->SetScale(95.f, 72.f, 0.f);
	pt6->SetUV(1, 1, 10, 1);
	m_MySceneObjects.emplace_back(pt6);


}

//更新
void Stage1Scene::Update()
{

}

int Stage1Scene::GetScore() const
{
	return (m_StrokeCount - m_Par);
}

// 終了処理
void Stage1Scene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
