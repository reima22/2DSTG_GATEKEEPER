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
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define TITLE_TEX			(3)		// タイトルのテクスチャ数
#define TITLE_POLYGON		(4)		// タイトル画面のポリゴン数
#define SIGN_POLYGON		(2)		// エンターサインの分割数
#define TITLE_LOGOX			(640)	// タイトルロゴのX座標
#define TITLE_FIRST_LOGOY	(-50)	// タイトルロゴの最初のY座標
#define TITLE_LAST_LOGOY	(200)	// タイトルロゴの最終Y座標
#define TITLE_LOGO_SIZEX	(480)	// タイトルロゴの幅
#define TITLE_LOGO_SIZEY	(80)	// タイトルロゴの高さ
#define TITLE_ENTER_POSY	(510)	// エンターサインのY軸の位置
#define TITLE_ENTER_SIZEX	(480)	// エンターサインの幅
#define TITLE_ENTER_SIZEY	(80)	// エンターサインの高さ
#define MOVESIGN			(20)	// エンターサインの移動量
#define RANKJUMP			(600)	// ランキングへの自動遷移カウント

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLE_TEX] = {};		// タイトル画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// バッファへのポインタ
TITLE title;

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
		"data/TEXTURE/title_bg.jpg",
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
		FVF_VERTEX_2D,							// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// 構造体の初期化
	title.Sign = ENTERSIGN_NONE;
	title.colSign = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	title.titleauto = TITLE_NONE;
	title.nCntMoveLogo = 0;
	title.nShiftCount = 0;
	title.nSignCounter = 0;
	title.nSignAppear = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトル画面(背景)の中心座標
	title.posTitle = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(title.posTitle.x - SCREEN_CENTERX, title.posTitle.y + SCREEN_CENTERY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(title.posTitle.x - SCREEN_CENTERX, title.posTitle.y - SCREEN_CENTERY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(title.posTitle.x + SCREEN_CENTERX, title.posTitle.y + SCREEN_CENTERY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(title.posTitle.x + SCREEN_CENTERX, title.posTitle.y - SCREEN_CENTERY, 0.0f);

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
	title.posTitlelogo = D3DXVECTOR3(TITLE_LOGOX, TITLE_FIRST_LOGOY, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(title.posTitlelogo.x - TITLE_LOGO_SIZEX, title.posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(title.posTitlelogo.x - TITLE_LOGO_SIZEX, title.posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(title.posTitlelogo.x + TITLE_LOGO_SIZEX, title.posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(title.posTitlelogo.x + TITLE_LOGO_SIZEX, title.posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

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

	for (int nCnt = 0; nCnt < SIGN_POLYGON; nCnt++,pVtx += 4)
	{
		// エンターサインの中心座標
		title.posTitleEnter[nCnt] = D3DXVECTOR3(-TITLE_ENTER_SIZEX + ((float)nCnt * (SCREEN_WIDTH + (TITLE_ENTER_SIZEX * 2))), TITLE_ENTER_POSY + ((float)nCnt *TITLE_ENTER_SIZEY), 0.0f);

		// ポリゴンの各頂点座標
		pVtx[8].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x - TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y + TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x - TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y - TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x + TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y + TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x + TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y - TITLE_ENTER_SIZEY / 2, 0.0f);

		// rhwの設定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[8].col = title.colSign;
		pVtx[9].col = title.colSign;
		pVtx[10].col = title.colSign;
		pVtx[11].col = title.colSign;

		// 頂点座標の設定
		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.5f + nCnt * 0.5f);
		pVtx[9].tex = D3DXVECTOR2(0.0f, nCnt * 0.5f);
		pVtx[10].tex = D3DXVECTOR2(1.0f, 0.5f + nCnt * 0.5f);
		pVtx[11].tex = D3DXVECTOR2(1.0f, nCnt * 0.5f);
	}

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
	for (int nCnt = 0; nCnt < TITLE_TEX; nCnt++)
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

	// フェードの取得
	fade = GetFade();

	// タイトルロゴの移動
	if (title.posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		title.nCntMoveLogo++;
		if (title.nCntMoveLogo % 2 == 0)
		{
			title.posTitlelogo.y += 2;
			if (title.posTitlelogo.y >= TITLE_LAST_LOGOY)
			{
				title.posTitlelogo.y = TITLE_LAST_LOGOY;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(title.posTitlelogo.x - TITLE_LOGO_SIZEX, title.posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(title.posTitlelogo.x - TITLE_LOGO_SIZEX, title.posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(title.posTitlelogo.x + TITLE_LOGO_SIZEX, title.posTitlelogo.y + TITLE_LOGO_SIZEY, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(title.posTitlelogo.x + TITLE_LOGO_SIZEX, title.posTitlelogo.y - TITLE_LOGO_SIZEY, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	if (title.posTitlelogo.y != TITLE_LAST_LOGOY)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			title.posTitlelogo.y = TITLE_LAST_LOGOY - 1;
		}
	}

	if (title.Sign == ENTERSIGN_NORMAL)
	{
		if (fade != FADE_OUT)
		{
			title.nShiftCount++;	// 自動画面遷移カウンターの増加
		}
		
		if (title.nShiftCount >= RANKJUMP)
		{ // 自動遷移実行
		  // フェードアウト
			SetFade(FADE_OUT, MODE_RANKING);
			title.titleauto = TITLE_AUTO;
		}
	}

	// エンターキー入力でゲーム画面へ遷移
	if (title.Sign == ENTERSIGN_NORMAL && title.nShiftCount < RANKJUMP)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			if (title.Sign != ENTERSIGN_ACTIVE)
			{
				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_START000);
			}

			// フェードアウト
			SetFade(FADE_OUT, MODE_TUTORIAL);

			// エンターサインの状態変化
			title.Sign = ENTERSIGN_ACTIVE;
		}
	}

	// エンターサインの状態
	EnterSign();
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
		if (nCnt == 3)
		{
			pDevice->SetTexture(0, g_pTextureTitle[2]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureTitle[nCnt]);
		}

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// 自動遷移情報の取得
//==============================================================================
TITLE GetTitle(void)
{
	return title;
}

//==============================================================================
// エンターサインの表示状態
//==============================================================================
void EnterSign(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// エンターサインの状態
	switch (title.Sign)
	{
	case ENTERSIGN_NORMAL:	// サインの点滅
		title.nSignCounter++;	// エンターサインカウンターの増加
		if (title.nSignCounter % 60 == 0)
		{
			title.colSign.a = 0.0f;	// 透明度0へ
		}
		else if (title.nSignCounter % 30 == 0 && title.nSignCounter % 60 != 0)
		{
			title.colSign.a = 1.0f;	// 透明度最大へ
		}

		break;

	case ENTERSIGN_NONE:
		// 移動状態への移行
		if (title.posTitlelogo.y == TITLE_LAST_LOGOY)
		{
			title.nSignAppear++;	// エンターサイン出現カウントの増加

			if (title.nSignAppear >= 30 || GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
			{
				title.Sign = ENTERSIGN_MOVE;
			}
		}

		break;

	case ENTERSIGN_ACTIVE:
		title.nSignCounter++;	// エンターサインカウンターの増加
		if (title.nSignCounter % 4 == 0)
		{
			title.colSign.a = 0.0f;	// 透明度0へ
		}
		else if (title.nSignCounter % 2 == 0 && title.nSignCounter % 4 != 0)
		{
			title.colSign.a = 1.0f	;// 透明度最大へ
		}

		break;

	case ENTERSIGN_MOVE:	// サインのアニメーション
		// 画面左端からサインの上半分が出現
		if (title.posTitleEnter[0].x != SCREEN_CENTERX)
		{
			title.posTitleEnter[0].x += MOVESIGN;
			if (title.posTitleEnter[0].x >= SCREEN_CENTERX)
			{
				title.posTitleEnter[0].x = SCREEN_CENTERX;
			}
		}

		// 画面右端からサインの下半分が出現
		if (title.posTitleEnter[1].x != SCREEN_CENTERX)
		{
			title.posTitleEnter[1].x -= MOVESIGN;
			if (title.posTitleEnter[1].x <= SCREEN_CENTERX)
			{
				title.posTitleEnter[1].x = SCREEN_CENTERX;
			}
		}

		// ボタン入力で短縮
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			for (int nCnt = 0; nCnt < SIGN_POLYGON; nCnt++)
			{
				title.posTitleEnter[nCnt].x = SCREEN_CENTERX;
			}
		}

		// 通常状態へ移行
		if (title.posTitleEnter[0].x == SCREEN_CENTERX)
		{
			title.Sign = ENTERSIGN_NORMAL;
		}

		break;

	default:
		break;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SIGN_POLYGON; nCnt++, pVtx += 4)
	{
		// ポリゴンの各頂点座標
		pVtx[8].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x - TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y + TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x - TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y - TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x + TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y + TITLE_ENTER_SIZEY / 2, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(title.posTitleEnter[nCnt].x + TITLE_ENTER_SIZEX, title.posTitleEnter[nCnt].y - TITLE_ENTER_SIZEY / 2, 0.0f);

		// 各頂点カラーの更新
		pVtx[8].col = title.colSign;
		pVtx[9].col = title.colSign;
		pVtx[10].col = title.colSign;
		pVtx[11].col = title.colSign;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}