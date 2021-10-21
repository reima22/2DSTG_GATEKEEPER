//==============================================================================
//
// メッシュ構造の軌跡の描画〔meshorbit.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "meshorbit.h"
#include "player.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshOrbit = NULL;	// インデックスバッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureMeshOrbit = NULL;
MeshOrbit g_MeshOrbit;

//==============================================================================
// メッシュ構造の軌跡の初期化処理
//==============================================================================
HRESULT InitMeshOrbit(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	WORD *pIdx;
	Player *pPlayer = GetPlayer();
	
	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/gradation000.jpg",
		&g_pTextureMeshOrbit);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * MESHPOINT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_MeshOrbit.pVtxBuff,
		NULL);

	g_MeshOrbit.aCol[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MeshOrbit.aCol[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	g_MeshOrbit.pMtxParent = &pPlayer->mtxWorld;

	g_MeshOrbit.aOffset[0] = pPlayer->aModel[0].pos;
	g_MeshOrbit.aOffset[1] = pPlayer->aModel[1].pos;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_MeshOrbit.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < (MESHPOINT / 2); nCnt++)
	{
		//pVtx[0].pos = g_MeshOrbit.aPosPoint[nCnt * 2];
		//pVtx[1].pos = g_MeshOrbit.aPosPoint[nCnt * 2 + 1];

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = g_MeshOrbit.aCol[0];
		pVtx[1].col = g_MeshOrbit.aCol[1];

		pVtx[0].tex = D3DXVECTOR2(1.0f * nCnt, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f * nCnt, 1.0f);

		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	g_MeshOrbit.pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * MESHPOINT,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshOrbit,
		NULL);

	// インデックスバッファをロックし、番号データへのポインタを取得
	g_pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < MESHPOINT; nCnt++)
	{
		pIdx[0] = (MESHPOINT / 2) + nCnt;
		pIdx[1] = pIdx[0] - (MESHPOINT / 2);
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffMeshOrbit->Unlock();

	return S_OK;
}

//==============================================================================
// メッシュ構造の軌跡の終了処理
//==============================================================================
void UninitMeshOrbit(void)
{
	// 頂点バッファの開放
	if (g_MeshOrbit.pVtxBuff != NULL)
	{
		g_MeshOrbit.pVtxBuff->Release();
		g_MeshOrbit.pVtxBuff = NULL;
	}

	// テクスチャの開放
	if (g_pTextureMeshOrbit != NULL)
	{
		g_pTextureMeshOrbit->Release();
		g_pTextureMeshOrbit = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshOrbit != NULL)
	{
		g_pIdxBuffMeshOrbit->Release();
		g_pIdxBuffMeshOrbit = NULL;
	}
}

//==============================================================================
// メッシュ構造の軌跡の更新処理
//==============================================================================
void UpdateMeshOrbit(void)
{

}

//==============================================================================
// メッシュ構造の軌跡の描画処理
//==============================================================================
void DrawMeshOrbit(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	Player *pPlayer = GetPlayer();
	VERTEX_3D *pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCnt = 0; nCnt < MESH_MAIN; nCnt++)
	{
		// マトリックスの初期化
		D3DXMatrixIdentity(&g_MeshOrbit.aMtxWorldPoint[nCnt]);

		// オフセットのワールドマトリックス算出
		D3DXMatrixTranslation(
			&mtxTrans,
			g_MeshOrbit.aOffset[nCnt].x,
			g_MeshOrbit.aOffset[nCnt].y,
			g_MeshOrbit.aOffset[nCnt].z);

		D3DXMatrixMultiply(
			&g_MeshOrbit.aMtxWorldPoint[nCnt],
			&g_MeshOrbit.aMtxWorldPoint[nCnt],
			&mtxTrans);

		// 親のマトリックスと掛け合わせる
		D3DXMatrixMultiply(
			&g_MeshOrbit.aMtxWorldPoint[nCnt],
			&g_MeshOrbit.aMtxWorldPoint[nCnt],
			g_MeshOrbit.pMtxParent);
	}

	// 頂点座標と頂点カラーをずらす
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_MeshOrbit.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = (MESHPOINT / 2 - 1); nCntOrbit >= 0; nCntOrbit--, pVtx += 2)
	{
		if (nCntOrbit == (MESHPOINT / 2))
		{
			g_MeshOrbit.aPosPoint[nCntOrbit * 2] = g_MeshOrbit.aOffset[0];
			g_MeshOrbit.aPosPoint[nCntOrbit * 2 - 1] = g_MeshOrbit.aOffset[1];
		}
		else
		{
			g_MeshOrbit.aPosPoint[nCntOrbit * 2] = g_MeshOrbit.aPosPoint[nCntOrbit * 2 + 2];
			g_MeshOrbit.aPosPoint[nCntOrbit * 2 - 1] = g_MeshOrbit.aPosPoint[nCntOrbit * 2 + 1];
		}

		pVtx[0].pos = g_MeshOrbit.aPosPoint[nCntOrbit * 2];
		pVtx[1].pos = g_MeshOrbit.aPosPoint[nCntOrbit * 2 - 1];	
	}

	// 頂点バッファをアンロックする
	g_MeshOrbit.pVtxBuff->Unlock();

	//for (int nCnt = 2; nCnt < MESHPOINT; nCnt += 2)
	//{
	//	// 数値をずらす
	//	g_MeshOrbit.aPosPoint[nCnt] = pos[0];
	//	g_MeshOrbit.aPosPoint[nCnt + 1] = pos[1];
	//	g_MeshOrbit.aColPoint[nCnt] = col[0];
	//	g_MeshOrbit.aColPoint[nCnt + 1] = col[1];
	//	// 変数の保存
	//	pos[0] = g_MeshOrbit.aPosPoint[nCnt];
	//	pos[1] = g_MeshOrbit.aPosPoint[nCnt + 1];
	//	col[0] = g_MeshOrbit.aColPoint[nCnt];
	//	col[1] = g_MeshOrbit.aColPoint[nCnt + 1];
	//}

	// 最新の頂点座標と頂点カラーを代入
	g_MeshOrbit.aPosPoint[0].x = g_MeshOrbit.aMtxWorldPoint[0]._41;
	g_MeshOrbit.aPosPoint[0].y = g_MeshOrbit.aMtxWorldPoint[0]._42;
	g_MeshOrbit.aPosPoint[0].z = g_MeshOrbit.aMtxWorldPoint[0]._43;
			   	
	g_MeshOrbit.aPosPoint[1].x = g_MeshOrbit.aMtxWorldPoint[1]._41;
	g_MeshOrbit.aPosPoint[1].y = g_MeshOrbit.aMtxWorldPoint[1]._42;
	g_MeshOrbit.aPosPoint[1].z = g_MeshOrbit.aMtxWorldPoint[1]._43;

	g_MeshOrbit.aColPoint[0] = g_MeshOrbit.aCol[0];
	g_MeshOrbit.aColPoint[1] = g_MeshOrbit.aCol[1];

	//// 位置を反映
	//D3DXMatrixTranslation(
	//	&mtxTrans, 
	//	g_MeshOrbit.pos.x, 
	//	g_MeshOrbit.pos.y,
	//	g_MeshOrbit.pos.z);

	//D3DXMatrixMultiply(
	//	g_MeshOrbit.pMtxParent,
	//	g_MeshOrbit.pMtxParent,
	//	&mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, g_MeshOrbit.pMtxParent);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_MeshOrbit.pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshOrbit);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshOrbit);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		MESHPOINT,				// 頂点数
		0,
		MESHPOINT - 2);		// プリミティブ数

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==============================================================================
// メッシュ構造の軌跡の設定
//==============================================================================
void SetMeshOrbit(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXCOLOR col0, D3DXCOLOR col1)
{
	g_MeshOrbit.aOffset[0] = pos0;
	g_MeshOrbit.aOffset[1] = pos1;
}

MeshOrbit GetOrbit(void)
{
	return g_MeshOrbit;
}