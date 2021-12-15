//==============================================================================
//
// ゲーム画面の描画〔game.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "item.h"
#include "pause.h"
#include "counter.h"
#include "life.h"
#include "effect.h"
#include "particle.h"
#include "bullet_ui.h"
#include "line.h"
#include "stock.h"
#include "warning.h"
#include "gamepad.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define ENEMY_POSX			(50.0f)		// 敵の初期X座標
#define ENEMY_POSY			(-20.0f)	// 敵の初期Y座標

//==============================================================================
// グローバル変数
//==============================================================================
bool g_bPause;

//==============================================================================
// ゲーム画面の初期化処理
//==============================================================================
HRESULT InitGame(void)
{
	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// 変数の初期化
	g_bPause = false;

	// 背景ポリゴンの初期化設定
	InitBg();

	// ユーザーインターフェースの初期化設定
	InitUI();
	InitBulletUI();
	InitLine();
	InitStock();

	// プレイヤーの初期化処理
	InitPlayer();

	// 敵の初期化設定
	InitEnemy();

	// 敵の初期配置
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY, 0.0f), 0, 5);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 70.0f, 0.0f), 1, 8);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 140.0f, 0.0f), 2, 12);
		SetEnemy(D3DXVECTOR3(ENEMY_POSX + ((float)nCnt * 90.0f), ENEMY_POSY - 210.0f, 0.0f), 3, 15);
	}

	// アイテムの初期化処理
	InitItem();

	// 弾の初期化設定
	InitBullet();

	// 爆発の初期化処理
	InitExplosion();

	// エフェクトの初期化処理
	InitParticle();

	// スコアの初期化処理
	InitScore();

	// カウンターの初期化処理
	InitCounter();

	// ポーズメニューの初期化処理
	InitPause();

	// 体力表示の初期化処理
	InitLife();

	// 警告画面の初期化設定
	InitWarning();

	// BGMの再生
	PlaySound(SOUND_LABEL_BGM001);

	return S_OK;
}

//==============================================================================
// ゲーム画面の終了処理
//==============================================================================
void UninitGame(void)
{
	// サウンドの停止処理
	StopSound();

	// 背景ポリゴンの終了処理
	UninitBg();

	// ユーザーインターフェースの終了処理
	UninitUI();
	UninitBulletUI();
	UninitLine();
	UninitStock();

	// 弾の終了処理
	UninitBullet();

	// プレイヤーの終了処理
	UninitPlayer();

	// 敵の終了処理
	UninitEnemy();

	// アイテムの終了処理
	UninitItem();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitParticle();

	// スコアの終了処理
	UninitScore();

	// カウンターの終了処理
	UninitCounter();

	// ポーズメニューの終了処理
	UninitPause();

	// 体力表示の終了処理
	UninitLife();

	// 警告画面の終了処理
	UninitWarning();
}

//==============================================================================
// ゲーム画面の更新処理
//==============================================================================
void UpdateGame(void)
{
	// ローカル変数宣言
	PLAYER *Player;
	PAUSE_MENU pause;
	FADE fade;
	ENEMYINFO info = GetInfo();		// 敵の全体情報の取得
	PHASESTATE phase = GetPhase();	// 警戒状態の取得

	// プレイヤーの取得
	Player = GetPlayer();

	// ポーズの取得
	pause = GetPause();

	// フェードの取得
	fade = GetFade();

	// ポーズメニューへ移行
	if (info.nStartCnt >= OPEN_CNT)
	{
		if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || IsButtonDown(KEYINFO::KEYINFO_PAUSE) == true)
		{
			// 音の再生
			PlaySound(SOUND_LABEL_SE_START000);
			g_bPause = g_bPause ? false : true;	// ポーズの切り替え
		}
	}

	if (g_bPause == true)
	{ // ポーズ時の設定
		UpdatePause();	// ポーズの更新処理

		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{ // 選択時の処理
			if (fade != FADE_OUT)
			{ // 選択処理の重複防止
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_START000);
			}
			
			if (pause == PAUSE_MENU_CONTINUE)
			{ // CONTINUEの時
				g_bPause = false;				// ポーズの解除(ゲームの続行)
			}
			else if(pause == PAUSE_MENU_RETRY)
			{ // RETRYの時
				SetFade(FADE_OUT, MODE_GAME);	// ゲームのやり直し
			}
			else if (pause == PAUSE_MENU_QUIT)
			{ // QUITの時
				SetFade(FADE_OUT, MODE_TITLE);	// タイトルへ移行
			}
		}
	}
	else
	{ // 非ポーズ時の処理
		if (info.boss != BIGENEMY_DEATH && Player->state != PLAYERSTATE_GAMEOVER)
		{ // ゲーム中の処理
		  // 背景ポリゴンの更新処理
			UpdateBg();

			// ユーザーインターフェースの更新処理
			UpdateUI();
			UpdateBulletUI();
			UpdateLine();
			UpdateStock();

			// 弾の更新処理
			UpdateBullet();

			// 敵の更新処理
			UpdateEnemy();

			// アイテムの更新処理
			UpdateItem();

			// プレイヤー更新時の条件
			if (info.nStartCnt >= OPEN_CNT && phase == PHASESTATE_NORMAL)
			{
				// プレイヤーの更新処理
				UpdatePlayer();
			}

			// 爆発の更新処理
			UpdateExplosion();

			// エフェクトの更新処理
			UpdateParticle();

			// スコアの更新処理
			UpdateScore();

			// カウンターの更新処理
			UpdateCounter();

			// 体力表示の更新処理
			UpdateLife();

			// 警告画面の更新処理
			UpdateWarning();
		}

		// リザルト画面への遷移
		if (info.boss == BIGENEMY_DEATH || Player->state == PLAYERSTATE_GAMEOVER)
		{
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}

//==============================================================================
// ゲーム画面の描画処理
//==============================================================================
void DrawGame(void)
{
	// 背景ポリゴンの描画処理
	DrawBg();

	// 敵の描画処理
	DrawEnemy();

	// アイテムの描画処理
	DrawItem();

	// 弾の描画処理
	DrawBullet();

	// 爆発の描画処理
	DrawExplosion();

	// エフェクトの更新処理
	DrawParticle();
	
	// ポリゴンの描画処理
	DrawPlayer();

	// 警告画面の描画処理
	DrawWarning();

	// ユーザーインターフェースの描画処理
	DrawUI();
	DrawBulletUI();
	DrawLine();
	DrawStock();

	// スコアの描画処理
	DrawScore();

	// カウンターの描画処理
	DrawCounter();

	// 体力表示の描画処理
	DrawLife();

	// ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}