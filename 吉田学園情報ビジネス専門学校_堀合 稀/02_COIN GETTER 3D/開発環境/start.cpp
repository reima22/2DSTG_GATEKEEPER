//==============================================================================
//
// ゲーム開始処理の描画〔start.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "start.h"
#include "fade.h"
#include "sound.h"

//==============================================================================
// グローバル変数
//==============================================================================
Start start;

//==============================================================================
// スタートの初期化処理
//==============================================================================
HRESULT InitStart(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/three.png",
		&start.pTexture[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/two.png",
		&start.pTexture[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/one.png",
		&start.pTexture[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/go.png",
		&start.pTexture[3]);

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * START_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&start.pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	start.nStartCnt = 0;
	start.bStart = false;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	start.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		start.startInfo[nCnt].pos = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);
		start.startInfo[nCnt].bUse = false;

		//スコアの頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x - 60.0f, start.startInfo[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x - 60.0f, start.startInfo[nCnt].pos.y - 60.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x + 60.0f, start.startInfo[nCnt].pos.y + 60.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(start.startInfo[nCnt].pos.x + 60.0f, start.startInfo[nCnt].pos.y - 60.0f, 0.0f);

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
	start.pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================
// スタートの終了処理
//==============================================================================
void UninitStart(void)
{
	// 頂点バッファの開放
	if (start.pVtxBuff != NULL)
	{
		start.pVtxBuff->Release();
		start.pVtxBuff = NULL;
	}

	for (int nCnt = 0; nCnt < START_POLY; nCnt++)
	{
		// テクスチャの開放
		if (start.pTexture[nCnt] != NULL)
		{
			start.pTexture[nCnt]->Release();
			start.pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// スタートの更新処理
//==============================================================================
void UpdateStart(void)
{
	// ローカル変数宣言
	FADE fade = GetFade();

	if (fade == FADE_NONE)
	{
		if (start.nStartCnt < 240)
		{
			start.nStartCnt++;
		}

		if (start.nStartCnt == 1)
		{
			start.startInfo[0].bUse = true;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 60)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = true;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 120)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = true;
			start.startInfo[3].bUse = false;
			PlaySound(SOUND_LABEL_SE_COUNTDOWN);
		}
		else if (start.nStartCnt == 180)
		{
			start.startInfo[0].bUse = false;
			start.startInfo[1].bUse = false;
			start.startInfo[2].bUse = false;
			start.startInfo[3].bUse = true;
			PlaySound(SOUND_LABEL_SE_GAMESTART);
		}
		else if (start.nStartCnt == 240)
		{
			start.startInfo[3].bUse = false;
			start.bStart = true;
		}
	}
}

//==============================================================================
// スタートの描画処理
//==============================================================================
void DrawStart(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, start.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < START_POLY; nCntTexture++)
	{
		pDevice->SetTexture(0, start.pTexture[nCntTexture]);

		if (start.startInfo[nCntTexture].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntTexture * 4,
				2);
		}
	}
}

//==============================================================================
// スタート情報の取得
//==============================================================================
Start GetStart(void)
{
	return start;
}