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
		KEYINFO_SAVE = 0,				// データの書き出し
		KEYINFO_UP,						// 上
		KEYINFO_DOWN,					// 下
		KEYINFO_LEFT,					// 左
		KEYINFO_RIGHT,					// 右
		KEYINFO_WAVE_FLUG,				// 波形フラグ
		KEYINFO_WAVE_SYNTHETIC,			// 合成切り替え
		KEYINFO_WAVE_CUTTEX,			// テクスチャ分割の切り替え
		KEYINFO_WAVE_TEX_CHANGE,		// 操作するテクスチャの切り替え
		KEYINFO_WIREFRAME,				// ワイヤーフレームとの切り替え
		KEYINFO_TURN_LEFT,				// カメラ旋回左
		KEYINFO_TURN_RIGHT,				// カメラ旋回右
		KEYINFO_CAMERA_UP,				// カメラ上昇
		KEYINFO_CAMERA_DOWN,			// カメラ下降
		KEYINFO_CAMERA_ZOOMIN,			// ズームイン
		KEYINFO_CAMERA_ZOOMOUT,			// ズームアウト
		KEYINFO_WAVE_HEIGHTUP,			// 波形の高さ上昇
		KEYINFO_WAVE_HEIGHTDOWN,		// 波形の高さ下降
		KEYINFO_WAVE_DISTANCEUP,		// 波形の間隔増加
		KEYINFO_WAVE_DISTANCEDOWN,		// 波形の間隔減少
		KEYINFO_WAVE_SPEEDUP,			// サイン波の大きさの増加
		KEYINFO_WAVE_SPEEDDOWN,			// サイン波の大きさの減少
		KEYINFO_WAVE_TEXMOVEROT_PLUS,	// テクスチャの流れる方向回転角+
		KEYINFO_WAVE_TEXMOVEROT_MINUS,	// テクスチャの流れる方向回転角-
		KEYINFO_WAVE_TEXMOVE_PLUS,		// テクスチャの流れる速さ+
		KEYINFO_WAVE_TEXMOVE_MINUS,		// テクスチャの流れる速さ-
		KEYINFO_WAVE_TEXTURE_IDX_PLUS,	// テクスチャインデックスの加算
		KEYINFO_WAVE_TEXTURE_IDX_MINUS,	// テクスチャインデックスの減算
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

// マウスクラス
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		MOUSEINFO_LEFT = 0,		// 左クリック
		MOUSEINFO_RIGHT,		// 右クリック
		MOUSEINFO_WHEEL,		// マウスホイール押し込み
		MOUSEINFO_BBACK,		// ブラウザバックボタン
		MOUSEINFO_MOVEON,		// 「進む」ボタン
		MOUSEINFO_MAX
	}MOUSEINFO;

	CInputMouse();
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	static CInputMouse *Create(HINSTANCE hInstance, HWND hWnd);

	bool GetPress(int nKey);

	//bool GetTrigger(int nKey);
	//bool GetRelease(int nKey);

	//bool GetRepeat(int nKey);

	//int GetCntRepeat(int nKey) { return m_nCountRepeat[nKey]; }

	DIMOUSESTATE2 GetMouseState(void) { return m_mouseState; }

	//DIMOUSESTATE2 GetMouseTrigger(void) { return m_mouseStateTrigger; }
	//DIMOUSESTATE2 GetMouseRelease(void) { return m_mouseStateRelease; }

	BYTE GetButton(int nKey) { return m_aButton[nKey]; }

	// カーソルの移動量
	int GetMouselX(void) { return m_mouseState.lX; }
	int GetMouselY(void) { return m_mouseState.lY; }
	int GetMouselZ(void) { return m_mouseState.lZ; }	// ホイールの回転量

private:
	DIMOUSESTATE2 m_mouseState;

	// ボタンの種類
	BYTE m_aButton[MOUSEINFO_MAX] = {};

	//DIMOUSESTATE2 m_mouseStateTrigger;
	//DIMOUSESTATE2 m_mouseStateRelease;
	//BYTE m_aMouseState[MOUSE_BOTTON_NUM];			// マウスプレス情報
	//BYTE m_aMouseStateTrigger[MOUSE_BOTTON_NUM];	// マウストリガー情報
	//BYTE m_aMouseStateRelease[MOUSE_BOTTON_NUM];	// マウスリリース情報
};

#endif
