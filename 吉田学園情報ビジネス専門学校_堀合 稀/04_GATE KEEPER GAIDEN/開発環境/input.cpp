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
	DIK_W,
	DIK_S,
	DIK_A,
	DIK_D,
	DIK_SPACE,
	DIK_P,
	DIK_N,
	DIK_M,
	DIK_BACKSPACE,
	DIK_T,			// フェードエフェクトα値加算
	DIK_G,			// フェードエフェクトα値減算
	DIK_Y,			// フェードエフェクト拡大
	DIK_H,			// フェードエフェクト縮小
};

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