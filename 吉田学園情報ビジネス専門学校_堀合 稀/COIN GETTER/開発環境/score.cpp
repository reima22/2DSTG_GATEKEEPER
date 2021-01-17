//==============================================================================
//
// スコアの描画〔score.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "score.h"
#include "game.h"
#include "math.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define POS_SCOREX	(1100.0f)	// スコアの上一桁の中心X座標
#define POS_SCOREY	(40.0f)	// スコアの上一桁の中心Y座標

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			// スコアテクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// スコアバッファへのポインタ
D3DXVECTOR3 g_aScore[MAX_SCORE];					// スコアの位置情報
int g_nScore;										// スコアの数値

//==============================================================================
// スコアの初期化処理
//==============================================================================
HRESULT InitScore(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nStage = GetStage();

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureScore);

	// スコアの位置座標の初期化
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		g_aScore[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// スコア値の初期化
	if (nStage == START_NUM)
	{
		g_nScore = 0;
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// スコアの中心座標の設定
		g_aScore[nCnt] = D3DXVECTOR3(POS_SCOREX + ((float)nCnt * 20.0f), POS_SCOREY, 0.0f);

		//スコアの頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3((float)g_aScore[nCnt].x - 10.0f, (float)g_aScore[nCnt].y + 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)g_aScore[nCnt].x - 10.0f, (float)g_aScore[nCnt].y - 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)g_aScore[nCnt].x + 10.0f, (float)g_aScore[nCnt].y + 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)g_aScore[nCnt].x + 10.0f, (float)g_aScore[nCnt].y - 20.0f, 0.0f);

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
	g_pVtxBuffScore->Unlock();

	return S_OK;
}

//==============================================================================
// スコアの終了処理
//==============================================================================
void UninitScore(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
}

//==============================================================================
// スコアの更新処理
//==============================================================================
void UpdateScore(void)
{
	// ローカル変数宣言
	int aNumber[MAX_SCORE];
	int nData = (int)pow(10, (MAX_SCORE - 1));

	// 各桁の計算とテクスチャの切り替え
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		aNumber[nCnt] = (g_nScore / nData) % 10;
		nData /= 10;
		SetVertexScore(nCnt, aNumber[nCnt]);
	}
}

//==============================================================================
// スコアの描画処理
//==============================================================================
void DrawScore(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// スコアの加算処理
//==============================================================================
void AddScore(int nValue)
{
	g_nScore += nValue;	// スコアを加算する
}

//==============================================================================
// スコアの頂点座標取得処理
//==============================================================================
void SetVertexScore(int nIdx,int nNumber)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * nIdx;

	// テクスチャの頂点座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==============================================================================
// スコアの取得
//==============================================================================
int GetScore(void)
{
	return g_nScore;	// スコアの値を取得する
}