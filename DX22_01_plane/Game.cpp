#include "Game.h"
#include "Renderer.h"
#include "input.h"

Game* Game::m_Instance;

// コンストラクタ
Game::Game()
{
	m_Scene = nullptr;
}

// デストラクタ
Game::~Game()
{
	delete m_Scene;
	DeleteAllObject();
}

// 初期化
void Game::Init()
{
	m_Instance = new Game;

	Input::Create();

	// 描画終了処理
	Renderer::Init();

	// カメラ初期化
	m_Instance->m_Camera.Init();
	m_Instance->m_Scene = new TitleScene;

}

// 更新
void Game::Update()
{
	Input::Update();

	m_Instance->m_Scene->Update();

	// カメラ更新
	m_Instance->m_Camera.Update();

	for (auto& o : m_Instance->m_Objects)
	{
		o->Update();
	}
}

// 描画
void Game::Draw()
{
	// 描画前処理
	Renderer::DrawStart();

	for (auto& o : m_Instance->m_Objects)
	{
		o->Draw(&m_Instance->m_Camera);
	}

	// 描画後処理
	Renderer::DrawEnd();
}

// 終了処理
void Game::Uninit()
{
	// カメラ終了処理
	m_Instance->m_Camera.Uninit();

	for (auto& o : m_Instance->m_Objects)
	{
		o->Uninit();
	}

	// 描画終了処理
	Renderer::Uninit();

	Input::Release();

	delete m_Instance;
}

Game* Game::GetInstance()
{
	return m_Instance;
}

void Game::ChangeScene(SceneName sName)
{
	if (m_Instance->m_Scene != nullptr)
	{
		delete m_Instance->m_Scene;
		m_Instance->m_Scene = nullptr;
	}

	switch (sName)
	{
	case TITLE:
		m_Instance->m_Scene = new TitleScene;
		break;
	}
}

void Game::DeleteObject(Object* pt)
{
	if (pt == NULL) return;

	pt->Uninit();

	std::erase_if(m_Instance->m_Objects,
		[pt](const std::unique_ptr<Object>& element)
		{
			return element.get() == pt;
		});
	m_Instance->m_Objects.shrink_to_fit();
}

void Game::DeleteAllObject()
{
	for (auto& o : m_Instance->m_Objects)
	{
		o->Uninit();
	}

	m_Instance->m_Objects.clear();
	m_Instance->m_Objects.shrink_to_fit();
}

