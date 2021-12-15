//==============================================================================
//
// 選択カーソルの描画〔select_cursor.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
//#define _CRT_SECURE_NO_WARNINGS
#include "select.h"
#include "select_cursor.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

#define CURSOR_NUM	(3)		// カーソルの数
#define CURSOR_SIZE (75.0f)	// カーソルのサイズ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureCursor[CURSOR_NUM] = {};		// 選択画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCursor = NULL;			// バッファへのポインタ
Cursor g_Cursor/*[CURSOR_NUM]*/;								// カーソルの構造体

//==============================================================================
// 選択画面の初期化処理
//==============================================================================
HRESULT InitCursor(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/cursor0.png",
		&g_pTextureCursor[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/cursor1.png",
		&g_pTextureCursor[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/cursor2.png",
		&g_pTextureCursor[2]);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CURSOR_NUM,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffCursor,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	for (int nCnt = 0; nCnt < CURSOR_NUM; nCnt++)
	{
		g_Cursor.cursor[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cursor.cursor[nCnt].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
		g_Cursor.cursor[nCnt].fWidth = CURSOR_SIZE;
		g_Cursor.cursor[nCnt].fHeight = CURSOR_SIZE;
		g_Cursor.cursor[nCnt].cursorType = (CURSORTYPE)nCnt;
	}
	g_Cursor.cursor[0].bUse = false;	// 1Pカーソル
	g_Cursor.cursor[1].bUse = false;	// 2Pカーソル
	g_Cursor.cursor[2].bUse = false;	// CPUカーソル
	g_Cursor.nCharNum = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CURSOR_NUM; nCnt++, pVtx += 4)
	{
		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_Cursor.cursor[nCnt].pos.x, g_Cursor.cursor[nCnt].pos.y + g_Cursor.cursor[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Cursor.cursor[nCnt].pos.x, g_Cursor.cursor[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Cursor.cursor[nCnt].pos.x + g_Cursor.cursor[nCnt].fWidth, g_Cursor.cursor[nCnt].pos.y + g_Cursor.cursor[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Cursor.cursor[nCnt].pos.x + g_Cursor.cursor[nCnt].fWidth, g_Cursor.cursor[nCnt].pos.y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = g_Cursor.cursor[nCnt].col;
		pVtx[1].col = g_Cursor.cursor[nCnt].col;
		pVtx[2].col = g_Cursor.cursor[nCnt].col;
		pVtx[3].col = g_Cursor.cursor[nCnt].col;

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffCursor->Unlock();

	return S_OK;
}

//==============================================================================
// 選択画面の終了処理
//==============================================================================
void UninitCursor(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffCursor != NULL)
	{
		g_pVtxBuffCursor->Release();
		g_pVtxBuffCursor = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < CURSOR_NUM; nCnt++)
	{
		if (g_pTextureCursor[nCnt] != NULL)
		{
			g_pTextureCursor[nCnt]->Release();
			g_pTextureCursor[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 選択画面の更新処理
//==============================================================================
void UpdateCursor(void)
{
	// ローカル変数宣言
	Select select = *GetSelect();
	FADE fade = GetFade();							// フェードの取得

	if (fade == FADE_NONE)
	{// フェードしていないとき
		g_Cursor.cursor[CURSORTYPE_PLAYER1].bUse = true;	// 1Pカーソルの有効化

		switch (select.nCntPage)
		{// 各選択時のカーソル
		case 0:	// モード選択
			g_Cursor.nCharNum = 0;
			if (GetKeyboardTrigger(KEYINFO_OK) == true || BuDo(KEYINFO::KEYINFO_OK, 0) == true)
			{
				g_Cursor.cursor[CURSORTYPE_PLAYER1].state = CURSORSTATE_BLINK;
			}
			
			break;

		case 1:	// キャラクター選択
			if (select.mode == PLAYMODE_1)
			{
				g_Cursor.cursor[CURSORTYPE_PLAYER2].bUse = true;	// 2Pカーソルの有効化
			}

			// 1P対CPUの時の選択
			if (g_Cursor.nCharNum == 0)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 0) == true)
				{
					g_Cursor.cursor[CURSORTYPE_PLAYER1].state = CURSORSTATE_BLINK;

					if (select.mode == PLAYMODE_0)
					{
						g_Cursor.cursor[CURSORTYPE_CPU].bUse = true;
					}

					g_Cursor.nCharNum++;
				}
			}
			else
			{
				if (g_Cursor.cursor[CURSORTYPE_CPU].bUse == true)
				{
					if (BuDo(KEYINFO::KEYINFO_OK, 0) == true)
					{
						g_Cursor.cursor[CURSORTYPE_CPU].state = CURSORSTATE_BLINK;
					}
				}
			}

			if (g_Cursor.cursor[CURSORTYPE_PLAYER2].bUse == true)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 1) == true)
				{
					g_Cursor.cursor[CURSORTYPE_PLAYER2].state = CURSORSTATE_BLINK;
				}
			}

			break;

		case 2:
			g_Cursor.nCharNum = 0;
			g_Cursor.cursor[CURSORTYPE_PLAYER2].bUse = false;
			g_Cursor.cursor[CURSORTYPE_CPU].bUse = false;

			break;

		default:
			break;
		}

		if (select.bBack == true)
		{
			g_Cursor.cursor[CURSORTYPE_PLAYER1].state = CURSORSTATE_ERASE;
			g_Cursor.cursor[CURSORTYPE_PLAYER2].state = CURSORSTATE_ERASE;
			g_Cursor.cursor[CURSORTYPE_CPU].state = CURSORSTATE_ERASE;
			g_Cursor.cursor[CURSORTYPE_PLAYER2].bUse = false;
			g_Cursor.cursor[CURSORTYPE_CPU].bUse = false;
		}

	}

	//if (g_Cursor.cursor[CURSORTYPE_PLAYER1].nCntBlink >= 60)
	//{
	//	select.bMove = true;
	//}

	CursorState();

	//if (GetKeyboardTrigger(KEYINFO_CANCEL) == true && select.nCntPage > 0)
	//{// キャンセル操作
	// // 音の再生
	//	PlaySound(SOUND_LABEL_SE_CANCEL);
	//	for (int nCnt = 0; nCnt < CURSORTYPE_MAX; nCnt++)
	//	{
	//		g_Cursor.cursor[nCnt].state = CURSORSTATE_ERASE;
	//	}
	//}

	// 頂点座標更新
	for (int nCnt = 0; nCnt < CURSORTYPE_MAX; nCnt++)
	{
		if (g_Cursor.cursor[nCnt].bUse == true)
		{
			SetVertexCursor(nCnt);
		}
	}
}


//==============================================================================
// 選択画面の描画処理
//==============================================================================
void DrawCursor(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CursorInfo *pCursor = &g_Cursor.cursor[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffCursor,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < CURSOR_NUM; nCnt++,pCursor++)
	{
		if (pCursor->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCursor[nCnt]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 頂点情報更新
//==============================================================================
void SetVertexCursor(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCursor->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * nIdx;

	// テクスチャの頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_Cursor.cursor[nIdx].pos.x, g_Cursor.cursor[nIdx].pos.y + g_Cursor.cursor[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Cursor.cursor[nIdx].pos.x, g_Cursor.cursor[nIdx].pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Cursor.cursor[nIdx].pos.x + g_Cursor.cursor[nIdx].fWidth, g_Cursor.cursor[nIdx].pos.y + g_Cursor.cursor[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Cursor.cursor[nIdx].pos.x + g_Cursor.cursor[nIdx].fWidth, g_Cursor.cursor[nIdx].pos.y, 0.0f);

	pVtx[0].col = g_Cursor.cursor[nIdx].col;
	pVtx[1].col = g_Cursor.cursor[nIdx].col;
	pVtx[2].col = g_Cursor.cursor[nIdx].col;
	pVtx[3].col = g_Cursor.cursor[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffCursor->Unlock();
}

// カーソルの制御
void CursorManager(void)
{
	Select select = *GetSelect();

	switch (select.nCntPage)
	{
	case 0:	// モード選択時
		switch (select.mode)
		{
		case PLAYMODE_0:
			g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(240.0f, 150.0f, 0.0f);
			break;

		case PLAYMODE_1:
			g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(820.0f, 150.0f, 0.0f);
			break;

		default:
			break;
		}
		break;

	case 1:	// キャラ選択時
		if (select.mode == PLAYMODE_0)
		{// 1人プレイモード
			switch (select.chara[0])
			{
			case CHARA_0:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(240.0f, 150.0f, 0.0f);
				break;

			case CHARA_1:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(530.0f, 150.0f, 0.0f);
				break;

			case CHARA_2:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(820.0f, 150.0f, 0.0f);
				break;

			default:
				break;
			}

			if (select.bDecide[CURSORTYPE_PLAYER1] == true)
			{// 1Pキャラクター決定時
				g_Cursor.cursor[CURSORTYPE_CPU].bUse = true;
				switch (select.chara[1])
				{// CPUカーソル
				case CHARA_0:
					g_Cursor.cursor[CURSORTYPE_CPU].pos = D3DXVECTOR3(385.0f, 150.0f, 0.0f);
					break;

				case CHARA_1:
					g_Cursor.cursor[CURSORTYPE_CPU].pos = D3DXVECTOR3(675.0f, 150.0f, 0.0f);
					break;

				case CHARA_2:
					g_Cursor.cursor[CURSORTYPE_CPU].pos = D3DXVECTOR3(965.0f, 150.0f, 0.0f);
					break;

				default:
					break;
				}
			}
		}
		else
		{	// 二人プレイ
			g_Cursor.cursor[CURSORTYPE_PLAYER2].bUse = true;

			switch (select.chara[0])
			{
			case CHARA_0:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(240.0f, 150.0f, 0.0f);
				break;

			case CHARA_1:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(530.0f, 150.0f, 0.0f);
				break;

			case CHARA_2:
				g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(820.0f, 150.0f, 0.0f);
				break;

			default:
				break;
			}

			switch (select.chara[1])
			{// CPUカーソル
			case CHARA_0:
				g_Cursor.cursor[CURSORTYPE_PLAYER2].pos = D3DXVECTOR3(385.0f, 150.0f, 0.0f);
				break;

			case CHARA_1:
				g_Cursor.cursor[CURSORTYPE_PLAYER2].pos = D3DXVECTOR3(675.0f, 150.0f, 0.0f);
				break;

			case CHARA_2:
				g_Cursor.cursor[CURSORTYPE_PLAYER2].pos = D3DXVECTOR3(965.0f, 150.0f, 0.0f);
				break;

			default:
				break;
			}
		}


		break;

	case 2:	// ステージ選択時
		switch (select.stage)
		{
		case STAGE_0:
			g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(240.0f, 150.0f, 0.0f);
			break;

		case STAGE_1:
			g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(530.0f, 150.0f, 0.0f);
			break;

		case STAGE_2:
			g_Cursor.cursor[CURSORTYPE_PLAYER1].pos = D3DXVECTOR3(820.0f, 150.0f, 0.0f);
			break;

		default:
			break;
		}

		break;

	default:
		break;
	}
}

// カーソルの取得
Cursor GetCursorS(void)
{
	return g_Cursor;
}

// カーソルの状態
void CursorState(void)
{
	// ローカル変数宣言
	CursorInfo *pCursor = &g_Cursor.cursor[0];
	Select select = *GetSelect();

	for (int nCnt = 0; nCnt < CURSORTYPE_MAX; nCnt++,pCursor++)
	{
		switch (pCursor->state)
		{
		case CURSORSTATE_NORMAL:	// 通常時
			pCursor->col.a = 1.0f;
			break;

		case CURSORSTATE_BLINK:		// 点滅時
			pCursor->nCntBlink++;
			if (pCursor->nCntBlink % 2 == 0)
			{
				pCursor->col.a = 1.0f;	// 出現
			}
			else
			{
				pCursor->col.a = 0.0f;	// 消滅
			}

			if (select.bMove == true)
			{
				g_Cursor.bBlinkEnd = true;
			}

			if (g_Cursor.bBlinkEnd == true)
			{
				pCursor->nCntBlink = 0;
				pCursor->state = CURSORSTATE_ERASE;	// 消滅状態へ
			}

			break;
			
		case CURSORSTATE_ERASE:		// 消滅時
			pCursor->col.a = 0.0f;	
			g_Cursor.bBlinkEnd = false;

			if (select.bMove == false)
			{	
				pCursor->state = CURSORSTATE_NORMAL;	// 通常状態へ
			}

			break;

		default:
			break;
		}
	}
}