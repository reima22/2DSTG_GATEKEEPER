//==============================================================================
//
// 敵の描画〔enemy.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "enemy.h"
#include "player.h"
#include "block.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define ENEMY_LEFT_REMIT	(-20)		// 敵配置のカウント基準
#define ENEMY_RIGHT_REMIT	(1300)		// スコア倍率低の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// 敵テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 敵バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の情報

//==============================================================================
// 敵の初期化処理
//==============================================================================
HRESULT InitEnemy(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// 乱数算出から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[4]);

	// 変数の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->fWidth = ENEMY_SIZEX;
		pEnemy->fHeight = ENEMY_SIZEY;
		pEnemy->nType = NULL;
		pEnemy->bUse = false;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->nLife = 0;
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEnemy->nAnimCnt = 0;
		pEnemy->fTex = 0.0f;
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 敵の中心座標の設定
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 敵の頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//==============================================================================
// 敵の終了処理
//==============================================================================
void UninitEnemy(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
	{
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 敵の更新処理
//==============================================================================
void UpdateEnemy(void)
{
	AnimEnemy();
	MoveEnemy();
}

//==============================================================================
// 敵の描画処理
//==============================================================================
void DrawEnemy(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntEnemy * 4,			// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 敵の設定
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fWidth, float fHeight, int nType, int nLife)
{
	// ローカル変数宣言
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// 位置を設定
			pEnemy->pos = pos;

			// 大きさを設定
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;

			// 頂点座標を設定
			SetVertexEnemy(nCntEnemy);

			// 移動量の設定
			pEnemy->move = move;

			// 種類の設定
			pEnemy->nType = nType;

			// 敵の体力
			pEnemy->nLife = nLife;

			pEnemy->bUse = true;
			break;
		}
	}
}

//==============================================================================
// 敵の頂点座標取得処理
//==============================================================================
void SetVertexEnemy(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += nIdx * 4;

	pEnemy += nIdx;

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y - g_aEnemy[nIdx].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + g_aEnemy[nIdx].fWidth, g_aEnemy[nIdx].pos.y - g_aEnemy[nIdx].fHeight, 0.0f);
	
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// 敵の移動
//==============================================================================
void MoveEnemy(void)
{
	// ローカル変数宣言
	//VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		//前回の位置を保存
		pEnemy->posOld = pEnemy->pos;
	}

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->move.y < GRAVITY_ENEMY && pEnemy->nType != 4 && pEnemy->nType != 5)
			{
				pEnemy->move.y += DROP_SPEEDUP;
			}

			// 移動量の代入
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.y += pEnemy->move.y;

			if (CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, ENEMY_SIZEX, ENEMY_SIZEY, ONTYPE_ENEMY) == false && pEnemy->nType == 1)
			{
				pEnemy->move.x *= -1;
				pEnemy->pos.y -= DROP_SPEEDUP;
			}
			//CollisionBlock(&pEnemy->pos, &pEnemy->posOld, &pEnemy->move, ENEMY_SIZEX, ENEMY_SIZEY, ONTYPE_ENEMY);

			// 画面左でループ(右側へ移動)
			if (pEnemy->pos.x < ENEMY_LEFT_REMIT)
			{
				pEnemy->pos.x = ENEMY_RIGHT_REMIT;
				if (pEnemy->pos.y >= 690.0f)
				{
					pEnemy->pos.y = 130.0f;
				}
			}

			// 画面右でループ(左側へ移動)
			if (pEnemy->pos.x > ENEMY_RIGHT_REMIT)
			{
				pEnemy->pos.x = ENEMY_LEFT_REMIT;
				if (pEnemy->pos.y >= 690.0f)
				{
					pEnemy->pos.y = 130.0f;
				}
			}

			SetVertexEnemy(nCnt);
		}
	}
}

//==============================================================================
// 敵の取得
//==============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==============================================================================
// 敵のアニメーション
//==============================================================================
void AnimEnemy(void)
{
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++, pVtx += 4)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->nType == 4)
			{
				pEnemy->nAnimCnt++;
				if (pEnemy->nAnimCnt % 20 == 0)
				{
					pEnemy->fTex += 0.5f;
					if (pEnemy->fTex >= 1.0f)
					{
						pEnemy->fTex = 0.0f;
					}
				}

				// テクスチャ頂点情報の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + pEnemy->fTex, 1.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f + pEnemy->fTex, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f + pEnemy->fTex, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f + pEnemy->fTex, 0.0f);

				SetVertexEnemy(nCnt);
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}