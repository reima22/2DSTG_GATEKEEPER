//==============================================================================
//
// ユーザーインターフェースの描画〔ui.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "ui.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define UI_POSX			(640)	// ユーザーインターフェースの中心X座標
#define UI_POSY			(676)	// ユーザーインターフェースの中心Y座標
#define UI_SIZEX		(640)	// ユーザーインターフェースの中心からの幅
#define UI_SIZEY		(44)	// ユーザーインターフェースの中心からの高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureUi = NULL;	// ユーザーインターフェーステクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posUi;							// ユーザーインターフェース(メイン)の中心の位置

//==============================================================================
// ユーザーインターフェースの初期化処理
//==============================================================================
HRESULT InitUI(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/Main_UI00.png",
		&g_pTextureUi);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void**)&pVtx, 0);

	// ユーザーインターフェースの中心座標
	g_posUi = D3DXVECTOR3(UI_POSX, UI_POSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posUi.x - UI_SIZEX, g_posUi.y + UI_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posUi.x - UI_SIZEX, g_posUi.y - UI_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posUi.x + UI_SIZEX, g_posUi.y + UI_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posUi.x + UI_SIZEX, g_posUi.y - UI_SIZEY, 0.0f);

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
	g_pVtxBuffUi->Unlock();

	return S_OK;
}

//==============================================================================
// ユーザーインターフェースの終了処理
//==============================================================================
void UninitUI(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}

	// テクスチャの開放
	if (g_pTextureUi != NULL)
	{
		g_pTextureUi->Release();
		g_pTextureUi = NULL;
	}
}

//==============================================================================
// ユーザーインターフェースの更新処理
//==============================================================================
void UpdateUI(void)
{
}

//==============================================================================
// ユーザーインターフェースの描画処理
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
		g_pVtxBuffUi,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureUi);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,				// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}