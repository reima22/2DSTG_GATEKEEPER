//==============================================================================
//
// チュートリアル画面の描画〔tutorial.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "gamepad.h"

// マクロ定義
#define TUTORIAL_NUM	(2)	// チュートリアルの数

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTutorial[2] = {};		// チュートリアル画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posTutorial[TUTORIAL_NUM];			// チュートリアル画面の位置
D3DXCOLOR col;

//==============================================================================
// チュートリアル画面の初期化処理
//==============================================================================
HRESULT InitTutorial(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_pTextureTutorial[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial000.jpg",
		&g_pTextureTutorial[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTORIAL_NUM,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL)))
	{
		return E_FAIL;
	}

	col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTORIAL_NUM; nCnt++,pVtx += 4)
	{
		// リザルト画面の中心座標
		g_posTutorial[nCnt] = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posTutorial[nCnt].x - SCREEN_CENTERX, g_posTutorial[nCnt].y + SCREEN_CENTERY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posTutorial[nCnt].x - SCREEN_CENTERX, g_posTutorial[nCnt].y - SCREEN_CENTERY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posTutorial[nCnt].x + SCREEN_CENTERX, g_posTutorial[nCnt].y + SCREEN_CENTERY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posTutorial[nCnt].x + SCREEN_CENTERX, g_posTutorial[nCnt].y - SCREEN_CENTERY, 0.0f);

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
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	return S_OK;
}

//==============================================================================
// チュートリアル画面の終了処理
//==============================================================================
void UninitTutorial(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TUTORIAL_NUM; nCnt++)
	{
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}
}

//==============================================================================
// チュートリアル画面の更新処理
//==============================================================================
void UpdateTutorial(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	FADE fade;

	// フェードの取得
	fade = GetFade();

	// チュートリアル画面の遷移
	if (col.a == 1.0f && fade != FADE_IN)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			col.a = 0.0f;
		}
	}
	else if (fade == FADE_NONE && col.a == 0.0f)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点カラーの設定
	pVtx[4].col = col;
	pVtx[5].col = col;
	pVtx[6].col = col;
	pVtx[7].col = col;

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//==============================================================================
// チュートリアル画面の描画処理
//==============================================================================
void DrawTutorial(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTutorial,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}