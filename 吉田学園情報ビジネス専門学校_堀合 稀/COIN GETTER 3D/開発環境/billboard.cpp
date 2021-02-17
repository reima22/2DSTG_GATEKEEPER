//==============================================================================
//
// ビルボードの描画〔billboard.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "billboard.h"
//#include "input.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;		// テクスチャへのポインタ
D3DXVECTOR3 g_posBillboard;							// 位置
D3DXMATRIX g_mtxWorldBillboard;						// ワールドマトリックス
//Billboard g_aBillboard[MAX_WALL];					// ビルボードの構造体

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT InitBillboard(void)
{
	// ローカル変数宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy003.png",
		&g_pTextureBillboard);

	g_posBillboard = D3DXVECTOR3(-30.0f, 30.0f, 0.0f);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCnt = 0; nCnt < MAX_WALL; nCnt++, pVtx += 4)
	//{
	//	// ポリゴンの各頂点座標
	//	pVtx[0].pos = D3DXVECTOR3(g_aWall[nCnt].pos.x - 100.0f, 0.0f, 100.0f);
	//	pVtx[1].pos = D3DXVECTOR3(g_aWall[nCnt].pos.x - 100.0f, g_aWall[nCnt].pos.y + 50.0f, 100.0f);
	//	pVtx[2].pos = D3DXVECTOR3(g_aWall[nCnt].pos.x + 100.0 , 0.0f, 100.0f);
	//	pVtx[3].pos = D3DXVECTOR3(g_aWall[nCnt].pos.x + 100.0f, g_aWall[nCnt].pos.y + 50.0f, 100.0f);

	//	// 法線ベクトルの設定
	//	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	//	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//	// 各頂点カラーの設定
	//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	// テクスチャ頂点情報の設定
	//	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	//	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	//	pVtx[2].tex = D3DXVECTOR2(2.0f, 1.0f);
	//	pVtx[3].tex = D3DXVECTOR2(2.0f, 0.0f);
	//}

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posBillboard.x - 10.0f, g_posBillboard.y - 10.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posBillboard.x - 10.0f, g_posBillboard.y + 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posBillboard.x + 10.0f, g_posBillboard.y - 10.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posBillboard.x + 10.0f, g_posBillboard.y + 10.0f, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBillboard->Unlock();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitBillboard(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}

	// テクスチャの開放
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdateBillboard(void)
{

}

//==============================================================================
// 描画処理
//==============================================================================
void DrawBillboard(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxTrans;	// 計算用マトリックス

	// デバイスの取得
	pDevice = GetDevice();

	// ライトをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテストの更新
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数

	// 元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);

	// ライトをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}