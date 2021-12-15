//==============================================================================
//
// 警告画面の描画〔warning.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "warning.h"
#include "enemy.h"
#include "sound.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureWarning = NULL;		// 警告画面のテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWarning = NULL;	// 警告画面のバッファへのポインタ
D3DXVECTOR3 g_posWarning;							// 警告画面の位置
int g_nCntAlert;									// 警告中カウント
float g_fAlpha;										// 画面のアルファ値
float g_fChange;									// アルファ値の変化値
PHASESTATE phase;									// 警告の状態
int g_nPhaseCnt;									// 警告表示カウント

//==============================================================================
// 警告画面の初期化処理
//==============================================================================
HRESULT InitWarning(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/warning.png",
		&g_apTextureWarning);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWarning,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_fAlpha = 0.0f;
	g_fChange = 0.03f;
	g_nCntAlert = 0;
	phase = PHASESTATE_NORMAL;
	g_nPhaseCnt = 0;

	// 頂点バッファをロックする
	g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

	// 警告画面の中心座標
	g_posWarning = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posWarning.x - (SCREEN_WIDTH / 2), g_posWarning.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posWarning.x - (SCREEN_WIDTH / 2), g_posWarning.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posWarning.x + (SCREEN_WIDTH / 2), g_posWarning.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posWarning.x + (SCREEN_WIDTH / 2), g_posWarning.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffWarning->Unlock();

	return S_OK;
}

//==============================================================================
// 警告画面の終了処理
//==============================================================================
void UninitWarning(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffWarning != NULL)
	{
		g_pVtxBuffWarning->Release();
		g_pVtxBuffWarning = NULL;
	}

	// テクスチャの開放
	if (g_apTextureWarning != NULL)
	{
		g_apTextureWarning->Release();
		g_apTextureWarning = NULL;
	}
}

//==============================================================================
// 警告画面の更新処理
//==============================================================================
void UpdateWarning(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ENEMYINFO info = GetInfo();	// 敵全体情報の取得

	// フェーズの切り替え
	if (info.nDestroy == BIGENEMY_APPEAR && g_nPhaseCnt < 180)
	{
		phase = PHASESTATE_WARNING;	// 警告状態へ移行

		// カウント開始
		g_nPhaseCnt++;

		// カウント終了
		if (g_nPhaseCnt >= 180)
		{
			phase = PHASESTATE_NORMAL;	// 通常状態へ移行

			// サウンドの再生
			PlaySound(SOUND_LABEL_BGM005);
		}
	}

	// フェーズの状態
	switch (phase)
	{
	case PHASESTATE_NORMAL:
		g_nCntAlert = 0;
		g_fAlpha = 0.0f;

		// 頂点バッファをロックする
		g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャの更新
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);

		// 頂点バッファをアンロックする
		g_pVtxBuffWarning->Unlock();

		break;

	case PHASESTATE_WARNING:
		// カウント開始
		if (g_nCntAlert <= 180)
		{
			g_nCntAlert++;
		}

		if (g_nCntAlert % 1 == 0)
		{
			// 透明度の変化
			g_fAlpha += g_fChange;

			// 透明度の状態で加減算の変化
			if (g_fAlpha <= 0.0f)
			{
				g_fChange = 0.03f;
			}
			else if (g_fAlpha >= 1.0f)
			{
				g_fChange = -0.03f;
			}

			// 頂点バッファをロックする
			g_pVtxBuffWarning->Lock(0, 0, (void**)&pVtx, 0);

			// テクスチャの更新
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlpha);

			// 頂点バッファをアンロックする
			g_pVtxBuffWarning->Unlock();
		}

		break;

	default:
		break;
	}
}

//==============================================================================
// 警告画面の描画処理
//==============================================================================
void DrawWarning(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffWarning,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_apTextureWarning);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//==============================================================================
// 警告画面の情報
//==============================================================================
PHASESTATE GetPhase(void)
{
	return phase;
}