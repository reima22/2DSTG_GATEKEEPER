//==============================================================================
//
// ゲームパッド入力処理〔gamepad.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
#include "main.h"
#include "input.h"

//==============================================================================
// パッド入力
//==============================================================================
class CGamepad : public CInput
{
public:
	// ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
	typedef struct
	{
		LPDIRECTINPUTDEVICE8* GamePadDevice;
		int FindCount;
	}DeviceEnumParameter;

	// ボタンの状態
	typedef enum
	{
		ButtonStateNone = 0,
		ButtonStateDown,
		ButtonStatePush,
		ButtonStateUp,
		ButtonStateMax,
	} ButtonState;

	CGamepad();
	~CGamepad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// パッド入力の初期化処理
	void Uninit(void);								// パッド入力の終了処理
	void Update(void);								// パッド入力の更新処理

	static BOOL CALLBACK DeviceFindCallBack(
		LPCDIDEVICEINSTANCE ipddi,
		LPVOID pvRef);

	bool GetPress(KEYINFO button);
	bool GetRelease(KEYINFO button);
	bool GetTrigger(KEYINFO button);

	// スティックの倒した分の数値取得
	int GetPadlX(void) { return m_nlX; }
	int GetPadlY(void) { return m_nlY; }

private:
	static LPDIRECTINPUT8 m_InputInterface;
	static LPDIRECTINPUTDEVICE8 m_Device;
	static ButtonState m_ButtonStates[KEYINFO::KEYINFO_MAX];

	// スティックを倒した値
	int m_nlX;
	int m_nlY;
};

// デバイスの列挙
typedef enum
{
	DIDEVTYPE_MOUSE = 0,	// マウス
	DIDEVTYPE_KEYBOARD,		// キーボード
	DIDEVTYPE_JOYSTICK,		// ジョイスティック
	DIDEVTYPE_DEVICE,		// その他
	DIDEVTYPE_MAX
} DIDEVTYPE;

#endif
