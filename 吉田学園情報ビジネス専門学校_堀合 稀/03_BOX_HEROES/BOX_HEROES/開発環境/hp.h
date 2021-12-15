//-----------------------------------------------------------------
//
// HPバーの処理 (hp.h)
// Author:shouta takahashi
//
//-----------------------------------------------------------------
#ifndef _HP_H_
#define _HP_H_
#include "main.h"

//-----------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------
#define MAXTEX_UI	(8)
#define MAX_TEX_UI	(4)					// テクスチャ最大数
#define UI_WIDTH	(640.0f)			// 幅
#define UI_HEIGHT	(100.0f)			// 高さ
#define UI_HARF_W	(UI_WIDTH / 2.0f)	// 幅サイズ半分
#define UI_HARF_H	(UI_HEIGHT / 2.0f)	// 高さサイズ半分
#define HP_LIMIT	(400.0f)			// 最大HP

//-----------------------------------------------------------------------------
// HPの割り当て
//-----------------------------------------------------------------------------
typedef enum
{
	PLAYER_0 = 0,	// プレイヤー1
	PLAYER_1,		// プレイヤー2
	PLAYER_MAX
}HP_Player;

//-----------------------------------------------------------------------------
// HPバーの構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 posFrame;
	D3DXVECTOR3 posBar;
	HP_Player hp;			// どちらのプレイヤーか
	float fBar;				// バーの表示率
}HP;

//===============
//プロトタイプ宣言
//===============
HRESULT InitHp(void);		// HPバーの初期化処理
void UninitHp(void);		// HPバーの終了処理
void UpdateHp(void);		// HPバーの更新処理
void DrawHp(void);			// HPバーの描画処理
void SetVertexHp(int nIdx);	// HPバーの頂点座標更新
void CalcuHp(				// HPの加減算処理
	int nHp,				// プレイヤーの種別
	float fValue,			// 数値
	bool bPlus);			// 加算・減算の区別
HP *GetHp(void);			// HP情報の取得

#endif
