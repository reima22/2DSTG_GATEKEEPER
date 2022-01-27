//==============================================================================
//
// 入力処理〔input.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define KEY_STATE	(256)	// 入力情報

// 入力クラス
class CInput
{
public:
	typedef enum 
	{
		KEYINFO_OK = 0,				// 決定
		KEYINFO_UP,					// 上
		KEYINFO_DOWN,				// 下
		KEYINFO_LEFT,				// 左
		KEYINFO_RIGHT,				// 右
		KEYINFO_WALK_UP,			// 上(徒歩)
		KEYINFO_WALK_DOWN,			// 下(徒歩)
		KEYINFO_WALK_LEFT,			// 左(徒歩)
		KEYINFO_WALK_RIGHT,			// 右(徒歩)
		KEYINFO_JUMP,				// ジャンプ
		KEYINFO_PAUSE,				// ポーズ
		KEYINFO_TUTO_BACK,			// チュートリアルページ戻し
		KEYINFO_TURN_LEFT,			// カメラ旋回左
		KEYINFO_TURN_RIGHT,			// カメラ旋回右
		KEYINFO_TURN_PLAYER_LEFT,	// プレイヤー旋回左
		KEYINFO_TURN_PLAYER_RIGHT,	// プレイヤー旋回右
		KEYINFO_MINUS_MOTION,		// モーション項目逆行(戻)
		KEYINFO_PLUS_MOTION,		// モーション項目順行(進)
		KEYINFO_CAMERA_UP,			// カメラ上昇
		KEYINFO_CAMERA_DOWN,		// カメラ下降
		KEYINFO_FOGSTART_PLUS,
		KEYINFO_FOGSTART_MINUS,
		KEYINFO_FOGEND_PLUS,
		KEYINFO_FOGEND_MINUS,
		KEYINFO_MAX
	}KEYINFO;

	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;	// 純粋仮想関数化

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;		// デバイスポインタ
	static LPDIRECTINPUT8 m_pInput;		// 全体で一つで良い(static:静的メンバ変数)	
	static int m_aKeyInfo[KEYINFO_MAX];	// 入力キー情報
};

class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	static CInputKeyboard *Create(HINSTANCE hInstance, HWND hWnd);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);

private:
	BYTE m_aKeyState[KEY_STATE];			// キープレス情報
	BYTE m_aKeyStateTrigger[KEY_STATE];		// キートリガー情報
	BYTE m_aKeyStateRelease[KEY_STATE];		// キーリリース情報
	BYTE m_aKeyStateRepeat[KEY_STATE];		// キーリピート情報

	int m_nCountRepeat;						// リピートカウント
};

#endif
