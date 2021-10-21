//==============================================================================
//
// ゲーム開始処理の描画〔start.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _START_H_
#define _START_H_
#include "main.h"

// 開始処理の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
	bool bUse;
}Start;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitStart(void);	// ゲーム開始処理の初期化処理
void UninitStart(void);		// ゲーム開始処理の終了処理
void UpdateStart(void);		// ゲーム開始処理の更新処理
void DrawStart(void);		// ゲーム開始処理の描画処理
int GetCount(void);

#endif