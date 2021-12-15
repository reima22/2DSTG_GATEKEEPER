//==============================================================================
//
// 敵の描画〔enemy.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ENEMY			(256)		// 敵の最大数
#define MAX_TYPE_ENEMY		(5)			// 敵の種類

//==============================================================================
// 敵の状態
//==============================================================================
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	// 通常状態
	ENEMYSTATE_DAMAGE,		// ダメージ状態
	ENEMYSTATE_MAX
} ENEMYSTATE;

//==============================================================================
// 敵の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 現在の位置
	D3DXVECTOR3 posOld;	// 前回の位置
	float fWidth;		// 幅
	float fHeight;		// 高さ
	int nType;			// 種類
	bool bUse;			// 使用しているかどうか
	ENEMYSTATE state;	// 状態
	int nCounterState;	// 状態カウンター
	int nLife;			// 体力
	D3DXVECTOR3 move;	// 移動量
	int nAnimCnt;
	float fTex;
} ENEMY;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitEnemy(void);		// 敵の初期化処理
void UninitEnemy(void);			// 敵の終了処理
void UpdateEnemy(void);			// 敵の更新処理
void DrawEnemy(void);			// 敵の描画処理
void SetEnemy(					// 敵の設定
	D3DXVECTOR3 pos,			// 位置
	D3DXVECTOR3 move,			// 移動量
	float fWidth,				// 幅
	float fHeight,				// 高さ
	int nType,int nLife);		// タイプ、体力
void SetVertexEnemy(int nIdx);	// 敵の頂点座標取得
void MoveEnemy(void);			// 敵の移動
ENEMY *GetEnemy(void);			// 敵の情報
void AnimEnemy(void);			// 敵のアニメーション

#endif
