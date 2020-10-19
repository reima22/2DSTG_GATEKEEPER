//==============================================================================
//
// 残機表示の描画〔stock.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "stock.h"
#include "player.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define STOCK_POSX		(600)	// 残機表示のX座標
#define STOCK_POSY		(676)	// 残機表示のY座標
#define STOCK_SIZEX		(20)	// 残機表示の幅
#define STOCK_SIZEY		(30)	// 残機表示の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureStock = NULL;		// ユーザーインターフェーステクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posStock;							// 残機表示の中心の位置

//==============================================================================
// 残機表示の初期化処理
//==============================================================================
HRESULT InitStock(void)
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
		&g_pTextureStock);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	// ユーザーインターフェースの中心座標
	g_posStock = D3DXVECTOR3(STOCK_POSX, STOCK_POSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posStock.x - STOCK_SIZEX, g_posStock.y + STOCK_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posStock.x - STOCK_SIZEX, g_posStock.y - STOCK_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posStock.x + STOCK_SIZEX, g_posStock.y + STOCK_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posStock.x + STOCK_SIZEX, g_posStock.y - STOCK_SIZEY, 0.0f);

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
	g_pVtxBuffStock->Unlock();

	return S_OK;
}

//==============================================================================
// 残機表示の終了処理
//==============================================================================
void UninitStock(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}

	// テクスチャの開放
	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
	}
}

//==============================================================================
// 残機表示の更新処理
//==============================================================================
void UpdateStock(void)
{
	VERTEX_2D *pVtx;
	PLAYER *Player;

	// プレイヤーの取得
	Player = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)Player->nStock / 10), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)Player->nStock / 10), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)Player->nStock / 10), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)Player->nStock / 10), 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffStock->Unlock();
}

//==============================================================================
// 残機表示の描画処理
//==============================================================================
void DrawStock(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffStock,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, g_pTextureStock);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}