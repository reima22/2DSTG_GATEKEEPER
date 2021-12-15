//==============================================================================
//
// 爆発の描画〔explosion.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_EXPLOSION	(256)	// 爆発の最大値
#define TEXTURE_CUT		(8)		// テクスチャー分割数
#define COUNTANIM		(4)		// 爆破アニメーションのカウント
#define EXPLOSION_SIZE	(20)	// 爆発のサイズ

//==============================================================================
// 爆発の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXCOLOR col;		// 色
	int nSize;			// 大きさ
	int nCounterAnim;	// アニメーションカウンター
	int nPatternAnim;	// アニメーションパターン№
	bool bUse;			// 使用しているかどうか
} EXPLOSION;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;		// 爆発テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 爆発バッファへのポインタ
EXPLOSION g_aExplosion[MAX_EXPLOSION];

//==============================================================================
// 爆発の初期化処理
//==============================================================================
HRESULT InitExplosion(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/explosion000.png",
		&g_pTextureExplosion);

	// g_aExplosionの初期化
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		g_aExplosion[nCntExplosion].nSize = EXPLOSION_SIZE;
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
	}
	

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f / TEXTURE_CUT, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TEXTURE_CUT, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();

	return S_OK;
}

//==============================================================================
// 爆発の終了処理
//==============================================================================
void UninitExplosion(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}

	// テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

//==============================================================================
// 爆発の更新処理
//==============================================================================
void UpdateExplosion(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;
	ENEMY *pEnemy;
	PLAYER *Player;
	
	// 爆発の取得
	pExplosion = &g_aExplosion[0];

	// 敵の取得
	pEnemy = GetEnemy();

	// プレイヤーの取得
	Player = GetPlayer();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx = pVtx + 4, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{ // 爆発が使用されていた場合
			pExplosion->nCounterAnim++;	// カウント開始

			// カウントが4の倍数で変化
			if ((pExplosion->nCounterAnim % COUNTANIM) == 0)
			{
				// テクスチャのパターンの更新
				pExplosion->nPatternAnim++;
				pExplosion->col.a += -0.1f;
				pExplosion->nSize += 3;

				// テクスチャの頂点座標の更新
				pVtx[0].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT), 1.0f);
				pVtx[1].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT) + (1.0f / TEXTURE_CUT), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(pExplosion->nPatternAnim * (1.0f / TEXTURE_CUT) + (1.0f / TEXTURE_CUT), 0.0f);

				// 頂点座標の更新
				pVtx[0].pos = D3DXVECTOR3(pExplosion->pos.x - pExplosion->nSize, pExplosion->pos.y + pExplosion->nSize, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pExplosion->pos.x - pExplosion->nSize, pExplosion->pos.y - pExplosion->nSize, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pExplosion->pos.x + pExplosion->nSize, pExplosion->pos.y + pExplosion->nSize, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pExplosion->pos.x + pExplosion->nSize, pExplosion->pos.y - pExplosion->nSize, 0.0f);

				// 各頂点カラーの設定
				pVtx[0].col = pExplosion->col;
				pVtx[1].col = pExplosion->col;
				pVtx[2].col = pExplosion->col;
				pVtx[3].col = pExplosion->col;

				// アニメーション終了
				if (pExplosion->nPatternAnim >= TEXTURE_CUT)
				{
					pExplosion->bUse = false;
					pExplosion->nCounterAnim = 0;
					pExplosion->nPatternAnim = 0;
					pExplosion->nSize = EXPLOSION_SIZE;
					pExplosion->col.a = 1.0f;
				}
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//==============================================================================
// 爆発の描画処理
//==============================================================================
void DrawExplosion(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffExplosion,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	// ポリゴンの描画
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntExplosion * 4,		// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 爆発の設定
//==============================================================================
void SetExplosion(D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	EXPLOSION *pExplosion;

	// 爆発の取得
	pExplosion = &g_aExplosion[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{ // 使用されていない場合
			// サウンドの再生
			//PlaySound(SOUND_LABEL_SE_EXPLOSION000);

			// 位置を設定
			pExplosion->pos = pos;

			// アニメーションの有効化
			pExplosion->bUse = true;
			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}