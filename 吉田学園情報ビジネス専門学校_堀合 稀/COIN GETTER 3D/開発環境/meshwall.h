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

// マクロ定義
#define MAX_MWALL		(4)
#define MAX_WALL_TYPE	(4)

// 壁の種類ごとの情報
typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureMeshwall = {};	// テクスチャへのポインタ
	char aFileName[TEXT_LENGTH];				// テキスト読み取り用変数
}WallType;

// 壁の詳細情報
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
	D3DXVECTOR3 posPoint[FOUR_POINT];	// 4頂点の位置
	D3DXVECTOR3 vecPoint[FOUR_POINT];	// 四辺のベクトル
	int nType;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuffMeshwall = NULL;	// バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuffMeshwall = NULL;		// インデックスバッファへのポインタ
}WallInfo;

// 壁の構造体
typedef struct
{
	WallInfo wallInfo[MAX_MWALL];		// 壁の詳細情報
	WallType wallType[MAX_WALL_TYPE];	// 壁の種類ごとの情報
	int nNumTex;						// テクスチャの数
	int nNumWall;						// 配置する壁の数

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

void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 move, float fWidthMax, float fWidthMin, float fDepthMax, float fDepthMin, float fHeightMax, float fHeightMin);

void LoadWall(void);		// 壁のテキスト読み込み

#endif
