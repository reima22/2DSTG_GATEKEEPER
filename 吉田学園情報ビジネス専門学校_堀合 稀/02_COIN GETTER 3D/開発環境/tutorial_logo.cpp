//==============================================================================
//
// チュートリアルロゴの描画〔tutorial.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "tutorial_logo.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define TUTOLOGO_POS0	(D3DXVECTOR3(1100.0f, 600.0f, 0.0f))
#define TUTOLOGO_POS1	(D3DXVECTOR3(950.0f, 680.0f, 0.0f))
#define TUTOLOGO_X		(140.0f)
#define TUTOLOGO_Y		(40.0f)
#define TUTOLOGO_COL	(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
//#define TUTOLOGO_NUM	(2)

//==============================================================================
// グローバル変数
//==============================================================================
TutoLogo g_TutoLogo;

//==============================================================================
// チュートリアル画面の初期化処理
//==============================================================================
HRESULT InitTutoLogo(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutologo00.png",
		&g_TutoLogo.tutologoInfo[0].pTextureTutoLogo);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/tutologo01.png",
		&g_TutoLogo.tutologoInfo[1].pTextureTutoLogo);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTOLOGO_NUM,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_TutoLogo.pVtxBuffTutoLogo,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_TutoLogo.tutologoInfo[0].pos = TUTOLOGO_POS0;
	g_TutoLogo.tutologoInfo[1].pos = TUTOLOGO_POS1;
	g_TutoLogo.tutologoInfo[0].bUse = true;
	g_TutoLogo.tutologoInfo[1].bUse = false;
	g_TutoLogo.bFadeTuto = false;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_TutoLogo.pVtxBuffTutoLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++, pVtx += 4)
	{
		// リザルト画面の中心座標
		
		g_TutoLogo.tutologoInfo[nCnt].col = TUTOLOGO_COL;

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x - TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y + TUTOLOGO_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x - TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y - TUTOLOGO_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x + TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y + TUTOLOGO_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_TutoLogo.tutologoInfo[nCnt].pos.x + TUTOLOGO_X, g_TutoLogo.tutologoInfo[nCnt].pos.y - TUTOLOGO_Y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[1].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[2].col = g_TutoLogo.tutologoInfo[nCnt].col;
		pVtx[3].col = g_TutoLogo.tutologoInfo[nCnt].col;

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// 頂点バッファをアンロックする
	g_TutoLogo.pVtxBuffTutoLogo->Unlock();

	return S_OK;
}

//==============================================================================
// チュートリアル画面の終了処理
//==============================================================================
void UninitTutoLogo(void)
{
	// 頂点バッファの開放
	if (g_TutoLogo.pVtxBuffTutoLogo != NULL)
	{
		g_TutoLogo.pVtxBuffTutoLogo->Release();
		g_TutoLogo.pVtxBuffTutoLogo = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++)
	{
		if (g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo != NULL)
		{			   
			g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo->Release();
			g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo = NULL;
		}
	}
}

//==============================================================================
// チュートリアル画面の更新処理
//==============================================================================
void UpdateTutoLogo(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	g_TutoLogo.nCntAnim++;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_TutoLogo.pVtxBuffTutoLogo->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLogo = 0; nCntLogo < TUTOLOGO_NUM; nCntLogo++,pVtx += 4)
	{
		if (g_TutoLogo.tutologoInfo[nCntLogo].bUse == true)
		{// 使用状態にあるロゴ判別
			if (g_TutoLogo.bFadeTuto == true)
			{// 画面遷移時
				if (g_TutoLogo.nCntAnim % 3 == 0)
				{// カウンターが偶数の時
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 1.0f;
				}
				else
				{
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 0.0f;
				}
			}
			else
			{// 通常時
				if (g_TutoLogo.nCntAnim % 60 == 0)
				{// カウンターが60の倍数の時
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 1.0f;
				}
				else if (g_TutoLogo.nCntAnim % 30 == 0 && g_TutoLogo.nCntAnim % 60 != 0)
				{// カウンターが30の倍数、且つ60の倍数でない時
					g_TutoLogo.tutologoInfo[nCntLogo].col.a = 0.0f;
				}
			}

			// 各頂点カラーの設定
			pVtx[0].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[1].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[2].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
			pVtx[3].col = g_TutoLogo.tutologoInfo[nCntLogo].col;
		}
	}

	// 頂点バッファをアンロックする
	g_TutoLogo.pVtxBuffTutoLogo->Unlock();
}

//==============================================================================
// チュートリアルロゴの描画処理
//==============================================================================
void DrawTutoLogo(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_TutoLogo.pVtxBuffTutoLogo,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < TUTOLOGO_NUM; nCnt++)
	{
		if (g_TutoLogo.tutologoInfo[nCnt].bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_TutoLogo.tutologoInfo[nCnt].pTextureTutoLogo);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// チュートリアルロゴ情報の取得
//==============================================================================
TutoLogo *GetTutoLogo(void)
{
	return &g_TutoLogo;
}