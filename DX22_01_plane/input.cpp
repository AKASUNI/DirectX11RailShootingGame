#include "input.h"
#include <iostream>

void Input::Init(HWND hWnd)
{
	//ウィンドウハンドルを登録
	SetHWnd(hWnd);
	VibrationTime = 0;
	mouseClientX = 0;
	mouseClientY = 0;
	mouseCenterX = 0;
	mouseCenterY = 0;
}

void Input::Uninit()
{
	//コントローラーの振動を止める
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);

	hWnd = nullptr;
}


void Input::Update()
{
	KeyBoardUpdate();
	MouseUpdate();
	ControllerUpdate();
	DebugControllerStatus();
}


/// <summary>
/// キーボード入力入力状態の更新
/// </summary>
void Input::KeyBoardUpdate()
{
	//前フレームのキー状態を保存
	for (int i = 0; i < 256; i++) {
		keyState_old[i] = keyState[i];
	}

	//現在のキー状態を取得
	for (int vk = 0; vk < 256; vk++) {
		keyState[vk] = (GetAsyncKeyState(vk) & 0x8000) ? 0x80 : 0;
	}
}


/// <summary>
/// マウス位置、入力状態の更新
/// </summary>
void Input::MouseUpdate()
{
	//前フレームのマウス状態を保存
	for (int i = 0; i < 3; i++)
	{
		mouseState_old[i] = mouseState[i];
	}


	//mouseの座標を更新
	POINT pt;

	//mouseのスクリーン座標を取得
	GetCursorPos(&pt);

	//スクリーン座標をクライアント座標に変換
	ScreenToClient(hWnd, &pt);

	//クライアント座標を取得
	mouseClientX = pt.x;
	mouseClientY = pt.y;

	//画面外に出たら画面内の座標に補正する
	RECT rc;
	GetClientRect(hWnd, &rc);
	int w = rc.right - rc.left;
	int h = rc.bottom - rc.top;
	mouseClientX = max(0, min(mouseClientX, w));
	mouseClientY = max(0, min(mouseClientY, h));


	//クライアント座標だとウィンドウの左上基準なので
	//ウィンドウの中央を基準とした座標に変換
	mouseCenterX = mouseClientX - (w / 2);
	mouseCenterY = (mouseClientY - (h / 2)) * -1;

	//マウスボタン更新
	//キーが押されていればtrueを格納、押されていなければfalseを格納
	mouseState[(int)Button::Mouse::Left] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 0x80 : 0;
	mouseState[(int)Button::Mouse::Right] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ? 0x80 : 0;
	mouseState[(int)Button::Mouse::Middle] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) ? 0x80 : 0;
}

/// <summary>
/// コントローラー入力状態の更新
/// </summary>
void Input::ControllerUpdate()
{
	//1フレーム前の入力を記録しておく
	controllerState_old = controllerState;

	//コントローラー入力を更新(XInput)
	XInputGetState(0, &controllerState);

	//振動継続時間をカウント
	if (VibrationTime > 0) {
		VibrationTime--;
		if (VibrationTime == 0) { //振動継続時間が経った時に振動を止める
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 0;
			vibration.wRightMotorSpeed = 0;
			XInputSetState(0, &vibration);
		}
	}
}

//ウィンドウハンドルセット（マウス座標を得るのに必要）
void Input::SetHWnd(HWND h) { hWnd = h; }

//マウス入力取得
//先頭ビットが立っていたらtrueを返し、それ以外はfalseを返す
bool Input::GetMousePress(Button::Mouse mouse) const
{
	int num = (int)mouse;
	return mouseState[num] & 0x80;
}

bool Input::GetMouseTrigger(Button::Mouse mouse) const
{
	int num = (int)mouse;
	return (mouseState[num] & 0x80) && !(mouseState_old[num] & 0x80);
}

bool Input::GetMouseRelease(Button::Mouse mouse) const
{
	int num = (int)mouse;
	return !(mouseState[num] & 0x80) && (mouseState_old[num] & 0x80);
}


//マウス座標取得
int Input::GetMouseClientX() const { return mouseClientX; }
int Input::GetMouseClientY() const { return mouseClientY; }
int Input::GetMouseCenterX() const { return mouseCenterX; }
int Input::GetMouseCenterY() const { return mouseCenterY; }

//マウスがウィンドウの範囲内か検知
bool Input::GetMouseInsideWindow() const
{
	//ウィンドウハンドルがnullまたは無効の場合何もしない
	if (!hWnd || !IsWindow(hWnd)) { return false; }

	//マウスカーソルの座標を取得し、ウィンドウハンドルの範囲内か検知する
	POINT cursorPos;
	if (GetCursorPos(&cursorPos)) {
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		return PtInRect(&windowRect, cursorPos);
	}
	return false;
}

//キー入力取得
bool Input::GetKeyPress(int key)   const { return keyState[key] & 0x80; }
bool Input::GetKeyTrigger(int key) const { return (keyState[key] & 0x80) && !(keyState_old[key] & 0x80); }
bool Input::GetKeyRelease(int key) const { return !(keyState[key] & 0x80) && (keyState_old[key] & 0x80); }


//左アナログスティック
DirectX::SimpleMath::Vector2 Input::GetLeftAnalogStick() const
{
	SHORT x = controllerState.Gamepad.sThumbLX;
	SHORT y = controllerState.Gamepad.sThumbLY;

	//値を-1～1に変換
	DirectX::SimpleMath::Vector2 res;
	res.x = x / 32767.0f;
	res.y = y / 32767.0f;
	return res;
}

//右アナログスティック
DirectX::SimpleMath::Vector2 Input::GetRightAnalogStick() const
{
	SHORT x = controllerState.Gamepad.sThumbRX; //-32768～32767
	SHORT y = controllerState.Gamepad.sThumbRY; //-32768～32767

	//値を-1～1に変換
	DirectX::SimpleMath::Vector2 res;
	res.x = x / 32767.0f;
	res.y = y / 32767.0f;
	return res;
}




//左トリガー
float Input::GetLeftTrigger() const
{
	BYTE t = controllerState.Gamepad.bLeftTrigger; //0～255

	//値を0～1に変換
	return t / 255.0f;
}
//右トリガー
float Input::GetRightTrigger() const
{
	BYTE t = controllerState.Gamepad.bRightTrigger; //0～255

	//値を0～1に変換
	return t / 255.0f;
}


//ボタン入力
bool Input::GetButtonPress(Button::XBox btn)  const//プレス
{
	WORD bit = static_cast<WORD>(btn);
	return (controllerState.Gamepad.wButtons & bit) != 0;
}

bool Input::GetButtonTrigger(Button::XBox btn) const//トリガー
{
	WORD bit = static_cast<WORD>(btn);
	return (controllerState.Gamepad.wButtons & bit) != 0 && (controllerState_old.Gamepad.wButtons & bit) == 0;
}

bool Input::GetButtonRelease(Button::XBox btn)  const//リリース
{
	WORD bit = static_cast<WORD>(btn);
	return (controllerState.Gamepad.wButtons & bit) == 0 && (controllerState_old.Gamepad.wButtons & bit) != 0;
}

//振動
void Input::SetVibration(int frame, float powor)
{
	//XINPUT_VIBRATION構造体のインスタンスを作成
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	//モーターの強度を設定（0～65535）
	vibration.wLeftMotorSpeed = (WORD)(powor * 65535.0f);
	vibration.wRightMotorSpeed = (WORD)(powor * 65535.0f);
	XInputSetState(0, &vibration);

	//振動継続時間を代入
	VibrationTime = frame;
}


void Input::DebugControllerStatus() const
{
	//ボタン表示
	//ペアクラスの最初にボタンの番号、二番目にテキスト
	std::pair<Button::XBox, const char*> buttons[] = {
		{ Button::XBox::A, "A" },
		{ Button::XBox::B, "B" },
		{ Button::XBox::X, "X" },
		{ Button::XBox::Y, "Y" },
		{ Button::XBox::Up,    "Up" },
		{ Button::XBox::Down,  "Down" },
		{ Button::XBox::Left,  "Left" },
		{ Button::XBox::Right, "Right" },
		{ Button::XBox::Start, "Start" },
		{ Button::XBox::Back,  "Back" },
		{ Button::XBox::LeftThumb,  "LeftThumb" },
		{ Button::XBox::RightThumb, "RightThumb" },
		{ Button::XBox::LeftShoulder,  "LeftShoulder" },
		{ Button::XBox::RightShoulder, "RightShoulder" },
	};

	//入力があれば文字列を出力する
	for (auto& b : buttons) {
		if (GetButtonTrigger(b.first)) std::cout << b.second << " Trigger\n";
		else if (GetButtonPress(b.first)) std::cout << b.second << " Press\n";
		else if (GetButtonRelease(b.first)) std::cout << b.second << " Release\n";
	}

	//スティック傾き表示
	auto left = GetLeftAnalogStick();
	auto right = GetRightAnalogStick();

	float lenLeft = std::sqrt(left.x * left.x + left.x * left.x);
	float lenRight = std::sqrt(right.x * right.x + right.y * right.y);

	//傾いていたら出力
	if (lenLeft > 0.1f)
		std::cout << "Left Stick  : (" << left.x << ", " << left.y << ")\n";

	if (lenRight > 0.1f)
		std::cout << "Right Stick : (" << right.x << ", " << right.y << ")\n";

	//トリガー押し込み度表示
	float lt = GetLeftTrigger();
	float rt = GetRightTrigger();

	if (lt > 0.01f)
		std::cout << "Left Trigger : " << lt << "\n";

	if (rt > 0.01f)
		std::cout << "Right Trigger: " << rt << "\n";


	//キーボード
	std::pair<Button::KeyBord, const char*> keys[] = {

		{ Button::KeyBord::A, "A" }, { Button::KeyBord::B, "B" },
		{ Button::KeyBord::C, "C" }, { Button::KeyBord::D, "D" },
		{ Button::KeyBord::E, "E" }, { Button::KeyBord::F, "F" },
		{ Button::KeyBord::G, "G" }, { Button::KeyBord::H, "H" },
		{ Button::KeyBord::I, "I" }, { Button::KeyBord::J, "J" },
		{ Button::KeyBord::K, "K" }, { Button::KeyBord::L, "L" },
		{ Button::KeyBord::M, "M" }, { Button::KeyBord::N, "N" },
		{ Button::KeyBord::O, "O" }, { Button::KeyBord::P, "P" },
		{ Button::KeyBord::Q, "Q" }, { Button::KeyBord::R, "R" },
		{ Button::KeyBord::S, "S" }, { Button::KeyBord::T, "T" },
		{ Button::KeyBord::U, "U" }, { Button::KeyBord::V, "V" },
		{ Button::KeyBord::W, "W" }, { Button::KeyBord::X, "X" },
		{ Button::KeyBord::Y, "Y" }, { Button::KeyBord::Z, "Z" },

		//数字
		{ Button::KeyBord::Num0, "0" }, { Button::KeyBord::Num1, "1" },
		{ Button::KeyBord::Num2, "2" }, { Button::KeyBord::Num3, "3" },
		{ Button::KeyBord::Num4, "4" }, { Button::KeyBord::Num5, "5" },
		{ Button::KeyBord::Num6, "6" }, { Button::KeyBord::Num7, "7" },
		{ Button::KeyBord::Num8, "8" }, { Button::KeyBord::Num9, "9" },

		{ Button::KeyBord::F1, "F1" },   { Button::KeyBord::F2, "F2" },
		{ Button::KeyBord::F3, "F3" },   { Button::KeyBord::F4, "F4" },
		{ Button::KeyBord::F5, "F5" },   { Button::KeyBord::F6, "F6" },
		{ Button::KeyBord::F7, "F7" },   { Button::KeyBord::F8, "F8" },
		{ Button::KeyBord::F9, "F9" },   { Button::KeyBord::F10, "F10" },
		{ Button::KeyBord::F11, "F11" }, { Button::KeyBord::F12, "F12" },

		{ Button::KeyBord::Up, "Up" },
		{ Button::KeyBord::Down, "Down" },
		{ Button::KeyBord::Left, "Left" },
		{ Button::KeyBord::Right, "Right" },

		{ Button::KeyBord::Shift, "Shift" },
		{ Button::KeyBord::Control, "Control" },
		{ Button::KeyBord::Alt, "Alt" },

		{ Button::KeyBord::LShift, "LShift" },
		{ Button::KeyBord::RShift, "RShift" },
		{ Button::KeyBord::LCtrl,  "LCtrl"  },
		{ Button::KeyBord::RCtrl,  "RCtrl"  },
		{ Button::KeyBord::LAlt,   "LAlt"   },
		{ Button::KeyBord::RAlt,   "RAlt"   },

		{ Button::KeyBord::Escape, "Escape" },
		{ Button::KeyBord::Space, "Space" },
		{ Button::KeyBord::Enter, "Enter" },
		{ Button::KeyBord::Backspace, "Backspace" },
		{ Button::KeyBord::Tab, "Tab" },
		{ Button::KeyBord::CapsLock, "CapsLock" },

		{ Button::KeyBord::NumPad0, "NumPad0" },
		{ Button::KeyBord::NumPad1, "NumPad1" },
		{ Button::KeyBord::NumPad2, "NumPad2" },
		{ Button::KeyBord::NumPad3, "NumPad3" },
		{ Button::KeyBord::NumPad4, "NumPad4" },
		{ Button::KeyBord::NumPad5, "NumPad5" },
		{ Button::KeyBord::NumPad6, "NumPad6" },
		{ Button::KeyBord::NumPad7, "NumPad7" },
		{ Button::KeyBord::NumPad8, "NumPad8" },
		{ Button::KeyBord::NumPad9, "NumPad9" },

		{ Button::KeyBord::NumPadMul, "*" },
		{ Button::KeyBord::NumPadAdd, "+" },
		{ Button::KeyBord::NumPadSep, "Separator" },
		{ Button::KeyBord::NumPadSub, "-" },
		{ Button::KeyBord::NumPadDot, "." },
		{ Button::KeyBord::NumPadDiv, "/" },

		{ Button::KeyBord::Insert, "Insert" },
		{ Button::KeyBord::Delete, "Delete" },
		{ Button::KeyBord::Home, "Home" },
		{ Button::KeyBord::End, "End" },
		{ Button::KeyBord::PageUp, "PageUp" },
		{ Button::KeyBord::PageDown, "PageDown" },
		{ Button::KeyBord::Pause, "Pause" },
	};

	for (auto& k : keys) {
		if (GetKeyTrigger((int)k.first)) std::cout << k.second << " Trigger\n";
		else if (GetKeyPress((int)k.first))   std::cout << k.second << " Press\n";
		else if (GetKeyRelease((int)k.first)) std::cout << k.second << " Release\n";
	}


	//すべてのマウスボタンをチェック
	std::pair<Button::Mouse, const char*> mButtons[] = {
		{ Button::Mouse::Left, "Left" },
		{ Button::Mouse::Right, "Right" },
		{ Button::Mouse::Middle, "Middle" }
	};

	for (auto& b : mButtons)
	{
		if (GetMouseTrigger(b.first))   std::cout << b.second << " Trigger\n";
		else if (GetMousePress(b.first)) std::cout << b.second << " Press\n";
		else if (GetMouseRelease(b.first)) std::cout << b.second << " Release\n";
	}


	//mouseの座標を確認
	//左クリックを押すと情報を表示する
	if (GetMouseTrigger(Button::Mouse::Left))
	{
		//クライアント座標
		std::cout << "Mouse Client Pos : ("
			<< GetMouseClientX() << ", "
			<< GetMouseClientY() << ")\n";

		//中央基準座標
		std::cout << "Mouse Center Pos : ("
			<< GetMouseCenterX() << ", "
			<< GetMouseCenterY() << ")\n";

		//画面内チェック
		if (GetMouseInsideWindow())
			std::cout << "Mouse Inside Window" << std::endl;
		else
			std::cout << "Mouse Outside Window" << std::endl;
	}
}
