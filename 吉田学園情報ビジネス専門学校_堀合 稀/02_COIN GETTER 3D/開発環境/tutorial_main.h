//==============================================================================
//
// チュートリアル画面の描画〔tutorial_main.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _TUTORIAL_MAIN_H_
#define _TUTORIAL_MAIN_H_
#include "main.h"

// マクロ定義
#define TUTOMAIN_NUM	(2)	// チュートリアル表示の種類

typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureTutoMain = NULL;			// チュートリアル画面テクスチャへのポインタ
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	bool bUse;
}TutoMainInfo;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTutoMain = NULL;	// バッファへのポインタ
	TutoMainInfo tutoMainInfo[TUTOMAIN_NUM];
}TutoMain;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitTutoMain(void);	// チュートリアル画面の初期化処理
void UninitTutoMain(void);	// チュートリアル画面の終了処理
void UpdateTutoMain(void);	// チュートリアル画面の更新処理
void DrawTutoMain(void);	// チュートリアル画面の描画処理

#endif