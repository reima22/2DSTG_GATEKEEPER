//==============================================================================
//
// 弾の描画〔bullet.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//==============================================================================
// 敵味方の弾の状態
//==============================================================================
typedef enum
{
	BULLETTYPE_PLAYER = 0,	// 自機の弾
	BULLETTYPE_ENEMY,		// 敵の弾
	BULLETTYPE_MAX
} BULLETTYPE;

//==============================================================================
// 自機の弾の装備状態
//==============================================================================
typedef enum
{
	SETBULLET_NORMAL = 0,	// 通常弾
	SETBULLET_WAVE,			// 波状攻撃
	SETBULLET_MISSILE,		// ミサイル
	SETBULLET_REVERSEWAVE,	// 後方波状攻撃
	SETBULLET_MAX
} SETBULLET;

//==============================================================================
// 弾の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動量
	int nLife;			// 寿命
	bool bUse;			// 使用しているかどうか
	int nType;			// 弾の種類
	BULLETTYPE type;	// 敵味方の種類
} BULLET;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBullet(void);		// 弾の初期化処理
void UninitBullet(void);		// 弾の終了処理
void UpdateBullet(void);		// 弾の更新処理
void DrawBullet(void);			// 弾の描画処理
void SetBullet(
	D3DXVECTOR3 pos, 
	D3DXVECTOR3 move, 
	int nLife,
	BULLETTYPE type);			// 弾の設定
void HitBullet(void);			// 弾の命中
void SetVertexBullet(int nIdx);	// 弾の頂点座標取得
SETBULLET GetState(void);		// 弾の状態情報取得

#endif