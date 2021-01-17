//==============================================================================
//
// アイテムの描画〔item.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "item.h"
#include "game.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureItem = NULL;		// アイテムテクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// バッファへのポインタ
ITEM g_aItem[MAX_ITEM];							// アイテムの情報
int g_nAnimCnt;									// アニメーションカウント
int g_nAnimCol;									// 色変化カウント

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;
	int nRand;

	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの取得
	pItem = &g_aItem[0];

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/coin000.png",
		&g_apTextureItem);

	nRand = RandItem();		// ハイスコアアイテムの選定

	// 構造体の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pItem->nType = NULL;
		pItem->bUse = false;
		pItem->fAnim = 0.0f;
		if (nCntItem == nRand)
		{
			pItem->bHiScore = true;
		}
		else
		{
			pItem->bHiScore = false;
		}
	}

	// 変数の初期化
	g_nAnimCnt = 0;
	g_nAnimCol = 0;

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
		pVtx[0].col = g_aItem[nCntItem].col;
		pVtx[1].col = g_aItem[nCntItem].col;
		pVtx[2].col = g_aItem[nCntItem].col;
		pVtx[3].col = g_aItem[nCntItem].col;

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
	if (g_apTextureItem != NULL)
	{
		g_apTextureItem->Release();
		g_apTextureItem = NULL;
	}
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void UpdateItem(void)
{
	// ローカル変数宣言
	ITEM *pItem;

	// アイテムの取得
	pItem = &g_aItem[0];

	g_nAnimCnt = (g_nAnimCnt + 1) % 8;		// アニメーションカウントの増加
	g_nAnimCol = (g_nAnimCol + 1) % 70;		// 色変化カウントの増加

	if (g_nAnimCnt == 0)
	{
		for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++, pItem++)
		{
			// テクスチャ座標の遷移
			pItem->fAnim += 0.25f;

			// 頂点座標を設定
			SetVertexItem(nCnt);
		}
	}

	// アイテムの取得
	pItem = &g_aItem[0];

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++, pItem++)
	{
		if (pItem->bHiScore == true)
		{
			if (g_nAnimCol <= 30)
			{// 赤色化
				pItem->col.g = 0.0f;
			}
			else
			{// 元に戻る
				pItem->col.g = 1.0f;
			}
		}

		// 頂点座標を設定
		SetVertexItem(nCnt);
	}
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
 			pDevice->SetTexture(0, g_apTextureItem);

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

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
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

	// 頂点カラーの更新
	pVtx[0].col = g_aItem[nIdx].col;
	pVtx[1].col = g_aItem[nIdx].col;
	pVtx[2].col = g_aItem[nIdx].col;
	pVtx[3].col = g_aItem[nIdx].col;

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + g_aItem[nIdx].fAnim, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + g_aItem[nIdx].fAnim, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f + g_aItem[nIdx].fAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + g_aItem[nIdx].fAnim, 0.0f);

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

//==============================================================================
// ランダムでハイスコアアイテムの選定
//==============================================================================
int RandItem(void)
{
	// ローカル変数宣言
	ITEM *pItem;
	int nStage = GetStage();
	int nHigh;

	// アイテムの取得
	pItem = &g_aItem[0];

	if (nStage == 0)
	{
		nHigh = rand() % COIN_STAGE1;
	}
	else if (nStage == 1)
	{
		nHigh = rand() % COIN_STAGE2;
	}
	else if(nStage == 2)
	{
		nHigh = rand() % COIN_STAGE3;
	}

	return nHigh;
}