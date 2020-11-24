//==============================================================================
//
// 背景ポリゴンの描画〔bg.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bg.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
#include "warning.h"
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_BG		(3)			// 背景の種類
#define MOVE_BG_A	(0.0005f)	// 背景Aの操作移動量
#define MOVE_BG_B	(0.001f)	// 背景Bの操作移動量 
#define MOVE_BG_C	(0.0015f)	// 背景Cの操作移動量

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureBg[MAX_BG] = {};	// 背景テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	// バッファへのポインタ
D3DXVECTOR3 g_posBg[MAX_BG];					// 背景の位置
int g_nCntAnim;									// アニメーションカウンター
float g_fPatternAnim[MAX_BG];					// アニメーションパターン
float g_fMoveBgX[MAX_BG];						// 操作による移動量X
float g_fMoveBgY[MAX_BG];						// 操作による移動量Y

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
		"data/TEXTURE/bg100.png",
		&g_apTextureBg[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bg101.png",
		&g_apTextureBg[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bg102.png",
		&g_apTextureBg[2]);

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
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		g_posBg[nCnt].x = SCREEN_WIDTH / 2;
		g_posBg[nCnt].y = SCREEN_HEIGHT / 2;
		g_fMoveBgX[nCnt] = 0.0f;
		g_fMoveBgY[nCnt] = 0.0f;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// 背景の中心座標
		g_posBg[nCnt] = D3DXVECTOR3(g_posBg[nCnt].x, g_posBg[nCnt].y, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

		pVtx += 4;
	}

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
	ENEMYINFO info = GetInfo();		// 敵全体情報の取得
	PHASESTATE phase = GetPhase();	// 警告状態の取得

	g_nCntAnim++;	// アニメーションカウンター更新

	if (info.nStartCnt >= OPEN_CNT && phase == PHASESTATE_NORMAL)
	{
		// プレイヤーの移動に合わせて背景テクスチャの更新
		if (GetKeyboardPress(KEYINFO_UP) == true || IsButtonPush(KEYINFO::KEYINFO_UP) == true)
		{ // 上へ
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;

			if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
			{
				g_fMoveBgX[0] -= MOVE_BG_A;
				g_fMoveBgX[1] -= MOVE_BG_B;
				g_fMoveBgX[2] -= MOVE_BG_C;
				//g_fMoveBgY[0] -= MOVE_BG_A;
				//g_fMoveBgY[1] -= MOVE_BG_B;
				//g_fMoveBgY[2] -= MOVE_BG_C;
			}
			else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
			{
				g_fMoveBgX[0] += MOVE_BG_A;
				g_fMoveBgX[1] += MOVE_BG_B;
				g_fMoveBgX[2] += MOVE_BG_C;
			}
			else if (GetKeyboardPress(KEYINFO_DOWN) == true || IsButtonPush(KEYINFO::KEYINFO_DOWN) == true)
			{
				g_fMoveBgY[0] += MOVE_BG_A;
				g_fMoveBgY[1] += MOVE_BG_B;
				g_fMoveBgY[2] += MOVE_BG_C;
			}
		}
		else if (GetKeyboardPress(KEYINFO_DOWN) == true || IsButtonPush(KEYINFO::KEYINFO_DOWN) == true)
		{ // 下へ
			g_fMoveBgY[0] += MOVE_BG_A;
			g_fMoveBgY[1] += MOVE_BG_B;
			g_fMoveBgY[2] += MOVE_BG_C;

			if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
			{
				g_fMoveBgX[0] -= MOVE_BG_A;
				g_fMoveBgX[1] -= MOVE_BG_B;
				g_fMoveBgX[2] -= MOVE_BG_C;
			}
			else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
			{
				g_fMoveBgX[0] += MOVE_BG_A;
				g_fMoveBgX[1] += MOVE_BG_B;
				g_fMoveBgX[2] += MOVE_BG_C;
			}
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
		{ // 左へ
			g_fMoveBgX[0] -= MOVE_BG_A;
			g_fMoveBgX[1] -= MOVE_BG_B;
			g_fMoveBgX[2] -= MOVE_BG_C;

			if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
			{
				g_fMoveBgX[0] += MOVE_BG_A;
				g_fMoveBgX[1] += MOVE_BG_B;
				g_fMoveBgX[2] += MOVE_BG_C;
			}
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{ // 右へ
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
		}
	}

	// テクスチャの更新
	if ((g_nCntAnim % 2) == 0)
	{
		// テクスチャのパターンの更新
		g_fPatternAnim[0] -= 0.001f;
		g_fPatternAnim[1] -= 0.003f;
		g_fPatternAnim[2] -= 0.005f;

		// 頂点バッファをロックする
		g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < MAX_BG; nCnt++, pVtx = pVtx + 4)
		{
			// テクスチャの頂点座標の更新
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[3].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
		}
		
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