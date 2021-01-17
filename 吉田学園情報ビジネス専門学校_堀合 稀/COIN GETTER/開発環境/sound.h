//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000,			// BGM0
	SOUND_LABEL_BGM001,			// BGM1
	SOUND_LABEL_BGM002,			// BGM2
	SOUND_LABEL_BGM003,			// BGM3
	SOUND_LABEL_BGM004,			// BGM4
	SOUND_LABEL_BGM005,			// BGM5
	SOUND_LABEL_BGM006,			// BGM6
	SOUND_LABEL_SE_HIT000,			// ヒット音
	SOUND_LABEL_SE_HIT001,			// ヒット音
	SOUND_LABEL_SE_START000,		// 決定
	SOUND_LABEL_SE_COIN,			// コイン取得
	SOUND_LABEL_SE_JUMP,			// ジャンプ
	SOUND_LABEL_SE_PAUSE_SELECT,	// ポーズカーソル移動
	SOUND_LABEL_SE_STOCK_LOST,		// 残機減少
	SOUND_LABEL_SE_SCORE_UP,		// スコア上昇
	SOUND_LABEL_SE_HIGHCOIN,		// ハイスコアコイン獲得
	SOUND_LABEL_SE_NODAMAGE,		// ノーダメージボーナス獲得
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
