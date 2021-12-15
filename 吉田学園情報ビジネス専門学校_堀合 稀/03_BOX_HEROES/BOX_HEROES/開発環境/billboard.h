//==============================================================================
//
// ビルボードの描画〔billboard.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_
#include "main.h"

// マクロ定義
//#define MAX_WALL	(4)	// 壁の最大数

typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	bool bUse;
} Billboard;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBillboard(void);	// ビルボードの初期化処理
void UninitBillboard(void);		// ビルボードの終了処理
void UpdateBillboard(void);		// ビルボードの更新処理
void DrawBillboard(void);		// ビルボードの描画処理

#endif
