//==============================================================================
//
// 壁の描画〔wall.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _WALL_H_
#define _WALL_H_
#include "main.h"

// マクロ定義
#define MAX_WALL	(4)		// 壁の最大数
#define WALL_WIDTH	(50.0f)	// 標準値
#define WALL_HEIGHT	(50.0f)	// 標準値

// 壁の構造体
typedef struct
{
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3	posMove;
	D3DXVECTOR3 rot;
	float fWidth;
	float fHeight;
	bool bUse;
} Wall;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitWall(void);	// 壁の初期化処理
void UninitWall(void);	// 壁の終了処理
void UpdateWall(void);	// 壁の更新処理
void DrawWall(void);	// 壁の描画処理
void SetWall(			// 壁の設定
D3DXVECTOR3 pos,			// 位置
D3DXVECTOR3 rot,			// 向き
float fWidth,				// 幅
float fHeight);				// 高さ

#endif
