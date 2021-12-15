//==============================================================================
//
// ゲーム開始処理の描画〔start.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "start.h"
#include "fade.h"
#include "sound.h"

// マクロ定義
#define START_POLY	(4)

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureStart[START_POLY] = {};			// スコアテクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;		// スコアバッファへのポインタ
Start start[START_POLY];
int g_nStartCnt;
//int g_nTimeCnt;

//==============================================================================
// スコアの初期化処理
//==============================================================================
HRESULT InitStart(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	//int nStage = GetStage();

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/three.png",
		&g_pTextureStart[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/two.png",
		&g_pTextureStart[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/one.png",
		&g_pTextureStart[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/go.png",
		&g_pTextureStart[3]);

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * START_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_nStartCnt = 0;
	//g_nTimeCnt = 3;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		start[nCnt].pos = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);
		start[nCnt].bUse = false;

		//スコアの頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(start[nCnt].pos.x - 60.0f, start[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(start[nCnt].pos.x - 60.0f, start[nCnt].pos.y - 60.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(start[nCnt].pos.x + 60.0f, start[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(start[nCnt].pos.x + 60.0f, start[nCnt].pos.y - 60.0f, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();

	return S_OK;
}

//==============================================================================
// スコアの終了処理
//==============================================================================
void UninitStart(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		// テクスチャの開放
		if (g_pTextureStart[nCnt] != NULL)
		{
			g_pTextureStart[nCnt]->Release();
			g_pTextureStart[nCnt] = NULL;
		}
	}
}

//==============================================================================
// スコアの更新処理
//==============================================================================
void UpdateStart(void)
{
	// ローカル変数宣言
	FADE fade = GetFade();

	if (fade == FADE_NONE)
	{
		if (g_nStartCnt < 240)
		{
			g_nStartCnt++;
		}

		if (g_nStartCnt == 1)
		{
			start[0].bUse = true;
			start[1].bUse = false;
			start[2].bUse = false;
			start[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (g_nStartCnt == 60)
		{
			start[0].bUse = false;
			start[1].bUse = true;
			start[2].bUse = false;
			start[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (g_nStartCnt == 120)
		{
			start[0].bUse = false;
			start[1].bUse = false;
			start[2].bUse = true;
			start[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (g_nStartCnt == 180)
		{
			start[0].bUse = false;
			start[1].bUse = false;
			start[2].bUse = false;
			start[3].bUse = true;
			PlaySound(SOUND_LABEL_SE_GAMESTART);
		}
		else if (g_nStartCnt == 240)
		{
			start[3].bUse = false;
		}
	}
}

//==============================================================================
// スコアの描画処理
//==============================================================================
void DrawStart(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	//Select *pSelect = GetSelect();

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < START_POLY; nCntTexture++)
	{
		pDevice->SetTexture(0, g_pTextureStart[nCntTexture]);

		if (start[nCntTexture].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntTexture * 4,
				2);
		}
	}
}

int GetCount(void)
{
	return g_nStartCnt;
}