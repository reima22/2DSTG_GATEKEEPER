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
	SOUND_LABEL_SE_SHOT000,		// 弾発射音(敵機)
	SOUND_LABEL_SE_SHOT001,		// 弾発射音(弾)
	SOUND_LABEL_SE_SHOT002,		// 弾発射音(ウェーブ)
	SOUND_LABEL_SE_SHOT003,		// 弾発射音(ミサイル)
	SOUND_LABEL_SE_HIT000,			// ヒット音
	SOUND_LABEL_SE_HIT001,			// ヒット音
	SOUND_LABEL_SE_EXPLOSION000,	// 爆発音0
	SOUND_LABEL_SE_EXPLOSION001,	// 爆発音1
	SOUND_LABEL_SE_EXPLOSION002,	// 爆発音2
	SOUND_LABEL_SE_EXPLOSION003,	// 爆発音3
	SOUND_LABEL_SE_START000,	// 決定
	SOUND_LABEL_SE_ITEM000,		// アイテム取得
	SOUND_LABEL_SE_ITEM001,		// 武装切り替え
	SOUND_LABEL_SE_WARNING,		// 警告
	SOUND_LABEL_SE_LINEOVER,		// 警告
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
