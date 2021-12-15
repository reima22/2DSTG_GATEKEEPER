//-----------------------------------------------------------------
//
// HPバーの処理 (hp.cpp)
// Author:shouta takahashi
//
//-----------------------------------------------------------------

//------------------------
//インクルードファイル
//------------------------
#include "hp.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "select.h"



//-----------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;
LPDIRECT3DTEXTURE9 g_pTextureHp[MAXTEX_UI] = {};

LPDIRECT3DTEXTURE9 g_pTextureHpBlue[MAX_TEX_UI] = {};
LPDIRECT3DTEXTURE9 g_pTextureHpRed[MAX_TEX_UI] = {};
HP g_Hp[PLAYER_MAX];

//-----------------------------------------------------------------
// HPバーの初期化処理
//-----------------------------------------------------------------
HRESULT InitHp(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// 変数の受け渡し
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_00.png", &g_pTextureHp[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_01.png", &g_pTextureHp[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_02.png", &g_pTextureHp[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_bar_blue.png", &g_pTextureHp[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_00.png", &g_pTextureHp[4]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_01.png", &g_pTextureHp[5]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_02.png", &g_pTextureHp[6]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_bar_red.png", &g_pTextureHp[7]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_00.png", &g_pTextureHpBlue[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_01.png", &g_pTextureHpBlue[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_blue_02.png", &g_pTextureHpBlue[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_bar_blue.png", &g_pTextureHpBlue[3]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_00.png", &g_pTextureHpRed[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_01.png", &g_pTextureHpRed[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_red_02.png", &g_pTextureHpRed[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hp/hp_bar_red.png", &g_pTextureHpRed[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_TEX_UI, // 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,					// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffHp,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	for (int nHp = 0; nHp < PLAYER_MAX; nHp++)
	{
		g_Hp[nHp].posFrame = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 50.0f, 0.0f);
		g_Hp[nHp].posBar = D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 50.0f, 0.0f);
		if (nHp != 0)
		{
			g_Hp[nHp].posFrame.x *= 3.0f;
			g_Hp[nHp].posBar.x *= 3.0f;
		}
		g_Hp[nHp].fBar = 20.0f;
		g_Hp[nHp].hp = (HP_Player)nHp;
	}

	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHp = 0; nCntHp < MAX_TEX_UI; nCntHp++)
	{
		// 頂点座標の設定
		//SetVertexHp(nCntHp);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定			
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		// 情報の更新
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------
// HPバーの終了処理
//-----------------------------------------------------------------
void UninitHp(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}

	// テクスチャの開放
	for (int nCntTexture = 0; nCntTexture < MAX_TEX_UI; nCntTexture++)
	{
		if (g_pTextureHpBlue[nCntTexture] != NULL)
		{
			g_pTextureHpBlue[nCntTexture]->Release();
			g_pTextureHpBlue[nCntTexture] = NULL;
		}

		if (g_pTextureHpRed[nCntTexture] != NULL)
		{
			g_pTextureHpRed[nCntTexture]->Release();
			g_pTextureHpRed[nCntTexture] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAXTEX_UI; nCnt++)
	{
		if (g_pTextureHp[nCnt] != NULL)
		{
			g_pTextureHp[nCnt]->Release();
			g_pTextureHp[nCnt] = NULL;
		}
	}
}

//-----------------------------------------------------------------
// HPバーの更新処理
//-----------------------------------------------------------------
void UpdateHp(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntHp = 0; nCntHp < PLAYER_MAX; nCntHp++)
	{
		// 頂点座標の設定
		SetVertexHp(nCntHp);

		if (GetKeyboardTrigger(KEYINFO_JUMP) == true)
		{
			CalcuHp(0, 5.0f,false);
		}

		if (GetKeyboardTrigger(KEYINFO_CANCEL) == true)
		{
			CalcuHp(0, 5.0f, true);
		}

		if (g_Hp[nCntHp].fBar >= HP_LIMIT)
		{
			pPlayer->state = PLAYERSTATE_CLEAR;
		}
	}
}

//-----------------------------------------------------------------
// HPバーの描画処理
//-----------------------------------------------------------------
void DrawHp(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	Select *pSelect = GetSelect();

	// 変数の受け渡し
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntTexture = 0; nCntTexture < MAX_TEX_UI; nCntTexture++)
	{
		// テクスチャの設定
		switch (nCntTexture)
		{
		case 0:
			if (pSelect->chara[0] == CHARA_0)
			{
				pDevice->SetTexture(0, g_pTextureHp[0]);
			}
			else if (pSelect->chara[0] == CHARA_1)
			{
				pDevice->SetTexture(0, g_pTextureHp[1]);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureHp[2]);
			}
			//if (pSelect->chara[0] == CHARA_0)
			//{
			//	pDevice->SetTexture(0, g_pTextureHpBlue[0]);
			//}
			//else if (pSelect->chara[0] == CHARA_1)
			//{
			//	pDevice->SetTexture(0, g_pTextureHpBlue[1]);
			//}
			//else
			//{
			//	pDevice->SetTexture(0, g_pTextureHpBlue[2]);
			//}
			break;

		case 1:			
			pDevice->SetTexture(0, g_pTextureHp[3]);
			//pDevice->SetTexture(0, g_pTextureHpBlue[3]);
			break;

		case 2:
			if (pSelect->chara[1] == CHARA_0)
			{
				pDevice->SetTexture(0, g_pTextureHp[4]);
			}
			else if (pSelect->chara[1] == CHARA_1)
			{
				pDevice->SetTexture(0, g_pTextureHp[5]);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureHp[6]);
			}
/*			if (pSelect->chara[1] == CHARA_0)
			{
				pDevice->SetTexture(0, g_pTextureHpRed[0]);
			}
			else if (pSelect->chara[1] == CHARA_1)
			{
				pDevice->SetTexture(0, g_pTextureHpRed[1]);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureHpRed[2]);
			}*/			
			break;

		case 3:
			pDevice->SetTexture(0, g_pTextureHp[7]);
			//pDevice->SetTexture(0, g_pTextureHpRed[3]);
			break;

		default:
			break;
		}
		
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTexture * 4, 2);
	}
}

//-----------------------------------------------------------------
// HPバーの頂点座標更新
//-----------------------------------------------------------------
void SetVertexHp(int nCntHp)
{
	// 構造体のポインタ変数
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntHp * 4;

	if (nCntHp == 0)
	{// 青
		pVtx[0].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x - UI_HARF_W, g_Hp[nCntHp].posFrame.y + UI_HARF_H, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x - UI_HARF_W, g_Hp[nCntHp].posFrame.y - UI_HARF_H, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x + UI_HARF_W, g_Hp[nCntHp].posFrame.y + UI_HARF_H, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x + UI_HARF_W, g_Hp[nCntHp].posFrame.y - UI_HARF_H, 0.0f);

		pVtx[4].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x - UI_HARF_W, g_Hp[nCntHp].posBar.y + UI_HARF_H, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x - UI_HARF_W, g_Hp[nCntHp].posBar.y - UI_HARF_H, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x + UI_HARF_W - (g_Hp[nCntHp].fBar), g_Hp[nCntHp].posBar.y + UI_HARF_H, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x + UI_HARF_W - (g_Hp[nCntHp].fBar), g_Hp[nCntHp].posBar.y - UI_HARF_H, 0.0f);

		pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(1.0f * ((UI_WIDTH - g_Hp[nCntHp].fBar) / UI_WIDTH), 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f * ((UI_WIDTH - g_Hp[nCntHp].fBar) / UI_WIDTH), 0.0f);
	}
	else if (nCntHp == 1)
	{// 赤
		pVtx[4].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x - UI_HARF_W, g_Hp[nCntHp].posFrame.y + UI_HARF_H, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x - UI_HARF_W, g_Hp[nCntHp].posFrame.y - UI_HARF_H, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x + UI_HARF_W, g_Hp[nCntHp].posFrame.y + UI_HARF_H, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(g_Hp[nCntHp].posFrame.x + UI_HARF_W, g_Hp[nCntHp].posFrame.y - UI_HARF_H, 0.0f);

		pVtx[8].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x - UI_HARF_W + (g_Hp[nCntHp].fBar), g_Hp[nCntHp].posBar.y + UI_HARF_H, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x - UI_HARF_W + (g_Hp[nCntHp].fBar), g_Hp[nCntHp].posBar.y - UI_HARF_H, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x + UI_HARF_W, g_Hp[nCntHp].posBar.y + UI_HARF_H, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_Hp[nCntHp].posBar.x + UI_HARF_W, g_Hp[nCntHp].posBar.y - UI_HARF_H, 0.0f);

		pVtx[8].tex = D3DXVECTOR2(1.0f * ((UI_WIDTH - g_Hp[nCntHp].fBar) / UI_WIDTH), 1.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f * ((UI_WIDTH - g_Hp[nCntHp].fBar) / UI_WIDTH), 0.0f);
		pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[11].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffHp->Unlock();
}

// バーの減算の更新
void CalcuHp(int nHp,float fValue, bool bPlus)
{
	if (bPlus == true)
	{
		g_Hp[nHp].fBar -= fValue;
	}
	else
	{
		g_Hp[nHp].fBar += fValue;
	}
}

HP *GetHp(void)
{
	return &g_Hp[0];
}