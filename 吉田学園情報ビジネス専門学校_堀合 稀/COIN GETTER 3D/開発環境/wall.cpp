//==============================================================================
//
// 壁の描画〔wall.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "wall.h"
#include "input.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	// バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		// テクスチャへのポインタ
//D3DXVECTOR3 g_posWall;							// 位置
//D3DXVECTOR3 g_rotWall;							// 向き
//D3DXMATRIX g_mtxWorldWall;						// ワールドマトリックス
Wall g_aWall[MAX_WALL];							// 壁の構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitWall(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block001.jpg",
		&g_pTextureWall);

	// 変数の初期化
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].posMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].fHeight = 0.0f;
		g_aWall[nCntWall].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++, pVtx += 4)
	{
		// ポリゴンの各頂点座標
		//pVtx[0].pos = D3DXVECTOR3(
		//	g_aWall[nCnt].pos.x - g_aWall[nCnt].fWidth,
		//	g_aWall[nCnt].pos.y,
		//	g_aWall[nCnt].pos.z);
		//pVtx[1].pos = D3DXVECTOR3(
		//	g_aWall[nCnt].pos.x - g_aWall[nCnt].fWidth,
		//	g_aWall[nCnt].pos.y + g_aWall[nCnt].fHeight,
		//	g_aWall[nCnt].pos.z);
		//pVtx[2].pos = D3DXVECTOR3(
		//	g_aWall[nCnt].pos.x + g_aWall[nCnt].fWidth,
		//	g_aWall[nCnt].pos.y,
		//	g_aWall[nCnt].pos.z);
		//pVtx[3].pos = D3DXVECTOR3(
		//	g_aWall[nCnt].pos.x + g_aWall[nCnt].fWidth,
		//	g_aWall[nCnt].pos.y + g_aWall[nCnt].fHeight,
		//	g_aWall[nCnt].pos.z);


		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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


		// テクスチャ頂点情報の設定
		//pVtx[0].tex = D3DXVECTOR2(0.0f, g_aWall[nCnt].fHeight / WALL_HEIGHT);
		//pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		//pVtx[2].tex = D3DXVECTOR2(g_aWall[nCnt].fWidth / WALL_WIDTH, g_aWall[nCnt].fHeight / WALL_HEIGHT);
		//pVtx[3].tex = D3DXVECTOR2(g_aWall[nCnt].fWidth / WALL_WIDTH, 0.0f);
	}

	//// ポリゴンの各頂点座標
	//pVtx[0].pos = D3DXVECTOR3(g_posWall.x - 100.0f, g_posWall.y , 100.0f);
	//pVtx[1].pos = D3DXVECTOR3(g_posWall.x - 100.0f, g_posWall.y + 50.0f, 100.0f);
	//pVtx[2].pos = D3DXVECTOR3(g_posWall.x + 100.0f, g_posWall.y , 100.0f);
	//pVtx[3].pos = D3DXVECTOR3(g_posWall.x + 100.0f, g_posWall.y + 50.0f, 100.0f);

	//// 法線ベクトルの設定
	//pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//// 各頂点カラーの設定
	//pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//// テクスチャ頂点情報の設定
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(2.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(2.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

	/*SetWall(D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 50.0f);
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f), 100.0f, 50.0f);
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(0.0f, D3DX_PI / -2, 0.0f), 100.0f, 50.0f);
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 90.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 100.0f, 50.0f);*/

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitWall(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	// テクスチャの開放
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateWall(void)
{
	// ローカル変数宣言
	Wall *pWall;
	pWall = &g_aWall[0];
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++, pVtx += 4)
	{
		if (pWall->bUse == true)
		{
			// ポリゴンの各頂点座標
			pVtx[0].pos = D3DXVECTOR3(
				pWall->posMove.x - pWall->fWidth, 
				pWall->posMove.y,
				pWall->posMove.z);
			pVtx[1].pos = D3DXVECTOR3(
				pWall->posMove.x - pWall->fWidth, 
				pWall->posMove.y + pWall->fHeight, 
				pWall->posMove.z);
			pVtx[2].pos = D3DXVECTOR3(
				pWall->posMove.x + pWall->fWidth, 
				pWall->posMove.y,
				pWall->posMove.z);
			pVtx[3].pos = D3DXVECTOR3(
				pWall->posMove.x + pWall->fWidth, 
				pWall->posMove.y + pWall->fHeight, 
				pWall->posMove.z);

			// テクスチャ頂点情報の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, pWall->fHeight / WALL_HEIGHT);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(pWall->fWidth / WALL_WIDTH, pWall->fHeight / WALL_HEIGHT);
			pVtx[3].tex = D3DXVECTOR2(pWall->fWidth / WALL_WIDTH, 0.0f);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawWall(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);

			// 向きの反映
			D3DXMatrixRotationYawPitchRoll(
				&mtxRot,
				g_aWall[nCntWall].rot.y,
				g_aWall[nCntWall].rot.x,
				g_aWall[nCntWall].rot.z);

			D3DXMatrixMultiply(
				&g_aWall[nCntWall].mtxWorld,
				&g_aWall[nCntWall].mtxWorld,
				&mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				g_aWall[nCntWall].pos.x,
				g_aWall[nCntWall].pos.y,
				g_aWall[nCntWall].pos.z);

			D3DXMatrixMultiply(
				&g_aWall[nCntWall].mtxWorld,
				&g_aWall[nCntWall].mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntWall * 4,			// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}	
	}

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&g_mtxWorldWall);

	//// 向きの反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotWall.y, g_rotWall.x, g_rotWall.z);
	//D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, g_posWall.x, g_posWall.y, g_posWall.z);
	//D3DXMatrixMultiply(&g_mtxWorldWall, &g_mtxWorldWall, &mtxTrans);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldWall);

	//// 頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	//// 頂点フォーマットの設定
	//pDevice->SetFVF(FVF_VERTEX_3D);

	//for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	//{
	//	if (g_aWall[nCntWall].bUse == true)
	//	{
	//		// テクスチャの設定
	//		pDevice->SetTexture(0, g_pTextureWall);

	//		// ポリゴンの描画
	//		pDevice->DrawPrimitive(
	//			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
	//			nCntWall * 4,			// 描画を開始する頂点インデックス
	//			2);						// 描画するプリミティブ数
	//	}
	//}

	//// テクスチャの設定
	//pDevice->SetTexture(0, g_pTextureWall);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(
	//	D3DPT_TRIANGLESTRIP,	// プリミティブの種類
	//	0,						// 描画を開始する頂点インデックス
	//	2);						// 描画するプリミティブ数
}

//==============================================================================
// 壁の設定
//==============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	// ローカル変数宣言
	Wall *pWall;
	pWall = &g_aWall[0];

	// 壁の設定
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == false)
		{
			pWall->posMove = pos;			// 位置

			pWall->rot = rot;			// 向き

			pWall->fWidth = fWidth;		// 幅

			pWall->fHeight = fHeight;	// 高さ

			pWall->bUse = true;			// 使用状態

			break;
		}
	}
}