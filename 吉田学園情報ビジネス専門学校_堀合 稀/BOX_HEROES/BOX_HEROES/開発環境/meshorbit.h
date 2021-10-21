//==============================================================================
//
// メッシュ軌跡の描画〔meshorbit.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ORBIT	(2)		// 軌跡の最大数
#define MESH_MAIN	(2)		// 基準頂点数
#define MESHPOINT	(20)	// 頂点最大数

//==============================================================================
// メッシュ構造軌跡の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 rot;						// 角度
	//D3DXMATRIX mtxWorld;					// ワールドマトリックス
	//bool bUse;							// 使用しているか

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;		// 頂点バッファへのポインタ
	D3DXMATRIX *pMtxParent;					// 親のマトリックスのポインタ
	D3DXVECTOR3 aOffset[MESH_MAIN];			// 両端のオフセット
	D3DXCOLOR aCol[MESH_MAIN];				// 両端の基準の色
	D3DXMATRIX aMtxWorldPoint[MESH_MAIN];	// 両端のワールドマトリックス
	D3DXVECTOR3 aPosPoint[MESHPOINT];		// 両端の各頂点座標
	D3DXCOLOR aColPoint[MESHPOINT];			// 両端の各頂点カラー
}MeshOrbit;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitMeshOrbit(void);	// メッシュ構造の軌跡の初期化処理
void UninitMeshOrbit(void);		// メッシュ構造の軌跡の終了処理
void UpdateMeshOrbit(void);		// メッシュ構造の軌跡の更新処理
void DrawMeshOrbit(void);		// メッシュ構造の軌跡の描画処理
void SetMeshOrbit(				// メッシュ構造の軌跡の設定
	D3DXVECTOR3 pos0,			// オフセット0の位置
	D3DXVECTOR3 pos1,			// オフセット1の位置
	D3DXCOLOR col0,				// 基準色0
	D3DXCOLOR col1);			// 基準色1
MeshOrbit GetOrbit(void);

#endif