//==============================================================================
//
// 入力処理（ゲームパッド）〔gamepad.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "input.h"
#include "gamepad.h"

// マクロ定義
#define MAX_CONTROLLER	(2)

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
//typedef struct 
//{
//	LPDIRECTINPUTDEVICE8* GamePadDevice;
//	int FindCount;
//}DeviceEnumParameter;

typedef struct
{
	LPDIRECTINPUTDEVICE8 Device;
	ButtonState ButtonStates[KEYINFO::KEYINFO_MAX];
}Controller;

//==============================================================================
// グローバル変数
//==============================================================================
static LPDIRECTINPUT8 g_InputInterface;
//static LPDIRECTINPUTDEVICE8 device;							// 入力デバイスへのポインタ
//static ButtonState g_ButtonStates[KEYINFO::KEYINFO_MAX];
Controller g_Control[MAX_CONTROLLER];
int g_nController = 0;

//==============================================================================
// パッド入力の初期化処理
//==============================================================================
HRESULT InitGamepad(HINSTANCE hInstance, HWND hWnd)
{
	// IDirectInput8インターフェースの作成
	if(FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_InputInterface,
		NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(g_InputInterface->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoysticksCallback,
		NULL,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	//if (FAILED(g_InputInterface->EnumDevices(
	//	DI8DEVTYPE_JOYSTICK,
	//	DeviceFindCallBack,
	//	&parameter,
	//	DIEDFL_ATTACHEDONLY)))
	//{
	//	return E_FAIL;
	//}

	//for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	//{
	//	// 入力デバイスの生成
	//	if (FAILED(g_InputInterface->CreateDevice(
	//		GUID_Joystick,
	//		&g_Control[nCnt].Device,
	//		NULL)))
	//	{
	//		return E_FAIL;
	//	}
	//}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DPROPAXISMODE_RELにしたら相対値)

	// 入力情報の初期化
	for (int nControl = 0; nControl < MAX_CONTROLLER; nControl++)
	{
		if (!g_Control[nControl].Device)
		{
			return E_FAIL;
		}

		for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
		{
			g_Control[nControl].ButtonStates[nCnt] = ButtonState::ButtonStateNone;
		}

		// データフォーマットの設定
		if (FAILED(g_Control[nControl].Device->SetDataFormat(
			&c_dfDIJoystick)))
		{
			return E_FAIL;
		}

		// 軸モードを変更
		if (FAILED(g_Control[nControl].Device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			return false;
		}

		// 左スティックの初期化
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -10000;
		diprg.lMax = 10000;

		// 右スティックの初期化
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = 0;

		if (FAILED(g_Control[nControl].Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}

		diprg.diph.dwObj = DIJOFS_Y;
		if (FAILED(g_Control[nControl].Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return false;
		}

		// 協調モードの設定
		if (FAILED(g_Control[nControl].Device->SetCooperativeLevel(
			hWnd,
			(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//==============================================================================
// パッド入力の終了処理
//==============================================================================
void UninitGamepad(void)
{
	for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_Control[nCnt].Device != NULL)
		{
			// アクセス権の破棄
			g_Control[nCnt].Device->Unacquire();

			g_Control[nCnt].Device->Release();
			g_Control[nCnt].Device = NULL;
		}
	}

	if (g_InputInterface != NULL)
	{
		g_InputInterface->Release();
		g_InputInterface = NULL;
	}
}

//==============================================================================
// パッド入力の更新処理
//==============================================================================
void UpdateGamepad(void)
{
	for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_Control[nCnt].Device == nullptr)
		{
			return;
		}

		DIJOYSTATE pad_data;

		HRESULT hr = g_Control[nCnt].Device->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);

		if (FAILED(hr))
		{
			if (FAILED(g_Control[nCnt].Device->Acquire()))
			{
				for (int nCntButton = 0; nCntButton < KEYINFO::KEYINFO_MAX; nCntButton++)
				{
					g_Control[nCnt].ButtonStates[nCntButton] = ButtonState::ButtonStateNone;
				}
				g_Control[nCnt].Device->Poll();
			}
			return;
		}

		ButtonSet(pad_data, nCnt);
	}
}

// ボタンの設定
void ButtonSet(DIJOYSTATE pad_data, int nCnt)
{
	// ローカル変数宣言
	bool is_push[MAX_CONTROLLER][KEYINFO::KEYINFO_MAX];
	int unresponsive_range = 200;
	float rightRange = 32767.0f;

	// 左スティックの範囲
	if (pad_data.lX < -unresponsive_range)
	{// 左に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_LEFT] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{// 右に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_RIGHT] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{// 上に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_UP] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{// 下に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_DOWN] = true;
	}

	// 右スティックの範囲
	if (pad_data.lZ < rightRange / 5.0f)
	{// 左に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_RIGHT_TURN] = true;
	}
	else if (pad_data.lZ > rightRange * 1.2f)
	{// 右に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_LEFT_TURN] = true;
	}

	if (pad_data.lRz < rightRange / 5.0f)
	{// 上に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_UP_SIGHT] = true;
	}
	else if (pad_data.lRz > rightRange * 1.2f)
	{// 下に傾けた
		is_push[nCnt][KEYINFO::KEYINFO_DOWN_SIGHT] = true;
	}

	// 十字キーの設定
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 8方向での制御
		switch (pad_data.rgdwPOV[0])
		{
			// 上
		case 0:
			is_push[nCnt][KEYINFO::KEYINFO_UP] = true;
			break;
			// 右上
		case 4500:
			is_push[nCnt][KEYINFO::KEYINFO_UP] = true;
			is_push[nCnt][KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右
		case 9000:
			is_push[nCnt][KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右下
		case 13500:
			is_push[nCnt][KEYINFO::KEYINFO_DOWN] = true;
			is_push[nCnt][KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 下
		case 18000:
			is_push[nCnt][KEYINFO::KEYINFO_DOWN] = true;
			break;
			// 左下
		case 22500:
			is_push[nCnt][KEYINFO::KEYINFO_DOWN] = true;
			is_push[nCnt][KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左
		case 27000:
			is_push[nCnt][KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左上
		case 31500:
			is_push[nCnt][KEYINFO::KEYINFO_UP] = true;
			is_push[nCnt][KEYINFO::KEYINFO_LEFT] = true;
			break;
		}
	}

	for (int nCntButton = 0; nCntButton < 32; nCntButton++)
	{
		if (!(pad_data.rgbButtons[nCntButton] & 0x80))
		{
			continue;
		}

		switch (nCntButton)
		{
		case 0:		// Xボタン
			is_push[nCnt][KEYINFO::KEYINFO_ATTACK] = true;
			break;

		case 2:		// Aボタン
			is_push[nCnt][KEYINFO::KEYINFO_OK] = true;
			break;

		case 3:		// Bボタン
			is_push[nCnt][KEYINFO::KEYINFO_CANCEL] = true;
			break;

		case 4:		// LBボタン
			is_push[nCnt][KEYINFO::KEYINFO_CAMERA_FRONT] = true;
			break;

		case 5:		// RBボタン
			is_push[nCnt][KEYINFO::KEYINFO_WEAPON_DROP] = true;
			break;

		case 6:		// LTボタン
			is_push[nCnt][KEYINFO::KEYINFO_CAMERA_FRONT] = true;
			break;

		case 7:		// RTボタン
			is_push[nCnt][KEYINFO::KEYINFO_WEAPON_DROP] = true;
			break;

		case 11:	// STARTボタン
			is_push[nCnt][KEYINFO::KEYINFO_PAUSE] = true;
			break;
		}
		//break;

	}

	// 入力情報からボタンの状態を更新する
	for (int nCntButton = 0; nCntButton < KEYINFO::KEYINFO_MAX; nCntButton++)
	{
		if (is_push[nCnt][nCntButton] == true)
		{
			if (g_Control[nCnt].ButtonStates[nCntButton] == ButtonState::ButtonStateNone)
			{
				g_Control[nCnt].ButtonStates[nCntButton] = ButtonState::ButtonStateDown;
			}
			else
			{
				g_Control[nCnt].ButtonStates[nCntButton] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (g_Control[nCnt].ButtonStates[nCntButton] == ButtonState::ButtonStatePush)
			{
				g_Control[nCnt].ButtonStates[nCntButton] = ButtonState::ButtonStateUp;
			}
			else
			{
				g_Control[nCnt].ButtonStates[nCntButton] = ButtonState::ButtonStateNone;
			}
		}
	}
}

// デバイス列挙コールバック関数
//BOOL CALLBACK DeviceFindCallBack(
//	LPCDIDEVICEINSTANCE ipddi,
//	LPVOID pvRef)
//{
//	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
//	LPDIRECTINPUTDEVICE8 device = nullptr;
//
//	// 既に発見しているなら終了
//	if (parameter->FindCount == MAX_CONTROLLER)
//	{
//		return DIENUM_STOP;
//	}
//
//	// 発見数をカウント
//	parameter->FindCount++;
//
//	return DIENUM_CONTINUE;
//}

BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT hr;

	hr = g_InputInterface->CreateDevice(pdidoi->guidInstance, &g_Control[g_nController].Device, NULL);

	g_nController++;

	return DIENUM_CONTINUE;

	//return DIENUM_STOP;
}

bool IsButtonPush(KEYINFO button)
{
	for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStatePush)
		{
			return true;
		}
	}


	return false;
}

bool IsButtonUp(KEYINFO button)
{
	for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStateUp)
		{
			return true;
		}
	}

	return false;
}

bool IsButtonDown(KEYINFO button)
{
	for (int nCnt = 0; nCnt < MAX_CONTROLLER; nCnt++)
	{
		if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStateDown)
		{
			return true;
		}
	}

	return false;
}

// プレス
bool BuPu(KEYINFO button, int nCnt)
{
	if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

// リリース
bool BuUp(KEYINFO button, int nCnt)
{
	if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

// トリガー
bool BuDo(KEYINFO button, int nCnt)
{
	if (g_Control[nCnt].ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}

// コントローラーの数を取得
int GetNumControl(void)
{
	return g_nController;
}