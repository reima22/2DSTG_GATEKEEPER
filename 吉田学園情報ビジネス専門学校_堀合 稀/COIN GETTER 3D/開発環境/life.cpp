//==============================================================================
//
// 体力の描画〔life.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "player.h"
#include "life.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define LIFE_SHIFT	(60)		// 体力のずらし配置
#define LIFE_POSX	(150.0f)	// 体力のX座標
#define LIFE_POSY	(40.0f)		// 体力のY座標
#define LIFE_SIZEX	(30.0f)		// 体力サイズの幅
#define LIFE_SIZEY	(30.0f)		// 体力サイズの高さ

//==============================================================================
// 体力の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 体力の位置
	float fLifeAlpha;	// 体力の透明度
} LIFESTATE;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		// テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	// バッファへのポインタ
LIFESTATE lifestate[MAX_LIFE];					// 体力の構造体

//==============================================================================
// 体力の初期化処理
//==============================================================================
HRESULT InitLife(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/life000.png",
		&g_pTextureLife);

	// 変数の初期化
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		lifestate[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		lifestate[nCnt].fLifeAlpha = 1.0f;
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		// 体力の中心座標の設定
		lifestate[nCntLife].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SHIFT), LIFE_POSY, 0.0f);

		// 体力の頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x - LIFE_SIZEX, lifestate[nCntLife].pos.y + LIFE_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x - LIFE_SIZEX, lifestate[nCntLife].pos.y - LIFE_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x + LIFE_SIZEX, lifestate[nCntLife].pos.y + LIFE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(lifestate[nCntLife].pos.x + LIFE_SIZEX, lifestate[nCntLife].pos.y - LIFE_SIZEY, 0.0f);

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
	g_pVtxBuffLife->Unlock();

	return S_OK;
}

//==============================================================================
// 体力の終了処理
//==============================================================================
void UninitLife(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	// テクスチャの開放
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}
}

//==============================================================================
// 体力の更新処理
//==============================================================================
void UpdateLife(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	Player *pPlayer;

	// プレイヤーの取得
	pPlayer = GetPlayer();

	// テクスチャの透明度の変更
	if (pPlayer->nLife != MAX_LIFE)
	{
		// 透明度最大
		lifestate[pPlayer->nLife].fLifeAlpha = 0.0f;

		if (pPlayer->nLife == 0)
		{ // 体力0の時
			for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
			{ // 透明度をすべて最大へ
				lifestate[nCnt].fLifeAlpha = 0.0f;
			}
		}
	}
	else if(pPlayer->nLife == MAX_LIFE)
	{ // 体力最大へ
		for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
		{
			// 透明度最小へ
			lifestate[nCnt].fLifeAlpha = 1.0f;
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++,pVtx = pVtx + 4)
	{
		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, lifestate[nCnt].fLifeAlpha);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//==============================================================================
// 体力の描画処理
//==============================================================================
void DrawLife(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffLife,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_LIFE; nCnt++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}