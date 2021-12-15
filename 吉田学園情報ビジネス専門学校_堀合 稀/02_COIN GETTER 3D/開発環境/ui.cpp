//==============================================================================
//
// UIの描画〔ui.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "ui.h"
#include "player.h"
#include "input.h"
//#include "enemy.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_UI		(3)		// UIの数
#define UI_SIZEX	(50.0f)	// UIの幅
#define UI_SIZEY	(30.0f)	// UIの高さ
#define UI_LIFE		(D3DXVECTOR3(60.0f, 40.0f, 0.0f))	// LIFEのUIの位置
#define UI_TIME		(D3DXVECTOR3(520.0f, 50.0f, 0.0f))	// TIMEのUIの位置
#define UI_SCORE	(D3DXVECTOR3(1025.0f, 40.0f, 0.0f))	// SCOREのUIの位置

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureUI[MAX_UI] = {};	// UIテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posUI[MAX_UI];					// UIの配置

//==============================================================================
// UIの初期化処理
//==============================================================================
HRESULT InitUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ui_life.png",
		&g_pTextureUI[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ui_time.png",
		&g_pTextureUI[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/ui_score.png",
		&g_pTextureUI[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_UI,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL)))
	{
		return E_FAIL;
	}

	// UIポリゴンの初期配置
	SetInitUI();

	return S_OK;
}

//==============================================================================
// UIの終了処理
//==============================================================================
void UninitUI(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (g_pTextureUI[nCnt] != NULL)
		{
			g_pTextureUI[nCnt]->Release();
			g_pTextureUI[nCnt] = NULL;
		}
	}
}

//==============================================================================
// UIの更新処理
//==============================================================================
void UpdateUI(void)
{
}

//==============================================================================
// UIの描画処理
//==============================================================================
void DrawUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffUI,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureUI[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// UIのポリゴンの初期設定
//==============================================================================
void SetInitUI(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);

	// UIの中心座標
	g_posUI[0] = UI_LIFE;	// LIFEのUI位置
	g_posUI[1] = UI_TIME;	// TIMEのUI位置
	g_posUI[2] = UI_SCORE;	// SCOREのUI位置

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++, pVtx += 4)
	{
		// UIの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posUI[nCnt].x - UI_SIZEX, g_posUI[nCnt].y + UI_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posUI[nCnt].x - UI_SIZEX, g_posUI[nCnt].y - UI_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posUI[nCnt].x + UI_SIZEX, g_posUI[nCnt].y + UI_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posUI[nCnt].x + UI_SIZEX, g_posUI[nCnt].y - UI_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(2, 255, 2, 255);
		pVtx[1].col = D3DCOLOR_RGBA(2, 255, 2, 255);
		pVtx[2].col = D3DCOLOR_RGBA(2, 255, 2, 255);
		pVtx[3].col = D3DCOLOR_RGBA(2, 255, 2, 255);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}