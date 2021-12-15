//==============================================================================
//
// 選択画面の描画〔select.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "select.h"
#include "input.h"
#include "fade.h"
#include "gamepad.h"
#include "select_cursor.h"
#include "sound.h"
#include <stdio.h>

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureSelect[ALL_TEX] = {};		// 選択画面テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;	// バッファへのポインタ
Select g_select;

//==============================================================================
// 選択画面の初期化処理
//==============================================================================
HRESULT InitSelect(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	LoadSelect();

	TexSetSelect();

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ALL_SET,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	for (int nCnt = 0; nCnt < ALL_SET; nCnt++)
	{
		g_select.selectInfo[nCnt].move = D3DXVECTOR3(0.0f, -0.0f, 0.0f);
		g_select.bDecide[nCnt] = false;
		g_select.chara[nCnt] = CHARA_0;
	}
	g_select.mode = PLAYMODE_1;
	g_select.stage = STAGE_0;
	g_select.bBack = false;
	g_select.b2PlayerOn = false;
	g_select.bMove = false;
	g_select.nCntAnim = 0;
	g_select.nCntPage = 0;
	g_select.nMoveCnt = 0;
	g_select.SType = SELECTTYPE_MODE;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ALL_SET; nCnt++, pVtx += 4)
	{
		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_select.selectInfo[nCnt].pos.x, g_select.selectInfo[nCnt].pos.y + g_select.selectInfo[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_select.selectInfo[nCnt].pos.x, g_select.selectInfo[nCnt].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_select.selectInfo[nCnt].pos.x + g_select.selectInfo[nCnt].fWidth, g_select.selectInfo[nCnt].pos.y + g_select.selectInfo[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_select.selectInfo[nCnt].pos.x + g_select.selectInfo[nCnt].fWidth, g_select.selectInfo[nCnt].pos.y, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = g_select.selectInfo[nCnt].col;
		pVtx[1].col = g_select.selectInfo[nCnt].col;
		pVtx[2].col = g_select.selectInfo[nCnt].col;
		pVtx[3].col = g_select.selectInfo[nCnt].col;

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();

	// 音の再生
	PlaySound(SOUND_LABEL_SELECT);

	return S_OK;
}

//==============================================================================
// 選択画面の終了処理
//==============================================================================
void UninitSelect(void)
{
	// サウンドの停止処理
	StopSound();

	// 頂点バッファの開放
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < ALL_TEX; nCnt++)
	{
		if (g_pTextureSelect[nCnt] != NULL)
		{
			g_pTextureSelect[nCnt]->Release();
			g_pTextureSelect[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 選択画面の更新処理
//==============================================================================
void UpdateSelect(void)
{
	SelectManager();
}

//==============================================================================
// 選択画面の描画処理
//==============================================================================
void DrawSelect(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SelectInfo *pSelect = &g_select.selectInfo[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffSelect,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < ALL_SET; nCnt++,pSelect++)
	{
		if (pSelect->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSelect[pSelect->nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCnt * 4,				// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 選択画面の管理
//==============================================================================
void SelectManager(void)
{
	// ローカル変数宣言
	FADE fade = GetFade();							// フェードの取得
	SelectInfo *pSelect = &g_select.selectInfo[0];	// 選択画面情報の取得
	Cursor *pCursor = &GetCursorS();

	// 移動実行
	if (g_select.bMove == true)
	{
		// カウント増加
		g_select.nMoveCnt++;

		// 移動終了とリセット
		if (g_select.nMoveCnt > (int)SCREEN_HEIGHT / 10)
		{
			g_select.nMoveCnt = 0;
			g_select.bMove = false;
			g_select.bDecide[0] = false;
			g_select.bDecide[1] = false;

			if (g_select.bBack == false)
			{
				g_select.nCntPage++;
			}
			else if (g_select.bBack == true)
			{
				g_select.nCntPage--;
				g_select.bBack = false;
			}
		}

		// 移動処理
		for (int nCnt = 0; nCnt < ALL_SET; nCnt++, pSelect++)
		{
			if (pSelect->bUse == true && pSelect->st != 0)
			{// 選択画面内のポリゴン移動
				if (g_select.bBack == false)
				{// 通常に選択されたときの移動力
					pSelect->move.y = -SELECT_MOVE;
				}
				else if (g_select.bBack == true)
				{// キャンセルされたときの移動
					pSelect->move.y = SELECT_MOVE;
				}

				if (g_select.bMove == true)
				{// 移動力の加算
					pSelect->pos += pSelect->move;
				}
			}
			SetVertexSelect(nCnt);	// 頂点座標の更新
		}
	}

	if (fade == FADE_NONE)
	{// フェードしていないとき
		switch (g_select.nCntPage)
		{
		case 0:
			if (pCursor->cursor[CURSORTYPE_PLAYER1].nCntBlink >= 60)
			{
				g_select.bMove = true;	// 移動開始
			}

			if (g_select.bDecide[CURSORTYPE_PLAYER1] == false)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 0) == true && g_select.bMove == false)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					g_select.bDecide[0] = true;

					if (g_select.mode == PLAYMODE_1)
					{
						g_select.b2PlayerOn = true;
					}
				}
			}
			break;

		case 1:
			if (pCursor->cursor[CURSORTYPE_PLAYER1].nCntBlink >= 60)
			{
				if (pCursor->cursor[CURSORTYPE_PLAYER2].nCntBlink >= 60 ||
					pCursor->cursor[CURSORTYPE_CPU].nCntBlink >= 60)
				{
					g_select.bMove = true;
				}
			}

			if (g_select.bDecide[0] == false)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 0) == true && g_select.bMove == false)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					g_select.bDecide[0] = true;
				}
			}

			if (pCursor->cursor[CURSORTYPE_PLAYER2].bUse == true)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 1) == true && g_select.bMove == false && g_select.bDecide[1] == false)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					g_select.bDecide[1] = true;
				}
			}
			else if (pCursor->cursor[CURSORTYPE_CPU].bUse == true)
			{
				if (BuDo(KEYINFO::KEYINFO_OK, 0) == true && g_select.bMove == false)
				{
					PlaySound(SOUND_LABEL_SE_DECIDE);

					g_select.bDecide[1] = true;
				}
			}

			break;

		case 2:

			if (BuDo(KEYINFO::KEYINFO_OK, 0) == true && g_select.bMove == false)
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);

				g_select.bDecide[0] = true;

				SetFade(FADE_OUT, MODE_TUTORIAL);	// 画面遷移
			}
			break;
		default:
			break;
		}

		if (g_select.nCntPage > 0)
		{// キャンセル操作
		 

			if (g_select.bMove == false) 
			{
				// 音の再生
				if (GetKeyboardTrigger(KEYINFO_CANCEL) == true || BuDo(KEYINFO::KEYINFO_CANCEL, 0) == true)
				{
					PlaySound(SOUND_LABEL_SE_CANCEL);
					g_select.bMove = true;	// 移動開始
					g_select.bBack = true;	// キャンセル
				}

				if (pCursor->cursor[CURSORTYPE_PLAYER2].bUse == true)
				{
					if (BuDo(KEYINFO::KEYINFO_CANCEL, 1) == true)
					{
						PlaySound(SOUND_LABEL_SE_CANCEL);
						g_select.bMove = true;	// 移動開始
						g_select.bBack = true;	// キャンセル
					}
				}
			}
			
		}

		//// 選択毎の移動条件
		//switch (g_select.nCntPage)
		//{
		//case 0:
		//	if (pCursor->cursor[CURSORTYPE_PLAYER1].nCntBlink >= 60)
		//	{
		//		g_select.bMove = true;	// 移動開始
		//	}
		//	break;

		//case 1:
		//	break;

		//case 2:
		//	g_select.bMove = true;
		//	break;

		//default:
		//	break;
		//}

		//if (g_select.b2PlayerOn == true && g_select.nCntPage == 1)
		//{
		//	if (BuDo(KEYINFO::KEYINFO_OK, 1) == true)
		//	{
		//		g_select.bDecide[1] = true;
		//	}
		//}

		CursorManager();
		
		g_select.SType = (SelectType)g_select.nCntPage;

		SelectCursor(g_select.SType);	// カーソル制御
	}
}

//==============================================================================
// テクスチャの設定
//==============================================================================
void TexSetSelect(void)
{
	// ローカル変数宣言
	//VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 背景テクスチャの読み込み
	for (int nCnt = 0; nCnt < g_select.nNumSelect; nCnt++)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			g_select.selectInfo[nCnt].aFileName,
			&g_pTextureSelect[nCnt]);
	}
}

//==============================================================================
// テキストロード
//==============================================================================
void LoadSelect(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aEqual[2] = { NULL };				// 「=」読み取り用変数
	int nCntFile = 0;						// Xファイルの数
	int nIdx = 0;							// パーツ数
	bool bComment = false;					// コメントアウトするか
	char aText[TEXT_LENGTH] = { NULL };		// テキスト読み取り用変数
	char aSelect[TEXT_LENGTH] = { NULL };	// ポリゴン情報読み込み用変数
	char aState[TEXT_LENGTH] = { NULL };	// 詳細情報読み取り用変数

	// strcmp読み取り用ポインタ
	char *pText = &aText[0];
	char *pSelect = &aSelect[0];
	char *pState = &aState[0];

	// データの読み込み
	pFile = fopen("data/TEXT/select.txt", "r");
	if (pFile != NULL)
	{ //ファイル展開可能
		while (strcmp("END_SCRIPT", pText) != 0)
		{
			aText[0] = { NULL };
			if (bComment == false)
			{// コメントアウトしていない
				fscanf(pFile, "%s", &aText[0]);

				if (aText[0] == '#')
				{// 文字列の先頭が「#」ならばコメントアウトへ
					bComment = true;
				}
				else
				{// 通常時
					if (strcmp("NUM_SELECT", pText) == 0)
					{// モデルの数
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%d", &g_select.nNumSelect);
					}
					else if (strcmp("SELECT_FILENAME", pText) == 0)
					{// Xファイル名の読み込み
						fscanf(pFile, "%s", &aEqual[0]);
						fscanf(pFile, "%s", &g_select.selectInfo[nCntFile].aFileName[0]);
						nCntFile++;
					}
					else if (strcmp("SELECT_SET", pText) == 0)
					{// 配置情報
						aSelect[0] = {};
						while (strcmp("END_SELECT_SET", pSelect) != 0)
						{// 配置情報の読み取り
							fscanf(pFile, "%s", &aSelect[0]);
							if (strcmp("POLYGONSET", pSelect) == 0)
							{
								aState[0] = {};
								while (strcmp("END_POLYGONSET", pState) != 0)
								{
									fscanf(pFile, "%s", &aState[0]);

									if (strcmp("POS", pState) == 0)
									{// 位置情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].pos.x);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].pos.y);
										if (g_select.selectInfo[nIdx].st > 1)
										{
											//g_select.selectInfo[nIdx].pos.x += SCREEN_HEIGHT * g_select.selectInfo[nIdx].st;
											g_select.selectInfo[nIdx].pos.y += SCREEN_HEIGHT * (g_select.selectInfo[nIdx].st - 1);
										}
									}
									else if (strcmp("COL", pState) == 0)
									{// 色情報
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.r);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.g);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.b);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].col.a);
									}
									else if (strcmp("WIDTH", pState) == 0)
									{// 幅
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].fWidth);
									}
									else if (strcmp("HEIGHT", pState) == 0)
									{// 高さ
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%f", &g_select.selectInfo[nIdx].fHeight);
									}
									else if (strcmp("TEXTYPE", pState) == 0)
									{
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_select.selectInfo[nIdx].nType);
									}
									else if (strcmp("SELECTTYPE", pState) == 0)
									{
										fscanf(pFile, "%s", &aEqual[0]);
										fscanf(pFile, "%d", &g_select.selectInfo[nIdx].st);
									}
								}
								g_select.selectInfo[nIdx].bUse = true;
								nIdx++;
							}
						}
					}
				}
			}
			else if (bComment == true)
			{// コメントアウト処理
			 // ローカル変数宣言
				char a = NULL;
				char b = NULL;
				fscanf(pFile, "%c", &a);
				while (a != '\n' && b != '\n')
				{
					fscanf(pFile, "%s", &aText[0]);
					fscanf(pFile, "%c", &b);
				}
				bComment = false;
			}
		}
		g_select.nNumTexSelect = nCntFile;
		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// 頂点情報更新
//==============================================================================
void SetVertexSelect(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * nIdx;

	// テクスチャの頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(g_select.selectInfo[nIdx].pos.x, g_select.selectInfo[nIdx].pos.y + g_select.selectInfo[nIdx].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_select.selectInfo[nIdx].pos.x, g_select.selectInfo[nIdx].pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_select.selectInfo[nIdx].pos.x + g_select.selectInfo[nIdx].fWidth, g_select.selectInfo[nIdx].pos.y + g_select.selectInfo[nIdx].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_select.selectInfo[nIdx].pos.x + g_select.selectInfo[nIdx].fWidth, g_select.selectInfo[nIdx].pos.y, 0.0f);

	pVtx[0].col = g_select.selectInfo[nIdx].col;
	pVtx[1].col = g_select.selectInfo[nIdx].col;
	pVtx[2].col = g_select.selectInfo[nIdx].col;
	pVtx[3].col = g_select.selectInfo[nIdx].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}

//==============================================================================
// 選択モードカーソルの制御
//==============================================================================
void SelectCursor(SelectType ST)
{
	Cursor *pCursor = &GetCursorS();

	if (g_select.bMove == false)
	{
		switch (ST)
		{
		case SELECTTYPE_MODE:	// プレイモード選択
			//if (g_select.bDecide[0] == false)
			//{
			//	if (GetKeyboardTrigger(KEYINFO_LEFT) == true || BuDo(KEYINFO::KEYINFO_LEFT, 0) == true)
			//	{
			//		// 音の再生
			//		PlaySound(SOUND_LABEL_SE_CURSOR);

			//		if (g_select.mode == PLAYMODE_0)
			//		{
			//			g_select.mode = PLAYMODE_1;
			//		}
			//		else
			//		{
			//			g_select.mode = PLAYMODE_0;
			//		}
			//	}
			//	else if (GetKeyboardTrigger(KEYINFO_RIGHT) == true || BuDo(KEYINFO::KEYINFO_RIGHT, 0) == true)
			//	{
			//		// 音の再生
			//		PlaySound(SOUND_LABEL_SE_CURSOR);

			//		if (g_select.mode == PLAYMODE_0)
			//		{
			//			g_select.mode = PLAYMODE_1;
			//		}
			//		else
			//		{
			//			g_select.mode = PLAYMODE_0;
			//		}
			//	}
			//}
			break;

		case SELECTTYPE_CHARA:	// キャラクター選択
			if (g_select.bDecide[0] == false)
			{
				if (/*GetKeyboardTrigger(KEYINFO_LEFT) == true || */BuDo(KEYINFO::KEYINFO_LEFT, 0) == true)
				{
					// 音の再生
					PlaySound(SOUND_LABEL_SE_CURSOR);

					if (g_select.chara[0] == CHARA_0)
					{
						g_select.chara[0] = CHARA_2;
					}
					else if (g_select.chara[0] == CHARA_1)
					{
						g_select.chara[0] = CHARA_0;
					}
					else
					{
						g_select.chara[0] = CHARA_1;
					}
				}
				else if (/*GetKeyboardTrigger(KEYINFO_RIGHT) == true || */BuDo(KEYINFO::KEYINFO_RIGHT, 0) == true)
				{
					// 音の再生
					PlaySound(SOUND_LABEL_SE_CURSOR);

					if (g_select.chara[0] == CHARA_0)
					{
						g_select.chara[0] = CHARA_1;
					}
					else if (g_select.chara[0] == CHARA_1)
					{
						g_select.chara[0] = CHARA_2;
					}
					else
					{
						g_select.chara[0] = CHARA_0;
					}
				}
			}

			if (g_select.bDecide[1] == false)
			{
				if (pCursor->cursor[CURSORTYPE_PLAYER2].bUse == true)
				{
					if (BuDo(KEYINFO::KEYINFO_LEFT, 1) == true)
					{
						// 音の再生
						PlaySound(SOUND_LABEL_SE_CURSOR);

						if (g_select.chara[1] == CHARA_0)
						{
							g_select.chara[1] = CHARA_2;
						}
						else if (g_select.chara[1] == CHARA_1)
						{
							g_select.chara[1] = CHARA_0;
						}
						else
						{
							g_select.chara[1] = CHARA_1;
						}
					}
					else if (BuDo(KEYINFO::KEYINFO_RIGHT, 1) == true)
					{
						// 音の再生
						PlaySound(SOUND_LABEL_SE_CURSOR);

						if (g_select.chara[1] == CHARA_0)
						{
							g_select.chara[1] = CHARA_1;
						}
						else if (g_select.chara[1] == CHARA_1)
						{
							g_select.chara[1] = CHARA_2;
						}
						else
						{
							g_select.chara[1] = CHARA_0;
						}
					}
				}
				else if (pCursor->cursor[CURSORTYPE_CPU].bUse == true)
				{
					if (BuDo(KEYINFO::KEYINFO_LEFT, 0) == true)
					{
						// 音の再生
						PlaySound(SOUND_LABEL_SE_CURSOR);

						if (g_select.chara[1] == CHARA_0)
						{
							g_select.chara[1] = CHARA_2;
						}
						else if (g_select.chara[1] == CHARA_1)
						{
							g_select.chara[1] = CHARA_0;
						}
						else
						{
							g_select.chara[1] = CHARA_1;
						}
					}
					else if (BuDo(KEYINFO::KEYINFO_RIGHT, 0) == true)
					{
						// 音の再生
						PlaySound(SOUND_LABEL_SE_CURSOR);

						if (g_select.chara[1] == CHARA_0)
						{
							g_select.chara[1] = CHARA_1;
						}
						else if (g_select.chara[1] == CHARA_1)
						{
							g_select.chara[1] = CHARA_2;
						}
						else
						{
							g_select.chara[1] = CHARA_0;
						}
					}
				}
			}
			
			break;

		case SELECTTYPE_STAGE:	// ステージ選択
			if (GetKeyboardTrigger(KEYINFO_LEFT) == true || BuDo(KEYINFO::KEYINFO_LEFT, 0) == true)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_CURSOR);

				if (g_select.stage == STAGE_0)
				{
					g_select.stage = STAGE_2;
				}
				else if (g_select.stage == STAGE_1)
				{
					g_select.stage = STAGE_0;
				}
				else
				{
					g_select.stage = STAGE_1;
				}
			}
			else if (GetKeyboardTrigger(KEYINFO_RIGHT) == true || BuDo(KEYINFO::KEYINFO_RIGHT, 0) == true)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_CURSOR);

				if (g_select.stage == STAGE_0)
				{
					g_select.stage = STAGE_1;
				}
				else if (g_select.stage == STAGE_1)
				{
					g_select.stage = STAGE_2;
				}
				else
				{
					g_select.stage = STAGE_0;
				}
			}
			break;

		default:
			break;
		}
	}
}

//==============================================================================
// 選択画面の取得
//==============================================================================
Select *GetSelect(void)
{
	return &g_select;
}