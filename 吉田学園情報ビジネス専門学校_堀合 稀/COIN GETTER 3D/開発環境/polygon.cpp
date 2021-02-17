//==============================================================================
//
// 3Dポリゴンの描画〔polygon.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "polygon.h"
#include "input.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	// バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePolygon = NULL;		// テクスチャへのポインタ
D3DXVECTOR3 g_posPolygon;							// 位置
D3DXVECTOR3 g_rotPolygon;							// 向き
D3DXMATRIX g_mtxWorldPolygon;						// ワールドマトリックス
D3DXVECTOR3 g_movePolygon;
D3DXVECTOR3 g_moveRot;

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitPolygon(void)
{
	// ローカル変数宣言
	//VERTEX_2D *pVtx;
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/block000.jpg",
		&g_pTexturePolygon);

	// 変数の初期化
	g_posPolygon = D3DXVECTOR3(0.0f, -40.0f, 0.0f);
	g_rotPolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_moveRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, 0.0f, g_posPolygon.z - 100.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posPolygon.x - 100.0f, 0.0f, g_posPolygon.z + 100.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, 0.0f, g_posPolygon.z - 100.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posPolygon.x + 100.0f, 0.0f, g_posPolygon.z + 100.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitPolygon(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}

	// テクスチャの開放
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdatePolygon(void)
{
	//if (GetKeyboardPress(KEYINFO_UP) == true)
	//{
	//	g_movePolygon.z = 1.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_DOWN) == true)
	//{
	//	g_movePolygon.z = -1.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_LEFT) == true)
	//{
	//	g_movePolygon.x = -1.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	//{
	//	g_movePolygon.x = 1.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_ON) == true)
	//{
	//	g_moveRot.y = 0.05f;
	//}
	//else
	//{
	//	g_moveRot.y = 0.0f;
	//}

	//if (GetKeyboardPress(KEYINFO_UNDER) == true)
	//{
	//	g_moveRot.y = -0.05f;
	//}
	//else
	//{
	//	g_moveRot.y = 0.0f;
	//}

	//g_posPolygon.x += g_movePolygon.x;
	//g_posPolygon.y += g_movePolygon.y;
	//g_posPolygon.z += g_movePolygon.z;
	//g_rotPolygon.x += g_moveRot.x;
	//g_rotPolygon.y += g_moveRot.y;
	//g_rotPolygon.z += g_moveRot.z;
}

//==============================================================================
// 描画処理
//==============================================================================
void DrawPolygon(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPolygon.y, g_rotPolygon.x, g_rotPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posPolygon.x, g_posPolygon.y, g_posPolygon.z);
	D3DXMatrixMultiply(&g_mtxWorldPolygon, &g_mtxWorldPolygon, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}