//==============================================================================
//
// ゲームパッド入力処理〔gamepad.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "input.h"
#include "gamepad.h"

// 静的メンバ変数宣言
LPDIRECTINPUT8 CGamepad::m_InputInterface;
LPDIRECTINPUTDEVICE8 CGamepad::m_Device;
CGamepad::ButtonState CGamepad::m_ButtonStates[KEYINFO::KEYINFO_MAX];

// コンストラクタ
CGamepad::CGamepad()
{
}

// デストラクタ
CGamepad::~CGamepad()
{
}

//==============================================================================
// パッド入力の初期化処理
//==============================================================================
HRESULT CGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// IDirectInput8インターフェースの作成
	if(FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_InputInterface,
		NULL)))
	{
		return E_FAIL;
	}

	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &m_Device;

	if (FAILED(m_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (FAILED(m_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// 入力デバイスの生成
	if (FAILED(m_InputInterface->CreateDevice(
		GUID_Joystick,
		&m_Device,
		NULL)))
	{
		return E_FAIL;
	}

	// 入力情報の初期化
	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		m_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
	}

	// データフォーマットの設定
	if (FAILED(m_Device->SetDataFormat(
		&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DPROPAXISMODE_RELにしたら相対値)
	
	// 軸モードを変更
	if (FAILED(m_Device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -2000;
	diprg.lMax = 2000;
	if (FAILED(m_Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(m_Device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// 協調モードの設定
	if (FAILED(m_Device->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_EXCLUSIVE))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================================
// パッド入力の終了処理
//==============================================================================
void CGamepad::Uninit(void)
{
	if (m_Device != NULL)
	{
		// アクセス権の破棄
		m_Device->Unacquire();

		m_Device->Release();
		m_Device = NULL;
	}

	if (m_InputInterface != NULL)
	{
		m_InputInterface->Release();
		m_InputInterface = NULL;
	}
}

//==============================================================================
// パッド入力の更新処理
//==============================================================================
void CGamepad::Update(void)
{
	if (m_Device == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;

	HRESULT hr = m_Device->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		if (FAILED(m_Device->Acquire()))
		{
			for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
			{
				m_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
			}
			m_Device->Poll();
		}
		return;
	}
	
	// 押下情報
	bool is_push[KEYINFO::KEYINFO_MAX];

	int unresponsive_range = 200;
	int nWalkRange = 1500;

	//if (pad_data.lX < -unresponsive_range && pad_data.lX > -nWalkRange)
	//{// 左に少し傾けた
	//	is_push[KEYINFO::KEYINFO_WALK_LEFT] = true;
	//}
	//else if (pad_data.lX > unresponsive_range && pad_data.lX < nWalkRange)
	//{// 右に傾けた
	//	is_push[KEYINFO::KEYINFO_WALK_RIGHT] = true;
	//}

	//if (pad_data.lY < -unresponsive_range && pad_data.lY > -nWalkRange)
	//{// 上に少し傾けた
	//	is_push[KEYINFO::KEYINFO_WALK_UP] = true;
	//}
	//else if (pad_data.lY > unresponsive_range && pad_data.lY < nWalkRange)
	//{// 下に少し傾けた
	//	is_push[KEYINFO::KEYINFO_WALK_DOWN] = true;
	//}

	if (pad_data.lX < -nWalkRange)
	{// 左に傾けた
		is_push[KEYINFO::KEYINFO_LEFT] = true;
	}
	else if (pad_data.lX > nWalkRange)
	{// 右に傾けた
		is_push[KEYINFO::KEYINFO_RIGHT] = true;
	}

	if (pad_data.lY < -nWalkRange)
	{// 上に傾けた
		is_push[KEYINFO::KEYINFO_UP] = true;
	}
	else if (pad_data.lY > nWalkRange)
	{// 下に傾けた
		is_push[KEYINFO::KEYINFO_DOWN] = true;
	}

	// 値の設定
	m_nlX = pad_data.lX;
	m_nlY = pad_data.lY;

	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		// 8方向での制御
		switch (pad_data.rgdwPOV[0])
		{
			// 上
		case 0:
			is_push[KEYINFO::KEYINFO_UP] = true;
			break;
			// 右上
		case 4500:
			is_push[KEYINFO::KEYINFO_UP] = true;
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右
		case 9000:
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 右下
		case 13500:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			is_push[KEYINFO::KEYINFO_RIGHT] = true;
			break;
			// 下
		case 18000:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			break;
			// 左下
		case 22500:
			is_push[KEYINFO::KEYINFO_DOWN] = true;
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左
		case 27000:
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
			// 左上
		case 31500:
			is_push[KEYINFO::KEYINFO_UP] = true;
			is_push[KEYINFO::KEYINFO_LEFT] = true;
			break;
		}
	}
	
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		if (!(pad_data.rgbButtons[nCnt] & 0x80))
		{
			continue;
		}
		
		switch (nCnt)
		{
		case 2:
			is_push[KEYINFO::KEYINFO_OK] = true;
			break;


		case 4:
			is_push[KEYINFO::KEYINFO_TURN_LEFT] = true;
			break;

		case 5:
			is_push[KEYINFO::KEYINFO_TURN_RIGHT] = true;
			break;
		}
	}

	// 入力情報からボタンの状態を更新する
	for (int nCnt = 0; nCnt < KEYINFO::KEYINFO_MAX; nCnt++)
	{
		if (is_push[nCnt] == true)
		{
			if (m_ButtonStates[nCnt] == ButtonState::ButtonStateNone)
			{// 押下
				m_ButtonStates[nCnt] = ButtonState::ButtonStateDown;
			}
			else
			{// 押し続ける
				m_ButtonStates[nCnt] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (m_ButtonStates[nCnt] == ButtonState::ButtonStatePush)
			{// 押しているところから離す
				m_ButtonStates[nCnt] = ButtonState::ButtonStateUp;
			}
			else
			{// 押していない
				m_ButtonStates[nCnt] = ButtonState::ButtonStateNone;
			}
		}
	}
}

// デバイス列挙コールバック関数
BOOL CALLBACK CGamepad::DeviceFindCallBack(
	LPCDIDEVICEINSTANCE ipddi,
	LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// 既に発見しているなら終了
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// 発見数をカウント
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

// プレス
bool CGamepad::GetPress(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

// リリース
bool CGamepad::GetRelease(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

// トリガー
bool CGamepad::GetTrigger(KEYINFO button)
{
	if (m_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}