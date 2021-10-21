//==============================================================================
//
// スコアの描画〔bullet_ui.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BULLET_UI_H_
#define _BULLET_UI_H_
#include "main.h"

// マクロ定義
#define MAX_BULLET_UI	(8)
#define MAX_UI_NUM		(3)
#define TEX_BULLET_UI	(2)

typedef struct
{
	D3DXVECTOR3 posLogo;
	D3DXVECTOR3 posNum[MAX_UI_NUM];
	int nBullet;
}BulletUI;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBulletUI(void);						// 弾のUIの初期化処理
void UninitBulletUI(void);						// 弾のUIの終了処理
void UpdateBulletUI(void);						// 弾のUIの更新処理
void DrawBulletUI(void);						// 弾のUIの描画処理
//void AddScore(int nValue);					// 弾のUIの加算処理
void SetVertexBulletUI(int nIdx, int nNumber,
int nCnt);	// 弾のUIの座標情報取得
//int GetScore(void);							// 弾のUI情報の取得

#endif