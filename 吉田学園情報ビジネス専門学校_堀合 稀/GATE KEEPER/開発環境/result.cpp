//==============================================================================
//
// リザルト画面の描画〔result.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "result.h"
#include "input.h"
#include "player.h"
#include "score.h"
#include "sound.h"
#include "fade.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define RESULT_POLYGON		(10)		// 最大ポリゴン数
#define TEXTURE_RESULT		(3)			// テクスチャの数
#define RESULTLOGO_POSX		(640.0f)	// リザルトロゴの中心X座標
#define RESULTLOGO_POSY		(200.0f)	// リザルトロゴの中心Y座標
#define RESULTLOGO_SIZEX	(240.0f)	// リザルトロゴの中心からの幅
#define RESULTLOGO_SIZEY	(40.0f)		// リザルトロゴの中心からの高さ
#define RESULT_SCOREX		(500)		// リザルトスコアの上一桁目のX座標
#define RESULT_SCOREY		(360)		// リザルトスコアの上一桁目のY座標
#define RESULT_SCORE_SIZEX	(20.0f)		// リザルトスコアの一桁の幅
#define RESULT_SCORE_SIZEY	(30.0f)		// リザルトスコアの一桁の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[TEXTURE_RESULT] = {};	// リザルト画面テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGameoverlogo = NULL;			// ゲームオーバーロゴテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			// バッファへのポインタ
D3DXVECTOR3 g_posResult;									// リザルト画面の位置
D3DXVECTOR3 g_posResultlogo;								// リザルトロゴの位置
D3DXVECTOR3 g_posResultScore[MAX_SCORE];					// リザルトスコアの位置

//==============================================================================
// リザルト画面の初期化処理
//==============================================================================
HRESULT InitResult(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	PLAYER *Player;

	// プレイヤーの取得
	Player = GetPlayer();

	// デバイスの取得
	pDevice = GetDevice();
	
	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result_bg.png",
		&g_pTextureResult[0]);

	// ロゴテクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result_logo000.png",
		&g_pTextureResult[1]);

	// スコアテクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureResult[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * RESULT_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_posResultScore[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// リザルト画面の中心座標
	g_posResult = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posResult.x - (SCREEN_WIDTH / 2), g_posResult.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posResult.x - (SCREEN_WIDTH / 2), g_posResult.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posResult.x + (SCREEN_WIDTH / 2), g_posResult.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posResult.x + (SCREEN_WIDTH / 2), g_posResult.y - (SCREEN_HEIGHT / 2), 0.0f);

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

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// リザルトの中心座標
	g_posResult = D3DXVECTOR3(RESULTLOGO_POSX, RESULTLOGO_POSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posResult.x - RESULTLOGO_SIZEX, g_posResult.y + RESULTLOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posResult.x - RESULTLOGO_SIZEX, g_posResult.y - RESULTLOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posResult.x + RESULTLOGO_SIZEX, g_posResult.y + RESULTLOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posResult.x + RESULTLOGO_SIZEX, g_posResult.y - RESULTLOGO_SIZEY, 0.0f);

	// rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0, 0.5);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 0.5);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// スコアの中心座標の設定
		g_posResultScore[nCnt] = D3DXVECTOR3(RESULT_SCOREX + ((float)nCnt * 40.0f), RESULT_SCOREY, 0.0f);

		//スコアの頂点座標の設定
		pVtx[8].pos = D3DXVECTOR3(g_posResultScore[nCnt].x - RESULT_SCORE_SIZEX, g_posResultScore[nCnt].y + RESULT_SCORE_SIZEY, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(g_posResultScore[nCnt].x - RESULT_SCORE_SIZEX, g_posResultScore[nCnt].y - RESULT_SCORE_SIZEY, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_posResultScore[nCnt].x + RESULT_SCORE_SIZEX, g_posResultScore[nCnt].y + RESULT_SCORE_SIZEY, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_posResultScore[nCnt].x + RESULT_SCORE_SIZEX, g_posResultScore[nCnt].y - RESULT_SCORE_SIZEY, 0.0f);

		// rhwの設定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ頂点情報の設定
		pVtx[8].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[10].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[11].tex = D3DXVECTOR2(0.1f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	if (Player->state == PLAYERSTATE_GAMEOVER)
	{
		// BGMの再生
		PlaySound(SOUND_LABEL_BGM003);
	}
	else
	{
		// BGMの再生
		PlaySound(SOUND_LABEL_BGM002);
	}

	return S_OK;
}

//==============================================================================
// リザルト画面の終了処理
//==============================================================================
void UninitResult(void)
{
	// サウンドの停止処理
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TEXTURE_RESULT; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
}

//==============================================================================
// リザルト画面の更新処理
//==============================================================================
void UpdateResult(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	FADE fade;
	PLAYER *Player;
	float fResult = 0.0f;
	int nScore = GetScore();	// スコアの取得
	int aNumber[MAX_SCORE];
	int nData = (int)pow(10, MAX_SCORE - 1);

	// フェードの取得
	fade = GetFade();

	// プレイヤーの取得
	Player = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャの切り替え
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		aNumber[nCnt] = (nScore / nData) % 10;
		nData /= 10;

		// テクスチャの頂点座標の更新
		pVtx[8].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[9].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 0.0f);
		pVtx[10].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[11].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 0.0f);

		// 頂点ポインタを4ずつ進める
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	if (Player->state == PLAYERSTATE_GAMEOVER)
	{
		fResult += 0.5f;
	}

	// 頂点座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f + fResult);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f + fResult);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.5f + fResult);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f + fResult);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.5f + fResult);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f + fResult);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 0.5f + fResult);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f + fResult);

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	// ランキング画面への切り替え
	if (GetKeyboardTrigger(KEYINFO_OK) == true && fade == FADE_NONE)
	{
		SetFade(FADE_OUT,MODE_RANKING);
	}
}

//==============================================================================
// リザルト画面の描画処理
//==============================================================================
void DrawResult(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < RESULT_POLYGON; nCnt++)
	{
		if (nCnt == 0)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[0]);
		}
		else if (nCnt == 1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[1]);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[2]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}			
}