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
#define PLAYER_SIZEX	(20)	// プレイヤーの幅
#define PLAYER_FUTSIZEX	(10)	// プレイヤーの足元の幅
#define PLAYER_SIZEY	(60)	// プレイヤーの高さ

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
	PLAYERSTATE_CLEAR,		// ゲームクリア
	PLAYERSTATE_GAMEOVER,	// ゲームオーバー
	PLAYERSTATE_MAX
} PLAYERSTATE;

//==============================================================================
// プレイヤーの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;			// 現在の位置
	D3DXVECTOR3 posOld;			// 前回の位置
	D3DXVECTOR3 move;			// プレイヤーの移動量
	float fWidth;				// 幅
	float fHeight;				// 高さ
	int nLife;					// プレイヤーの体力
	int nStock;					// プレイヤーの残機
	PLAYERSTATE state;			// プレイヤーの状態
	int nCounterState;			// 状態変化中のカウント
	int nCounterDisp;			// 点滅状態のカウント
	D3DXCOLOR colorPlayer;		// プレイヤーの色・透明度
	int nCntAnime;				// アニメーションカウント
	bool bRight;				// 左右の向き判定
	float fAnime;				// プレイヤーのアニメーション遷移
	bool bJump;					// 空中にいるかどうか
	int nGetCoin;				// コインの獲得数
	int nTrueCoin;				// クリアに必要なコイン数
} PLAYER;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPlayer(void);				// プレイヤーの初期化処理
void UninitPlayer(void);				// プレイヤーの終了処理
void UpdatePlayer(void);				// プレイヤーの更新処理
void DrawPlayer(void);					// プレイヤーの描画処理
PLAYER *GetPlayer(void);				// プレイヤーの情報の取得
void ItemGet(void);						// アイテムとの当たり判定
bool HitPlayer(int nDamage);			// 被ダメージの処理
void TouchEnemy(void);					// 敵との接触の処理
void MovePlayer(void);					// プレイヤーの移動
void PlayerState(PLAYERSTATE state);	// プレイヤーの状態
void PlayerAnim(void);					// プレイヤーのテクスチャアニメーション
void SetPlayer(void);					// ステージごとのプレイヤーの初期配置

#endif