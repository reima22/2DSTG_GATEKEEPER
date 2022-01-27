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
#include <vector>

//==============================================================================
// 名前空間
//==============================================================================
using namespace std;

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
#define SCREEN_WIDTH	(1280)		// 画面の幅
#define SCREEN_HEIGHT	(720)		// 画面の高さ
#define SCREEN_CENTER	(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))	// 画面の中央
#define SCREEN_SIZE		(D3DXVECTOR2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f))		// 中央座標からの画面サイズ
#define COLOR_DEFAULT	(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))								// D3DXCOLOR型変数の初期値
#define VECTOR2_NULL	(D3DXVECTOR2(0.0f, 0.0f))										// D3DXVECTOR2型変数のNULL値
#define VECTOR3_NULL	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))									// D3DXVECTOR3型変数のNULL値
#define PI_DEFAULT		(D3DX_PI * 2.0f)
#define CNT_ONE_SECOND	(60)															// 1秒間のカウント数
	
//==============================================================================
// プロトタイプ宣言
//==============================================================================
int GetFPSCnt(void);	// FPSカウンタ取得

#endif