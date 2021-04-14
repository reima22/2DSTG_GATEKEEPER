//==============================================================================
//
// チュートリアル画面の描画〔tutorial.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "tutorial_main.h"
#include "tutorial_logo.h"
#include "input.h"
#include "fade.h"
#include "gamepad.h"

// マクロ定義
//#define TUTORIAL_NUM	(2)	// チュートリアルの数

//==============================================================================
// グローバル変数
//==============================================================================
TutoMain g_TutoMain;

//==============================================================================
// チュートリアル画面の初期化処理
//==============================================================================
HRESULT InitTutoMain(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial000.jpg",
		&g_TutoMain.tutoMainInfo[0].pTextureTutoMain);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutorial001.jpg",
		&g_TutoMain.tutoMainInfo[1].pTextureTutoMain);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTOMAIN_NUM,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_TutoMain.pVtxBuffTutoMain,
		NULL)))
	{
		return E_FAIL;
	}

	g_TutoMain.tutoMainInfo[0].bUse = true;
	g_TutoMain.tutoMainInfo[1].bUse = false;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_TutoMain.pVtxBuffTutoMain->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTOMAIN_NUM; nCnt++, pVtx += 4)
	{
		// リザルト画面の中心座標
		g_TutoMain.tutoMainInfo[nCnt].pos = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x - SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y + SCREEN_CENTERY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x - SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y - SCREEN_CENTERY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x + SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y + SCREEN_CENTERY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutoMain.tutoMainInfo[nCnt].pos.x + SCREEN_CENTERX, g_TutoMain.tutoMainInfo[nCnt].pos.y - SCREEN_CENTERY, 0.0f);

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
	g_TutoMain.pVtxBuffTutoMain->Unlock();

	return S_OK;
}

//==============================================================================
// チュートリアル画面の終了処理
//==============================================================================
void UninitTutoMain(void)
{
	// 頂点バッファの開放
	if (g_TutoMain.pVtxBuffTutoMain != NULL)
	{
		g_TutoMain.pVtxBuffTutoMain->Release();
		g_TutoMain.pVtxBuffTutoMain = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TUTOMAIN_NUM; nCnt++)
	{
		if (g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain != NULL)
		{
			g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain->Release();
			g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain = NULL;
		}
	}
}

//==============================================================================
// チュートリアル画面の更新処理
//==============================================================================
void UpdateTutoMain(void)
{
	// ローカル変数宣言
	FADE fade;
	TutoLogo *Logo = GetTutoLogo();

	// フェードの取得
	fade = GetFade();

	// チュートリアル画面の遷移
	if (fade != FADE_IN)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			if (g_TutoMain.tutoMainInfo[0].bUse == true)
			{// チュートリアル一枚目の切り替え
				Logo->tutologoInfo[0].bUse = false;
				Logo->tutologoInfo[1].bUse = true;

				g_TutoMain.tutoMainInfo[0].bUse = false;
				g_TutoMain.tutoMainInfo[1].bUse = true;
			}
			else
			{// フェードアウトから画面遷移処理へ
				Logo->bFadeTuto = true;
				SetFade(FADE_OUT, MODE_GAME);
			}
		}
	}
	//else if (fade == FADE_NONE)
	//{
	//	if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	//	{
	//		
	//	}
	//}
}

//==============================================================================
// チュートリアル画面の描画処理
//==============================================================================
void DrawTutoMain(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_TutoMain.pVtxBuffTutoMain,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_TutoMain.tutoMainInfo[nCnt].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_TutoMain.tutoMainInfo[nCnt].pTextureTutoMain);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}