//==============================================================================
//
// ブロック移動点の描画〔blockpoint.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "blockpoint.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlockPoint = NULL;			// ブロックテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlockPoint = NULL;	// バッファへのポインタ
BLOCKPOINT g_aBlockPoint[POINT_MAX];					// ブロック移動点の情報

//==============================================================================
// ブロックの初期化処理
//==============================================================================
HRESULT InitBlockPoint(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/point000.png",
		&g_pTextureBlockPoint);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POINT_MAX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlockPoint,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++)
	{
		g_aBlockPoint[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlockPoint[nCnt].fHeight = 0.0f;
		g_aBlockPoint[nCnt].fWidth = 0.0f;
		g_aBlockPoint[nCnt].bUse = false;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++, pVtx += 4)
	{
		// ブロックの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x, g_aBlockPoint[nCnt].pos.y + g_aBlockPoint[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x, g_aBlockPoint[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x + BLOCK_SIZEX, g_aBlockPoint[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlockPoint[nCnt].pos.x + BLOCK_SIZEX, g_aBlockPoint[nCnt].pos.y, 0.0f);

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
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlockPoint->Unlock();

	return S_OK;
}

//==============================================================================
// ブロックの終了処理
//==============================================================================
void UninitBlockPoint(void)
{
	// ローカル変数宣言
	BLOCKPOINT *pBlockPoint;

	// ブロックの取得
	pBlockPoint = &g_aBlockPoint[0];

	// ブロックの移動
	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++, pBlockPoint++)
	{
		if (pBlockPoint->bUse == true)
		{
			pBlockPoint->bUse = false;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBlockPoint != NULL)
	{
		g_pVtxBuffBlockPoint->Release();
		g_pVtxBuffBlockPoint = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBlockPoint != NULL)
	{				   
		g_pTextureBlockPoint->Release();
		g_pTextureBlockPoint = NULL;
	}
}

//==============================================================================
// ブロックの更新処理
//==============================================================================
void UpdateBlockPoint(void)
{
	// ローカル変数宣言
	BLOCKPOINT *pBlockPoint;

	// 移動点の取得
	pBlockPoint = &g_aBlockPoint[0];

	// 移動点の頂点座標更新
	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++)
	{
		if (pBlockPoint->bUse == true)
		{
			// ブロックの頂点座標取得
			SetVertexPoint(nCnt);
		}
	}
}

//==============================================================================
// ブロックの描画処理
//==============================================================================
void DrawBlockPoint(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	BLOCKPOINT *pBlockPoint = &g_aBlockPoint[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlockPoint,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++,pBlockPoint++)
	{
		if (pBlockPoint->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlockPoint);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 移動点の取得
//==============================================================================
BLOCKPOINT *GetPoint(void)
{
	return &g_aBlockPoint[0];
}

//==============================================================================
// ブロックの設定
//==============================================================================
void SetPoint(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	BLOCKPOINT *pBlockPoint = &g_aBlockPoint[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < POINT_MAX; nCnt++,pBlockPoint++)
	{
		if (pBlockPoint->bUse == false)
		{
			// 位置を設定
			pBlockPoint->pos = pos;

			// 幅の設定
			pBlockPoint->fWidth = fWidth;

			// 高さの設定
			pBlockPoint->fHeight = fHeight;

			// 頂点座標を設定
			SetVertexBlock(nCnt);

			// 使用状態へ移行
			pBlockPoint->bUse = true;

			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlockPoint->Unlock();
}

//==============================================================================
// ブロックの頂点座標取得処理
//==============================================================================
void SetVertexPoint(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	BLOCKPOINT *pBlockPoint;

	// 敵の取得
	pBlockPoint = &g_aBlockPoint[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlockPoint->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += nIdx * 4;

	pBlockPoint += nIdx;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(pBlockPoint->pos.x, pBlockPoint->pos.y + pBlockPoint->fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pBlockPoint->pos.x, pBlockPoint->pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pBlockPoint->pos.x + pBlockPoint->fWidth, pBlockPoint->pos.y + pBlockPoint->fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pBlockPoint->pos.x + pBlockPoint->fWidth, pBlockPoint->pos.y, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBlockPoint->Unlock();
}