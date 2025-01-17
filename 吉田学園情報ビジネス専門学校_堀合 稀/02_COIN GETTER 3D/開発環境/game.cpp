//==============================================================================
//
// ゲーム画面の描画〔game.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "player.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "shadow.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "bullet.h"
#include "object.h"
//#include "bg.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "item.h"
#include "pause.h"
#include "life.h"
#include "particle.h"
#include "timer.h"
#include "gamepad.h"
#include "nodamage.h"
#include "start.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

//==============================================================================
// マクロ定義
//==============================================================================
//#define VERTICAL_STEP	(90)	// 縦移動するブロックのカウント間隔
//#define SIDE_STEP		(240)	// 横移動するブロックのカウント間隔
//#define BLOCKX			(46)	// ブロック配置のcsvファイルの列数
//#define BLOCKY			(24)	// ブロック配置のcsvファイルの行数

//==============================================================================
// グローバル変数
//==============================================================================
bool g_bPause;				// ポーズの状態
bool g_bGamebgm;	// ゲームBGMのON・OFF

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
	//InitBg();

	// タイマーの初期化処理
	InitTimer();

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 影の初期化処理
	InitShadow();

	// アイテムの初期化処理
	InitItem();

	// プレイヤーの初期化処理
	InitPlayer();

	InitObject();

	InitEnemy();

	// メッシュフィールドの初期化
	InitMeshfield();

	// メッシュ壁の初期化処理
	InitMeshwall();

	//// ビルボードの初期化
	//InitBillboard();

	// 弾の初期化
	InitBullet();

	// エフェクトの初期化処理
	InitParticle();

	// スコアの初期化処理
	InitScore();

	// ポーズメニューの初期化処理
	InitPause();

	// 体力表示の初期化処理
	InitLife();

	// UIの初期化設定
	InitUI();

	// ノーダメージボーナスの初期化処理
	InitNodamage();

	// ゲーム開始の初期化処理
	InitStart();

	g_bGamebgm = GAME_BGM;	// ゲームBGMのON・OFF

	return S_OK;
}

//==============================================================================
// ゲーム画面の終了処理
//==============================================================================
void UninitGame(void)
{
	// サウンドの停止処理
	StopSound();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// ポリゴンの終了処理
	UninitObject();

	UninitItem();

	// メッシュフィールドの終了処理
	UninitMeshfield();


	// メッシュ壁の終了処理
	UninitMeshwall();

	// 背景ポリゴンの終了処理
	//UninitBg();

	// プレイヤーの終了処理
	UninitPlayer();

	// ビルボードの終了処理
	//UninitBillboard();

	// 弾の終了処理
	UninitBullet();

	// 影の終了処理
	UninitShadow();

	// 敵の終了処理
	UninitEnemy();

	// エフェクトの終了処理
	UninitParticle();

	// スコアの終了処理
	UninitScore();

	// タイマーの終了処理
	UninitTimer();

	// ポーズメニューの終了処理
	UninitPause();

	// 体力表示の終了処理
	UninitLife();

	// UIの終了処理
	UninitUI();

	// ノーダメージボーナスの終了処理
	UninitNodamage();

	// ゲーム開始の終了処理
	UninitStart();
}

//==============================================================================
// ゲーム画面の更新処理
//==============================================================================
void UpdateGame(void)
{
	// ローカル変数宣言
	Player *pPlayer;
	PAUSE pause;
	FADE fade;
	int nTime = GetTimer();
	Start start = GetStart();

	// プレイヤーの取得
	pPlayer = GetPlayer();

	// ポーズの取得
	pause = GetPause();

	// フェードの取得
	fade = GetFade();

	// ゲーム開始の更新処理
	UpdateStart();

	// メッシュ壁の更新処理
	UpdateMeshwall();

	// ゲーム開始の可否
	if (start.bStart == true)
	{
		// BGMの再生処理
		if (g_bGamebgm == true)
		{
			// BGMの再生
			PlaySound(SOUND_LABEL_BGM_GAME);
			g_bGamebgm = false;
		}

		// ポーズメニューへ移行
		if (pPlayer->state != PLAYERSTATE_CLEAR && pPlayer->state != PLAYERSTATE_GAMEOVER && pause.colOption.a <= 0.0f && fade == FADE_NONE)
		{
			if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || IsButtonDown(KEYINFO::KEYINFO_PAUSE) == true)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_DECIDE);

				g_bPause = g_bPause ? false : true;	// ポーズの切り替え
			}
		}

		if (g_bPause == true)
		{ // ポーズ時の設定
			UpdatePause();	// ポーズの更新処理

			if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
			{ // 選択時の画面遷移処理
				if (fade != FADE_OUT && pause.colOption.a <= 0.0f)
				{ // 選択処理の重複防止
				  // サウンドの再生
					PlaySound(SOUND_LABEL_SE_DECIDE);
				}

				if (pause.PauseMenu == PAUSE_MENU_CONTINUE)
				{ // CONTINUEの時
					g_bPause = false;				// ポーズの解除(ゲームの続行)
				}
				else if (pause.PauseMenu == PAUSE_MENU_RETRY)
				{ // RETRYの時
					SetFade(FADE_OUT, MODE_GAME);	// ゲームのやり直し
													//g_nGameCnt = START_NUM;
				}
				else if (pause.PauseMenu == PAUSE_MENU_QUIT)
				{ // QUITの時
					SetFade(FADE_OUT, MODE_TITLE);	// タイトルへ移行
				}
			}
		}
		else
		{ // 非ポーズ時の処理
			if (pPlayer->state != PLAYERSTATE_CLEAR && pPlayer->state != PLAYERSTATE_GAMEOVER)
			{ // ゲーム中の処理
			  // 背景ポリゴンの更新処理
			  //UpdateBg();
				// 体力の更新処理
				UpdateLife();
				// 敵の更新処理
				UpdateEnemy();

				// カメラの更新処理
				UpdateCamera();

				// ライトの更新処理
				UpdateLight();

				// ビルボードの更新処理
				//UpdateBillboard();

				// 弾の更新処理
				UpdateBullet();

				// プレイヤーの更新処理
				UpdatePlayer();

				// メッシュフィールドの更新処理
				UpdateMeshfield();

				//// メッシュ壁の更新処理
				//UpdateMeshwall();

				UpdateObject();

				UpdateItem();

				// 影の更新処理
				UpdateShadow();
			}

			// エフェクトの更新処理
			UpdateParticle();

			if (pPlayer->state != PLAYERSTATE_GAMEOVER)
			{
				// スコアの更新処理
				UpdateScore();

				// タイマーの更新処理
				UpdateTimer();

				// ノーダメージボーナスの更新処理
				UpdateNodamage();
			}
	}
	
		// 画面の遷移（ゲームクリア時）
		if (nTime <= 0 && pPlayer->state == PLAYERSTATE_CLEAR)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// ゲームモード続行
		}
		//else if(nTime <= 0 && g_nGameCnt == MAX_STAGE)
		//{
		//	SetFade(FADE_OUT, MODE_RESULT);	// リザルトへ
		//}
		
		// 画面の遷移（ゲームオーバー時）
		if (pPlayer->state == PLAYERSTATE_GAMEOVER)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// リザルトへ
		}
	}
}

//==============================================================================
// ゲーム画面の描画処理
//==============================================================================
void DrawGame(void)
{
	//// 背景ポリゴンの描画処理
	//DrawBg();

	//// 敵の描画処理
	DrawEnemy();

	// メッシュフィールドの描画処理
	DrawMeshfield();

	// メッシュ壁の描画処理
	DrawMeshwall();

	// モデルの描画処理
	DrawPlayer();

	DrawObject();

	DrawItem();

	// 影の描画処理
	DrawShadow();

	// 弾の描画処理
	DrawBullet();

	// ビルボードの描画処理
	//DrawBillboard();

	// エフェクトの更新処理
	DrawParticle();

	// UIの描画処理
	DrawUI();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();

	// 体力表示の描画処理
	DrawLife();

	// ノーダメージボーナスの描画処理
	DrawNodamage();

	// ゲーム開始の描画処理
	DrawStart();

	// ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//==============================================================================
// ゲームステージカウントの初期化
//==============================================================================
//HRESULT InitGameCnt(void)
//{
//	//// 変数の初期化
//	//g_nGameCnt = 0;
//
//	//return S_OK;
//}