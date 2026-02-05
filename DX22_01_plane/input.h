#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <Xinput.h>					 //XInputを使うためのヘッダーファイル
#pragma comment (lib, "xinput.lib") //XInputを使うために必要

#include <SimpleMath.h>

namespace Button {
	enum class XBox : WORD
	{
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
		Up = 0x0001,			//十字キー上
		Down = 0x0002,			//十字キー下
		Left = 0x0004,			//十字キー左
		Right = 0x0008,			//十字キー右
		Start = 0x0010,			//スタートボタン
		Back = 0x0020,	//バックボタン
		LeftThumb = 0x0040,	//左スティック押し込み
		RightThumb = 0x0080,	//右スティック押し込み
		LeftShoulder = 0x0100, //L
		RightShoulder = 0x0200  //R
	};

	enum class KeyBord : int
	{
		A = 'A', B = 'B', C = 'C', D = 'D', E = 'E',
		F = 'F', G = 'G', H = 'H', I = 'I', J = 'J',
		K = 'K', L = 'L', M = 'M', N = 'N', O = 'O',
		P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T',
		U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y',
		Z = 'Z',

		Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3', Num4 = '4',
		Num5 = '5', Num6 = '6', Num7 = '7', Num8 = '8', Num9 = '9',

		F1 = 0x70, F2 = 0x71, F3 = 0x72, F4 = 0x73,
		F5 = 0x74, F6 = 0x75, F7 = 0x76, F8 = 0x77,
		F9 = 0x78, F10 = 0x79, F11 = 0x7A, F12 = 0x7B,

		Up = 0x26,
		Down = 0x28,
		Left = 0x25,
		Right = 0x27,

		Shift = 0x10,
		Control = 0x11,
		Alt = 0x12,

		LShift = 0xA0,
		RShift = 0xA1,
		LCtrl = 0xA2,
		RCtrl = 0xA3,
		LAlt = 0xA4,
		RAlt = 0xA5,

		Escape = 0x1B,
		Space = 0x20,
		Enter = 0x0D,
		Backspace = 0x08,
		Tab = 0x09,
		CapsLock = 0x14,


		NumPad0 = 0x60, NumPad1 = 0x61, NumPad2 = 0x62, NumPad3 = 0x63,
		NumPad4 = 0x64, NumPad5 = 0x65, NumPad6 = 0x66, NumPad7 = 0x67,
		NumPad8 = 0x68, NumPad9 = 0x69,

		NumPadMul = 0x6A,
		NumPadAdd = 0x6B,
		NumPadSep = 0x6C,
		NumPadSub = 0x6D,
		NumPadDot = 0x6E,
		NumPadDiv = 0x6F,

		Insert = 0x2D,
		Delete = 0x2E,
		Home = 0x24,
		End = 0x23,
		PageUp = 0x21,
		PageDown = 0x22,
		Pause = 0x13,
	};

	enum class Mouse : int
	{
		Left = 0,   // 左ボタン
		Right = 1,   // 右ボタン
		Middle = 2,   // 中ボタン
	};
}


//外部からいつでもアクセスできるようにシングルトン化
class Input {
private:
	//キー入力情報を保存する変数
	BYTE keyState[256] = {};
	BYTE keyState_old[256] = {};

	//マウス情報を保存する変数
	HWND hWnd;			//ウィンドウハンドル
	int mouseClientX;	//クライアントの座標(左上基準)
	int mouseClientY;
	int mouseCenterX;	//スクリーン中央基準の座標
	int mouseCenterY;

	//マウスボタン（左・右・中）
	BYTE mouseState[3] = {};      //現在
	BYTE mouseState_old[3] = {};  //1フレーム前


	//コントローラー入力情報を保存する変数
	XINPUT_STATE controllerState = {};
	XINPUT_STATE controllerState_old = {};

	int VibrationTime; //振動継続時間をカウントする変数


	//コンストラクタを private に
	Input() : mouseClientX(0), mouseClientY(0), mouseCenterX(0), mouseCenterY(0), VibrationTime(0), hWnd(nullptr) {}

	~Input() {};

	Input(const Input&) = delete;            //コピー禁止
	Input& operator=(const Input&) = delete; //代入禁止

public:
	//シングルトン取得
	static Input& GetInstance() {
		static Input instance; //一度だけ作られる
		return instance;
	}


	void Init(HWND hWnd);
	void Uninit();
	void Update(); //更新

	//キーボードの処理
	void KeyBoardUpdate();
	bool GetKeyPress(int key)   const;   //プレス(押している間ずっと)
	bool GetKeyTrigger(int key)	const;	 //トリガー(押し始めた時)
	bool GetKeyRelease(int key)	const;	//リリース(押し終わった時)

	//マウスの処理
	void MouseUpdate();
	void SetHWnd(HWND h);               // ウィンドウハンドルセット

	//マウスボタン
	bool GetMousePress(Button::Mouse mouse)	const;   // 押し続けている
	bool GetMouseTrigger(Button::Mouse mouse)	const;   // 押した瞬間
	bool GetMouseRelease(Button::Mouse mouse)	const;	   // 離した瞬間

	//座標取得
	int GetMouseClientX() const;
	int GetMouseClientY() const;
	int GetMouseCenterX() const;
	int GetMouseCenterY() const;

	//マウスが画面の範囲内か検知
	bool GetMouseInsideWindow() const;

	//コントローラーの処理
	void ControllerUpdate();

	DirectX::SimpleMath::Vector2 GetLeftAnalogStick()  const; //アナログスティック傾き
	DirectX::SimpleMath::Vector2 GetRightAnalogStick() const;

	//コントローラーのボタン
	float GetLeftTrigger()		 const;	//トリガー押し込み度検知
	float GetRightTrigger()		 const;
	bool  GetButtonPress(Button::XBox btn)	 const;	//プレス
	bool  GetButtonTrigger(Button::XBox btn) const;	//トリガー
	bool  GetButtonRelease(Button::XBox btn) const;	//リリース

	//振動
	//flame：振動を継続する時間(単位：フレーム)
	//powoe：振動の強さ(0～1)
	void SetVibration(int frame = 1, float powor = 1);


	//各デバイスの入力状態を確認するデバッグ関数
	void DebugControllerStatus() const;
};


