//==============================================================================
//
// ゲーム画面の描画〔game.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "player.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "bullet.h"
#include "object.h"
//#include "bg.h"
//#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "ui.h"
#include "fade.h"
#include "item.h"
#include "pause.h"
#include "life.h"
#include "particle.h"
//#include "block.h"
#include "timer.h"
#include "gamepad.h"
//#include "nodamage.h"
#include "time.h"
#include "stdlib.h"
#include "stdio.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define VERTICAL_STEP	(90)	// 縦移動するブロックのカウント間隔
#define SIDE_STEP		(240)	// 横移動するブロックのカウント間隔
#define BLOCKX			(46)	// ブロック配置のcsvファイルの列数
#define BLOCKY			(24)	// ブロック配置のcsvファイルの行数

//==============================================================================
// グローバル変数
//==============================================================================
bool g_bPause;				// ポーズの状態
int g_nGameCnt = START_NUM;	// ステージカウント
int g_nStartCnt;
int g_aMapData[BLOCKY][BLOCKX];

//==============================================================================
// ゲーム画面の初期化処理
//==============================================================================
HRESULT InitGame(void)
{
	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// 変数の初期化
	g_bPause = false;
	g_nStartCnt = g_nGameCnt;

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


	// ポリゴンの初期化処理
	InitPolygon();

	// メッシュフィールドの初期化
	InitMeshfield();

	// 壁の初期化処理
	InitWall();

	// メッシュ壁の初期化処理
	InitMeshwall();

	// ビルボードの初期化
	InitBillboard();

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
	//InitNodamage();

	// BGMの再生
	//if (g_nGameCnt == 0)
	//{
	//	PlaySound(SOUND_LABEL_BGM001);
	//}
	//else if (g_nGameCnt == 1)
	//{
	//	PlaySound(SOUND_LABEL_BGM005);	
	//}
	//else if (g_nGameCnt == 2)
	//{
	//	PlaySound(SOUND_LABEL_BGM006);
	//}

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
	UninitPolygon();

	UninitObject();

	UninitItem();

	// メッシュフィールドの終了処理
	UninitMeshfield();

	// 壁の終了処理
	UninitWall();

	// メッシュ壁の終了処理
	UninitMeshwall();

	// 背景ポリゴンの終了処理
	//UninitBg();

	// プレイヤーの終了処理
	UninitPlayer();

	// ビルボードの終了処理
	UninitBillboard();

	// 弾の終了処理
	UninitBullet();

	// 影の終了処理
	UninitShadow();

	// 敵の終了処理
	//UninitEnemy();
	// アイテムの終了処理
	//UninitItem();

	// エフェクトの終了処理
	UninitParticle();

	// スコアの終了処理
	UninitScore();

	// タイマーの終了処理
	//UninitTimer();

	// ポーズメニューの終了処理
	UninitPause();

	// 体力表示の終了処理
	UninitLife();
	// UIの終了処理
	UninitUI();
	// ノーダメージボーナスの終了処理
	//UninitNodamage();
}

//==============================================================================
// ゲーム画面の更新処理
//==============================================================================
void UpdateGame(void)
{
	// ローカル変数宣言
	Player *player;
	PAUSE pause;
	FADE fade;
	//int nTime = GetTimer();
	int nGameCnt = g_nGameCnt;

	// プレイヤーの取得
	player = GetPlayer();

	// ポーズの取得
	pause = GetPause();

	// フェードの取得
	fade = GetFade();

	// カメラの設定
	//SetCamera();
	//SetEffect(D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0.02f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.05f, 30);

	// ポーズメニューへ移行
	if (/*player->state != PLAYERSTATE_CLEAR && player->state != PLAYERSTATE_GAMEOVER &&*/ pause.colOption.a <= 0.0f && fade == FADE_NONE)
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
		{ // 選択時の画面遷移処理
			if (fade != FADE_OUT && pause.colOption.a <= 0.0f)
			{ // 選択処理の重複防止
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_START000);
			}
			
			if (pause.PauseMenu == PAUSE_MENU_CONTINUE)
			{ // CONTINUEの時
				g_bPause = false;				// ポーズの解除(ゲームの続行)
			}
			else if(pause.PauseMenu == PAUSE_MENU_RETRY)
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
		//if (player->state != PLAYERSTATE_CLEAR && player->state != PLAYERSTATE_GAMEOVER)
		{ // ゲーム中の処理
		  // 背景ポリゴンの更新処理
			//UpdateBg();

			// 敵の更新処理
			//UpdateEnemy();

			// カメラの更新処理
			UpdateCamera();

			// ライトの更新処理
			UpdateLight();

			// ビルボードの更新処理
			UpdateBillboard();

			// 弾の更新処理
			UpdateBullet();

			// プレイヤーの更新処理
			UpdatePlayer();

			// ポリゴンの更新処理
			UpdatePolygon();

			// メッシュフィールドの更新処理
			UpdateMeshfield();

			// 壁の更新処理
			UpdateWall();

			// メッシュ壁の更新処理
			UpdateMeshwall();

			UpdateObject();

			UpdateItem();

			// 影の更新処理
			UpdateShadow();
		}

		// エフェクトの更新処理
		UpdateParticle();

		//if (player->state != PLAYERSTATE_GAMEOVER)
		{
			// スコアの更新処理
			UpdateScore();

			// タイマーの更新処理
			UpdateTimer();

			// ノーダメージボーナスの更新処理
			//UpdateNodamage();
		}
	
		//// 画面の遷移（ゲームクリア時）
		//if (nTime <= 0 && Player->state == PLAYERSTATE_CLEAR && g_nGameCnt < MAX_STAGE && g_nStartCnt == g_nGameCnt)
		//{
		//	g_nGameCnt++;					// ステージカウント加算
		//	SetFade(FADE_OUT, MODE_GAME);	// ゲームモード続行
		//}
		//else if(nTime <= 0 && g_nGameCnt == MAX_STAGE)
		//{
		//	SetFade(FADE_OUT, MODE_RESULT);	// リザルトへ
		//}
		
		// 画面の遷移（ゲームオーバー時）
		if (player->state == PLAYERSTATE_GAMEOVER)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// リザルトへ
		}

		if (GetKeyboardTrigger(KEYINFO_OK) == true && g_bPause == false)
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
	//// 背景ポリゴンの描画処理
	//DrawBg();
	//// 敵の描画処理
	//DrawEnemy();

	// ポリゴンの描画処理
	DrawPolygon();

	// メッシュフィールドの描画処理
	DrawMeshfield();

	// 壁の描画処理
	DrawWall();

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
	DrawBillboard();

	// エフェクトの更新処理
	DrawParticle();

	//// ブロックの描画処理
	//DrawBlock();

	// UIの描画処理
	DrawUI();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();

	// 体力表示の描画処理
	DrawLife();

	//// ノーダメージボーナスの描画処理
	//DrawNodamage();

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