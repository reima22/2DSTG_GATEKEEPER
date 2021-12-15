//==============================================================================
//
// アイテムの描画〔item.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ITEM		(128)		// アイテムの最大数
#define MAX_ITEM_TYPE	(8)			// アイテム種類の最大数
#define MAX_ITEM_TEX	(4)			// アイテムの最大テクスチャ数

//==============================================================================
// アイテムの種類ごとの情報
//==============================================================================
typedef struct
{
	LPDIRECT3DTEXTURE9 pTexture[MAX_ITEM_TEX] = {};	// テクスチャへのポインタ
	LPD3DXMESH pMesh = NULL;						// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;					// マテリアル(材質情報)へのポインタ
	DWORD nNumMat = 0;								// マテリアルの数
	char aFileName[TEXT_LENGTH];					// パス名読み取り用変数
	D3DXVECTOR3 vtxMinType, vtxMaxType;				// モデルの各座標の最大値・最小値
}ItemType;

//==============================================================================
// アイテムの詳細情報
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 角度
	D3DXMATRIX mtxWorld;				// ワールドマトリックス
	int nType;							// 種類
	bool bUse;							// 使用しているかどうか
	D3DXVECTOR3 vtxMinItem, vtxMaxItem;	// モデルの各座標の最大値・最小値
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4頂点の位置
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// 四辺のベクトル
	int nIdx;							// 影番号
	bool bHiScore;						// 高スコア状態のコイン
}ItemInfo;

//==============================================================================
// アイテムの構造体
//==============================================================================
typedef struct
{
	ItemInfo itemInfo[MAX_ITEM];		// アイテムの詳細情報
	ItemType itemType[MAX_ITEM_TYPE];	// アイテムの種類ごとの情報
	int nNumItem;						// アイテムの種類数
	int nSetItem;						// テキストからの配置数カウント
	int nCntItem;						// 配置数カウント
	int nCntAnim;						// アニメーションカウント
	D3DMATERIAL9 matHigh;				// ハイスコアアイテムのマテリアル
} Item;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitItem(void);			// アイテムの初期化処理
void UninitItem(void);			// アイテムの終了処理
void UpdateItem(void);			// アイテムの更新処理
void DrawItem(void);			// アイテムの描画処理
void SetItem(					// アイテムの配置
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int nType);					
void TouchItem(					// アイテムの取得処理
	D3DXVECTOR3 *pPos,
	float fInRadius,
	float fHeight);				
Item *GetItem(void);			// アイテムの情報
int RandItem(void);				// ランダムでハイスコアアイテムの選定
void LoadItem(void);			// アイテムの読み込み

#endif
