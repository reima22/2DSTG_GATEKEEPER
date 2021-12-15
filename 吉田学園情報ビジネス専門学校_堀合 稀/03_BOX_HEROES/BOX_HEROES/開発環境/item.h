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
#define MAX_ITEM		(8)		// アイテムの最大数


typedef enum
{
	ITEMTYPE_KNIFE= 0,		// ナイフ
	ITEMTYPE_HANDGUN,		// ハンドガン
	ITEMTYPE_MACHINEGUN,	// マシンガン
	ITEMTYPE_RANCHER,		// ロケットランチャー
	ITEMTYPE_MAX
}ItemType;

typedef struct
{
	LPD3DXMESH pMeshItem;
	LPD3DXBUFFER pBuffMatItem;
	DWORD nNumMatItem;
	char aFileName[64];
	D3DXVECTOR3 vtxMinItem, vtxMaxItem;
}ItemTypeInfo;

//==============================================================================
// アイテムの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 rot;						// 角度
	D3DXMATRIX mtxWorld;					// ワールドマトリックス
	D3DXCOLOR col;							// 色
	ItemType type;
	bool bUse;								// 使用しているかどうか
	D3DXVECTOR3 vtxMinItem, vtxMaxItem;		// モデルの各座標の最大値・最小値
	D3DXVECTOR3 posPoint[FOUR_POINT];		// 4頂点の位置
	D3DXVECTOR3 vecPoint[FOUR_POINT];		// 四辺のベクトル
	bool bSet;
	int nIdx;
} ItemInfo;

typedef struct
{
	ItemInfo iteminfo[MAX_ITEM];
	ItemTypeInfo typeinfo[ITEMTYPE_MAX];
	int nNumType;
	int nSetItem;
	int nSetCnt;
}Item;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitItem(void);			// アイテムの初期化処理
void UninitItem(void);			// アイテムの終了処理
void UpdateItem(void);			// アイテムの更新処理
void DrawItem(void);			// アイテムの描画処理
void SetItem(D3DXVECTOR3 pos,int nType);	// アイテムの配置
void TouchItem(
	D3DXVECTOR3 *pPos,
	float fInRadius,
	float fHeight,
	int nCntPlayer);			// アイテムの取得
Item *GetItem(void);			// アイテムの情報
void RandItem(int nStage);				// ランダムアイテムの選定
void LoadItem(void);

#endif
