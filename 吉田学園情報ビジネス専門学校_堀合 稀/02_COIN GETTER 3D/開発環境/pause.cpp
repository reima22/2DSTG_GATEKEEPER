//==============================================================================
//
// ポーズメニューの描画〔pause.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "pause.h"
#include "pause_logo.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "gamepad.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[PAUSE_POLYGON] = {};	// ポーズテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			// バッファへのポインタ
PAUSE Pause;											// ポーズの構造体

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

	// テクスチャの読み込み
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
		"data/TEXTURE/option_logo.png",
		&g_pTexturePause[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/quit_logo.png",
		&g_pTexturePause[4]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/option.jpg",
		&g_pTexturePause[5]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PAUSE_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,				// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	Pause.fContinue = 0.0f;
	Pause.fRetry = 0.5f;
	Pause.fOption = 0.5f;
	Pause.fQuit = 0.5f;
	Pause.colOption = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	Pause.bOption = false;
	Pause.PauseMenu = PAUSE_MENU_CONTINUE;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 半透過の中心座標
	Pause.posBackScreen = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

	// 半透過背景の各頂点座標
	pVtx[0].pos = D3DXVECTOR3(Pause.posBackScreen.x - SCREEN_CENTERX, Pause.posBackScreen.y + SCREEN_CENTERY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Pause.posBackScreen.x - SCREEN_CENTERX, Pause.posBackScreen.y - SCREEN_CENTERY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Pause.posBackScreen.x + SCREEN_CENTERX, Pause.posBackScreen.y + SCREEN_CENTERY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Pause.posBackScreen.x + SCREEN_CENTERX, Pause.posBackScreen.y - SCREEN_CENTERY, 0.0f);

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

	// ポーズメニュー背景の中心座標
	Pause.posWindow = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

	// ポーズメニュー背景の各頂点座標
	pVtx[4].pos = D3DXVECTOR3(Pause.posWindow.x - PAUSE_WINDOWX, Pause.posWindow.y + PAUSE_WINDOWY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(Pause.posWindow.x - PAUSE_WINDOWX, Pause.posWindow.y - PAUSE_WINDOWY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(Pause.posWindow.x + PAUSE_WINDOWX, Pause.posWindow.y + PAUSE_WINDOWY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(Pause.posWindow.x + PAUSE_WINDOWX, Pause.posWindow.y - PAUSE_WINDOWY, 0.0f);

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

	for (int nCnt = 0; nCnt < 4; nCnt++, pVtx += 4)
	{
		// ポーズメニューの中心座標
		Pause.posText[nCnt] = D3DXVECTOR3(SCREEN_WIDTH / 2, PAUSETEXT_POSY + (nCnt * 90.0f), 0.0f);

		// ポーズメニューの各頂点座標
		pVtx[8].pos = D3DXVECTOR3(Pause.posText[nCnt].x - PAUSE_SIZEX, Pause.posText[nCnt].y + PAUSE_SIZEY, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(Pause.posText[nCnt].x - PAUSE_SIZEX, Pause.posText[nCnt].y - PAUSE_SIZEY, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(Pause.posText[nCnt].x + PAUSE_SIZEX, Pause.posText[nCnt].y + PAUSE_SIZEY, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(Pause.posText[nCnt].x + PAUSE_SIZEX, Pause.posText[nCnt].y - PAUSE_SIZEY, 0.0f);

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


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 「あそびかた」表示の中心座標
	Pause.posBackScreen = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	// 「あそびかた」表示の各頂点座標
	pVtx[24].pos = D3DXVECTOR3(Pause.posBackScreen.x - (SCREEN_WIDTH / 2), Pause.posBackScreen.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[25].pos = D3DXVECTOR3(Pause.posBackScreen.x - (SCREEN_WIDTH / 2), Pause.posBackScreen.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[26].pos = D3DXVECTOR3(Pause.posBackScreen.x + (SCREEN_WIDTH / 2), Pause.posBackScreen.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[27].pos = D3DXVECTOR3(Pause.posBackScreen.x + (SCREEN_WIDTH / 2), Pause.posBackScreen.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhwの設定
	pVtx[24].rhw = 1.0f;
	pVtx[25].rhw = 1.0f;
	pVtx[26].rhw = 1.0f;
	pVtx[27].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[24].col = Pause.colOption;
	pVtx[25].col = Pause.colOption;
	pVtx[26].col = Pause.colOption;
	pVtx[27].col = Pause.colOption;

	// 頂点座標の設定
	pVtx[24].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[25].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[26].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[27].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	// ロゴの初期化処理
	InitPauseLogo();

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

	// ロゴの終了処理
	UninitPauseLogo();
}

//==============================================================================
// ポーズメニューの更新処理
//==============================================================================
void UpdatePause(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	FADE fade = GetFade();

	// ポーズからゲーム続行
	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(KEYINFO_PAUSE) == true || IsButtonDown(KEYINFO::KEYINFO_PAUSE) == true)
		{
			if (Pause.PauseMenu != PAUSE_MENU_CONTINUE && Pause.colOption.a <= 0.0f)
			{
				Pause.PauseMenu = PAUSE_MENU_CONTINUE;
				Pause.bOption = false;
				Pause.colOption.a = 0.0f;
			}
		}
	}

	// 「あそびかた」の表示
	if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
	{
		if (Pause.PauseMenu == PAUSE_MENU_OPTION && Pause.colOption.a <= 0.0f)
		{// 「あそびかた」を選択
			Pause.bOption = true;	// 表示処理をON
		}	
	}

	if (Pause.bOption == true)
	{// フェードイン処理
		if (Pause.colOption.a < 1.0f)
		{
			Pause.colOption.a += 0.05f;
		}
		else if (Pause.colOption.a >= 1.0f)
		{// フェードアウトへ移行
			if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
			{
				Pause.bOption = false;
			}
		}
	}
	else if (Pause.bOption == false)
	{// フェードアウト処理
		if (Pause.colOption.a > 0.0f)
		{
			Pause.colOption.a -= 0.05f;
		}
	}

	// 選択画面
	if (Pause.colOption.a <= 0.0f && fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(KEYINFO_DOWN) == true || IsButtonDown(KEYINFO::KEYINFO_DOWN) == true)
		{
			// 音の再生
			PlaySound(SOUND_LABEL_SE_PAUSE_SELECT);

			if (Pause.PauseMenu == PAUSE_MENU_CONTINUE)
			{ // カーソルが「CONTINUE」の時
				Pause.PauseMenu = PAUSE_MENU_RETRY;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_RETRY)
			{ // カーソルが「RETRY」の時
				Pause.PauseMenu = PAUSE_MENU_OPTION;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_OPTION)
			{ // カーソルが「QUIT」の時
				Pause.PauseMenu = PAUSE_MENU_QUIT;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_QUIT)
			{ // カーソルが「QUIT」の時
				Pause.PauseMenu = PAUSE_MENU_CONTINUE;
			}
		}
		else if (GetKeyboardTrigger(KEYINFO_UP) == true || IsButtonDown(KEYINFO::KEYINFO_UP) == true)
		{
			// 音の再生
			PlaySound(SOUND_LABEL_SE_PAUSE_SELECT);

			if (Pause.PauseMenu == PAUSE_MENU_CONTINUE)
			{ // カーソルが「CONTINUE」の時
				Pause.PauseMenu = PAUSE_MENU_QUIT;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_RETRY)
			{ // カーソルが「RETRY」の時
				Pause.PauseMenu = PAUSE_MENU_CONTINUE;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_OPTION)
			{ // カーソルが「QUIT」の時
				Pause.PauseMenu = PAUSE_MENU_RETRY;
			}
			else if (Pause.PauseMenu == PAUSE_MENU_QUIT)
			{ // カーソルが「QUIT」の時
				Pause.PauseMenu = PAUSE_MENU_OPTION;
			}
		}
	}

	// テクスチャの切り替え
	switch (Pause.PauseMenu)
	{
	case PAUSE_MENU_CONTINUE:	// カーソルが「CONTINUE」の時
		Pause.fContinue = 0.0f;
		Pause.fRetry = 0.5f;
		Pause.fOption = 0.5f;
		Pause.fQuit = 0.5f;
		break;

	case PAUSE_MENU_RETRY:		// カーソルが「RETRY」の時
		Pause.fContinue = 0.5f;
		Pause.fRetry = 0.0f;
		Pause.fOption = 0.5f;
		Pause.fQuit = 0.5f;
		break;

	case PAUSE_MENU_OPTION:		// カーソルが「OPTION」の時
		Pause.fContinue = 0.5f;
		Pause.fRetry = 0.5f;
		Pause.fOption = 0.0f;
		Pause.fQuit = 0.5f;
		break;

	case PAUSE_MENU_QUIT:		// カーソルが「QUIT」の時
		Pause.fContinue = 0.5f;
		Pause.fRetry = 0.5f;
		Pause.fOption = 0.5f;
		Pause.fQuit = 0.0f;
		break;

	default:
		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 各テクスチャの更新
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.5f + Pause.fContinue);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f + Pause.fContinue);
	pVtx[10].tex = D3DXVECTOR2(1.0f, 0.5f + Pause.fContinue);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 0.0f + Pause.fContinue);

	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.5f + Pause.fRetry);
	pVtx[13].tex = D3DXVECTOR2(0.0f, 0.0f + Pause.fRetry);
	pVtx[14].tex = D3DXVECTOR2(1.0f, 0.5f + Pause.fRetry);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 0.0f + Pause.fRetry);

	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.5f + Pause.fOption);
	pVtx[17].tex = D3DXVECTOR2(0.0f, 0.0f + Pause.fOption);
	pVtx[18].tex = D3DXVECTOR2(1.0f, 0.5f + Pause.fOption);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 0.0f + Pause.fOption);

	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.5f + Pause.fQuit);
	pVtx[21].tex = D3DXVECTOR2(0.0f, 0.0f + Pause.fQuit);
	pVtx[22].tex = D3DXVECTOR2(1.0f, 0.5f + Pause.fQuit);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 0.0f + Pause.fQuit);

	pVtx[24].col = Pause.colOption;
	pVtx[25].col = Pause.colOption;
	pVtx[26].col = Pause.colOption;
	pVtx[27].col = Pause.colOption;

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

	// ロゴの更新処理
	UpdatePauseLogo();
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
	for (int nCnt = 0; nCnt < 7; nCnt++)
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

	// ロゴの描画処理
	DrawPauseLogo();
}

//==============================================================================
// ポーズの情報取得
//==============================================================================
PAUSE GetPause(void)
{
	return Pause;
}