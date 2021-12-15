//=============================================================================
//
// メイン処理 [main.h]
// Author : Mare Horiai
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#pragma once
#include <windows.h>
#include "d3dx9.h"						// 描画処理に必要
#define DIRECTINPUT_VERSION	(0x0800)
#include "dinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド再生に必要 
#include <time.h>

//==============================================================================
// ライブラリのリンク
//==============================================================================
#pragma comment(lib, "d3d9.lib")	// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")	// 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")	// コンポーネント(部品)使用に必要
#pragma comment(lib, "dinput8.lib")	// 入力処理に必要
#pragma comment(lib, "winmm.lib")

//==============================================================================
// マクロ定義
//==============================================================================
#define SCREEN_WIDTH		(1280)		// 画面の幅
#define SCREEN_HEIGHT		(720)		// 画面の高さ
#define SCREEN_CENTER		(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2, 0.0f))
#define SCREEN_CENTER_SIZE	(D3DXVECTOR2(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2))
#define VECTOR2_NULL		(D3DXVECTOR2(0.0f, 0.0f))
#define VECTOR3_NULL		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define PI_RESET			(D3DX_PI * 2.0f)	// 回転角の修正値

//==============================================================================
// プロトタイプ宣言
//==============================================================================
int GetFPSCnt(void);	// FPSカウンタ取得

#endif