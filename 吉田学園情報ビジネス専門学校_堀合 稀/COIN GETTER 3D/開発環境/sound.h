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
	SOUND_LABEL_BGM_TITLE,			// タイトルBGM
	SOUND_LABEL_BGM_GAME,			// ゲームBGM
	SOUND_LABEL_BGM_RESULT_CLEAR,	// ゲームクリアリザルトBGM
	SOUND_LABEL_BGM_RESULT_FAILED,	// ゲームオーバーリザルトBGM
	SOUND_LABEL_BGM_RANKING,		// ランキングBGM
	SOUND_LABEL_SE_DAMAGE,			// 被ダメージ音
	SOUND_LABEL_SE_DECIDE,			// 決定
	SOUND_LABEL_SE_COIN,			// コイン取得
	SOUND_LABEL_SE_JUMP,			// ジャンプ
	SOUND_LABEL_SE_PAUSE_SELECT,	// ポーズカーソル移動
	SOUND_LABEL_SE_SCORE_UP,		// スコア上昇
	SOUND_LABEL_SE_NODAMAGE,		// ノーダメージボーナス獲得
	SOUND_LABEL_SE_STOMP,			// 敵を踏む音
	SOUND_LABEL_SE_RANKIN,			// ランクイン音
	SOUND_LABEL_SE_COUNTDOWN,		// スタートカウントダウン音
	SOUND_LABEL_SE_GAMESTART,		// ゲーム開始音
	SOUND_LABEL_SE_HIGHSCORE,		// ハイスコアアイテム獲得音
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
