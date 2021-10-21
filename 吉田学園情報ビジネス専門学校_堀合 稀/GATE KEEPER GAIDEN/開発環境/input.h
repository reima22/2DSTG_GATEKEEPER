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
		KEYINFO_OK = 0,			// 決定
		KEYINFO_UP,				// 上
		KEYINFO_DOWN,			// 下
		KEYINFO_LEFT,			// 左
		KEYINFO_RIGHT,			// 右
		KEYINFO_SHOT,			// 弾発射
		KEYINFO_PAUSE,			// ポーズ
		KEYINFO_CHANGE_LEFT,	// 弾替え左
		KEYINFO_CHANGE_RIGHT,	// 弾替え右
		KEYINFO_TUTO_BACK,
		KEYINFO_ALPHA_PLUS,			// α値加算
		KEYINFO_ALPHA_MINUS,		// α値減算
		KEYINFO_FE_BIG,				// フェードエフェクト拡大
		KEYINFO_FE_SMALL,			// フェードエフェクト縮小
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

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);

private:

	BYTE m_aKeyState[KEY_STATE];			// キープレス情報
	BYTE m_aKeyStateTrigger[KEY_STATE];		// キートリガー情報
	BYTE m_aKeyStateRelease[KEY_STATE];		// キーリリース情報
};

#endif
