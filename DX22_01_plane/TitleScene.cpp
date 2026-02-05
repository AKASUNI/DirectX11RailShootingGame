
#include "TitleScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"

// コンストラクタ
TitleScene::TitleScene()
{
	Init();
}

// デストラクタ
TitleScene::~TitleScene()
{
	Uninit();
}

// 初期化
void TitleScene::Init()
{
	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	pt->SetTexture("assets/texture/background1.png");
	pt->SetPosition(0.f, 0.f, 0.f);
	pt->SetRotation(0.f, 0.f, 0.f);
	pt->SetScale(1280.f, 720.f, 0.f);
	m_MySceneObjects.emplace_back(pt);
}

// 更新
void TitleScene::Update()
{

}

// 終了処理
void TitleScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
