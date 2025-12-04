#include "TestCube.h"
#include "input.h"

using namespace DirectX::SimpleMath;

#include <iostream>

//=======================================
//初期化処理
//=======================================
void TestCube::Init()
{
	// 頂点データ
	//m_Vertices.resize(8);
	m_Vertices.resize(24);


	//m_Vertices[0].position = Vector3(-10, 10, 10);	// ①左奥上
	//m_Vertices[1].position = Vector3(10, 10, 10);	    // ②右奥上
	//m_Vertices[2].position = Vector3(10, -10, 10);	// ③右奥下
	//m_Vertices[3].position = Vector3(-10, -10, 10);	// ④左奥下
	//m_Vertices[4].position = Vector3(-10, 10, -10);	// ⑤左奥下
	//m_Vertices[5].position = Vector3(10, 10, -10);	// ⑥右奥下
	//m_Vertices[6].position = Vector3(10, -10, -10); 　// ⑦右手前下
	//m_Vertices[7].position = Vector3(-10, -10, -10);　// ⑧左手前下


	//1つめ 1,2,3,4,
	{
		m_Vertices[0].position = Vector3(-10, 10, 10);
		m_Vertices[1].position = Vector3(10, 10, 10);
		m_Vertices[2].position = Vector3(10, -10, 10);
		m_Vertices[3].position = Vector3(-10, -10, 10);

		m_Vertices[0].normal = Vector3(0, 0, 1);
		m_Vertices[1].normal = Vector3(0, 0, 1);
		m_Vertices[2].normal = Vector3(0, 0, 1);
		m_Vertices[3].normal = Vector3(0, 0, 1);

		m_Vertices[0].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[1].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[2].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[3].color = Color(1.f, 1.f, 1.f, 1.f);

		//m_Vertices[0].uv = Vector2(0.33f,   0.f);
		//m_Vertices[1].uv = Vector2(0.0f, 0.f);
		//m_Vertices[2].uv = Vector2(0.f,   0.5f);
		//m_Vertices[3].uv = Vector2(0.33f, 0.5f);

		m_Vertices[0].uv = Vector2(0.f, 0.f);
		m_Vertices[1].uv = Vector2(0.33f, 0.f);
		m_Vertices[2].uv = Vector2(0.33f, 0.5f);
		m_Vertices[3].uv = Vector2(0.f, 0.5f);
		
	}

	//2つめ 1,4,8,5
	{
		m_Vertices[4].position = Vector3(-10, 10, 10);
		m_Vertices[5].position = Vector3(-10, -10, 10);
		m_Vertices[6].position = Vector3(-10, -10, -10);
		m_Vertices[7].position = Vector3(-10, 10, -10);

		m_Vertices[4].normal = Vector3(-1, 0, 0);
		m_Vertices[5].normal = Vector3(-1, 0, 0);
		m_Vertices[6].normal = Vector3(-1, 0, 0);
		m_Vertices[7].normal = Vector3(-1, 0, 0);

		m_Vertices[4].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[5].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[6].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[7].color = Color(1.f, 1.f, 1.f, 1.f);

		m_Vertices[4].uv = Vector2(0.33f, 0.5f);
		m_Vertices[5].uv = Vector2(0.33f, 1.f);
		m_Vertices[6].uv = Vector2(0.66f, 1.f);
		m_Vertices[7].uv = Vector2(0.66f, 0.5f);
	}

	//3つめ 5,6,2,1
	{
		m_Vertices[8].position  = Vector3(-10, 10, -10);
		m_Vertices[9].position  = Vector3( 10, 10, -10);
		m_Vertices[10].position = Vector3( 10, 10,  10);
		m_Vertices[11].position = Vector3(-10, 10,  10);

		m_Vertices[8].normal = Vector3(0, 1, 0);
		m_Vertices[9].normal = Vector3(0, 1, 0);
		m_Vertices[10].normal = Vector3(0, 1, 0);
		m_Vertices[11].normal = Vector3(0, 1, 0);

		m_Vertices[8].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[9].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[10].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[11].color = Color(1.f, 1.f, 1.f, 1.f);

		m_Vertices[8].uv = Vector2(0.f, 0.f);
		m_Vertices[9].uv = Vector2(1.f, 0.f);
		m_Vertices[10].uv = Vector2(1.f, 1.f);
		m_Vertices[11].uv = Vector2(0.f, 1.f);
	}


	//4つめ2,6,7,3
	{
		m_Vertices[12].position = Vector3( 10,  10,  10);
		m_Vertices[13].position = Vector3( 10,  10, -10);
		m_Vertices[14].position = Vector3( 10, -10, -10);
		m_Vertices[15].position = Vector3( 10, -10,  10);

		m_Vertices[12].normal = Vector3(1, 0, 0);
		m_Vertices[13].normal = Vector3(1, 0, 0);
		m_Vertices[14].normal = Vector3(1, 0, 0);
		m_Vertices[15].normal = Vector3(1, 0, 0);

		m_Vertices[12].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[13].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[14].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[15].color = Color(1.f, 1.f, 1.f, 1.f);

		m_Vertices[12].uv = Vector2(0.f, 0.f);
		m_Vertices[13].uv = Vector2(1.f, 0.f);
		m_Vertices[14].uv = Vector2(1.f, 1.f);
		m_Vertices[15].uv = Vector2(0.f, 1.f);
	}


	//5つめ3,7,8,4
	{
		m_Vertices[16].position = Vector3( 10, -10,  10);
		m_Vertices[17].position = Vector3( 10, -10, -10);
		m_Vertices[18].position = Vector3(-10, -10, -10);
		m_Vertices[19].position = Vector3(-10, -10,  10);

		m_Vertices[16].normal = Vector3(0, -1, 0);
		m_Vertices[17].normal = Vector3(0, -1, 0);
		m_Vertices[18].normal = Vector3(0, -1, 0);
		m_Vertices[19].normal = Vector3(0, -1, 0);

		m_Vertices[16].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[17].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[18].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[19].color = Color(1.f, 1.f, 1.f, 1.f);

		m_Vertices[16].uv = Vector2(0.f, 0.f);
		m_Vertices[17].uv = Vector2(1.f, 0.f);
		m_Vertices[18].uv = Vector2(1.f, 1.f);
		m_Vertices[19].uv = Vector2(0.f, 1.f);
	}

	

	//6つめ8,7,6,5
	{
		m_Vertices[20].position = Vector3(-10, -10, -10);
		m_Vertices[21].position = Vector3( 10, -10, -10);
		m_Vertices[22].position = Vector3( 10,  10, -10);
		m_Vertices[23].position = Vector3(-10,  10, -10);

		m_Vertices[20].normal = Vector3(0, 0, -1);
		m_Vertices[21].normal = Vector3(0, 0, -1);
		m_Vertices[22].normal = Vector3(0, 0, -1);
		m_Vertices[23].normal = Vector3(0, 0, -1);

		m_Vertices[20].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[21].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[22].color = Color(1.f, 1.f, 1.f, 1.f);
		m_Vertices[23].color = Color(1.f, 1.f, 1.f, 1.f);

		m_Vertices[20].uv = Vector2(0.f, 0.f);
		m_Vertices[21].uv = Vector2(1.f, 0.f);
		m_Vertices[22].uv = Vector2(1.f, 1.f);
		m_Vertices[23].uv = Vector2(0.f, 1.f);
	}



	// 頂点バッファ生成
	m_VertexBuffer.Create(m_Vertices);

	// インデックスバッファ
	m_Indices.resize(32);

	m_Indices = {
		// 前面 (+Z)
		0, 2, 1,    0, 3, 2,
		// 左面 (-X)
		4, 6, 5,    4, 7, 6,
		// 上面 (+Y)
		8, 10, 9,   8, 11, 10,
		// 右面 (+X)
		12, 14, 13, 12, 15, 14,
		// 下面 (-Y)
		16, 18, 17, 16, 19, 18,
		// 背面 (-Z)
		20, 22, 21, 20, 23, 22
	};

	// インデックスバッファ生成
	m_IndexBuffer.Create(m_Indices);

	// シェーダオブジェクト生成
	/*m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");*/
	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	//テクスチャロード
	bool sts = m_Texture.Load("assets/texture/dice.png");
	assert(sts == true);
}

//=======================================
//更新処理
//=======================================
void TestCube::Update()
{

}

void TestCube::Rotate(DirectX::SimpleMath::Vector3 axis, float angle)
{
	m_Rotation += axis * angle;
	std::cout << m_Rotation.x << std::endl;
	std::cout << m_Rotation.y << std::endl;
	std::cout << m_Rotation.z << std::endl;
}

//=======================================
//描画処理
//=======================================
void TestCube::Draw(Camera* cam)
{
	//カメラを選択する
	cam->SetCamera();

	// SRT情報作成
	Matrix r = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);
	Matrix t = Matrix::CreateTranslation(m_Position.x, m_Position.y, m_Position.z);
	Matrix s = Matrix::CreateScale(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix worldmtx;
	worldmtx = s * r * t;
	Renderer::SetWorldMatrix(&worldmtx); // GPUにセット

	// 描画の処理
	ID3D11DeviceContext* devicecontext;
	devicecontext = Renderer::GetDeviceContext();


	// トポロジーをセット（プリミティブタイプ）
	//devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();


	devicecontext->DrawIndexed(
		m_Indices.size(),	// 描画するインデックス数
		0,					// 最初のインデックスバッファの位置
		0);
}

//=======================================
//終了処理
//=======================================
void TestCube::Uninit()
{

}
