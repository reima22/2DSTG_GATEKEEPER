//==============================================================================
//
// ポーズロゴの描画〔pause_logo.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "pause_logo.h"
#include "pause.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define PAUSELOGO_POS	(D3DXVECTOR3(SCREEN_CENTERX, 100.0f, 0.0f))
#define PAUSELOGO_X		(180.0f)
#define PAUSELOGO_Y		(50.0f)
#define PAUSELOGO_COL	(D3DXCOLOR(1.0f, 1.0f, 0.01f, 1.0f))

//==============================================================================
// グローバル変数
//==============================================================================
PauseLogo g_PauseLogo;

//==============================================================================
// チュートリアル画面の初期化処理
//==============================================================================
HRESULT InitPauseLogo(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/pause_logo.png",
		&g_PauseLogo.pTexture);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_PauseLogo.pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_PauseLogo.pos = PAUSELOGO_POS;
	g_PauseLogo.col = PAUSELOGO_COL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_PauseLogo.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_PauseLogo.pos.x - PAUSELOGO_X, g_PauseLogo.pos.y + PAUSELOGO_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_PauseLogo.pos.x - PAUSELOGO_X, g_PauseLogo.pos.y - PAUSELOGO_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_PauseLogo.pos.x + PAUSELOGO_X, g_PauseLogo.pos.y + PAUSELOGO_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_PauseLogo.pos.x + PAUSELOGO_X, g_PauseLogo.pos.y - PAUSELOGO_Y, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = g_PauseLogo.col;
	pVtx[1].col = g_PauseLogo.col;
	pVtx[2].col = g_PauseLogo.col;
	pVtx[3].col = g_PauseLogo.col;

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_PauseLogo.pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// チュートリアル画面の終了処理
//==============================================================================
void UninitPauseLogo(void)
{
	// 頂点バッファの開放
	if (g_PauseLogo.pVtxBuff != NULL)
	{
		g_PauseLogo.pVtxBuff->Release();
		g_PauseLogo.pVtxBuff = NULL;
	}

	// テクスチャの開放
	if (g_PauseLogo.pTexture != NULL)
	{
		g_PauseLogo.pTexture->Release();
		g_PauseLogo.pTexture = NULL;
	}
}

//==============================================================================
// チュートリアル画面の更新処理
//==============================================================================
void UpdatePauseLogo(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	PAUSE pause = GetPause();

	if (pause.bOption == true)
	{
		g_PauseLogo.col.a = 0.0f;
	}
	else
	{
		if (g_PauseLogo.bPlus == true)
		{
			g_PauseLogo.col.a += 0.01f;
		}
		else
		{
			g_PauseLogo.col.a -= 0.01f;
		}

		if (g_PauseLogo.col.a <= 0.5f)
		{
			g_PauseLogo.bPlus = true;
		}
		else if (g_PauseLogo.col.a >= 1.0f)
		{
			g_PauseLogo.bPlus = false;
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_PauseLogo.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点カラーの設定
	pVtx[0].col = g_PauseLogo.col;
	pVtx[1].col = g_PauseLogo.col;
	pVtx[2].col = g_PauseLogo.col;
	pVtx[3].col = g_PauseLogo.col;

	// 頂点バッファをアンロックする
	g_PauseLogo.pVtxBuff->Unlock();
}

//==============================================================================
// チュートリアルロゴの描画処理
//==============================================================================
void DrawPauseLogo(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_PauseLogo.pVtxBuff,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_PauseLogo.pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,				// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}
