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

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_OBJECT		(32)	// オブジェクトの最大数
#define MAX_TEX_OBJECT	(4)		// テクスチャ数の最大
#define COLLISION_PARTS	(4)		// 当たり判定の面の数
#define MAX_OBJECT_TYPE	(4)		// オブジェクトの種類最大数

//==============================================================================
// オブジェクトの種類ごとの構造体
//==============================================================================
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX_OBJECT] = {};	// テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;							// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;						// マテリアル(材質情報)へのポインタ
	DWORD nNumMat = NULL;								// マテリアルの数
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;				// モデルの各座標の最大値・最小値
	char aFileName[TEXT_LENGTH];
}ObjectType;

//==============================================================================
// オブジェクトごとの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 角度
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 move;							// 移動力
	D3DXVECTOR3 rotDest;						// 目的の角度
	int nIdx;									// 丸影設定番号
	D3DXVECTOR3 aPos[COLLISION_PARTS];			// オブジェクト4頂点
	D3DXVECTOR3 aVec[COLLISION_PARTS];			// オブジェクト4辺のベクトル
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;		// モデルの各座標の最大値・最小値
	float fPlayerVec[COLLISION_PARTS];			// 当たり判定用変数
	int nType;									// オブジェクトの種類
	bool bUse;									// 使用状態
}ObjectInfo;

//==============================================================================
// オブジェクトの構造体
//==============================================================================
typedef struct
{
	ObjectInfo objectInfo[MAX_OBJECT];
	ObjectType objectType[MAX_OBJECT_TYPE];
	int nNumObject;
	int nSetObject;
}Object;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitObject(void);	// 3Dオブジェクトの初期化処理
void UninitObject(void);	// 3Dオブジェクトの終了処理
void UpdateObject(void);	// 3Dオブジェクトの更新処理
void DrawObject(void);		// 3Dオブジェクトの描画処理
Object *GetObject(void);	// 3Dオブジェクトの取得
void SetObject(				// 3Dオブジェクトの設定
	D3DXVECTOR3 pos,
	int nType,
	D3DXVECTOR3 rot);
bool CollisionVec(			// 3Dオブジェクトの当たり判定
	D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pPosOld,
	D3DXVECTOR3 *pMove,
	float fInRadius,
	float fHeight);
void LoadObject(void);		// オブジェクトの読み込み

#endif