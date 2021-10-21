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
#define MAX_OBJECT		(128)	// オブジェクトの最大数
#define MAX_TYPE_OBJECT	(32)	// テクスチャ数の最大
#define MAX_TEX_OBJECT	(16)
#define COLLISION_PARTS	(4)		// 当たり判定の面の数

// オブジェクト種類の列挙
//typedef enum
//{
//	OBJECTTYPE_BALOON = 0,
//	OBJECTTYPE_BLOCK,
//	OBJECTTYPE_MAX
//}OBJECTTYPE;

// オブジェクトの種類ごとの構造体
typedef struct
{
	LPD3DXMESH pMeshObject;
	LPD3DXBUFFER pBuffMatObject;	// マテリアル(材質情報)へのポインタ
	DWORD nNumMatObject;
	LPDIRECT3DTEXTURE9 pTextureObject[MAX_TEX_OBJECT];
	char aFileName[64];
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// モデルの各座標の最大値・最小値
} ObjectType;

// オブジェクトの構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotDest;
	int nIdx;
	D3DXVECTOR3 aPos[COLLISION_PARTS];			// オブジェクト4頂点
	D3DXVECTOR3 aVec[COLLISION_PARTS];			// オブジェクト4辺のベクトル
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;		// モデルの各座標の最大値・最小値
	float fPlayerVec[COLLISION_PARTS];			// 当たり判定用変数
	bool bBreak;
	int nType;									// オブジェクトの種類
	bool bUse;
} ObjectInfo;

typedef struct
{
	ObjectInfo objectInfo[MAX_OBJECT];
	ObjectType type[MAX_TYPE_OBJECT];
	int nNumType;
} Object;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitObject(void);	// 3Dオブジェクトの初期化処理
void UninitObject(void);	// 3Dオブジェクトの終了処理
void UpdateObject(void);	// 3Dオブジェクトの更新処理
void DrawObject(void);		// 3Dオブジェクトの描画処理
Object *GetObject(void);	// 3Dオブジェクトの取得
void StageObject(int nStage);	// ステージごとのオブジェクト配置

void SetObject(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType,bool bBreak);

bool CollisionVec(
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fInRadius,
	float fHeight,
	int nCntPlayer);

void LoadObject(void);

#endif