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
	SOUND_LABEL_TITLE,			// BGM0
	SOUND_LABEL_SELECT,			// BGM1
	SOUND_LABEL_STAGE0,			// BGM3
	SOUND_LABEL_STAGE1,			// BGM4
	SOUND_LABEL_STAGE2,			// BGM5
	SOUND_LABEL_RESULT,			// BGM6
	SOUND_LABEL_SE_ITEM,			// アイテム取得
	SOUND_LABEL_SE_JUMP,			// ジャンプ
	SOUND_LABEL_SE_PAUSE_SELECT,	// ポーズカーソル移動
	SOUND_LABEL_SE_CANCEL,			// キャンセル音
	SOUND_LABEL_SE_START,			// ゲームスタート
	SOUND_LABEL_SE_DECIDE,			// 決定
	SOUND_LABEL_SE_CURSOR,			// カーソル移動
	SOUND_LABEL_SE_SMASH,			// 打撃音
	SOUND_LABEL_SE_SLASH,			// 斬撃音
	SOUND_LABEL_SE_SHOT,			// 射撃音
	SOUND_LABEL_SE_EXPLOSION,		// 爆発音
	SOUND_LABEL_SE_SMASH_HIT,		// 打撃ヒット音
	SOUND_LABEL_SE_SLASH_HIT,		// 斬撃ヒット音
	SOUND_LABEL_SE_SHOT_HIT,		// 射撃ヒット音
	SOUND_LABEL_SE_EXPLOSION_HIT,	// 爆撃ヒット音
	SOUND_LABEL_SE_SETITEM,
	SOUND_LABEL_SE_COUNTDOWN,		// カウントダウン
	SOUND_LABEL_SE_GAMESTART,		// カウントダウン
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
