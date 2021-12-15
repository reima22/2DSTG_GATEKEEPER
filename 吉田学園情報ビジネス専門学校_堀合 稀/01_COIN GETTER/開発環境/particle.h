//==============================================================================
//
// パーティクルの描画〔particle.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitParticle(void);		// パーティクルの初期化処理
void UninitParticle(void);		// パーティクルの終了処理
void UpdateParticle(void);		// パーティクルの更新処理
void DrawParticle(void);		// パーティクルの描画処理
void SetEffect(					// エフェクトの設定
	D3DXVECTOR3 pos,			// 位置
	float fMove,				// 移動量
	D3DXCOLOR color,			// 色・透明度
	float fRadius,				// 半径
	float fAlphaDown,			// 透過度
	int nEffect);				// エフェクトの個数

#endif