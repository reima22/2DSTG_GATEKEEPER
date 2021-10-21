//==============================================================================
//
// スコアの描画〔score.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bullet_ui.h"
#include "player.h"
#include "game.h"
#include "math.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define UI_SIZE		(D3DXVECTOR3(100.0f,60.0f,0.0f))
#define NUM_SIZE	(D3DXVECTOR3(20.0f,60.0f,0.0f))

#define POS_1P		(D3DXVECTOR3(0.0f,660.0f,0.0f))
#define POS_2P		(D3DXVECTOR3(640.0f,660.0f,0.0f))

#define POS_1P_NUM	(D3DXVECTOR3(110.0f,660.0f,0.0f))
#define POS_2P_NUM	(D3DXVECTOR3(750.0f,660.0f,0.0f)) 

#define POS_SCOREX	(1100.0f)	// スコアの上一桁の中心X座標
#define POS_SCOREY	(40.0f)	// スコアの上一桁の中心Y座標

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBulletUI[TEX_BULLET_UI] = {};	// テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletUI = NULL;			// バッファへのポインタ
BulletUI bu[PLAYERNUM_MAX];
										
//==============================================================================
// スコアの初期化処理
//==============================================================================
HRESULT InitBulletUI(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	Player *pPlayer = GetPlayer();

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_ui.png",
		&g_pTextureBulletUI[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/number000.png",
		&g_pTextureBulletUI[1]);

	// スコアの位置座標の初期化
	for (int nCnt = 0; nCnt < PLAYERNUM_MAX; nCnt++)
	{
		for (int nCntUI = 0; nCntUI < MAX_UI_NUM; nCntUI++)
		{
			bu[nCnt].posNum[nCntUI] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}



	bu[0].posLogo = POS_1P;
	bu[1].posLogo = POS_2P;

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		bu[0].posNum[nCnt] = D3DXVECTOR3(POS_1P_NUM.x + nCnt * 30.0f, POS_1P_NUM.y, 0.0f);
		bu[1].posNum[nCnt] = D3DXVECTOR3(POS_2P_NUM.x + nCnt * 30.0f, POS_2P_NUM.y, 0.0f);
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletUI,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET_UI; nCnt++,pVtx += 4)
	{
		if (nCnt <= 1)
		{
			//スコアの頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(bu[nCnt].posLogo.x, bu[nCnt].posLogo.y + UI_SIZE.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(bu[nCnt].posLogo.x, bu[nCnt].posLogo.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(bu[nCnt].posLogo.x + UI_SIZE.x, bu[nCnt].posLogo.y + UI_SIZE.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(bu[nCnt].posLogo.x + UI_SIZE.x, bu[nCnt].posLogo.y, 0.0f);
		}
		else
		{
			if (nCnt == 2)
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[0].posNum[0].x, bu[0].posNum[0].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[0].posNum[0].x, bu[0].posNum[0].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[0].posNum[0].x + NUM_SIZE.x, bu[0].posNum[0].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[0].posNum[0].x + NUM_SIZE.x, bu[0].posNum[0].y, 0.0f);
			}																 
			else if (nCnt == 3)
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[0].posNum[1].x, bu[0].posNum[1].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[0].posNum[1].x, bu[0].posNum[1].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[0].posNum[1].x + NUM_SIZE.x, bu[0].posNum[1].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[0].posNum[1].x + NUM_SIZE.x, bu[0].posNum[1].y, 0.0f);
			}
			else if (nCnt == 4)
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[0].posNum[2].x, bu[0].posNum[2].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[0].posNum[2].x, bu[0].posNum[2].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[0].posNum[2].x + NUM_SIZE.x, bu[0].posNum[2].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[0].posNum[2].x + NUM_SIZE.x, bu[0].posNum[2].y, 0.0f);
			}
			else if (nCnt == 5)
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[1].posNum[0].x, bu[1].posNum[0].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[1].posNum[0].x, bu[1].posNum[0].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[1].posNum[0].x + NUM_SIZE.x, bu[1].posNum[0].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[1].posNum[0].x + NUM_SIZE.x, bu[1].posNum[0].y, 0.0f);
			}
			else if (nCnt == 6)
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[1].posNum[1].x, bu[1].posNum[1].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[1].posNum[1].x, bu[1].posNum[1].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[1].posNum[1].x + NUM_SIZE.x, bu[1].posNum[1].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[1].posNum[1].x + NUM_SIZE.x, bu[1].posNum[1].y, 0.0f);
			}
			else
			{
				//スコアの頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(bu[1].posNum[2].x, bu[1].posNum[2].y + NUM_SIZE.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(bu[1].posNum[2].x, bu[1].posNum[2].y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(bu[1].posNum[2].x + NUM_SIZE.x, bu[1].posNum[2].y + NUM_SIZE.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(bu[1].posNum[2].x + NUM_SIZE.x, bu[1].posNum[2].y, 0.0f);
			}

		}


		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletUI->Unlock();

	return S_OK;
}

//==============================================================================
// スコアの終了処理
//==============================================================================
void UninitBulletUI(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBulletUI != NULL)
	{
		g_pVtxBuffBulletUI->Release();
		g_pVtxBuffBulletUI = NULL;
	}

	for (int nCnt = 0; nCnt < TEX_BULLET_UI; nCnt++)
	{
		// テクスチャの開放
		if (g_pTextureBulletUI[nCnt] != NULL)
		{
			g_pTextureBulletUI[nCnt]->Release();
			g_pTextureBulletUI[nCnt] = NULL;
		}
	}
}

//==============================================================================
// スコアの更新処理
//==============================================================================
void UpdateBulletUI(void)
{
	// ローカル変数宣言
	int aNumber[MAX_UI_NUM];
	int nData = (int)pow(10, (MAX_UI_NUM - 1));
	Player *pPlayer = GetPlayer();

	bu[0].nBullet = pPlayer->nBullet;
	pPlayer++;
	bu[1].nBullet = pPlayer->nBullet;

	// 各桁の計算とテクスチャの切り替え
	for (int nCntUI = 0; nCntUI < PLAYERNUM_MAX; nCntUI++)
	{
		nData = (int)pow(10, (MAX_UI_NUM - 1));

		for (int nCnt = 0; nCnt < MAX_UI_NUM; nCnt++)
		{
			aNumber[nCnt] = (bu[nCntUI].nBullet / nData) % 10;
			nData /= 10;
			SetVertexBulletUI(nCnt, aNumber[nCnt], nCntUI);
		}
	}
}

//==============================================================================
// スコアの描画処理
//==============================================================================
void DrawBulletUI(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBulletUI,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_BULLET_UI; nCnt++)
	{
		if (nCnt <= 1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBulletUI[0]);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBulletUI[1]);
		}


		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	// プリミティブの種類
			nCnt * 4,				// 描画を開始する頂点インデックス
			2);						// 描画するプリミティブ数
	}
}

//==============================================================================
// スコアの頂点座標取得処理
//==============================================================================
void SetVertexBulletUI(int nIdx, int nNumber, int nCnt)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBulletUI->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * (nIdx + 2);

	if (nCnt == 1)
	{
		pVtx += 4 * 3;
	}

	// テクスチャの頂点座標の更新
	pVtx[0].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + ((float)nNumber / 10), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + ((float)nNumber / 10), 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBulletUI->Unlock();
}