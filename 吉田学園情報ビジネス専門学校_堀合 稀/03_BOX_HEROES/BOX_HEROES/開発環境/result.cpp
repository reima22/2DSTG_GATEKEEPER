//==============================================================================
//
// リザルト画面の描画〔result.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "result.h"
#include "input.h"
#include "player.h"
#include "hp.h"
#include "sound.h"
#include "fade.h"
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define RESULT_POLYGON		(5)		// 最大ポリゴン数
#define TEXTURE_RESULT		(5)		// テクスチャの数
#define P_SIZE				(D3DXVECTOR3(100.0f,50.0f,0.0f))
#define P_POS_1				(D3DXVECTOR3(320.0f,200.0f,0.0f))
#define P_POS_2				(D3DXVECTOR3(960.0f,200.0f,0.0f))

#define WL_SIZE				(D3DXVECTOR3(200.0f,50.0f,0.0f))
#define WL_POS_1			(D3DXVECTOR3(320.0f,400.0f,0.0f))
#define WL_POS_2			(D3DXVECTOR3(960.0f,400.0f,0.0f))

//#define RESULTLOGO_POSX		(640.0f)	// リザルトロゴの中心X座標
//#define RESULTLOGO_POSY		(200.0f)	// リザルトロゴの中心Y座標
//#define RESULTLOGO_SIZEX	(240.0f)	// リザルトロゴの中心からの幅
//#define RESULTLOGO_SIZEY	(40.0f)		// リザルトロゴの中心からの高さ
//#define RESULT_PLATE_SIZEX	(240.0f)	// リザルトスコアプレートの幅
//#define RESULT_PLATE_SIZEY	(40.0f)		// リザルトスコアプレートの高さ
//#define RESULT_SCOREX		(500)		// リザルトスコアの上一桁目のX座標
//#define RESULT_SCOREY		(360)		// リザルトスコアの上一桁目のY座標
//#define RESULT_SCORE_SIZEX	(20.0f)		// リザルトスコアの一桁の幅
//#define RESULT_SCORE_SIZEY	(30.0f)		// リザルトスコアの一桁の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[TEXTURE_RESULT] = {};	// リザルト画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			// バッファへのポインタ
D3DXVECTOR3 g_posResult[RESULT_POLYGON];					// リザルト画面の位置

//D3DXVECTOR3 g_posResultlogo;								// リザルトロゴの位置
//D3DXVECTOR3 g_posResultPlate;								// スコアのプレート
//D3DXVECTOR3 g_posResultScore[MAX_SCORE];					// リザルトスコアの位置

//==============================================================================
// リザルト画面の初期化処理
//==============================================================================
HRESULT InitResult(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	//テキスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice, 
		"data/TEXTURE/result/result_bg.jpg", 
		&g_pTextureResult[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result/1p.png",
		&g_pTextureResult[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result/2p.png",
		&g_pTextureResult[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result/winner.png",
		&g_pTextureResult[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/result/loser.png",
		&g_pTextureResult[4]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * RESULT_POLYGON,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,								// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL)))
	{
		return E_FAIL;
	}

	g_posResult[0] = D3DXVECTOR3(SCREEN_CENTERX, SCREEN_CENTERY, 0.0f);
	g_posResult[1] = P_POS_1;
	g_posResult[2] = P_POS_2;
	g_posResult[3] = WL_POS_1;
	g_posResult[4] = WL_POS_2;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < RESULT_POLYGON; nCnt++,pVtx += 4)
	{
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
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	bool bResultbgm = RESULT_BGM;
	if (bResultbgm == true)
	{
		// BGMの再生
		PlaySound(SOUND_LABEL_RESULT);
	}

	return S_OK;
}

//==============================================================================
// リザルト画面の終了処理
//==============================================================================
void UninitResult(void)
{
	// サウンドの停止処理
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TEXTURE_RESULT; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}
}

//==============================================================================
// リザルト画面の更新処理
//==============================================================================
void UpdateResult(void)
{
	// ローカル変数宣言
	FADE fade = GetFade();

	for (int nCnt = 0; nCnt < RESULT_POLYGON; nCnt++)
	{
		SetVtxResult(nCnt);
	}
	
	// ランキング画面への切り替え
	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(KEYINFO_OK) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
	}
}

//==============================================================================
// リザルト画面の描画処理
//==============================================================================
void DrawResult(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	HP *pHP = GetHp();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (int nCnt = 0; nCnt < RESULT_POLYGON; nCnt++)
	{
		if (nCnt <= 2)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult[nCnt]);
		}
		else
		{
			// テクスチャの設定
			if (pHP->fBar >= HP_LIMIT)
			{
				if (nCnt == 3)
				{
					pDevice->SetTexture(0, g_pTextureResult[4]);
				}
				else
				{
					pDevice->SetTexture(0, g_pTextureResult[3]);
				}
			}
			else
			{
				if (nCnt == 3)
				{
					pDevice->SetTexture(0, g_pTextureResult[3]);
				}
				else
				{
					pDevice->SetTexture(0, g_pTextureResult[4]);
				}
			}
		}

		

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}			
}

//==============================================================================
// 頂点座標更新
//==============================================================================
void SetVtxResult(int nCnt)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	switch (nCnt)
	{
	case 0:	// 背景
		pVtx[0].pos = D3DXVECTOR3(g_posResult[nCnt].x - SCREEN_CENTERX, g_posResult[nCnt].y + SCREEN_CENTERY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResult[nCnt].x - SCREEN_CENTERX, g_posResult[nCnt].y - SCREEN_CENTERY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResult[nCnt].x + SCREEN_CENTERX, g_posResult[nCnt].y + SCREEN_CENTERY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResult[nCnt].x + SCREEN_CENTERX, g_posResult[nCnt].y - SCREEN_CENTERY, 0.0f);
		break;

	case 1:
		pVtx[0].pos = D3DXVECTOR3(g_posResult[nCnt].x - P_SIZE.x, g_posResult[nCnt].y + P_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResult[nCnt].x - P_SIZE.x, g_posResult[nCnt].y - P_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResult[nCnt].x + P_SIZE.x, g_posResult[nCnt].y + P_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResult[nCnt].x + P_SIZE.x, g_posResult[nCnt].y - P_SIZE.y, 0.0f);
		break;

	case 2:
		pVtx[0].pos = D3DXVECTOR3(g_posResult[nCnt].x - P_SIZE.x, g_posResult[nCnt].y + P_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResult[nCnt].x - P_SIZE.x, g_posResult[nCnt].y - P_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResult[nCnt].x + P_SIZE.x, g_posResult[nCnt].y + P_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResult[nCnt].x + P_SIZE.x, g_posResult[nCnt].y - P_SIZE.y, 0.0f);
		break;

	case 3:
		pVtx[0].pos = D3DXVECTOR3(g_posResult[nCnt].x - WL_SIZE.x, g_posResult[nCnt].y + WL_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResult[nCnt].x - WL_SIZE.x, g_posResult[nCnt].y - WL_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResult[nCnt].x + WL_SIZE.x, g_posResult[nCnt].y + WL_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResult[nCnt].x + WL_SIZE.x, g_posResult[nCnt].y - WL_SIZE.y, 0.0f);
		break;

	case 4:
		pVtx[0].pos = D3DXVECTOR3(g_posResult[nCnt].x - WL_SIZE.x, g_posResult[nCnt].y + WL_SIZE.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posResult[nCnt].x - WL_SIZE.x, g_posResult[nCnt].y - WL_SIZE.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posResult[nCnt].x + WL_SIZE.x, g_posResult[nCnt].y + WL_SIZE.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posResult[nCnt].x + WL_SIZE.x, g_posResult[nCnt].y - WL_SIZE.y, 0.0f);
		break;

	default:
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}