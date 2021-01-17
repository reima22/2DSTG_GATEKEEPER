//==============================================================================
//
// 背景ポリゴンの描画〔bg.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bg.h"
#include "player.h"
#include "input.h"
#include "game.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_BG		(2)			// 背景の種類
#define MTNPOSY		(620.0f)	// 山背景のY座標
#define MTNSIZEY	(100.0f)	// 山背景の高さ

//==============================================================================
// 背景の構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 posBg;	// 空背景の位置
	D3DXVECTOR3 posMtn;	// 山背景の位置
	D3DXCOLOR colBg;	// 空背景の色
	D3DXCOLOR colMtn;	// 山背景の色
	float fMoveBgX;		// 背景のX軸移動量
	int nCntAnim;		// アニメーションカウンター
} BG;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureBg[MAX_BG] = {};	// 背景テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	// バッファへのポインタ
BG Bg;
//D3DXVECTOR3 g_posBg;							// 背景の位置
//D3DXVECTOR3 g_posMtn;							// 山の位置
//int g_nCntAnim;								// アニメーションカウンター
//float g_fMoveBgX;								// 空背景の移動量X

//==============================================================================
// 背景ポリゴンの初期化処理
//==============================================================================
HRESULT InitBg(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/sky000.jpg",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/mountain000.png",
		&g_apTextureBg[1]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BG,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	Bg.colBg = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Bg.colMtn = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// 背景の中心座標
	Bg.posBg = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(Bg.posBg.x - (SCREEN_WIDTH / 2), Bg.posBg.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Bg.posBg.x - (SCREEN_WIDTH / 2), Bg.posBg.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Bg.posBg.x + (SCREEN_WIDTH / 2), Bg.posBg.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Bg.posBg.x + (SCREEN_WIDTH / 2), Bg.posBg.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = Bg.colBg;
	pVtx[1].col = Bg.colBg;
	pVtx[2].col = Bg.colBg;
	pVtx[3].col = Bg.colBg;

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / 4.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 4.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// 背景の中心座標
	Bg.posMtn = D3DXVECTOR3(SCREEN_WIDTH / 2, MTNPOSY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(Bg.posMtn.x - (SCREEN_WIDTH / 2), Bg.posMtn.y + MTNSIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(Bg.posMtn.x - (SCREEN_WIDTH / 2), Bg.posMtn.y - MTNSIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(Bg.posMtn.x + (SCREEN_WIDTH / 2), Bg.posMtn.y + MTNSIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(Bg.posMtn.x + (SCREEN_WIDTH / 2), Bg.posMtn.y - MTNSIZEY, 0.0f);

	// rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[4].col = Bg.colMtn;
	pVtx[5].col = Bg.colMtn;
	pVtx[6].col = Bg.colMtn;
	pVtx[7].col = Bg.colMtn;

	// 頂点座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[5].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.75f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.75f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();

	return S_OK;
}

//==============================================================================
// 背景ポリゴンの終了処理
//==============================================================================
void UninitBg(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (g_apTextureBg[nCnt] != NULL)
		{
			g_apTextureBg[nCnt]->Release();
			g_apTextureBg[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 背景ポリゴンの更新処理
//==============================================================================
void UpdateBg(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	int nStage = GetStage();	// ステージの取得

	StageBg(nStage);	// ステージごとの背景の設定

	Bg.nCntAnim++;		// アニメーションカウンター更新

	// テクスチャの更新
	if ((Bg.nCntAnim % 2) == 0)
	{
		// テクスチャのパターンの更新
		Bg.fMoveBgX += 0.0002f;

		// 頂点バッファをロックする
		g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャの頂点座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + Bg.fMoveBgX, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f + Bg.fMoveBgX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / 4.0f) + Bg.fMoveBgX, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / 4.0f) + Bg.fMoveBgX, 0.0f);
		
		// 頂点バッファをアンロックする
		g_pVtxBuffBg->Unlock();
	}
}

//==============================================================================
// 背景ポリゴンの描画処理
//==============================================================================
void DrawBg(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBg,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureBg[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// ステージごとの背景色設定
//==============================================================================
void StageBg(int nStage)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	if (nStage == 1)
	{
		Bg.colBg = D3DXCOLOR(0.8f, 0.5f, 0.3f, 1.0f);
		Bg.colMtn = D3DXCOLOR(0.8f, 0.5f, 0.3f, 1.0f);
	}
	else if (nStage == 2)
	{
		Bg.colBg = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		Bg.colMtn.a = 0.0f;
	}

	// 頂点バッファをロックする
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点カラーの設定
	pVtx[0].col = Bg.colBg;
	pVtx[1].col = Bg.colBg;
	pVtx[2].col = Bg.colBg;
	pVtx[3].col = Bg.colBg;

	pVtx[4].col = Bg.colMtn;
	pVtx[5].col = Bg.colMtn;
	pVtx[6].col = Bg.colMtn;
	pVtx[7].col = Bg.colMtn;

	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}