//==============================================================================
//
// 影の描画〔shadow.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_SHADOW	(128)	// 影の最大数

//==============================================================================
// 影の構造体
//==============================================================================
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	float fWidth;			// 横幅
	float fDepth;			// 奥行き
	bool bUse;				// 使用しているか
} Shadow;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitShadow(void);		// 影の初期化処理
void UninitShadow(void);		// 影の終了処理
void UpdateShadow(void);		// 影の更新処理
void DrawShadow(void);			// 影の描画処理
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth);
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos);
Shadow *GetShadow(void);

#endif