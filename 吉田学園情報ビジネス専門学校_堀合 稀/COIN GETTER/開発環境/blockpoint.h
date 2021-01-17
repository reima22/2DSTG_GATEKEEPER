//==============================================================================
//
// ブロック移動点の描画〔blockpoint.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _BLOCKPOINT_H_
#define _BLOCKPOINT_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define POINT_MAX	(64)	// 移動点の最大数
#define POINT_SIZEX	(30)	// 移動点の幅
#define POINT_SIZEY	(30)	// 移動点の高さ

//==============================================================================
// ブロック移動点の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	float fWidth;		// 幅
	float fHeight;		// 高さ
	bool bUse;			// 使用しているかどうか
} BLOCKPOINT;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBlockPoint(void);	// ブロック移動点の初期化処理
void UninitBlockPoint(void);	// ブロック移動点の終了処理
void UpdateBlockPoint(void);	// ブロック移動点の更新処理
void DrawBlockPoint(void);		// ブロック移動点の描画処理
BLOCKPOINT *GetPoint(void);		// 移動点の取得
void SetPoint(					// 移動点の設定
	D3DXVECTOR3 pos,			// 位置
	float fWidth,				// 幅
	float fHeight);				// 高さ
void SetVertexPoint(int nIdx);	// 移動点の頂点座標取得

#endif