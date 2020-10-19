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
void SetEffect(
	D3DXVECTOR3 pos, 
	float fMove,  
	D3DXCOLOR color,
	float fRadius,
	float fAlphaDown);			// エフェクトの設定

#endif