////==============================================================================
////
//// 爆発の描画〔explosion.cpp〕
//// AUTHOR : MARE HORIAI
////
////==============================================================================
//#include "effect.h"
//#include "enemy.h"
//#include "player.h"
//#include "sound.h"
//
////==============================================================================
//// 爆発の構造体
////==============================================================================
//typedef struct
//{
//	D3DXVECTOR3 pos;	// 位置
//	D3DXCOLOR color;	// 色・透明度
//	float fAlphaDown;	// α減少
//	int nSizeX;			// 幅
//	int nSizeY;			// 高さ
//	bool bUse;			// 使用しているかどうか
//} EFFECT;
//
////==============================================================================
//// グローバル変数
////==============================================================================
//LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;			// エフェクトテクスチャーへのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// バッファへのポインタ
//EFFECT g_aEffect[MAX_EFFECT];
//
////==============================================================================
//// エフェクトの初期化処理
////==============================================================================
//HRESULT InitEffect(void)
//{
//	// ローカル変数宣言
//	VERTEX_2D *pVtx;
//	LPDIRECT3DDEVICE9 pDevice;
//
//	// デバイスの取得
//	pDevice = GetDevice();
//
//	// テクスチャの読み込み
//	D3DXCreateTextureFromFile(
//		pDevice,
//		"data/TEXTURE/effect000.jpg",
//		&g_pTextureEffect);
//
//	// g_aExplosionの初期化
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_aEffect[nCntEffect].color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
//		g_aEffect[nCntEffect].fAlphaDown = 0.0f;
//		g_aEffect[nCntEffect].nSizeX = 0;
//		g_aEffect[nCntEffect].nSizeY = 0;
//		g_aEffect[nCntEffect].bUse = false;
//	}
//
//	// 頂点情報の設定
//	if (FAILED(pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffEffect,
//		NULL)))
//	{
//		return E_FAIL;
//	}
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		// rhwの設定
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//
//		// 各頂点カラーの設定
//		pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
//
//		// テクスチャ頂点情報の設定
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
//
//		pVtx += 4;	// 頂点ポインタを4ずつ進める
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffEffect->Unlock();
//
//	return S_OK;
//}
//
////==============================================================================
//// エフェクトの終了処理
////==============================================================================
//void UninitEffect(void)
//{
//	// 頂点バッファの開放
//	if (g_pVtxBuffEffect != NULL)
//	{
//		g_pVtxBuffEffect->Release();
//		g_pVtxBuffEffect = NULL;
//	}
//
//	// テクスチャの開放
//	if (g_pTextureEffect != NULL)
//	{
//		g_pTextureEffect->Release();
//		g_pTextureEffect = NULL;
//	}
//}
//
////==============================================================================
//// エフェクトの更新処理
////==============================================================================
//void UpdateEffect(void)
//{
//	// ローカル変数宣言
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//	VERTEX_2D *pVtx;
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pVtx += 4, pEffect++)
//	{
//		if (pEffect->bUse == true)
//		{ // エフェクトが使用されていた場合
//
//			g_aEffect[nCntEffect].color.a -= g_aEffect[nCntEffect].fAlphaDown;
//
//			if (g_aEffect[nCntEffect].color.a <= 0)
//			{
//				g_aEffect[nCntEffect].color.a = 0.0f;
//				g_aEffect[nCntEffect].bUse = false;
//			}
//		}
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffEffect->Unlock();
//}
//
////==============================================================================
//// エフェクトの描画処理
////==============================================================================
//void DrawEffect(void)
//{
//	// ローカル変数宣言
//	LPDIRECT3DDEVICE9 pDevice;
//
//	// デバイスの取得
//	pDevice = GetDevice();
//
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(
//		0,
//		g_pVtxBuffEffect,
//		0,
//		sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureEffect);
//
//	// ポリゴンの描画
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
//	{
//		if (g_aEffect[nCntEffect].bUse == true)
//		{
//			pDevice->DrawPrimitive(
//				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
//				nCntEffect * 4,			// 描画を開始する頂点インデックス
//				2);						// 描画するプリミティブ数
//		}
//	}
//
//	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//}
//
////==============================================================================
//// エフェクトの設定
////==============================================================================
//void SetEffect(D3DXVECTOR3 pos,int nType)
//{
//	// ローカル変数宣言
//	VERTEX_2D *pVtx;
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
//	{
//		if (pEffect->bUse == false)
//		{ // 使用されていない場合
//			// サウンドの再生
//			//PlaySound(SOUND_LABEL_SE_EXPLOSION000);
//
//			// 位置を設定
//			pEffect->pos = pos;
//
//			// エフェクトの透明度設定
// 			pEffect->color.a = 1.0f;
//
//			// エフェクトの頂点座標設定
//			SetVertexEffect(nCntEffect, nType);
//
//			// アニメーションの有効化
//			pEffect->bUse = true;
//			break;
//		}
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffEffect->Unlock();
//}
//
////==============================================================================
//// エフェクトの頂点座標設定
////==============================================================================
//void SetVertexEffect(int nIdx,int nType)
//{
//	// ローカル変数宣言
//	VERTEX_2D *pVtx;
//	EFFECT *pEffect;
//	pEffect = &g_aEffect[0];
//
//	// 頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点ポインタを4ずつ進める
//	pVtx += 4 * nIdx;
//
//	// バレットポインタを進める
//	pEffect += nIdx;
//
//	if (nType == 0)
//	{ // 弾A(通常弾)の場合
//	  // 頂点座標の設定
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 10, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 10, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 10, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 10, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.15f;
//	}
//	else if (nType == 1)
//	{ // 弾B(ウェーブ)の場合
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.09f;
//	}
//	else if (nType == 2)
//	{ // 弾C(ミサイル)の場合
//	  // ミサイルの各頂点座標
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 5, g_aEffect[nIdx].pos.y + 5, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 5, g_aEffect[nIdx].pos.y - 5, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 5, g_aEffect[nIdx].pos.y + 5, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 5, g_aEffect[nIdx].pos.y - 5, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.15f;
//	}
//	else if (nType == 3)
//	{ // 弾D(リバースウェーブ)の場合
//		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x - 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y + 10, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nIdx].pos.x + 30, g_aEffect[nIdx].pos.y - 10, 0.0f);
//
//		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, g_aEffect[nIdx].color.a);
//
//		g_aEffect[nIdx].fAlphaDown = 0.09f;
//	}
//
//	// 頂点バッファをアンロックする
//	g_pVtxBuffEffect->Unlock();
//}