//==============================================================================
//
// ブロックの描画〔block.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "block.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "gamepad.h"
#include "blockpoint.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlock = NULL;			// ブロックテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		// バッファへのポインタ
BLOCK g_aBlock[BLOCK_MAX];							// ブロックの情報

//==============================================================================
// ブロックの初期化処理
//==============================================================================
HRESULT InitBlock(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block000.jpg",
		&g_pTextureBlock);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BLOCK_MAX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		g_aBlock[nCnt].bUse = false;
		g_aBlock[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCnt].type = ONTYPE_NONE;
		g_aBlock[nCnt].nMoveCnt = 0;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pVtx += 4)
	{
		// ブロックの中心座標
		g_aBlock[nCnt].pos = D3DXVECTOR3(0.0f, -30.0f, 0.0f);

		// ブロックの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x, g_aBlock[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x, g_aBlock[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + BLOCK_SIZEX, g_aBlock[nCnt].pos.y + BLOCK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCnt].pos.x + BLOCK_SIZEX, g_aBlock[nCnt].pos.y, 0.0f);

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
	g_pVtxBuffBlock->Unlock();

	return S_OK;
}

//==============================================================================
// ブロックの終了処理
//==============================================================================
void UninitBlock(void)
{
	// ローカル変数宣言
	BLOCK *pBlock;

	// ブロックの取得
	pBlock = &g_aBlock[0];

	// ブロックの移動
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}
}

//==============================================================================
// ブロックの更新処理
//==============================================================================
void UpdateBlock(void)
{
	// ローカル変数宣言
	BLOCK *pBlock;

	// ブロックの取得
	pBlock = &g_aBlock[0];

	// ブロックの横移動
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		pBlock->nMoveCnt++;

		if (pBlock->bUse == true)
		{
			if (pBlock->move.x != 0)
			{ // 横移動の間隔
				if (pBlock->nMoveCnt % pBlock->nMaxCnt == 0)
				{
					pBlock->move.x *= -1;
				}
			}

			// ブロックの移動処理
			pBlock->pos += pBlock->move;
			
			// ブロックの頂点座標取得
			SetVertexBlock(nCnt);
		}
	}

	// ブロックの取得
	pBlock = &g_aBlock[0];

	// ブロックの縦移動
	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if (pBlock->move.y != 0.0f)
			{ // 縦移動の間隔
				if (pBlock->nMoveCnt % pBlock->nMaxCnt == 0)
				{
					pBlock->move.y *= -1;
				}
			}

			// ブロックの頂点座標取得
			SetVertexBlock(nCnt);
		}
	}
}

//==============================================================================
// ブロックの描画処理
//==============================================================================
void DrawBlock(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBlock);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// ブロックの設定
//==============================================================================
void SetBlock(D3DXVECTOR3 pos,D3DXVECTOR3 move, float fWidth, float fHeight,int nMaxCnt)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < BLOCK_MAX; nCnt++)
	{
		if (g_aBlock[nCnt].bUse == false)
		{
			// 位置を設定
			g_aBlock[nCnt].pos = pos;

			// 移動力を設定
			g_aBlock[nCnt].move = move;

			// 幅の設定
			g_aBlock[nCnt].fWidth = fWidth;

			// 高さの設定
			g_aBlock[nCnt].fHeight = fHeight;

			// 移動カウントの設定
			g_aBlock[nCnt].nMaxCnt = nMaxCnt;

			// 頂点座標を設定
			SetVertexBlock(nCnt);

			// 使用状態へ移行
			g_aBlock[nCnt].bUse = true;

			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//==============================================================================
// ブロックの頂点座標取得処理
//==============================================================================
void SetVertexBlock(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	BLOCK *pBlock;

	// 敵の取得
	pBlock = &g_aBlock[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += nIdx * 4;

	pBlock += nIdx;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x, g_aBlock[nIdx].pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y + g_aBlock[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aBlock[nIdx].pos.x + g_aBlock[nIdx].fWidth, g_aBlock[nIdx].pos.y, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//==============================================================================
// ブロックの情報取得
//==============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//==============================================================================
// ブロックの当たり判定の設定
//==============================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, ONTYPE type)
{
	// ローカル変数宣言
	BLOCK *pBlock;
	bool bLand = false;		// 着地したかどうか
	pBlock = &g_aBlock[0];	// ブロックの取得

	// 当たり判定
	for (int nCntBlock = 0; nCntBlock < BLOCK_MAX; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->type = type;	// 触れているものの種類

			if (pPos->y >= pPosOld->y && pPos->y >= pBlock->pos.y && pPosOld->y - 2 <= pBlock->pos.y)
			{ // 足元の判定
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth > pBlock->pos.x &&
					pPos->x - fWidth < pBlock->pos.x + BLOCK_SIZEX)
				{ // ブロックの当たり判定
					pPos->y = pBlock->pos.y;	// 足場の更新
					bLand = true;				// 着地判定
					//pPosOld = pPos;				// 前回の位置の更新

					if (pBlock->type == ONTYPE_PLAYER)
					{ // プレイヤーの判定
						if (GetKeyboardPress(KEYINFO_LEFT) == false &&
							GetKeyboardPress(KEYINFO_RIGHT) == false &&
							IsButtonPush(KEYINFO::KEYINFO_LEFT) == false &&
							IsButtonPush(KEYINFO::KEYINFO_RIGHT) == false)
						{ // 左右移動操作を行わない
							pMove->x = pBlock->move.x;
							pMove->y = pBlock->move.y;
						}
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{
						if (pBlock->move.y != 0)
						{
							pMove->y = pBlock->move.y;
						}
					}

					pMove->y = 0.0f;	// 足場への定着

					if (pBlock->move.y > 0.0f)
					{ // 足場下降時
						pMove->y = pBlock->move.y;
					}

					if (pBlock->move.y < 0.0f)
					{
						pPos->y += pBlock->move.y;
					}
				}
			}
			else if (pPos->y <= pPosOld->y && pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY && pPosOld->y - fHeight >= pBlock->pos.y + BLOCK_SIZEY)
			{ // 頭上の判定
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + (fWidth + pMove->x) > pBlock->pos.x &&
					pPos->x - (fWidth + pMove->x) < pBlock->pos.x + BLOCK_SIZEX)
				{ // ブロックの当たり判定
					pPos->y = pBlock->pos.y + BLOCK_SIZEY + fHeight;
					pMove->y = 0.1f;
					//pPosOld = pPos;
				}
			}
			else if (pPos->x >= pPosOld->x && pPos->x < pBlock->pos.x)
			{ // ブロック左側面の判定
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth >= pBlock->pos.x &&
					pPos->x - fWidth <= pBlock->pos.x + BLOCK_SIZEX)
				{ // ブロックの当たり判定
					pPos->x = pBlock->pos.x - fWidth - 1;

					if (pBlock->type == ONTYPE_PLAYER)
					{ // プレイヤーの判定
						pMove->x = 0.0f;
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{ // 敵の判定
						pMove->x *= -1.0f;
					}

					//pPosOld = pPos;
				}
			}
			else if (pPos->x <= pPosOld->x && pPos->x > pBlock->pos.x)
			{ // ブロック右側面の判定
				if (pPos->y >= pBlock->pos.y &&
					pPos->y - fHeight <= pBlock->pos.y + BLOCK_SIZEY &&
					pPos->x + fWidth >= pBlock->pos.x &&
					pPos->x - fWidth <= pBlock->pos.x + BLOCK_SIZEX)
				{ // ブロックの当たり判定
 					pPos->x = pBlock->pos.x + BLOCK_SIZEX + fWidth + 1;

					if (pBlock->type == ONTYPE_PLAYER)
					{ // プレイヤーの判定
						pMove->x = 0.0f;
					}
					else if (pBlock->type == ONTYPE_ENEMY)
					{
						pMove->x *= -1.0f;
					}

					//pPosOld = pPos;
				}
			}

		}
	}

	return bLand;
}