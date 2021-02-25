//==============================================================================
//
// タイマーの描画〔timer.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "timer.h"
#include "score.h"
#include "player.h"
#include "sound.h"
#include "game.h"
#include "nodamage.h"
#include "input.h"
#include "gamepad.h"
#include "math.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define POS_TIMERX	(600.0f)	// タイマーの上一桁の中心X座標
#define POS_TIMERY	(50.0f)		// タイマーの上一桁の中心Y座標
#define TIMER_SIZEX	(20.0f)		// タイマーの数字の幅
#define TIMER_SIZEY	(40.0f)		// タイマーの数字の高さ
//#define TIMER_0		(100)		// ステージ1のタイマー
//#define TIMER_1		(80)		// ステージ2のタイマー
//#define TIMER_2		(120)		// ステージ3のタイマー

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			// スコアテクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// スコアバッファへのポインタ
D3DXVECTOR3 g_aTimer[MAX_TIMER];					// スコアの位置情報
int g_nTimer;										// スコアの数値
int g_nCntDownTimer;								// タイマー減少カウンター
int g_nClearCnt;									// クリア時の待機カウンター

//==============================================================================
// タイマーの初期化処理
//==============================================================================
HRESULT InitTimer(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureTimer);

	// タイマーの初期化
	//SetTimer();
	g_nTimer = 120;
	g_nCntDownTimer = 1;
	g_nClearCnt = 0;

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_TIMER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// スコアの中心座標の設定
		g_aTimer[nCnt] = D3DXVECTOR3(POS_TIMERX + ((float)nCnt * 40.0f), POS_TIMERY, 0.0f);

		//スコアの頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((float)g_aTimer[nCnt].x - TIMER_SIZEX, (float)g_aTimer[nCnt].y + TIMER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)g_aTimer[nCnt].x - TIMER_SIZEX, (float)g_aTimer[nCnt].y - TIMER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)g_aTimer[nCnt].x + TIMER_SIZEX, (float)g_aTimer[nCnt].y + TIMER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)g_aTimer[nCnt].x + TIMER_SIZEX, (float)g_aTimer[nCnt].y - TIMER_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();

	return S_OK;
}

//==============================================================================
// タイマーの終了処理
//==============================================================================
void UninitTimer(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}
}

//==============================================================================
// タイマーの更新処理
//==============================================================================
void UpdateTimer(void)
{
	// ローカル変数宣言
	int aNumber[MAX_TIMER];
	int nData = (int)pow(10, (MAX_TIMER - 1));
	BONUS Bonus = GetBonus();
	Player *pPlayer;
	pPlayer = GetPlayer();

	// タイマーカウントの加算
	g_nCntDownTimer++;

	// 1秒ごとに減少
	if (g_nCntDownTimer % 60 == 0 && g_nTimer > 0 && pPlayer->state != PLAYERSTATE_CLEAR)
	{
		g_nTimer--;
	}

	if (pPlayer->state == PLAYERSTATE_CLEAR && Bonus.nBonus == 0)
	{
		g_nClearCnt++;	//クリア時の待機カウンター増加

		if (g_nClearCnt > 90 && g_nTimer > 0)
		{// 約1秒半経過でタイムカウントの得点換算
			g_nTimer--;
			AddScore(2000);
			PlaySound(SOUND_LABEL_SE_SCORE_UP);

			if (GetKeyboardTrigger(KEYINFO_OK) == true /*|| GetKeyboardTrigger(KEYINFO_JUMP) == true*/ || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
			{// ジャンプor決定ボタン入力で処理の省略
				AddScore(g_nTimer * 2000);
				g_nTimer -= g_nTimer;
				
				PlaySound(SOUND_LABEL_SE_SCORE_UP);
			}
		}
	}

	if (g_nTimer <= 0 && pPlayer->state != PLAYERSTATE_CLEAR)
	{
		pPlayer->state = PLAYERSTATE_GAMEOVER;
	}

	// 各桁の計算とテクスチャの切り替え
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		aNumber[nCnt] = (g_nTimer / nData) % 10;
		nData /= 10;
		SetVertexTimer(nCnt, aNumber[nCnt]);
	}
}

//==============================================================================
// タイマーの描画処理
//==============================================================================
void DrawTimer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTimer,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// タイマーの頂点座標取得処理
//==============================================================================
void SetVertexTimer(int nIdx, int nNumber)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * nIdx;

	// テクスチャの頂点座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//==============================================================================
// タイマーの取得
//==============================================================================
int GetTimer(void)
{
	return g_nTimer;	// タイマーの値を取得する
}

//==============================================================================
// ステージごとのタイマーの設定
//==============================================================================
void SetTimer(void)
{
	//// ローカル変数宣言
	//int nStage = GetStage();

	//switch (nStage)
	//{
	//case 0:
	//	g_nTimer = TIMER_0;

	//	break;

	//case 1:
	//	g_nTimer = TIMER_1;

	//	break;

	//case 2:
	//	g_nTimer = TIMER_2;

	//	break;

	//default:
	//	break;
	//}
	
}