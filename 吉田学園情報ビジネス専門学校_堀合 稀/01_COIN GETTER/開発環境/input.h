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
	KEYINFO_UP,
	KEYINFO_DOWN,
	KEYINFO_LEFT,
	KEYINFO_RIGHT,
	KEYINFO_JUMP,
	KEYINFO_PAUSE,
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