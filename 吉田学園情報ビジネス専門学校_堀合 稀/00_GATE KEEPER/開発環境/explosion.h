//==============================================================================
//
// 爆発の描画〔explosion.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitExplosion(void);		// 爆発の初期化処理
void UninitExplosion(void);			// 爆発の終了処理
void UpdateExplosion(void);			// 爆発の更新処理
void DrawExplosion(void);			// 爆発の描画処理
void SetExplosion(D3DXVECTOR3 pos);	// 爆発の設定

#endif