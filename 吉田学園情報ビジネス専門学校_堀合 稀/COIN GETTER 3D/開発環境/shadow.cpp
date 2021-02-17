//==============================================================================
//
// 影の描画〔shadow.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "shadow.h"
#include "input.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			// テクスチャへのポインタ
Shadow g_aShadow[MAX_SHADOW];						// 影の構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitShadow(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/shadow000.jpg",
		&g_pTextureShadow);

	// 変数の初期化
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].fWidth = 1.0f;
		g_aShadow[nCntShadow].fDepth = 1.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++,pVtx += 4)
	{
		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aShadow[nCnt].pos.x - g_aShadow[nCnt].fWidth, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z - g_aShadow[nCnt].fDepth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCnt].pos.x - g_aShadow[nCnt].fWidth, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z + g_aShadow[nCnt].fDepth);
		pVtx[2].pos = D3DXVECTOR3(g_aShadow[nCnt].pos.x + g_aShadow[nCnt].fWidth, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z - g_aShadow[nCnt].fDepth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCnt].pos.x + g_aShadow[nCnt].fWidth, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z + g_aShadow[nCnt].fDepth);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.85f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitShadow(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	// テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateShadow(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void DrawShadow(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	Shadow *pShadow = &g_aShadow[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 減算合成の生成
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++,pShadow++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// 向きを反映
			pShadow->mtxWorld._11 = pShadow->fWidth;
			pShadow->mtxWorld._33 = pShadow->fDepth;

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}

	//// 頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_3D);

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureShadow);

	//for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	//{
	//	if (g_aShadow[nCnt].bUse == true)
	//	{
	//		// ポリゴンの描画
	//		pDevice->DrawPrimitive(
	//			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
	//			nCnt * 4,				// 描画を開始する頂点インデックス
	//			2);						// 描画するプリミティブ数
	//	}
	//}

	// 通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// 影の設定
//==============================================================================
int SetShadow(D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	// ローカル変数宣言
	Shadow *pShadow;
	pShadow = &g_aShadow[0];
	int nIdx = -1;

	// 影の設定
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++,pShadow++)
	{
		if (pShadow->bUse == false)
		{
			pShadow->pos = pos;			// 位置の設定

			pShadow->fWidth = fWidth;	// 幅の設定

			pShadow->fDepth = fDepth;	// 奥行の設定

			pShadow->bUse = true;		// 使用状態への移行

			nIdx = nCnt;				// インデックスへの代入

			break;
		}
	}

	return nIdx;	// nIdx番目として返す
}

//==============================================================================
// 影の追従
//==============================================================================
void SetPositionShadow(int nIdx, D3DXVECTOR3 pos)
{
	g_aShadow[nIdx].pos = pos;

	//// ローカル変数宣言
	//VERTEX_3D *pVtx;
	//Shadow *pShadow;
	//pShadow = &g_aShadow[0];

	//// 頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//// ポインタの移動
	//pVtx += nIdx * 4;
	//pShadow += nIdx;

	//// ポリゴンの各頂点座標
	//pVtx[0].pos = D3DXVECTOR3(pos.x - pShadow->fWidth, 0.0f, pos.z - pShadow->fDepth);
	//pVtx[1].pos = D3DXVECTOR3(pos.x - pShadow->fWidth, 0.0f, pos.z + pShadow->fDepth);
	//pVtx[2].pos = D3DXVECTOR3(pos.x + pShadow->fWidth, 0.0f, pos.z - pShadow->fDepth);
	//pVtx[3].pos = D3DXVECTOR3(pos.x + pShadow->fWidth, 0.0f, pos.z + pShadow->fDepth);

	//// 頂点バッファをアンロックする
	//g_pVtxBuffShadow->Unlock();
}

Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}