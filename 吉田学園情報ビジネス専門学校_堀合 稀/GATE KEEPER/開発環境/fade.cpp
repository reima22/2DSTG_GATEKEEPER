//==============================================================================
//
// フェードの描画〔fade.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "fade.h"
#include "input.h"

//==============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// フェードのバッファへのポインタ
FADE g_fade;									// フェードの状態
MODE g_modeNext;								// 次のモード
D3DXCOLOR g_colorFade;							// フェード色
D3DXVECTOR3 g_posFade;							// フェード画面の位置

//==============================================================================
// フェードの初期化処理
//==============================================================================
HRESULT InitFade(MODE modeNext)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// フェードするポリゴンの中心座標
	g_posFade = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posFade.x - (SCREEN_WIDTH / 2), g_posFade.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posFade.x - (SCREEN_WIDTH / 2), g_posFade.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posFade.x + (SCREEN_WIDTH / 2), g_posFade.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posFade.x + (SCREEN_WIDTH / 2), g_posFade.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	return S_OK;
}

//==============================================================================
// フェードの終了処理
//==============================================================================
void UninitFade(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==============================================================================
// フェードの更新処理
//==============================================================================
void UpdateFade(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	MODE mode;

	// モードの取得
	mode = GetMode();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	if (g_fade == FADE_IN)
	{
		g_colorFade.a -= 0.01f;

		if (g_colorFade.a <= 0.0f || GetKeyboardTrigger(KEYINFO_OK) == true)
		{
			if (mode != MODE_GAME)
			{
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}
			else
			{
				if (g_colorFade <= 0.0f)
				{
					g_fade = FADE_NONE;
				}
			}
		}
	}
	else if (g_fade == FADE_OUT)
	{
		g_colorFade.a += 0.01f;

		if (g_colorFade.a >= 1.0f)
		{
			g_colorFade.a = 1.0f;
			g_fade = FADE_IN;
			SetMode(g_modeNext);
		}
	}
}

//==============================================================================
// フェードの描画処理
//==============================================================================
void DrawFade(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//==============================================================================
// フェードの設定
//==============================================================================
void SetFade(FADE fade, MODE modeNext)
{
	g_fade = fade;
	g_modeNext = modeNext;
}

//==============================================================================
// フェード情報の取得
//==============================================================================
FADE GetFade(void)
{
	return g_fade;
}