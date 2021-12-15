//==============================================================================
//
// ノーダメージボーナスの描画〔nodamage.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _NODAMAGE_H_
#define _NODAMAGE_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define TEX_BONUS	(2)			// テクスチャの数
#define MAX_BONUS	(6)			// ボーナスの桁数
#define MAX_POLY	(7)			// ポリゴンの総数
#define LOGO_POSX	(150.0f)	// 表示のX座標
#define LOGO_POSY	(100.0f)	// 表示のY座標
#define LOGO_SIZEX	(150.0f)	// ロゴサイズの幅
#define LOGO_SIZEY	(20.0f)		// ロゴサイズの高さ
#define BONUS_POSX	(120.0f)	// ボーナススコアの1桁目のX座標
#define BONUS_POSY	(150.0f)	// ボーナススコアの1桁目のY座標
#define BONUS_SIZEX	(10.0f)		// ボーナススコア1桁の幅
#define BONUS_SIZEY	(20.0f)		// ボーナススコア1桁の高さ

//==============================================================================
// ボーナスの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 posNodamage;			// ボーナス表示の位置
	D3DXVECTOR3 posBonus[MAX_BONUS];	// ボーナススコアの位置
	D3DXCOLOR colNodamage;				// 色・透明度
	int nBonus;							// ボーナススコア
	int nBonusCnt;						// ボーナス処理のカウント
} BONUS;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitNodamage(void);	// ノーダメージボーナスの初期化処理
void UninitNodamage(void);	// ノーダメージボーナスの終了処理
void UpdateNodamage(void);	// ノーダメージボーナスの更新処理
void DrawNodamage(void);	// ノーダメージボーナスの描画処理
void AddScoreBonus(void);
BONUS GetBonus(void);

#endif
