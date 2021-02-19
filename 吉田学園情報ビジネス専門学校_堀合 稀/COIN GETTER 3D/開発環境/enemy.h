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

// モデルの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// モデルの各座標の最大値・最小値
	int nIdx;
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4頂点の位置
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// 四辺のベクトル
	float fVec[4];						// 計算用ベクトル
	int nMovePattern;					// 移動パターン管理
	bool bUse;
} Enemy;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitEnemy(void);	// 敵の初期化処理
void UninitEnemy(void);		// 敵の終了処理
void UpdateEnemy(void);		// 敵の更新処理
void DrawEnemy(void);		// 敵の描画処理
Enemy *GetEnemy(void);		// 敵の取得
void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move,int nMove);
bool TouchEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

//bool CollisionObject(		// 当たり判定
//	D3DXVECTOR3 *pPos,
//	D3DXVECTOR3 *pPosOld,
//	D3DXVECTOR3 *pMove,
//	float fWidthMax,
//	float fWidthMin,
//	float fDepthMax,
//	float fDepthMin);
//
//bool CollisionVec(
//	D3DXVECTOR3 *pPos,
//	D3DXVECTOR3 *pPosOld,
//	D3DXVECTOR3 *pMove,
//	float fWidthMax,
//	float fWidthMin,
//	float fDepthMax,
//	float fDepthMin,
//	float fHeightMax,
//	float fheightMin);

#endif
