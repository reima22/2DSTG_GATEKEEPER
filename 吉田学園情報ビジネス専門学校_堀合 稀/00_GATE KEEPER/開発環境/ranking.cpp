//==============================================================================
//
// ランキング画面の描画〔ranking.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "ranking.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "sound.h"
#include "gamepad.h"
#include "stdio.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define RANK_NUM			(5)			// ランキング順位数
#define MAX_POLY			(47)		// 使用するポリゴン数
#define RANKINGLOGO_POSX	(640.0f)	// ランキングロゴの中心X座標
#define RANKINGLOGO_POSY	(80.0f)		// ランキングロゴの中心Y座標
#define RANKINGLOGO_SIZEX	(240.0f)	// ランキングロゴの中心からの幅
#define RANKINGLOGO_SIZEY	(40.0f)		// ランキングロゴの中心からの高さ
#define RANKINGRANK_POSX	(320.0f)	// 順位表示のX座標位置
#define RANKINGRANK_POSY	(200.0f)	// 順位表示のY座標位置
#define RANKINGRANK_SIZEX	(50.0f)		// 順位表示の幅
#define RANKINGRANK_SIZEY	(30.0f)		// 順位表示の高さ
#define RANKING_SCOREX		(550.0f)	// スコア表示のX座標開始位置
#define RANKING_SCOREY		(200.0f)	// スコア表示のY座標開始位置
#define RANKING_SCORE_SIZEX	(20.0f)		// スコア表示の幅
#define RANKING_SCORE_SIZEY	(30.0f)		// スコア表示の高さ 

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;			// ランキング画面テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingLogo = NULL;		// ランキングロゴテクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;		// ランキング順位テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureRankingScore = NULL;		// ランキングスコアテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		// バッファへのポインタ
D3DXVECTOR3 g_posRanking;								// ランキング画面の位置
D3DXVECTOR3 g_posRankingLogo;							// ランキングロゴの位置
D3DXVECTOR3 g_posRankingRank[RANK_NUM];					// 順位表示の位置
D3DXVECTOR3 g_posRankingScore[RANK_NUM * MAX_SCORE];	// ランキングスコアの位置
int g_aScore[RANK_NUM];									// ランキングのスコア
int g_nColorCnt;										// 点滅色のカウント
bool g_aRankInScore[RANK_NUM];							// 新規ランクインの判断

//==============================================================================
// ランキング画面の初期化処理
//==============================================================================
HRESULT InitRanking(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nScore = GetScore();	// スコアの取得

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ranking_bg.png",
		&g_pTextureRanking);

	// ロゴテクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ranking_logo.png",
		&g_pTextureRankingLogo);

	// 順位テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ranking_rank.png",
		&g_pTextureRankingRank);

	// スコアテクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureRankingScore);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_POLY,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_nColorCnt = 0;
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		g_aRankInScore[nCnt] = false;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ランキング画面の中心座標
	g_posRanking = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posRanking.x - (SCREEN_WIDTH / 2), g_posRanking.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posRanking.x - (SCREEN_WIDTH / 2), g_posRanking.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posRanking.x + (SCREEN_WIDTH / 2), g_posRanking.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posRanking.x + (SCREEN_WIDTH / 2), g_posRanking.y - (SCREEN_HEIGHT / 2), 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// ランキングロゴの中心座標
	g_posRankingLogo = D3DXVECTOR3(RANKINGLOGO_POSX, RANKINGLOGO_POSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKINGLOGO_SIZEX, g_posRankingLogo.y + RANKINGLOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posRankingLogo.x - RANKINGLOGO_SIZEX, g_posRankingLogo.y - RANKINGLOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKINGLOGO_SIZEX, g_posRankingLogo.y + RANKINGLOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posRankingLogo.x + RANKINGLOGO_SIZEX, g_posRankingLogo.y - RANKINGLOGO_SIZEY, 0.0f);

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
	pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 順位表示の設定
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		// 順位表示の中心座標の設定
		g_posRankingRank[nCnt] = D3DXVECTOR3(RANKINGRANK_POSX , RANKINGRANK_POSY + (nCnt * 90), 0.0f);

		//スコアの頂点座標の設定
		pVtx[8].pos = D3DXVECTOR3(g_posRankingRank[nCnt].x - RANKINGRANK_SIZEX, g_posRankingRank[nCnt].y + RANKINGRANK_SIZEY, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(g_posRankingRank[nCnt].x - RANKINGRANK_SIZEX, g_posRankingRank[nCnt].y - RANKINGRANK_SIZEY, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_posRankingRank[nCnt].x + RANKINGRANK_SIZEX, g_posRankingRank[nCnt].y + RANKINGRANK_SIZEY, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_posRankingRank[nCnt].x + RANKINGRANK_SIZEX, g_posRankingRank[nCnt].y - RANKINGRANK_SIZEY, 0.0f);

		// rhwの設定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[8].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[9].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[10].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[11].col = D3DCOLOR_RGBA(0, 255, 0, 255);

		// テクスチャ頂点情報の設定
		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.2f);
		pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[10].tex = D3DXVECTOR2(1.0f, 0.2f);
		pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 順位表示の設定
	for (int nCntRank = 0; nCntRank < RANK_NUM; nCntRank++)
	{
		for (int nCnt = 0; nCnt < (MAX_SCORE); nCnt++)
		{
			// 順位表示の中心座標の設定
			g_posRankingScore[nCnt] = D3DXVECTOR3(RANKING_SCOREX + (nCnt * 40), RANKING_SCOREY + (nCntRank * 90), 0.0f);

			//スコアの頂点座標の設定
			pVtx[28].pos = D3DXVECTOR3(g_posRankingScore[nCnt].x - RANKING_SCORE_SIZEX, g_posRankingScore[nCnt].y + RANKING_SCORE_SIZEY, 0.0f);
			pVtx[29].pos = D3DXVECTOR3(g_posRankingScore[nCnt].x - RANKING_SCORE_SIZEX, g_posRankingScore[nCnt].y - RANKING_SCORE_SIZEY, 0.0f);
			pVtx[30].pos = D3DXVECTOR3(g_posRankingScore[nCnt].x + RANKING_SCORE_SIZEX, g_posRankingScore[nCnt].y + RANKING_SCORE_SIZEY, 0.0f);
			pVtx[31].pos = D3DXVECTOR3(g_posRankingScore[nCnt].x + RANKING_SCORE_SIZEX, g_posRankingScore[nCnt].y - RANKING_SCORE_SIZEY, 0.0f);

			// rhwの設定
			pVtx[28].rhw = 1.0f;
			pVtx[29].rhw = 1.0f;
			pVtx[30].rhw = 1.0f;
			pVtx[31].rhw = 1.0f;

			// 各頂点カラーの設定
			pVtx[28].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[29].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[30].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[31].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ頂点情報の設定
			pVtx[28].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[29].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[30].tex = D3DXVECTOR2(0.1f, 1.0f);
			pVtx[31].tex = D3DXVECTOR2(0.1f, 0.0f);

			pVtx += 4;	// 頂点ポインタを4ずつ進める
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	// BGMの再生
	PlaySound(SOUND_LABEL_BGM004);

	// ランキングの読み込み
	LoadData();

	// ランキングの並び替え
	SetRanking(nScore);

	return S_OK;
}

//==============================================================================
// ランキング画面の終了処理
//==============================================================================
void UninitRanking(void)
{
	// サウンドの停止処理
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	// テクスチャの開放
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	if (g_pTextureRankingLogo != NULL)
	{
		g_pTextureRankingLogo->Release();
		g_pTextureRankingLogo = NULL;
	}

	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;
	}

	if (g_pTextureRankingScore != NULL)
	{
		g_pTextureRankingScore->Release();
		g_pTextureRankingScore = NULL;
	}
}

//==============================================================================
// ランキング画面の更新処理
//==============================================================================
void UpdateRanking(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	FADE fade;
	int nScore = GetScore();	// スコアの取得
	float aRank[RANK_NUM] = { 0.0f,0.2f,0.4f,0.6f,0.8f };
	int aNumber[RANK_NUM][MAX_SCORE];
	int nNewCnt = 0;
	float fRed = 0.0f;
	float fBlue = 0.0f;

	// フェードの取得
	fade = GetFade();

	// 変色カウントの増加
	g_nColorCnt++;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 順位の表示
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (nCnt != 0)
		{
			if (g_aScore[nCnt] == g_aScore[nCnt - 1])
			{
				aRank[nCnt] = aRank[nCnt - 1];
			}
		}
		
		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.2f + aRank[nCnt]);
		pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f + aRank[nCnt]);
		pVtx[10].tex = D3DXVECTOR2(1.0f, 0.2f + aRank[nCnt]);
		pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f + aRank[nCnt]);

		pVtx += 4;
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 各桁のテクスチャの切り替え
	for (int nCntRank = 0; nCntRank < RANK_NUM; nCntRank++)
	{
		int nData = (int)pow(10, MAX_SCORE - 1);
		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			aNumber[nCntRank][nCnt] = (g_aScore[nCntRank] / nData) % 10;
			nData /= 10;

			if (g_aScore[0] == nScore)
			{
				g_aRankInScore[0] = true;
			}
			else if (g_aScore[0] != nScore && g_aScore[nCntRank] == nScore && g_aRankInScore[nCntRank - 1] == false)
			{
				g_aRankInScore[nCntRank] = true;
			}

			// テクスチャの頂点座標の更新
			pVtx[28].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCntRank][nCnt] / 10), 1.0f);
			pVtx[29].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCntRank][nCnt] / 10), 0.0f);
			pVtx[30].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCntRank][nCnt] / 10), 1.0f);
			pVtx[31].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCntRank][nCnt] / 10), 0.0f);

			// 頂点ポインタを4ずつ進める
			pVtx += 4;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	// 新規ランクインの明示
	for (int nCnt = 0; nCnt < RANK_NUM; nCnt++)
	{
		if (g_aRankInScore[nCnt] == true)
		{
			for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++, pVtx = pVtx + 4)
			{
				if (g_nColorCnt % 8 == 0)
				{
					fRed = 1.0f;
					fBlue = 1.0f;

					// 各頂点カラーの設定
					pVtx[28].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[29].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[30].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[31].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
				}
				
				if(g_nColorCnt % 4 == 0 && g_nColorCnt % 8 != 0)
				{
					fRed = 0.0f;
					fBlue = 0.0f;

					// 各頂点カラーの設定
					pVtx[28].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[29].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[30].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
					pVtx[31].col = D3DXCOLOR(fRed, 1.0f, fBlue, 1.0f);
				}
			}
		}
		else
		{
			pVtx += 4 * MAX_SCORE;
		}
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	// タイトルへの切り替え
	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//==============================================================================
// ランキング画面の描画処理
//==============================================================================
void DrawRanking(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffRanking,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 各テクスチャの設定
	for (int nCnt = 0; nCnt < MAX_POLY; nCnt++)
	{
		if (nCnt == 0)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRanking);
		}
		else if (nCnt == 1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRankingLogo);
		}
		else if (nCnt >= 2 && nCnt < (2 + RANK_NUM))
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRankingRank);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRankingScore);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// ランキングの並び替え
//==============================================================================
void SetRanking(int nScore)
{
	// ローカル変数宣言
	int nSubData;

	// ランキング外の除外
	if (g_aScore[RANK_NUM - 1] <= nScore)
	{ // スコアがランキング内の場合
		g_aScore[RANK_NUM - 1] = nScore;
	}

	// 計算処理
	for (int nCnt = 0; nCnt < RANK_NUM - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < RANK_NUM; nCnt2++)
		{
			if (g_aScore[nCnt] < g_aScore[nCnt2])
			{
				nSubData = g_aScore[nCnt];
				g_aScore[nCnt] = g_aScore[nCnt2];
				g_aScore[nCnt2] = nSubData;
			}
		}
	} 

	// データの保存処理
	SaveData();
}

//==============================================================================
// ファイルの読み込み
//==============================================================================
void LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	int nCount;

	// データの読み込み
	pFile = fopen("ranking.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		for (nCount = 0; nCount < RANK_NUM; nCount++)
		{
			fscanf(pFile, "%d", &g_aScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// データ保存処理
//==============================================================================
void SaveData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	int nCount;

	// ファイルの展開
	pFile = fopen("ranking.txt", "w");
	if (pFile != NULL)
	{ // ファイル展開可能
		for (nCount = 0; nCount < RANK_NUM; nCount++)
		{
			fprintf(pFile, "%d\n", g_aScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}