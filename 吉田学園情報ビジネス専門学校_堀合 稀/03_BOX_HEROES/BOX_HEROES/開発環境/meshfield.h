//==============================================================================
//
// メッシュフィールドの描画〔meshfield.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_
#include "main.h"

// メッシュフィールドの構造体
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 角度
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	bool bUse;				// 使用しているか
	int nAllPoint;			// 総頂点数
	int nPolygon;			// ポリゴン数
	int nIdxPoint;			// インデックスバッファの必要な確保数
	float fWidth;			// ポリゴン一つあたりの幅
	float fDepth;			// ポリゴン一つあたりの奥行
	int nWidth;				// 横幅の頂点数
	int nDepth;				// 奥行の頂点数
	float fWidthMax;		// 横幅の最大
	float fDepthMax;		// 奥行の最大
}MeshField;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitMeshfield(void);	// 3Dポリゴンの初期化処理
void UninitMeshfield(void);		// 3Dポリゴンの終了処理
void UpdateMeshfield(void);		// 3Dポリゴンの更新処理
void DrawMeshfield(void);		// 3Dポリゴンの描画処理
void SetField(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	int nWidth,
	int nDepth,
	float fWidth,
	float fDepth);

void SetTex(int nStage);

#endif
