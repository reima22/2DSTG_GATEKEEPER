//==============================================================================
//
// 入力処理（キーボード）〔input.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//==============================================================================
// キー入力
//==============================================================================
typedef enum
{
	KEYINFO_OK = 0,
	KEYINFO_ATTACK,
	KEYINFO_UP,
	KEYINFO_DOWN,
	KEYINFO_LEFT,
	KEYINFO_RIGHT,
	KEYINFO_UP_CENTER,
	KEYINFO_DOWN_CENTER,
	KEYINFO_LEFT_TURN,
	KEYINFO_RIGHT_TURN,
	KEYINFO_UP_SIGHT,
	KEYINFO_DOWN_SIGHT,
	KEYINFO_UP_MODEL,
	KEYINFO_DOWN_MODEL,
	KEYINFO_TURN_LEFT_MODEL,
	KEYINFO_TURN_RIGHT_MODEL,
	KEYINFO_SHOT,
	KEYINFO_ZOOMIN,
	KEYINFO_ZOOMOUT,
	KEYINFO_FRONT_PLUS,
	KEYINFO_FRONT_MINUS,
	KEYINFO_PAUSE,
	KEYINFO_PRESET,
	KEYINFO_CRESET,
	KEYINFO_MAX
} KEYINFO;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	// 入力の初期化処理
void UninitKeyboard(void);								// 入力の終了処理
void UpdateKeyboard(void);								// 入力の更新処理
bool GetKeyboardPress(int nKey);						// キーボード情報の取得
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);

#endif