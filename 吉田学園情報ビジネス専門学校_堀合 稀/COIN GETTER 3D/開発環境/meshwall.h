//==============================================================================
//
// メッシュ壁の描画〔meshwall.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _MESHWALL_H_
#define _MESHWALL_H_
#include "main.h"

// 壁の構造体
typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;		// 初期位置
	D3DXVECTOR3	posMove;	// 初期位置からの移動した位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fWidthMax;
	float fHeightMax;
	int nWidth;				// 列数
	int nHeight;			// 行数
	int nWidthPoint;		// 幅の頂点数
	int nHeightPoint;		// 高さの頂点数
	int nAllPoint;
	int nPolygon;
	int nIdxPoint;
	bool bUse;				// 使用状態
} Meshwall;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitMeshwall(void);		// メッシュ壁の初期化処理
void UninitMeshwall(void);		// メッシュ壁の終了処理
void UpdateMeshwall(void);		// メッシュ壁の更新処理
void DrawMeshwall(void);		// メッシュ壁の描画処理
void SetMeshwall(				// メッシュ壁の設定処理
	D3DXVECTOR3 pos, 
	D3DXVECTOR3 rot, 
	float fWidth, 
	float fHeight,
	int nWidth,
	int nHeight);			

#endif
