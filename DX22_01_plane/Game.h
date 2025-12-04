#pragma once
#include <iostream>
#include "input.h"

#include "Renderer.h"
#include "TitleScene.h"
#include "Stage1Scene.h"
#include "ResultScene.h"


enum SceneName {
	TITLE,
	STAGE1,
	RESULT
};

 
class Game
{
private:
	static Game* m_Instance;
	Scene* m_Scene;

	// カメラ
	Camera  m_Camera;

	// テストオブジェクト
	//TestPlane m_Plane;
	//TestCube m_Cube;
	//TestModel m_Model;
	//GolfBall m_Ball;
	std::vector<std::unique_ptr<Object>> m_Objects;
	//Ground  m_ground;
	Input input;

public:
	Game(); // コンストラクタ
	~Game(); // デストラクタ

	static void Init(); // 初期化
	static void Update(); // 更新
	static void Draw(); // 描画
	static void Uninit(); // 終了処理

	static Game* GetInstance();
	void ChangeScene(SceneName sName);
	void DeleteObject(Object* pt);
	void DeleteAllObject();

	template<typename T> T* AddObject()
	{
		T* pt = new T;
		m_Instance->m_Objects.emplace_back(pt);
		pt->Init();
		return pt;
	}

	template<typename T> std::vector<T*> GetObjects()
	{
		std::vector<T*> res;
		for (auto& o : m_Instance->m_Objects)
		{
			if (T* derivedObj = dynamic_cast<T*>(o.get())) {
				res.emplace_back(derivedObj);
			}
		}
		return res;
	}

};
