//==============================================================================
//
// ポーズメニューの描画〔pause.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "pause.h"
#include "input.h"
#include "sound.h"


//==============================================================================
// マクロ定義
//==============================================================================
#define PAUSE_POLYGON	(5)			// ポーズ画面の使用するポリゴン数
#define PAUSETEXT		(3)			// ポーズ画面の選択肢数
#define PAUSE_WINDOWX	(160)		// ポーズメニューの幅
#define PAUSE_WINDOWY	(160)		// ポーズメニューの高さ
#define PAUSE_SIZEX		(140)		// ポーズテキストの幅
#define PAUSE_SIZEY		(40)		// ポーズテキストの高さ
#define PAUSETEXT_POSY	(260)		// ポーズテキスト1行目の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_POLYGON] = {};	// ポーズテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			// バッファへのポインタ
D3DXVECTOR3 g_posBackScreen;							// ポーズ背景の位置
D3DXVECTOR3 g_posWindow;								// ポーズウィンドウの位置
D3DXVECTOR3 g_posText[PAUSETEXT];						// ポーズテキストの位置
float g_fContinue;										// コンテニューテキストのテクスチャ位置
float g_fRetry;											// リトライテキストのテクスチャ位置
float g_fQuit;											// やり直しテキストのテクスチャ位置
PAUSE_MENU PauseMenu;									// ポーズの状態

//==============================================================================
// ポーズメニューの初期化処理
//==============================================================================
HRESULT InitPause(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/pause_BS.png",
		&g_pTexturePause[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/continue_logo.png",
		&g_pTexturePause[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/retry_logo.png",
		&g_pTexturePause[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/quit_logo.png",
		&g_pTexturePause[3]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 5,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	g_fContinue = 0.0f;
	g_fRetry = 0.5f;
	g_fQuit = 0.5f;

	PauseMenu = PAUSE_MENU_CONTINUE;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// ユーザーインターフェースの中心座標
	g_posBackScreen = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posBackScreen.x - (SCREEN_WIDTH / 2), g_posBackScreen.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posBackScreen.x - (SCREEN_WIDTH / 2), g_posBackScreen.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posBackScreen.x + (SCREEN_WIDTH / 2), g_posBackScreen.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posBackScreen.x + (SCREEN_WIDTH / 2), g_posBackScreen.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	// 頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// ユーザーインターフェースの中心座標
	g_posWindow = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posWindow.x - PAUSE_WINDOWX, g_posWindow.y + PAUSE_WINDOWY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posWindow.x - PAUSE_WINDOWX, g_posWindow.y - PAUSE_WINDOWY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posWindow.x + PAUSE_WINDOWX, g_posWindow.y + PAUSE_WINDOWY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posWindow.x + PAUSE_WINDOWX, g_posWindow.y - PAUSE_WINDOWY, 0.0f);

	// rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < 3; nCnt++, pVtx += 4)
	{
		// ユーザーインターフェースの中心座標
		g_posText[nCnt] = D3DXVECTOR3(SCREEN_WIDTH / 2, PAUSETEXT_POSY + (nCnt * 100.0f), 0.0f);

		// ポリゴンの各頂点座標
		pVtx[8].pos = D3DXVECTOR3(g_posText[nCnt].x - PAUSE_SIZEX, g_posText[nCnt].y + PAUSE_SIZEY, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(g_posText[nCnt].x - PAUSE_SIZEX, g_posText[nCnt].y - PAUSE_SIZEY, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(g_posText[nCnt].x + PAUSE_SIZEX, g_posText[nCnt].y + PAUSE_SIZEY, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(g_posText[nCnt].x + PAUSE_SIZEX, g_posText[nCnt].y - PAUSE_SIZEY, 0.0f);

		// rhwの設定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// 頂点座標の設定
		pVtx[8].tex = D3DXVECTOR2(0.0, 0.5);
		pVtx[9].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[10].tex = D3DXVECTOR2(1.0, 0.5);
		pVtx[11].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	return S_OK;
}

//==============================================================================
// ポーズメニューの終了処理
//==============================================================================
void UninitPause(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < PAUSE_POLYGON; nCnt++)
	{
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}
}

//==============================================================================
// ポーズメニューの更新処理
//==============================================================================
void UpdatePause(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// ポーズからゲーム続行
	if (GetKeyboardTrigger(KEYINFO_PAUSE) == true)
	{
		if (PauseMenu != PAUSE_MENU_CONTINUE)
		{
			PauseMenu = PAUSE_MENU_CONTINUE;
		}
	}

	// 選択画面
	if (GetKeyboardTrigger(KEYINFO_DOWN) == true)
	{
		// 音の再生
		PlaySound(SOUND_LABEL_SE_HIT000);

		if (PauseMenu == PAUSE_MENU_CONTINUE)
		{ // カーソルが「CONTINUE」の時
			PauseMenu = PAUSE_MENU_RETRY;
		}
		else if (PauseMenu == PAUSE_MENU_RETRY)
		{ // カーソルが「RETRY」の時
			PauseMenu = PAUSE_MENU_QUIT;
		}
		else if (PauseMenu == PAUSE_MENU_QUIT)
		{ // カーソルが「QUIT」の時
			PauseMenu = PAUSE_MENU_CONTINUE;
		}
	}
	else if (GetKeyboardTrigger(KEYINFO_UP) == true)
	{
		// 音の再生
		PlaySound(SOUND_LABEL_SE_HIT000);

		if (PauseMenu == PAUSE_MENU_CONTINUE)
		{ // カーソルが「CONTINUE」の時
			PauseMenu = PAUSE_MENU_QUIT;
		}
		else if (PauseMenu == PAUSE_MENU_RETRY)
		{ // カーソルが「RETRY」の時
			PauseMenu = PAUSE_MENU_CONTINUE;
		}
		else if (PauseMenu == PAUSE_MENU_QUIT)
		{ // カーソルが「QUIT」の時
			PauseMenu = PAUSE_MENU_RETRY;
		}
	}

	// テクスチャの切り替え
	switch (PauseMenu)
	{
	case PAUSE_MENU_CONTINUE:	// カーソルが「CONTINUE」の時
		g_fContinue = 0.0f;
		g_fRetry = 0.5f;
		g_fQuit = 0.5f;
		break;

	case PAUSE_MENU_RETRY:		// カーソルが「RETRY」の時
		g_fContinue = 0.5f;
		g_fRetry = 0.0f;
		g_fQuit = 0.5f;
		break;

	case PAUSE_MENU_QUIT:		// カーソルが「QUIT」の時
		g_fContinue = 0.5f;
		g_fRetry = 0.5f;
		g_fQuit = 0.0f;
		break;

	default:
		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 各テクスチャの更新
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.5f + g_fContinue);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f + g_fContinue);
	pVtx[10].tex = D3DXVECTOR2(1.0f, 0.5f + g_fContinue);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f + g_fContinue);

	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.5f + g_fRetry);
	pVtx[13].tex = D3DXVECTOR2(0.0f, 0.0f + g_fRetry);
	pVtx[14].tex = D3DXVECTOR2(1.0f, 0.5f + g_fRetry);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 0.0f + g_fRetry);

	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.5f + g_fQuit);
	pVtx[17].tex = D3DXVECTOR2(0.0f, 0.0f + g_fQuit);
	pVtx[18].tex = D3DXVECTOR2(1.0f, 0.5f + g_fQuit);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 0.0f + g_fQuit);

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//==============================================================================
// ポーズメニューの描画処理
//==============================================================================
void DrawPause(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		if (nCnt == 0)
		{ // 背景暗転のテクスチャ
			pDevice->SetTexture(0, NULL);
		}
		else
		{ // ポーズメニューのテクスチャ
			pDevice->SetTexture(0, g_pTexturePause[nCnt - 1]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// ポーズの情報取得
//==============================================================================
PAUSE_MENU GetPause(void)
{
	return PauseMenu;
}