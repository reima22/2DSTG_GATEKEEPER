//==============================================================================
//
// カウンターの描画〔counter.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "counter.h"
#include "enemy.h"
#include "player.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define COUNTER_SIZEX	(10)	// カウンターの幅
#define COUNTER_SIZEY	(20)	// カウンターの高さ
#define DESTROY_POSX	(720)	// 撃破数のX座標
#define DESTROY_POSY	(690)	// 撃破数のY座標
#define COUNTER_IN_POSX	(840)	// 侵入数のX座標
#define COUNTER_IN_POSY	(690)	// 侵入数のY座標
#define DESTROY_MAX		(3)		// 撃破数の桁数
#define COUNTER_IN_MAX	(2)		// 侵入数の桁数
#define ALL_MAX			(5)		// 使用する総桁数

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureCounter = NULL;		// カウントテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCounter = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posDestroy[DESTROY_MAX];				// 撃破数表示の中心の位置
D3DXVECTOR3 g_posInCount[COUNTER_IN_MAX];			// 侵入数表示の中心の位置				

//==============================================================================
// カウンターの初期化処理
//==============================================================================
HRESULT InitCounter(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureCounter);	

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ALL_MAX,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCounter,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < DESTROY_MAX; nCnt++, pVtx = pVtx + 4)
	{
		// 撃破数カウンター1桁目の中心座標
		g_posDestroy[nCnt] = D3DXVECTOR3(DESTROY_POSX + ((float)nCnt * 20.0f), DESTROY_POSY, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posDestroy[nCnt].x - COUNTER_SIZEX, g_posDestroy[nCnt].y + COUNTER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posDestroy[nCnt].x - COUNTER_SIZEX, g_posDestroy[nCnt].y - COUNTER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posDestroy[nCnt].x + COUNTER_SIZEX, g_posDestroy[nCnt].y + COUNTER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posDestroy[nCnt].x + COUNTER_SIZEX, g_posDestroy[nCnt].y - COUNTER_SIZEY, 0.0f);

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

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCounter = 0; nCounter < COUNTER_IN_MAX; nCounter++, pVtx += 4)
	{
		// 侵入数カウンターの中心座標
		g_posInCount[nCounter] = D3DXVECTOR3(COUNTER_IN_POSX + ((float)nCounter * 20.0f), COUNTER_IN_POSY, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[12].pos = D3DXVECTOR3(g_posInCount[nCounter].x - COUNTER_SIZEX, g_posInCount[nCounter].y + COUNTER_SIZEY, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(g_posInCount[nCounter].x - COUNTER_SIZEX, g_posInCount[nCounter].y - COUNTER_SIZEY, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(g_posInCount[nCounter].x + COUNTER_SIZEX, g_posInCount[nCounter].y + COUNTER_SIZEY, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(g_posInCount[nCounter].x + COUNTER_SIZEX, g_posInCount[nCounter].y - COUNTER_SIZEY, 0.0f);

		// rhwの設定
		pVtx[12].rhw = 1.0f;
		pVtx[13].rhw = 1.0f;
		pVtx[14].rhw = 1.0f;
		pVtx[15].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 頂点座標の設定
		pVtx[12].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();

	return S_OK;
}

//==============================================================================
// カウンターの終了処理
//==============================================================================
void UninitCounter(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCounter != NULL)
	{
		g_pVtxBuffCounter->Release();
		g_pVtxBuffCounter = NULL;
	}

	// テクスチャの開放
	if (g_pTextureCounter != NULL)
	{
		g_pTextureCounter->Release();
		g_pTextureCounter = NULL;
	}
}

//==============================================================================
// カウンターの更新処理
//==============================================================================
void UpdateCounter(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	PLAYER *Player;
	ENEMY *pEnemy;
	ENEMYINFO info = GetInfo();
	int aNumberD[DESTROY_MAX];						// 撃破数の各桁数
	int aNumberC[COUNTER_IN_MAX];					// 侵入数の各桁数
	int nDataD = (int)pow(10, DESTROY_MAX - 1);		// 撃破数の桁数
	int nDataC = (int)pow(10, COUNTER_IN_MAX - 1);	// 侵入数の桁数

	// プレイヤーの取得
	Player = GetPlayer();

	// 敵の取得
	pEnemy = GetEnemy();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	// 撃破数カウンター頂点座標の更新
	for (int nCnt = 0; nCnt < DESTROY_MAX; nCnt++, pVtx = pVtx + 4)
	{
		aNumberD[nCnt] = (info.nDestroy / nDataD) % 10;
		nDataD /= 10;

		pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)aNumberD[nCnt] / 10), 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)aNumberD[nCnt] / 10), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)aNumberD[nCnt] / 10), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)aNumberD[nCnt] / 10), 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCounter->Lock(0, 0, (void**)&pVtx, 0);

	// 侵入カウンターの頂点座標の更新
	for (int nCnt = 0; nCnt < COUNTER_IN_MAX; nCnt++, pVtx += 4)
	{
		aNumberC[nCnt] = (Player->nLineOver / nDataC) % 10;
		nDataC /= 10;

		pVtx[12].tex = D3DXVECTOR2(0.0f + ((float)aNumberC[nCnt] / 10), 1.0f);
		pVtx[13].tex = D3DXVECTOR2(0.0f + ((float)aNumberC[nCnt] / 10), 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.1f + ((float)aNumberC[nCnt] / 10), 1.0f);
		pVtx[15].tex = D3DXVECTOR2(0.1f + ((float)aNumberC[nCnt] / 10), 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCounter->Unlock();
}

//==============================================================================
// カウンターの描画処理
//==============================================================================
void DrawCounter(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffCounter,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < ALL_MAX; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureCounter);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}