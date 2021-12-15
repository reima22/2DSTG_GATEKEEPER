//==============================================================================
//
// ゲーム画面の描画〔game.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
#include "player.h"
#include "bg.h"
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
#include "block.h"
#include "timer.h"
#include "gamepad.h"
#include "nodamage.h"
#include "time.h"
#include "blockpoint.h"
#include "stdlib.h"
#include "stdio.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define VERTICAL_STEP	(90)	// 縦移動するブロックのカウント間隔
#define SIDE_STEP		(240)	// 横移動するブロックのカウント間隔
#define BLOCKX			(46)	// ブロック配置のcsvファイルの列数
#define BLOCKY			(24)	// ブロック配置のcsvファイルの行数
#define PATTERN_MAX		(8)		// 配置パターンの最大数
#define POINT_SET		(15)	// 移動点の配置間隔

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
	InitBg();

	// タイマーの初期化処理
	InitTimer();

	// アイテムの初期化処理
	InitItem();

	// アイテムの配置
	PutItem(g_nGameCnt);

	// プレイヤーの初期化処理
	InitPlayer();

	// 敵の初期化設定
	InitEnemy();

	// 敵の配置
	PutEnemy(g_nGameCnt);

	// 移動点の初期化処理
	InitBlockPoint();

	// ブロックの初期化処理
	InitBlock();

	// ブロックの配置
	PutBlock(g_nGameCnt);

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

	// BGMの再生
	bool bBgm = GAME_BGM;
	if (bBgm == true)
	{
		if (g_nGameCnt == 0)
		{
			PlaySound(SOUND_LABEL_BGM001);
		}
		else if (g_nGameCnt == 1)
		{
			PlaySound(SOUND_LABEL_BGM005);
		}
		else if (g_nGameCnt == 2)
		{
			PlaySound(SOUND_LABEL_BGM006);
		}
	}

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

	// プレイヤーの終了処理
	UninitPlayer();

	// 敵の終了処理
	UninitEnemy();

	// ブロックの終了処理
	UninitBlock();

	// 移動点の終了処理
	UninitBlockPoint();

	// アイテムの終了処理
	UninitItem();

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
}

//==============================================================================
// ゲーム画面の更新処理
//==============================================================================
void UpdateGame(void)
{
	// ローカル変数宣言
	PLAYER *Player;
	PAUSE pause;
	FADE fade;
	int nTime = GetTimer();
	int nGameCnt = g_nGameCnt;

	// プレイヤーの取得
	Player = GetPlayer();

	// ポーズの取得
	pause = GetPause();

	// フェードの取得
	fade = GetFade();

	// ポーズメニューへ移行
	if (Player->state != PLAYERSTATE_CLEAR && Player->state != PLAYERSTATE_GAMEOVER && pause.colOption.a <= 0.0f && fade == FADE_NONE)
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
				g_nGameCnt = START_NUM;
			}
			else if (pause.PauseMenu == PAUSE_MENU_QUIT)
			{ // QUITの時
				SetFade(FADE_OUT, MODE_TITLE);	// タイトルへ移行
			}
		}
	}
	else
	{ // 非ポーズ時の処理
		if (Player->state != PLAYERSTATE_CLEAR && Player->state != PLAYERSTATE_GAMEOVER)
		{ // ゲーム中の処理
		  // 背景ポリゴンの更新処理
			UpdateBg();

			// 敵の更新処理
			UpdateEnemy();

			// アイテムの更新処理
			UpdateItem();

			// プレイヤーの更新処理
			UpdatePlayer();

			// 移動点の更新処理
			UpdateBlockPoint();

			// ブロックの更新処理
			UpdateBlock();

			// 体力表示の更新処理
			UpdateLife();

			// UIの更新処理
			UpdateUI();
		}

		// エフェクトの更新処理
		UpdateParticle();

		if (Player->state != PLAYERSTATE_GAMEOVER)
		{
			// スコアの更新処理
			UpdateScore();

			// タイマーの更新処理
			UpdateTimer();

			// ノーダメージボーナスの更新処理
			UpdateNodamage();
		}
	
		// 画面の遷移（ゲームクリア時）
		if (nTime <= 0 && Player->state == PLAYERSTATE_CLEAR && g_nGameCnt < MAX_STAGE && g_nStartCnt == g_nGameCnt)
		{
			g_nGameCnt++;					// ステージカウント加算
			SetFade(FADE_OUT, MODE_GAME);	// ゲームモード続行
		}
		else if(nTime <= 0 && g_nGameCnt == MAX_STAGE)
		{
			SetFade(FADE_OUT, MODE_RESULT);	// リザルトへ
		}
		
		// 画面の遷移（ゲームオーバー時）
		if (Player->state == PLAYERSTATE_GAMEOVER)
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
	// 背景ポリゴンの描画処理
	DrawBg();

	// 移動点の描画処理
	DrawBlockPoint();

	// 敵の描画処理
	DrawEnemy();

	// アイテムの描画処理
	DrawItem();

	// ポリゴンの描画処理
	DrawPlayer();

	// エフェクトの描画処理
	DrawParticle();

	// ブロックの描画処理
	DrawBlock();

	// UIの描画処理
	DrawUI();

	// スコアの描画処理
	DrawScore();

	// タイマーの描画処理
	DrawTimer();

	// カウンターの描画処理
	//DrawCounter();

	// 体力表示の描画処理
	DrawLife();

	// ノーダメージボーナスの描画処理
	DrawNodamage();

	// ポーズの描画処理
	if (g_bPause == true)
	{
		DrawPause();
	}
}

//==============================================================================
// ゲームステージカウントの初期化
//==============================================================================
HRESULT InitGameCnt(void)
{
	// 変数の初期化
	g_nGameCnt = 0;

	return S_OK;
}

//==============================================================================
// 敵の配置
//==============================================================================
void PutEnemy(int nStage)
{
	// ステージによる配置変更
	if (nStage == 0)
	{
		// 敵の配置
		SetEnemy(D3DXVECTOR3(40.0f, 690.0f, 0.0f), D3DXVECTOR3(-2.5f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 0, 0);
		SetEnemy(D3DXVECTOR3(1240.0f, 690.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 0, 0);
		SetEnemy(D3DXVECTOR3(1240.0f, 500.0f, 0.0f), D3DXVECTOR3(2.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 1, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 150.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
	}
	else if (nStage == 1)
	{
		// 敵の配置
		SetEnemy(D3DXVECTOR3(320.0f, 650.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 510.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(320.0f, 370.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(640.0f, 230.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
		SetEnemy(D3DXVECTOR3(320.0f, 110.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 1, 0);
	}
	else if (nStage == 2)
	{
		// 敵の配置
		SetEnemy(D3DXVECTOR3(1240.0f, 150.0f, 0.0f), D3DXVECTOR3(3.0f, 0.0f, 0.0f), ENEMY_SIZEX, ENEMY_SIZEY, 4, 0);
	}
}

//==============================================================================
// アイテムの配置
//==============================================================================
void PutItem(int nStage)
{
	// ステージによる配置変更
	if (nStage == 0)	// 合計48枚
	{
		// アイテムの配置設定
		for (int nCnt = 0; nCnt < 6; nCnt++)	// 12枚
		{
			// 浮き床位置
			SetItem(D3DXVECTOR3(390.0f + (nCnt * 100), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(420.0f + (nCnt * 100), 230.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 8; nCnt++)	// 16枚
		{
			// 地面位置
			SetItem(D3DXVECTOR3(30.0f + (nCnt * 70), 650.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1250.0f - (nCnt * 70), 650.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 6; nCnt++)	// 12枚
		{
			// 下段位置
			SetItem(D3DXVECTOR3(40.0f + (nCnt * 70), 460.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1240.0f - (nCnt * 70), 460.0f, 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)	// 8枚
		{
			// 上段位置
			SetItem(D3DXVECTOR3(40.0f + (nCnt * 70), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1240.0f - (nCnt * 70), 120.0f, 0.0f), 0);
		}
	}
	else if (nStage == 1)	// 合計70枚
	{
		// アイテムの配置設定
		for (int nCnt = 0; nCnt < 14; nCnt++)	// 14枚
		{
			SetItem(D3DXVECTOR3(230.0f + (nCnt * 70), 650.0f, 0.0f), 0);	// 地面位置
			
			SetItem(D3DXVECTOR3(350.0f + (nCnt * 70), 510.0f, 0.0f), 0);	// 1段目
			
			SetItem(D3DXVECTOR3(100.0f + (nCnt * 70), 370.0f, 0.0f), 0);	// 2段目
			
			SetItem(D3DXVECTOR3(350.0f + (nCnt * 70), 230.0f, 0.0f), 0);	// 3段目
			
			SetItem(D3DXVECTOR3(100.0f + (nCnt * 70), 90.0f, 0.0f), 0);		// 4段目
		}
	}
	else if (nStage == 2)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)	// 30枚
		{
			SetItem(D3DXVECTOR3(70.0f, 170.0f + (nCnt * 50), 0.0f), 0);
			SetItem(D3DXVECTOR3(100.0f, 170.0f + (nCnt * 50), 0.0f), 0);
			SetItem(D3DXVECTOR3(315.0f, 120.0f + (nCnt * 90), 0.0f), 0);
			SetItem(D3DXVECTOR3(520.0f + (nCnt * 80), 390.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1115.0f, 120.0f + (nCnt * 90), 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 5; nCnt++)	// 10枚
		{
			SetItem(D3DXVECTOR3(550.0f + (nCnt * 80), 520.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(1250.0f, 220.0f + (nCnt * 80), 0.0f), 0);
		}

		for (int nCnt = 0; nCnt < 7; nCnt++)	// 14枚
		{
			SetItem(D3DXVECTOR3(480.0f + (nCnt * 70), 120.0f, 0.0f), 0);
			SetItem(D3DXVECTOR3(500.0f + (nCnt * 70), 230.0f, 0.0f), 0);
		}
	}
}

//==============================================================================
// ブロックの配置
//==============================================================================
void PutBlock(int nStage)
{
	// csvファイルの読み込み
	LoadBlock(nStage);
	int aType[PATTERN_MAX] = {};			// タイプごとの配置数
	D3DXVECTOR3 posA[PATTERN_MAX] = {};		// 最初に設置した位置
	D3DXVECTOR3 moveA[PATTERN_MAX] = {};	// ブロックの移動力
	int aCounter[PATTERN_MAX] = {};			// カウンター最大値
	bool bGet[PATTERN_MAX] = { false };		// 最初に設置する位置を取得済みか

	// ブロックの配置
	for (int nCntY = 0; nCntY < BLOCKY; nCntY++)
	{
		for (int nCntX = 0; nCntX < BLOCKX; nCntX++)
		{
			// 振り分けた数字ごとの配置・移動パターン
			if (g_aMapData[nCntY][nCntX] == 1)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-50.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int nCounter = 0;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 2)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-50.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
				int nCounter = 90;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 3)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				int nCounter = 0;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 4)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
				int nCounter = 210;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 5)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
				int nCounter = 210;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 6)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(2.0f, 0.0f, 0.0f);
				int nCounter = 150;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, 150);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
			else if (g_aMapData[nCntY][nCntX] == 7)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(-60.0f + (float)nCntX * BLOCK_SIZEX, (float)nCntY * BLOCK_SIZEY, 0);
				D3DXVECTOR3 move = D3DXVECTOR3(-2.0f, 0.0f, 0.0f);
				int nCounter = 120;
				SetBlock(pos, move, BLOCK_SIZEX, BLOCK_SIZEY, nCounter);
				aType[g_aMapData[nCntY][nCntX]]++;

				// 変数の代入
				if (bGet[g_aMapData[nCntY][nCntX]] == false)
				{
					posA[g_aMapData[nCntY][nCntX]] = pos;
					moveA[g_aMapData[nCntY][nCntX]] = move;
					aCounter[g_aMapData[nCntY][nCntX]] = nCounter;
					bGet[g_aMapData[nCntY][nCntX]] = true;
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < PATTERN_MAX; nCnt++)
	{
		if (moveA[nCnt] != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
		{
			if (moveA[nCnt].y != 0.0f)
			{// 縦移動ブロック
				posA[nCnt].x = posA[nCnt].x + ((float)POINT_SIZEX * (float)aType[nCnt] / 2.0f) - (float)POINT_SIZEX / 2.0f;

				// 移動点の配置
				for (int nSet = 0; nSet < aCounter[nCnt]; nSet++)
				{
					if (nSet % POINT_SET == 0)
					{
						SetPoint(D3DXVECTOR3(posA[nCnt].x, posA[nCnt].y + moveA[nCnt].y * (float)nSet, 0.0f), POINT_SIZEX, POINT_SIZEY);
					}
				}
			}
			else if (moveA[nCnt].x != 0.0f)
			{// 横移動ブロック
				if (moveA[nCnt].x < 0.0f)
				{// 左移動スタート
					posA[nCnt].x = posA[nCnt].x + moveA[nCnt].x * (float)aCounter[nCnt];	// 初期位置の修正
					moveA[nCnt] *= -1;

					// 移動点の配置
					for (int nSet = 0; nSet < (aCounter[nCnt] + (aType[nCnt] * POINT_SET)); nSet++)
					{
						if (nSet % POINT_SET == 0)
						{
							SetPoint(D3DXVECTOR3(posA[nCnt].x + moveA[nCnt].x * (float)nSet, posA[nCnt].y, posA[nCnt].z), POINT_SIZEX, POINT_SIZEY);
						}
					}
				}
				else
				{// 右移動スタート
					for (int nSet = 0; nSet < (aCounter[nCnt] + (aType[nCnt] * POINT_SET)); nSet++)
					{
						// 移動点の配置
						if (nSet % POINT_SET == 0)
						{
							SetPoint(D3DXVECTOR3(posA[nCnt].x + moveA[nCnt].x * (float)nSet, posA[nCnt].y, posA[nCnt].z), POINT_SIZEX, POINT_SIZEY);
						}
					}
				}
			}
		}
	}
}

//==============================================================================
// ステージカウントの取得
//==============================================================================
int GetStage(void)
{
	return g_nGameCnt;
}

//==============================================================================
// ブロックデータの読み込み
//==============================================================================
void LoadBlock(int nStage)
{
	// ローカル変数宣言
	FILE *pFile = NULL;
	char aLine[256];
	int nCntX = 0;
	int nCntY = 0;

	// ファイルを開く
	if (nStage == 0)
	{
		pFile = fopen("data/STAGE/stage00.csv", "r");
	}
	else if (nStage == 1)
	{
		pFile = fopen("data/STAGE/stage01.csv", "r");
	}
	else if (nStage == 2)
	{
		pFile = fopen("data/STAGE/stage02.csv", "r");
	}

	while (fgets(aLine, 256, pFile) != NULL)
	{
		nCntX = 0;
		char *pToken = strtok(aLine, ",");

		while (pToken != NULL)
		{
			int nNum = atoi(pToken);
			g_aMapData[nCntY][nCntX] = nNum;
			pToken = strtok(NULL, ",");
			nCntX++;
		}
		nCntY++;
	}
	fclose(pFile);
}