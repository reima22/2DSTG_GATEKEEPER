//==============================================================================
//
// プレイヤーの描画〔player.h〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define PATTERNANIM		(5.0f)	// アニメーションパターン数
#define BULLET_PLUS		(3)		// 追加弾の数
#define PLAYER_LIFE		(3)		// プレイヤーのライフ値
#define PLAYER_STOCK	(2)		// 初期残機数
#define PLAYER_SIZEX	(20)	// プレイヤーの幅
#define PLAYER_SIZEY	(20)	// プレイヤーの高さ

//==============================================================================
// プレイヤーの状態
//==============================================================================
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	// 出現
	PLAYERSTATE_NORMAL,		// 通常
	PLAYERSTATE_DAMAGE,		// ダメージ
	PLAYERSTATE_WAIT,		// 出現待ち
	PLAYERSTATE_DEATH,		// 死亡
	PLAYERSTATE_GAMEOVER,	// ゲームオーバー
	PLAYERSTATE_MAX
} PLAYERSTATE;

//==============================================================================
// プレイヤーのステータス
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXCOLOR colorPlayer;		// 色
	int nLife;					// 体力
	PLAYERSTATE state;			// 状態
	int nCounterState;			// 状態カウント
	int nCounterDisp;			// 点滅カウント
	int nStock;					// 残機
	bool aBullet[BULLET_PLUS];	// 追加弾の保有状態
	int nLineOver;				// ライン越えカウント
	D3DXVECTOR3 rot;			// プレイヤーの回転角
	float fLength;				// プレイヤーの対角線の長さ
	float fAngle;				// プレイヤーの対角線の角度
	D3DXVECTOR3 move;			// プレイヤーの移動量
} PLAYER;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPlayer(void);		// プレイヤーの初期化処理
void UninitPlayer(void);		// プレイヤーの終了処理
void UpdatePlayer(void);		// プレイヤーの更新処理
void DrawPlayer(void);			// プレイヤーの描画処理
PLAYER *GetPlayer(void);		// プレイヤーの情報の取得
bool HitPlayer(int nDamage);	// プレイヤーの当たり判定
void TouchEnemy(void);			// 敵との接触
void ItemGet(void);				// アイテムの入手
void PlayerMove(void);			// プレイヤーの行動

#endif