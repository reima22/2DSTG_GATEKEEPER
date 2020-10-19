//==============================================================================
//
// アイテムの描画〔item.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "item.h"
#include "score.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureItem[MAX_TYPE_ITEM] = {};	// アイテムテクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;			// バッファへのポインタ
ITEM g_aItem[MAX_ITEM];									// アイテムの情報

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの取得
	pItem = &g_aItem[0];

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/item000.png",
		&g_apTextureItem[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/item001.png",
		&g_apTextureItem[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/item002.png",
		&g_apTextureItem[2]);

	// 変数の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->nType = NULL;
		pItem->bUse = false;
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// 敵の中心座標の設定
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 敵の頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZEX, g_aItem[nCntItem].pos.y + ITEM_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_SIZEX, g_aItem[nCntItem].pos.y - ITEM_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZEX, g_aItem[nCntItem].pos.y + ITEM_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_SIZEX, g_aItem[nCntItem].pos.y - ITEM_SIZEY, 0.0f);

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

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

	return S_OK;
}

//==============================================================================
// アイテムの終了処理
//==============================================================================
void UninitItem(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
	{
		if (g_apTextureItem[nCnt] != NULL)
		{
			g_apTextureItem[nCnt]->Release();
			g_apTextureItem[nCnt] = NULL;
		}
	}
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void UpdateItem(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ITEM *pItem;

	// アイテムの取得
	pItem = &g_aItem[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++, pVtx = pVtx + 4)
	{
		if (pItem->bUse == true)
		{ // アイテムが使用されていた(現れていた)場合
		  // 位置の更新
			pItem->pos.y += 3.0f;

			// 頂点座標の更新
			SetVertexItem(nCntItem);

			// 画面外でのアイテムの消滅
			if (pItem->pos.y >= SCREEN_HEIGHT)
			{
				pItem->bUse = false;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//==============================================================================
// アイテムの描画処理
//==============================================================================
void DrawItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの取得
	pItem = &g_aItem[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffItem,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			// テクスチャの設定
 			pDevice->SetTexture(0, g_apTextureItem[pItem->nType]);

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntItem * 4,			// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// アイテムの設定
//==============================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	// ローカル変数宣言
	ITEM *pItem;
	VERTEX_2D *pVtx;

	// アイテムの取得
	pItem = &g_aItem[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < 56; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{ // アイテムが使用されていない場合
			// 位置を設定
			pItem->pos = pos;

			// 頂点座標を設定
			SetVertexItem(nCntItem);

			// 種類の設定
			pItem->nType = nType;

			// アイテムの有効化
			pItem->bUse = true;
			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//==============================================================================
// アイテムの頂点座標取得処理
//==============================================================================
void SetVertexItem(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ITEM *pItem;

	// アイテムの取得
	pItem = &g_aItem[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += nIdx * 4;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aItem[nIdx].pos.x - ITEM_SIZEX, g_aItem[nIdx].pos.y + ITEM_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aItem[nIdx].pos.x - ITEM_SIZEX, g_aItem[nIdx].pos.y - ITEM_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aItem[nIdx].pos.x + ITEM_SIZEX, g_aItem[nIdx].pos.y + ITEM_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aItem[nIdx].pos.x + ITEM_SIZEX, g_aItem[nIdx].pos.y - ITEM_SIZEY, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//==============================================================================
// 敵の取得
//==============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}