//==============================================================================
//
// ノーダメージボーナスの描画〔nodamage.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "nodamage.h"
#include "player.h"
#include "score.h"	
#include "sound.h"
#include "game.h"
#include "input.h"
#include "gamepad.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureNodamage[TEX_BONUS] = {};	// テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNodamage = NULL;		// バッファへのポインタ
BONUS Bonus;											// ボーナスの構造体

//==============================================================================
// ノーダメージボーナスの初期化処理
//==============================================================================
HRESULT InitNodamage(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	int nStage = GetStage();

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/nodamage_logo.png",
		&g_pTextureNodamage[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureNodamage[1]);

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNodamage,
		NULL)))
	{
		return E_FAIL;
	}

	// 変数の初期化
	Bonus.colNodamage = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	Bonus.nBonusCnt = 0;
	if (nStage == 0)
	{
		Bonus.nBonus = BONUS_STAGE1;
	}
	else if (nStage == 1)
	{
		Bonus.nBonus = BONUS_STAGE2;
	}
	else if (nStage == 2)
	{
		Bonus.nBonus = BONUS_STAGE3;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の中心座標の設定
	Bonus.posNodamage = D3DXVECTOR3(LOGO_POSX, LOGO_POSY, 0.0f);
	

	// 弾の頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(Bonus.posNodamage.x - LOGO_SIZEX, Bonus.posNodamage.y + LOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Bonus.posNodamage.x - LOGO_SIZEX, Bonus.posNodamage.y - LOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Bonus.posNodamage.x + LOGO_SIZEX, Bonus.posNodamage.y + LOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Bonus.posNodamage.x + LOGO_SIZEX, Bonus.posNodamage.y - LOGO_SIZEY, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[0].col = Bonus.colNodamage;
	pVtx[1].col = Bonus.colNodamage;
	pVtx[2].col = Bonus.colNodamage;
	pVtx[3].col = Bonus.colNodamage;

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffNodamage->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BONUS; nCnt++,pVtx += 4)
	{
		// 弾の中心座標の設定
		Bonus.posBonus[nCnt] = D3DXVECTOR3(BONUS_POSX + (nCnt * 30.0f), BONUS_POSY, 0.0f);

		// 弾の頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x - BONUS_SIZEX, Bonus.posBonus[nCnt].y + BONUS_SIZEY, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x - BONUS_SIZEX, Bonus.posBonus[nCnt].y - BONUS_SIZEY, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x + BONUS_SIZEX, Bonus.posBonus[nCnt].y + BONUS_SIZEY, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(Bonus.posBonus[nCnt].x + BONUS_SIZEX, Bonus.posBonus[nCnt].y - BONUS_SIZEY, 0.0f);

		// rhwの設定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[4].col = Bonus.colNodamage;
		pVtx[5].col = Bonus.colNodamage;
		pVtx[6].col = Bonus.colNodamage;
		pVtx[7].col = Bonus.colNodamage;

		// テクスチャ頂点情報の設定
		pVtx[4].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffNodamage->Unlock();

	return S_OK;
}

//==============================================================================
// ノーダメージボーナスの終了処理
//==============================================================================
void UninitNodamage(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffNodamage != NULL)
	{
		g_pVtxBuffNodamage->Release();
		g_pVtxBuffNodamage = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < TEX_BONUS; nCnt++)
	{
		if (g_pTextureNodamage[nCnt] != NULL)
		{
			g_pTextureNodamage[nCnt]->Release();
			g_pTextureNodamage[nCnt] = NULL;
		}
	}
}

//==============================================================================
// ノーダメージボーナスの更新処理
//==============================================================================
void UpdateNodamage(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	int aNumber[MAX_BONUS];
	int nData = (int)pow(10, (MAX_BONUS - 1));

	AddScoreBonus();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	// 各桁のテクスチャの切り替え
	for (int nCnt = 0; nCnt < MAX_BONUS; nCnt++)
	{
		aNumber[nCnt] = (Bonus.nBonus / nData) % 10;
		nData /= 10;

		// テクスチャの頂点座標の更新
		pVtx[4].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[5].tex = D3DXVECTOR2(0.0f + ((float)aNumber[nCnt] / 10), 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f + ((float)aNumber[nCnt] / 10), 0.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffNodamage->Unlock();
}

//==============================================================================
// ノーダメージボーナスの描画処理
//==============================================================================
void DrawNodamage(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffNodamage,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_POLY; nCnt++)
	{
		if (nCnt == 0)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureNodamage[0]);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureNodamage[1]);
		}

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// ボーナス加算の処理
//==============================================================================
void AddScoreBonus(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	PLAYER *Player;
	// プレイヤーの取得
	Player = GetPlayer();

	// ノーダメージクリア成功
	if (Player->state == PLAYERSTATE_CLEAR && Player->nLife == MAX_LIFE)
	{
		Bonus.nBonusCnt++;	// カウントの加算
		if (Bonus.nBonusCnt == 40)
		{
			PlaySound(SOUND_LABEL_SE_NODAMAGE);	// サウンド再生
		}

		if (Bonus.nBonusCnt < 180)
		{// 約3秒間
			if (Bonus.nBonusCnt % 20 == 0 && Bonus.nBonusCnt % 40 != 0)
			{// 3分の1秒ごとに点滅
				Bonus.colNodamage.a = 0.0f;
			}
			else if (Bonus.nBonusCnt % 40 == 0)
			{
				Bonus.colNodamage.a = 1.0f;
			}
		}
		else if (Bonus.nBonusCnt >= 180)
		{// 約3秒経過後、スコアへの換算
			Bonus.colNodamage.a = 1.0f;	// 透明度解消

			if (Bonus.nBonus > 0)
			{// スコア加算処理（ボーナスが0になるまで）
				Bonus.nBonus -= 2000;				// ボーナス差し引く
				AddScore(2000);						// スコアの加算
				PlaySound(SOUND_LABEL_SE_SCORE_UP);	// サウンド再生

				if (GetKeyboardTrigger(KEYINFO_OK) == true || GetKeyboardTrigger(KEYINFO_JUMP) == true || 
					IsButtonDown(KEYINFO::KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
				{// ボタン入力で省略
					AddScore(Bonus.nBonus);
					Bonus.nBonus -= Bonus.nBonus;
					PlaySound(SOUND_LABEL_SE_SCORE_UP);	// サウンド再生
				}

				if (Bonus.nBonus <= 0)
				{
					Bonus.nBonus = 0;
				}
			}
		}
	}
	else if(Player->state == PLAYERSTATE_CLEAR && Player->nLife != MAX_LIFE)
	{// 失敗
		Bonus.nBonus = 0;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNodamage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POLY; nCnt++, pVtx += 4)
	{
		// 各頂点カラーの設定
		pVtx[0].col = Bonus.colNodamage;
		pVtx[1].col = Bonus.colNodamage;
		pVtx[2].col = Bonus.colNodamage;
		pVtx[3].col = Bonus.colNodamage;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffNodamage->Unlock();
}

//==============================================================================
// ボーナスの取得
//==============================================================================
BONUS GetBonus(void)
{
	return Bonus;
}