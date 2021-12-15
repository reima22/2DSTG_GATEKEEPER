//==============================================================================
//
// ブロックの描画〔block.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define BLOCK_MAX	(256)	// ブロックの最大個数
#define BLOCK_SIZEX	(30)	// ブロックの幅
#define BLOCK_SIZEY	(30)	// ブロックの高さ

//==============================================================================
// ブロックに触れているものの種類
//==============================================================================
typedef enum
{
	ONTYPE_NONE,	// 何も乗っていない
	ONTYPE_PLAYER,	// プレイヤー
	ONTYPE_ENEMY,	// 敵
	ONTYPE_MAX
}ONTYPE;

//==============================================================================
// ブロックの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bUse;			// 使用しているかどうか
	D3DXVECTOR3 move;	// 移動力
	ONTYPE type;		// 接触しているものの種類
	int nMoveCnt;		// ブロックの移動カウント
	int nMaxCnt;		// 移動カウントの上限
} BLOCK;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBlock(void);		// ブロックの初期化処理
void UninitBlock(void);			// ブロックの終了処理
void UpdateBlock(void);			// ブロックの更新処理
void DrawBlock(void);			// ブロックの描画処理
void SetBlock(					// ブロックの設定
	D3DXVECTOR3 pos,			// 位置
	D3DXVECTOR3 move,			// 移動量
	float fWidth,				// 幅
	float fHeight,				// 高さ
	int nMaxCnt);				// カウントの上限
void SetVertexBlock(int nIdx);	// ブロックの頂点座標取得
BLOCK *GetBlock(void);			// ブロックの情報取得
bool CollisionBlock(			// ブロックの当たり判定の設定
	D3DXVECTOR3 *pPos,			// 現在の位置
	D3DXVECTOR3 *pPosOld,		// 前回の位置
	D3DXVECTOR3 *pMove,			// 移動量
	float fWidth,				// 幅
	float fHeight,				// 高さ
	ONTYPE type);				// ブロック触れているものの種類

#endif