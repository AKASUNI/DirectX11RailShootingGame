#include "ResultScene.h"
#include "Game.h"
#include "Input.h"
#include "Texture2D.h"

// コンストラクタ
ResultScene::ResultScene()
{
	Init();
}

// デストラクタ
ResultScene::~ResultScene()
{
	Uninit();
}

// 初期化
void ResultScene::Init()
{
	Texture2D* pt = Game::GetInstance()->AddObject<Texture2D>();
	pt->SetTexture("assets/texture/background2.png");
	pt->SetScale(1280.f, 720.f, 0.f);
	m_MySceneObjects.emplace_back(pt);


	Texture2D* pt2 = Game::GetInstance()->AddObject<Texture2D>();
	pt2->SetTexture("assets/texture/resultString.png");
	pt2->SetScale(700.f, 100.f, 0.f);
	pt2->SetUV(1,5,1,13);
	m_MySceneObjects.emplace_back(pt2);

	Texture2D* pt3 = Game::GetInstance()->AddObject<Texture2D>();
	pt3->SetTexture("assets/texture/golf_jou_man.png");
	pt3->SetPosition(300.f, 0.f, 0.f);
	pt3->SetScale(361.f, 400.f, 0.f);
	m_MySceneObjects.emplace_back(pt3);

}

void ResultScene::SetScore(int c)
{
	Texture2D* stringObj = dynamic_cast<Texture2D*>(m_MySceneObjects[1]);

	switch (c)
	{
	case -4:
		stringObj->SetUV(1, 2, 1, 13);	//コンドル
		break;

	case -3:
		stringObj->SetUV(1, 3, 1, 13);	//アルバトロス
		break;

	case -2:
		stringObj->SetUV(1, 4, 1, 13);	//イーグル
		break;

	case -1:
		stringObj->SetUV(1, 5, 1, 13);	//パーディ
		break;

	case 0:
		stringObj->SetUV(1, 6, 1, 13);	//パー
		break;

	case 1:
		stringObj->SetUV(1, 7, 1, 13);	//ボギー
		break;

	case 2:
		stringObj->SetUV(1, 8, 1, 13);	//ダブルボギー
		break;

	case 3:
		stringObj->SetUV(1, 9, 1, 13);	//トリプルボギー
		break;

	case 4:
		stringObj->SetUV(1, 10, 1, 13);	//
		break;

	case 5:
		stringObj->SetUV(1, 11, 1, 13);	//
		break;

	case 6:
		stringObj->SetUV(1, 12, 1, 13);	//
		break;

	default:
		stringObj->SetUV(1, 13, 1, 13);	//　７以上
		break;
	}
}

// 更新
void ResultScene::Update()
{
	// エンターキーを押してタイトルへ
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Game::GetInstance()->ChangeScene(TITLE);
	}
}

// 終了処理
void ResultScene::Uninit()
{
	// このシーンのオブジェクトを削除する
	for (auto& o : m_MySceneObjects) {
		Game::GetInstance()->DeleteObject(o);
	}
	m_MySceneObjects.clear();
}
