//==============================================================================
//
// ポーズロゴの描画〔pause_logo.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PAUSE_LOGO_H_
#define _PAUSE_LOGO_H_
#include "main.h"

typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;			// テクスチャのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// バッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXCOLOR col;								// 色
	bool bPlus;									// 透明度加減算判定
	int nCntAnim;								// アニメーションカウント
}PauseLogo;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPauseLogo(void);	// チュートリアルの初期化処理
void UninitPauseLogo(void);	// チュートリアルの終了処理
void UpdatePauseLogo(void);	// チュートリアルの更新処理
void DrawPauseLogo(void);	// チュートリアルの描画処理

#endif