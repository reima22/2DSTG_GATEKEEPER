//==============================================================================
//
// 3Dモデルの描画〔object.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

// マクロ定義
#define COLLISION_PARTS	(4)	// 当たり判定の面の数

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
	D3DXVECTOR3 aPos[COLLISION_PARTS];
	D3DXVECTOR3 aVec[COLLISION_PARTS];
	float fPlayerVec[COLLISION_PARTS];
} Object;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitObject(void);	// 3Dオブジェクトの初期化処理
void UninitObject(void);	// 3Dオブジェクトの終了処理
void UpdateObject(void);	// 3Dオブジェクトの更新処理
void DrawObject(void);		// 3Dオブジェクトの描画処理
Object *GetObject(void);	// 3Dオブジェクトの取得
bool CollisionObject(		// 当たり判定
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fWidthMax,
	float fWidthMin,
	float fDepthMax,
	float fDepthMin);

bool CollisionVec(
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fWidthMax,
	float fWidthMin,
	float fDepthMax,
	float fDepthMin,
	float fHeightMax,
	float fheightMin);

#endif