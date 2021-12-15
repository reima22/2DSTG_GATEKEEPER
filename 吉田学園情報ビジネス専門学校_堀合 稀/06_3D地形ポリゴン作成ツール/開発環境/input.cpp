//==============================================================================
//
// 入力処理〔input.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "input.h"

//==============================================================================
// 静的メンバ変数
//==============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;
int CInput::m_aKeyInfo[] =
{
	DIK_RETURN,
	DIK_W,			// プレイヤー移動
	DIK_S,			// プレイヤー移動
	DIK_A,			// プレイヤー移動
	DIK_D,			// プレイヤー移動
	DIK_Y,
	DIK_H,
	DIK_U,
	DIK_J,
	DIK_M,
	DIK_UP,			// カメラ移動
	DIK_DOWN,		// カメラ移動
	DIK_LEFT,		// カメラ移動
	DIK_RIGHT,		// カメラ移動
	DIK_SPACE,
	DIK_Z,			// カメラ旋回
	DIK_C,			// カメラ旋回
	DIK_X,			// カメラのリセット
	DIK_R,			// カメラ上昇
	DIK_F,			// カメラ下降
	DIK_T,			// ズームイン
	DIK_G,			// ズームアウト
	DIK_B,			// テクスチャインデックス後退
	DIK_V,			// テクスチャインデックス進行
	DIK_1,			// テクスチャの分割
	DIK_2,			// ワイヤーフレーム判定
	DIK_F1,			// 保存コマンド
	DIK_F2,			// 編集モードの切り替え
	DIK_F3			// モード切替
};
//DIMOUSESTATE2 CInputMouse::m_mouseState2;
//BYTE CInputMouse::m_aButton[MOUSEINFO_MAX] =
//{
//
//};

//==============================================================================
// コンストラクタ
//==============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//==============================================================================
// デストラクタ
//==============================================================================
CInput::~CInput()
{
}

//==============================================================================
// 初期化
//==============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInput,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==============================================================================
// 終了
//==============================================================================
void CInput::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		// アクセス権の破棄
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==============================================================================
// キーボードのコンストラクタ
//==============================================================================
CInputKeyboard::CInputKeyboard()
{
	// キー情報の初期化
	memset(m_aKeyState,NULL,sizeof(m_aKeyState));
	memset(m_aKeyStateTrigger, NULL, sizeof(m_aKeyStateTrigger));
	memset(m_aKeyStateRelease, NULL, sizeof(m_aKeyStateRelease));
}

// キーボードのデストラクタ
CInputKeyboard::~CInputKeyboard()
{

}

//==============================================================================
// キーボード初期化
//==============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysKeyboard,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// アクセス権の取得
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================
// キーボード終了
//==============================================================================
void CInputKeyboard::Uninit(void)
{
	// 終了処理
	CInput::Uninit();
}

//==============================================================================
// キーボード更新
//==============================================================================
void CInputKeyboard::Update(void)
{
	// ローカル変数宣言
	BYTE aKeyState[KEY_STATE];

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(
		sizeof(aKeyState),
		&aKeyState[0])))
	{
		for (int nKey = 0; nKey < KEY_STATE; nKey++)
		{
			m_aKeyStateTrigger[nKey] = (m_aKeyState[nKey] & aKeyState[nKey]) ^ aKeyState[nKey];
			m_aKeyStateRelease[nKey] = (m_aKeyState[nKey] | aKeyState[nKey]) ^ aKeyState[nKey];
			m_aKeyState[nKey] = aKeyState[nKey];
		}
	}
	else
	{
		// アクセス権の取得
		m_pDevice->Acquire();
	}
}

// キーボードの生成
CInputKeyboard *CInputKeyboard::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ローカル変数宣言
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = new CInputKeyboard;

	// 初期化
	if (pInputKeyboard != NULL)
	{
		pInputKeyboard->Init(hInstance, hWnd);
	}

	return pInputKeyboard;
}

//==============================================================================
// プレス処理
//==============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	// キーボード情報の取得
	return (m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// トリガー処理
//==============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	// キーボード情報の取得
	return (m_aKeyStateTrigger[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// リリース処理
//==============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	// キーボード情報の取得
	return (m_aKeyStateRelease[m_aKeyInfo[nKey]] & 0x80) ? true : false;
}

//==============================================================================
// リピート処理
//==============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	if ((m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false == true)
	{
		m_nCountRepeat[nKey]++;
	}
	else
	{
		m_nCountRepeat[nKey] = 0;
	}

	// キーボード情報の取得
	if (m_nCountRepeat[nKey] == 1)
	{
		return (m_aKeyStateTrigger[m_aKeyInfo[nKey]] & 0x80) ? true : false;
	}
	else if (m_nCountRepeat[nKey] > REPEAT_CNT)
	{
		return (m_aKeyState[m_aKeyInfo[nKey]] & 0x80) ? true : false;
	}
	else
	{
		return false;
	}
}

//==============================================================================
// マウスのコンストラクタ
//==============================================================================
CInputMouse::CInputMouse()
{
	//// キー情報の初期化
	//memset(m_aKeyState, NULL, sizeof(m_aKeyState));
	//memset(m_aKeyStateTrigger, NULL, sizeof(m_aKeyStateTrigger));
	//memset(m_aKeyStateRelease, NULL, sizeof(m_aKeyStateRelease));
}

// マウスのデストラクタ
CInputMouse::~CInputMouse()
{

}

//==============================================================================
// マウス初期化
//==============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 初期化処理
	CInput::Init(hInstance, hWnd);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(
		GUID_SysMouse,
		&m_pDevice,
		NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットの設定
	if (FAILED(m_pDevice->SetDataFormat(
		&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードの設定
	if (FAILED(m_pDevice->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// アクセス権の取得
	m_pDevice->Acquire();

	return S_OK;
}

//==============================================================================
// マウス終了
//==============================================================================
void CInputMouse::Uninit(void)
{
	// 終了処理
	CInput::Uninit();
}

//==============================================================================
// マウス更新
//==============================================================================
void CInputMouse::Update(void)
{
	// ローカル変数宣言
	//BYTE aMouseState[MOUSE_BOTTON_NUM];
	DIMOUSESTATE2 mouseState;

	// デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(
		sizeof(mouseState),
		&mouseState)))
	{
		
		//DIMOUSESTATE m_mouseState;
		//DIMOUSESTATE m_mouseStateTrigger;
		//DIMOUSESTATE m_mouseStateRelease;
		//m_mouseStateTrigger = (m_mouseState & mouseState) ^ mouseState;
		//m_mouseStateRelease = (m_mouseState | mouseState) ^ mouseState;

		m_mouseState = mouseState;

		for (int nCntButton = 0; nCntButton < MOUSEINFO_MAX; nCntButton++)
		{
			m_aButton[nCntButton] = m_mouseState.rgbButtons[nCntButton];
		}
	}
	else
	{
		// アクセス権の取得
		m_pDevice->Acquire();
	}
}

//==============================================================================
// マウスの生成
//==============================================================================
CInputMouse *CInputMouse::Create(HINSTANCE hInstance, HWND hWnd)
{
	// ローカル変数宣言
	CInputMouse *pInputMouse;
	pInputMouse = new CInputMouse;

	// 初期化
	if (pInputMouse != NULL)
	{
		pInputMouse->Init(hInstance, hWnd);
	}

	return pInputMouse;
}

//==============================================================================
// プレス処理
//==============================================================================
bool CInputMouse::GetPress(int nKey)
{
	// キーボード情報の取得
	return (nKey & 0x80) ? true : false;
}

////==============================================================================
//// トリガー処理
////==============================================================================
//bool CInputMouse::GetTrigger(int nKey)
//{
//	// キーボード情報の取得
//	return (nKey & 0x80) ? true : false;
//}
//
////==============================================================================
//// リリース処理
////==============================================================================
//bool CInputMouse::GetRelease(int nKey)
//{
//	// キーボード情報の取得
//	return (nKey & 0x80) ? true : false;
//}