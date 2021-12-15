//==============================================================================
//
// 入力処理〔input.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

// マクロ定義
#define REPEAT_CNT	(30)	// リピートカウントの上限
#define MOUSE_BOTTON_NUM	(4)	// マウスボタン数


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
		KEYINFO_AREA_PLUS,		// 編集エリアの拡大
		KEYINFO_AREA_MINUS,		// 編集エリアの縮小
		KEYINFO_MESH_UP,		// メッシュ上昇
		KEYINFO_MESH_DOWN,		// メッシュ下降
		KEYINFO_MESH_RESET,		// 頂点の高さのリセット
		KEYINFO_CAMERA_FRONT,	// カメラ移動前進
		KEYINFO_CAMERA_BACK,	// カメラ移動後退
		KEYINFO_CAMERA_LEFT,	// カメラ移動左
		KEYINFO_CAMERA_RIGHT,	// カメラ移動右
		KEYINFO_JUMP,			// ジャンプ
		KEYINFO_TURN_LEFT,		// カメラ旋回左
		KEYINFO_TURN_RIGHT,		// カメラ旋回右
		KEYINFO_CAMERA_RESET,	// カメラのリセット
		KEYINFO_CAMERA_UP,		// カメラ上昇
		KEYINFO_CAMERA_DOWN,	// カメラ下降
		KEYINFO_ZOOMIN,			// カメラのズームイン
		KEYINFO_ZOOMOUT,		// カメラのズームアウト
		KEYINFO_TEXIDX_PLUS,	// テクスチャのインデックス前進
		KEYINFO_TEXIDX_MINUS,	// テクスチャのインデックス後退
		KEYINFO_TEXCUT,			// テクスチャの分割
		KEYINFO_WIRE,			// ワイヤーフレームの判定
		KEYINFO_SAVE_MESHINFO,	// メッシュ情報保存
		KEYINFO_LOAD_MESHINFO,	// メッシュ読み込み
		KEYINFO_MODECHANGE,		// 編集モードの切り替え
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

// キーボードクラス
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

	int GetCntRepeat(int nKey) { return m_nCountRepeat[nKey]; }

private:
	BYTE m_aKeyState[KEY_STATE];			// キープレス情報
	BYTE m_aKeyStateTrigger[KEY_STATE];		// キートリガー情報
	BYTE m_aKeyStateRelease[KEY_STATE];		// キーリリース情報
	int m_nCountRepeat[KEY_STATE];			// リピートカウント
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
