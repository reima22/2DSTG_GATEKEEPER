//==============================================================================
//
// パーティクルの描画〔particle.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "particle.h"
//#include "enemy.h"
#include "player.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_PARTICLE	(4096)	// エフェクトポリゴン数の最大値

//==============================================================================
// パーティクルの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動
	D3DXCOLOR color;	// 色・透明度
	float fRadius;		// 半径
	float fAlphaDown;	// 透明度低下
	int nLife;			// 寿命
	bool bUse;			// 使用しているかどうか
	D3DXMATRIX mtxWorld;
	int nPattern;
} PARTICLE;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;		// バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureParticle = NULL;			// パーティクルテクスチャーへのポインタ
PARTICLE g_aParticle[MAX_PARTICLE];						// パーティクルの構造体

//==============================================================================
// パーティクルの初期化処理
//==============================================================================
HRESULT InitParticle(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/effect000.jpg",
		&g_pTextureParticle);

	// 構造体の初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].fAlphaDown = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
		g_aParticle[nCntParticle].nPattern = 0;
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// 位置と色の設定
		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].fRadius, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();



	return S_OK;
}

//==============================================================================
// パーティクルの終了処理
//==============================================================================
void UninitParticle(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}

	// テクスチャの開放
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
}

//==============================================================================
// パーティクルの更新処理
//==============================================================================
void UpdateParticle(void)
{
	// ローカル変数宣言
	PARTICLE *pParticle;
	VERTEX_3D *pVtx;

	// パーティクルの取得
	pParticle = &g_aParticle[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += 4, pParticle++)
	{
		if (pParticle->bUse == true)
		{ // エフェクトが使用されていた場合
			if(pParticle->nPattern == 0)
			{
				// 拡散の抑制
				pParticle->move.x *= 0.9f;
				pParticle->move.y *= 0.9f;
				pParticle->move.z *= 0.9f;
			}

			// 移動更新
			pParticle->pos += pParticle->move;

			// 透明度の減少
			pParticle->color.a -= pParticle->fAlphaDown;

			// 位置の設定
			pVtx[0].pos = D3DXVECTOR3(-pParticle->fRadius, -pParticle->fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(-pParticle->fRadius, pParticle->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pParticle->fRadius, -pParticle->fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pParticle->fRadius, pParticle->fRadius, 0.0f);

			// 各頂点カラーの設定
			pVtx[0].col = pParticle->color;
			pVtx[1].col = pParticle->color;
			pVtx[2].col = pParticle->color;
			pVtx[3].col = pParticle->color;

			// パーティクルの無効化
			if (pParticle->color.a <= 0)
			{
				pParticle->color.a = 0.0f;
				pParticle->bUse = false;
			}

			// 寿命の管理
			pParticle->nLife--;
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//==============================================================================
// パーティクルの描画処理
//==============================================================================
void DrawParticle(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	PARTICLE *pParticle = &g_aParticle[0];
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// 加算合成によるアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストの更新
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_PARTICLE; nCnt++,pParticle++)
	{
		// ポリゴンの生成
		if (pParticle->bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pParticle->mtxWorld);

			// ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&pParticle->mtxWorld, NULL, &mtxView);
			pParticle->mtxWorld._41 = 0.0f;
			pParticle->mtxWorld._42 = 0.0f;
			pParticle->mtxWorld._43 = 0.0f;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pParticle->pos.x, pParticle->pos.y, pParticle->pos.z);
			D3DXMatrixMultiply(&pParticle->mtxWorld, &pParticle->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pParticle->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
	
	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 通常のアルファブレンディング
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// エフェクトの設定
//==============================================================================
void SetEffect(D3DXVECTOR3 pos, float fMove, D3DXCOLOR color, float fRadius, float fAlphaDown,int nEffect)
{
	// ローカル変数宣言
	PARTICLE *pParticle;

	// パーティクルの取得
	pParticle = &g_aParticle[0];

	for (int nCntAppear = 0; nCntAppear < nEffect; nCntAppear++)
	{
		for (int nCntParticle = 0; nCntParticle < nEffect; nCntParticle++, pParticle++)
		{
			if (pParticle->bUse == false/* && pParticle->nPattern == 0*/)
			{ // 使用されていない場合
				// 位置を設定
				pParticle->pos = pos;

				// パーティクルの角度と動き
				float fAngle;
				fAngle = (float)((rand() % 628) - 314) / 100.0f;
				pParticle->move.x = (sinf(fAngle) * (float)(rand() % 500 + 1) * fMove);
				pParticle->move.y = (cosf(fAngle) * (float)(rand() % 500 + 1) * fMove);
				pParticle->move.z = (cosf(fAngle) * (float)(rand() % 500 + 1) * fMove);

				// パーティクルの色設定
				pParticle->color = color;

				// パーティクル1つの大きさ
 				pParticle->fRadius = fRadius;

				// パーティクルの透明度減少値
				pParticle->fAlphaDown = fAlphaDown;

				// パーティクルの寿命設定
				pParticle->nLife = 200;
				
				// パーティクルの有効化
				pParticle->bUse = true;
				break;
			}
		}
	}
}

//==============================================================================
// ランキングエフェクトの設定
//==============================================================================
void SetEffect1(D3DXVECTOR3 pos, float fMove, D3DXCOLOR color, float fRadius, float fAlphaDown, int nEffect,int nPattern)
{
	// ローカル変数宣言
	PARTICLE *pParticle;

	// パーティクルの取得
	pParticle = &g_aParticle[0];

	for (int nCntAppear = 0; nCntAppear < nEffect; nCntAppear++)
	{
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++, pParticle++)
		{
			if (pParticle->bUse == false)
			{ // 使用されていない場合
			  // 位置を設定
				pParticle->pos = pos;
				pParticle->nPattern = nPattern;

				// 移動量
				pParticle->move.z = fMove;

				// パーティクルの色設定
				pParticle->color = color;

				// パーティクル1つの大きさ
				pParticle->fRadius = fRadius;

				// パーティクルの透明度減少値
				pParticle->fAlphaDown = fAlphaDown;

				// パーティクルの寿命設定
				pParticle->nLife = 200;

				// パーティクルの有効化
				pParticle->bUse = true;
				break;
			}
		}
	}
}