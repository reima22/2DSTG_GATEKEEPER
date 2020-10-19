//==============================================================================
//
// タイトル画面の描画〔title.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define TITLE_POLYGON		(3)		// タイトル画面のポリゴン数
#define TITLE_LOGOX			(640)	// タイトルロゴのX座標
#define TITLE_FIRST_LOGOY	(-50)	// タイトルロゴの最初のY座標
#define TITLE_LAST_LOGOY	(200)	// タイトルロゴの最終Y座標
#define TITLE_LOGO_SIZEX	(480)	// タイトルロゴの幅
#define TITLE_LOGO_SIZEY	(80)	// タイトルロゴの高さ
#define TITLE_ENTERX		(640)	// エンターサインのX座標
#define TITLE_ENTERY		(550)	// エンターサインのY座標
#define TITLE_ENTER_SIZEX	(240)	// エンターサインの幅
#define TITLE_ENTER_SIZEY	(40)	// エンターサインの高さ

//==============================================================================
// エンターサインの状態
//==============================================================================
typedef enum
{
	ENTERSIGN_NORMAL = 0,	// 通常状態
	ENTERSIGN_NONE,			// 非表示状態
	ENTERSIGN_ACTIVE,		// エンターキー入力時の点滅
	ENTERSIGN_MAX
} ENTERSIGN;

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_POLYGON] = {};		// タイトル画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;				// バッファへのポインタ
D3DXVECTOR3 g_posTitle;										// タイトル画面の位置
D3DXVECTOR3 g_posTitlelogo;									// タイトルロゴの位置
D3DXVECTOR3 g_posTitleEnter;								// エンターサインの位置
int g_nCntMoveLogo;											// ロゴの移動カウンター
ENTERSIGN Sign;												// エンターサインの状態
int g_nSignCounter;											// エンターサインの点滅カウンター
int g_nShiftCount;											// 自動画面遷移のカウンター

//==============================================================================
// タイトル画面の初期化処理
//==============================================================================
HRESULT InitTitle(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();
	
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bg104.png",
		&g_pTextureTitle[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/title000.png",
		&g_pTextureTitle[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/press_enter.png",
		&g_pTextureTitle[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TITLE_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// エンターサインの初期化
	Sign = ENTERSIGN_NONE;
	g_nSignCounter = 0;
	g_nShiftCount = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトル画面の中心座標
	g_posTitle = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトル画面の中心座標
	g_posTitlelogo = D3DXVECTOR3(TITLE_LOGOX, TITLE_FIRST_LOGOY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

	// rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// エンターサインの中心座標
	g_posTitleEnter = D3DXVECTOR3(TITLE_ENTERX, TITLE_ENTERY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[8].pos = D3DXVECTOR3(g_posTitleEnter.x - TITLE_ENTER_SIZEX, g_posTitleEnter.y + TITLE_ENTER_SIZEY, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posTitleEnter.x - TITLE_ENTER_SIZEX, g_posTitleEnter.y - TITLE_ENTER_SIZEY, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posTitleEnter.x + TITLE_ENTER_SIZEX, g_posTitleEnter.y + TITLE_ENTER_SIZEY, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posTitleEnter.x + TITLE_ENTER_SIZEX, g_posTitleEnter.y - TITLE_ENTER_SIZEY, 0.0f);

	// rhwの設定
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点座標の設定
	pVtx[8].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[9].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[10].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[11].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
	
	// BGMの再生
	PlaySound(SOUND_LABEL_BGM000);

	return S_OK;
}

//==============================================================================
// タイトル画面の終了処理
//==============================================================================
void UninitTitle(void)
{
	// サウンドの停止処理
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TITLE_POLYGON; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}
}

//==============================================================================
// タイトル画面の更新処理
//==============================================================================
void UpdateTitle(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	FADE fade;
	int nSign = 255;

	// フェードの取得
	fade = GetFade();

	// タイトルロゴの移動
	if (g_posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		g_nCntMoveLogo++;
		if (g_nCntMoveLogo % 2 == 0)
		{
			g_posTitlelogo.y += 2;
			if (g_posTitlelogo.y >= TITLE_LAST_LOGOY)
			{
				g_posTitlelogo.y = TITLE_LAST_LOGOY;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + TITLE_LOGO_SIZEX, g_posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
	

	if (GetKeyboardTrigger(KEYINFO_OK) == true && g_posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		g_posTitlelogo.y = TITLE_LAST_LOGOY - 1;
	}

	if (g_posTitlelogo.y == TITLE_LAST_LOGOY)
	{
		g_nSignCounter++;	// エンターサインカウンターの増加
		g_nShiftCount++;	// 自動画面遷移カウンターの増加
		if (g_nShiftCount >= 300)
		{ // 自動遷移実行
			// フェードアウト
			SetFade(FADE_OUT, MODE_RANKING);
		}
	}

	// エンターキー入力でゲーム画面へ遷移
	if (GetKeyboardTrigger(KEYINFO_OK) == true && g_posTitlelogo.y == TITLE_LAST_LOGOY && g_nShiftCount < 300)
	{
		if (Sign != ENTERSIGN_ACTIVE)
		{
			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_START000);
		}

		// フェードアウト
		SetFade(FADE_OUT,MODE_TUTORIAL);

		// エンターサインの状態変化
		Sign = ENTERSIGN_ACTIVE;
	}

	// エンターサインの状態
	switch (Sign)
	{
	case ENTERSIGN_NORMAL:
		if (g_nSignCounter % 60 == 0)
		{
			// 透明度0へ
			nSign = 0;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 30 == 0 && g_nSignCounter % 60 != 0)
		{
			// 透明度最大へ
			nSign = 255;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}

		break;

	case ENTERSIGN_NONE:
		// 透明度最小へ
		nSign = 0;

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

		// 各頂点カラーの設定
		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

		// 頂点バッファをアンロックする
		g_pVtxBuffTitle->Unlock();

		// 通常時への移行
		if (g_posTitlelogo.y == TITLE_LAST_LOGOY)
		{
			Sign = ENTERSIGN_NORMAL;
		}

		break;

	case ENTERSIGN_ACTIVE:
		if (g_nSignCounter % 4 == 0)
		{
			// 透明度0へ
			nSign = 255;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 2 == 0 && g_nSignCounter % 4 != 0)
		{
			// 透明度最大へ
			nSign = 0;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}

		break;

	default:
		break;
	}
}

//==============================================================================
// タイトル画面の描画処理
//==============================================================================
void DrawTitle(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < TITLE_POLYGON; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCnt]);
		
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

