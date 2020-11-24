//==============================================================================
//
// 弾の描画〔bullet.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "bullet.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "particle.h"
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define ATK_BULLETA		(3)		// 弾Aの攻撃力
#define ATK_BULLETB		(2)		// 弾Bの攻撃力
#define ATK_BULLETC		(4)		// 弾Cの攻撃力
#define ATK_BULLETD		(2)		// 弾Dの攻撃力
#define BULLETA_SIZEX	(5)		// 弾Aの幅
#define BULLETA_SIZEY	(10)	// 弾Aの高さ
#define BULLETB_SIZEX	(25)	// 弾Bの幅
#define BULLETB_SIZEY	(10)	// 弾Bの高さ
#define BULLETC_SIZEX	(5)		// 弾Cの幅
#define BULLETC_SIZEY	(10)	// 弾Cの高さ
#define BULLETD_SIZEX	(25)	// 弾Dの幅
#define BULLETD_SIZEY	(10)	// 弾Dの高さ
#define ENEMY_BULLETX	(10)	// 敵の弾の幅
#define ENEMY_BULLETY	(10)	// 敵の弾の高さ

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBullet[BULLET_TYPE] = {};	// 弾テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// 弾バッファへのポインタ
BULLET g_aBullet[BULLETNUM];							// 弾の情報
SETBULLET State;										// 弾の状態
int g_nCntType;											// 弾の装填状態カウント
D3DXVECTOR3 g_rotMissile;								// ミサイルの回転角
float g_fLengthMissile;									// ミサイルの対角線の長さ
float g_fAngleMissile;									// ミサイルの対角線の角度

//==============================================================================
// 弾の初期化処理
//==============================================================================
HRESULT InitBullet(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBullet[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet001.png",
		&g_pTextureBullet[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet002.png",
		&g_pTextureBullet[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet003.png",
		&g_pTextureBullet[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/bullet_enemy000.png",
		&g_pTextureBullet[4]);

	// 変数の初期化処理
	for (int nCntBullet = 0; nCntBullet < BULLETNUM; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 0;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nType = 0;
	}
	g_nCntType = 0;
	g_rotMissile.z = 0.0f;
	State = SETBULLET_NORMAL;

	// ミサイルの回転角
	g_rotMissile = D3DXVECTOR3(0.0f, 0.0f, g_rotMissile.z);

	// ミサイルの対角線の長さ
	g_fLengthMissile = sqrtf(BULLETC_SIZEX * BULLETC_SIZEX + BULLETC_SIZEY * BULLETC_SIZEY);

	// ミサイルの対角線の角度
	g_fAngleMissile = atan2f(BULLETC_SIZEX, BULLETC_SIZEY);

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * BULLETNUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < BULLETNUM; nCntBullet++)
	{
		// 弾の中心座標の設定
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f ,0.0f);

		// 弾の頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETA_SIZEX, g_aBullet[nCntBullet].pos.y + BULLETA_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLETA_SIZEX, g_aBullet[nCntBullet].pos.y - BULLETA_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETA_SIZEX, g_aBullet[nCntBullet].pos.y + BULLETA_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLETA_SIZEX, g_aBullet[nCntBullet].pos.y - BULLETA_SIZEY, 0.0f);

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

		// テクスチャ頂点情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;	// 頂点ポインタを4ずつ進める
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

	return S_OK;
}

//==============================================================================
// 弾の終了処理
//==============================================================================
void UninitBullet(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < (BULLET_TYPE); nCnt++)
	{
		if (g_pTextureBullet[nCnt] != NULL)
		{
			g_pTextureBullet[nCnt]->Release();
			g_pTextureBullet[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 弾の更新処理
//==============================================================================
void UpdateBullet(void)
{
	// ローカル変数宣言
	PLAYER *Player;

	// プレイヤーの取得
	Player = GetPlayer();

	// 切り替えキーで弾の装填情報の変更
	if (g_nCntType == 0 && Player->aBullet[0] == true)
	{ // 通常弾からウェーブへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 1;
			State = SETBULLET_WAVE;
		}
	}
	else if(g_nCntType == 0 && Player->aBullet[0] == false && Player->aBullet[1] == true)
	{ // 通常弾からミサイルへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 2;
			State = SETBULLET_MISSILE;
		}
	}
	else if (g_nCntType == 0 && Player->aBullet[0] == false && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // 通常弾からリバースウェーブへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 3;
			State = SETBULLET_REVERSEWAVE;
		}
	}
	else if (g_nCntType == 1 && Player->aBullet[1] == true)
	{ // ウェーブからミサイルへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 2;
			State = SETBULLET_MISSILE;
		}
	}
	else if (g_nCntType == 1 && Player->aBullet[1] == false && Player->aBullet[2] == true)
	{ // ウェーブからリバースウェーブへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 3;
			State = SETBULLET_REVERSEWAVE;
		}
	}
	else if (g_nCntType == 1 && Player->aBullet[1] == false && Player->aBullet[2] == false)
	{ // ウェーブから通常弾へ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 0;
			State = SETBULLET_NORMAL;
		}
	}
	else if (g_nCntType == 2 && Player->aBullet[2] == true)
	{ // ミサイルからリバースウェーブへ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			State = SETBULLET_REVERSEWAVE;
			g_nCntType = 3;
		}
	}
	else if (g_nCntType == 2 && Player->aBullet[2] == false)
	{ // ミサイルから通常弾へ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 0;
			State = SETBULLET_NORMAL;
		}
	}
	else if (g_nCntType == 3)
	{ // リバースウェーブから通常弾へ
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true || IsButtonDown(KEYINFO::KEYINFO_CHANGE) == true)
		{
			g_nCntType = 0;
			State = SETBULLET_NORMAL;
		}
	}

	// 弾の命中判定
	HitBullet();
}

//==============================================================================
// 弾の描画処理
//==============================================================================
void DrawBullet(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	BULLET *pBullet;

	// 弾の取得
	pBullet = &g_aBullet[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBullet,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < BULLETNUM; nCntBullet++,pBullet++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet[pBullet->nType]);

		// ポリゴンの生成
		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntBullet * 4,			// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}	
	}					
}

//==============================================================================
// 弾の設定
//==============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	BULLET *pBullet;

	// 弾の取得
	pBullet = &g_aBullet[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 弾の設定
	for (int nCntBullet = 0; nCntBullet < BULLETNUM; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			// 位置を設定
			pBullet->pos = pos;

			// 移動量を設定
			pBullet->move = move;

			// 寿命を設定
			pBullet->nLife = nLife;

			// 弾の種類の設定
			pBullet->type = type;

			// 敵味方の判定
			if (pBullet->type == BULLETTYPE_ENEMY)
			{
				pBullet->nType = 4;
			}
			else
			{
				// タイプを設定
				pBullet->nType = g_nCntType % (BULLET_TYPE - 1);
			}

			// 頂点座標を設定
			SetVertexBullet(nCntBullet);

			// 弾を使用する
			pBullet->bUse = true;
			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==============================================================================
// 弾の命中
//==============================================================================
void HitBullet(void)
{
	// ローカル変数宣言
	BULLET *pBullet;
	ENEMY *pEnemy;
	PLAYER *Player;
	int nScore = 0;

	// 弾の取得
	pBullet = &g_aBullet[0];

	// 敵の取得
	pEnemy = GetEnemy();

	// プレイヤーの取得
	Player = GetPlayer();

	// 弾の当たり判定と敵味方の判定
	for (int nCntBullet = 0; nCntBullet < BULLETNUM; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{ // 弾が使用されていた場合
			if (pBullet->nType == 0)
			{ // 弾A(通常弾)の場合
			  // パーティクルの設定
				SetEffect(pBullet->pos, 0.0f, D3DXCOLOR(0.9f, 0.01f, 0.01f, 1.0f), 7.0f, 0.15f);

				// 位置の更新
				pBullet->pos += pBullet->move;

				// 敵との当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{ // 敵が有効の場合
						if (pBullet->pos.y - BULLETA_SIZEY <= pEnemy->pos.y + ENEMY_SIZEY &&
							pBullet->pos.x + BULLETA_SIZEX >= pEnemy->pos.x - ENEMY_SIZEX &&
							pBullet->pos.x - BULLETA_SIZEX <= pEnemy->pos.x + ENEMY_SIZEX &&
							pBullet->pos.y + BULLETA_SIZEY >= pEnemy->pos.y - ENEMY_SIZEY &&
							pEnemy->nType != 5)
						{ // 通常敵の命中範囲
							if (pBullet->type == BULLETTYPE_PLAYER)
							{
								// 敵の当たり判定とダメージ
								HitEnemy(nCntEnemy, ATK_BULLETA);

								// 弾の破棄
								pBullet->bUse = false;
							}
						}

						if (pBullet->pos.y - BULLETA_SIZEY <= pEnemy->pos.y + BIGENEMY_SIZEY &&
							pBullet->pos.x + BULLETA_SIZEX >= pEnemy->pos.x - BIGENEMY_SIZEX &&
							pBullet->pos.x - BULLETA_SIZEX <= pEnemy->pos.x + BIGENEMY_SIZEX &&
							pBullet->pos.y + BULLETA_SIZEY >= pEnemy->pos.y - BIGENEMY_SIZEY &&
							pEnemy->nType == 5 && pEnemy->pos.y == 120)
						{ // ボス敵の命中範囲
							if (pBullet->type == BULLETTYPE_PLAYER)
							{
								// 敵の当たり判定とダメージ
								HitEnemy(nCntEnemy, ATK_BULLETA);

								// 弾の破棄
								pBullet->bUse = false;
							}
						}
					}
				}
			}
			else if (pBullet->nType == 1)
			{ // 弾B(ウェーブ)の場合
			  // パーティクルの設定
				SetEffect(pBullet->pos, 0.0f, D3DXCOLOR(0.9f, 0.7f, 0.2f, 0.01f), 15.0f, 0.001f);

				// 位置の更新
				pBullet->pos += pBullet->move;

				// 敵との当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{ // 敵が有効の場合
						if (pBullet->pos.y - BULLETB_SIZEY <= pEnemy->pos.y + ENEMY_SIZEY &&
							pBullet->pos.x + 10 >= pEnemy->pos.x - ENEMY_SIZEX &&
							pBullet->pos.x - 10 <= pEnemy->pos.x + ENEMY_SIZEX &&
							pBullet->pos.y + BULLETB_SIZEY >= pEnemy->pos.y - ENEMY_SIZEY &&
							pEnemy->nType != 5)
						{ // 通常敵の命中範囲
							// 敵の当たり判定とダメージ
							HitEnemy(nCntEnemy, ATK_BULLETB);

							// 弾の破棄
							pBullet->bUse = false;
						}
					}

					if (pBullet->pos.y - BULLETB_SIZEY <= pEnemy->pos.y + BIGENEMY_SIZEY &&
						pBullet->pos.x + 10 >= pEnemy->pos.x - BIGENEMY_SIZEX &&
						pBullet->pos.x - 10 <= pEnemy->pos.x + BIGENEMY_SIZEX &&
						pBullet->pos.y + BULLETB_SIZEY >= pEnemy->pos.y - BIGENEMY_SIZEY &&
						pEnemy->nType == 5 && pEnemy->pos.y == 120)
					{ // ボス敵の命中範囲
						if (pBullet->type == BULLETTYPE_PLAYER)
						{
							// 敵の当たり判定とダメージ
							HitEnemy(nCntEnemy, ATK_BULLETA);

							// 弾の破棄
							pBullet->bUse = false;
						}
					}
				}
			}
			else if (pBullet->nType == 2)
			{ // 弾C(ミサイル)の場合
			  // パーティクルの設定
				SetEffect(pBullet->pos, 0.009f, D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f), 2.0f, 0.1f);

				// 位置の更新
				pBullet->pos += pBullet->move;

				// 敵との当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{ // 敵が有効の場合
						if (pBullet->pos.y - BULLETC_SIZEY <= pEnemy->pos.y + ENEMY_SIZEY &&
							pBullet->pos.x + BULLETC_SIZEX >= pEnemy->pos.x - ENEMY_SIZEX &&
							pBullet->pos.x - BULLETC_SIZEX <= pEnemy->pos.x + ENEMY_SIZEX &&
							pBullet->pos.y + BULLETC_SIZEY >= pEnemy->pos.y - ENEMY_SIZEY &&
							pEnemy->nType != 5)
						{ // 通常敵の命中範囲
							// 敵の当たり判定とダメージ
							HitEnemy(nCntEnemy, ATK_BULLETC);

							// 弾の破棄
							pBullet->bUse = false;
						}
					}

					if (pBullet->pos.y - BULLETC_SIZEY <= pEnemy->pos.y + BIGENEMY_SIZEY &&
						pBullet->pos.x + BULLETC_SIZEX >= pEnemy->pos.x - BIGENEMY_SIZEX &&
						pBullet->pos.x - BULLETC_SIZEX <= pEnemy->pos.x + BIGENEMY_SIZEX &&
						pBullet->pos.y + BULLETC_SIZEY >= pEnemy->pos.y - BIGENEMY_SIZEY &&
						pEnemy->nType == 5 && pEnemy->pos.y == 120)
					{ // ボス敵の命中範囲
						if (pBullet->type == BULLETTYPE_PLAYER)
						{
							// 敵の当たり判定とダメージ
							HitEnemy(nCntEnemy, ATK_BULLETA);

							// 弾の破棄
							pBullet->bUse = false;
						}
					}
				}
			}
			else if (pBullet->nType == 3)
			{ // 弾D(逆ウェーブ)の場合
			  // パーティクルの設定
				SetEffect(pBullet->pos, 0.0f, D3DXCOLOR(0.01f, 0.01f, 0.9f, 1.0f), 15.0f, 0.09f);

				// 位置の更新
				pBullet->pos += pBullet->move;

				// 敵との当たり判定
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{ // 敵が有効の場合
						if (pBullet->pos.y - BULLETD_SIZEY <= pEnemy->pos.y + ENEMY_SIZEY &&
							pBullet->pos.x + BULLETD_SIZEX >= pEnemy->pos.x - ENEMY_SIZEX &&
							pBullet->pos.x - BULLETD_SIZEX <= pEnemy->pos.x + ENEMY_SIZEX &&
							pBullet->pos.y + BULLETD_SIZEY >= pEnemy->pos.y - ENEMY_SIZEY &&
							pEnemy->nType != 5)
						{ // 通常敵の命中範囲
							// 敵の当たり判定とダメージ
							HitEnemy(nCntEnemy, ATK_BULLETD);

							// 弾の破棄
							pBullet->bUse = false;
						}

						if (pBullet->pos.y - BULLETD_SIZEY <= pEnemy->pos.y + BIGENEMY_SIZEY &&
							pBullet->pos.x + BULLETD_SIZEX >= pEnemy->pos.x - BIGENEMY_SIZEX &&
							pBullet->pos.x - BULLETD_SIZEX <= pEnemy->pos.x + BIGENEMY_SIZEX &&
							pBullet->pos.y + BULLETD_SIZEY >= pEnemy->pos.y - BIGENEMY_SIZEY &&
							pEnemy->nType == 5 && pEnemy->pos.y == 120)
						{ // ボス敵の命中範囲
							if (pBullet->type == BULLETTYPE_PLAYER)
							{
								// 敵の当たり判定とダメージ
								HitEnemy(nCntEnemy, ATK_BULLETA);

								// 弾の破棄
								pBullet->bUse = false;
							}
						}
					}
				}
			}
			else if (pBullet->nType == 4)
			{ // 敵の弾の場合
			  // 位置の更新
				pBullet->pos += pBullet->move;

				// 自機との当たり判定
				if (Player->state == PLAYERSTATE_NORMAL)
				{ // 敵が有効の場合
					if (pBullet->pos.y - 10 <= Player->pos.y + 20 &&
						pBullet->pos.x + 10 >= Player->pos.x - 20 &&
						pBullet->pos.x - 10 <= Player->pos.x + 20 &&
						pBullet->pos.y + 10 >= Player->pos.y - 20)
					{
						// 敵の当たり判定とダメージ
						HitPlayer(1);

						// 弾の破棄
						pBullet->bUse = false;
					}
				}
			}

			// 敵情報の更新
			pEnemy = GetEnemy();

			// 頂点座標の更新
			SetVertexBullet(nCntBullet);

			// 寿命チェック
			pBullet->nLife -= 1;
			if (pBullet->nLife <= 0)
			{
				SetExplosion(pBullet->pos);
				pBullet->bUse = false;
			}

			// 画面外チェック
			if (pBullet->pos.y <= 0.0f || pBullet->pos.y >= SCREEN_HEIGHT || pBullet->pos.x <= 0.0f || pBullet->pos.x >= SCREEN_WIDTH)
			{
				pBullet->bUse = false;
			}
		}
	}
}

//==============================================================================
// 弾の頂点座標の設定
//==============================================================================
void SetVertexBullet(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	BULLET *pBullet;

	// 弾の取得
	pBullet = &g_aBullet[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0,0,(void**)&pVtx,0);

	// 頂点ポインタを4ずつ進める
	pVtx += 4 * nIdx;

	// バレットポインタを進める
	pBullet += nIdx;

	// 弾の種類に応じて大きさを変化
	if (pBullet->nType == 0)
	{ // 弾A(通常弾)の場合
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETA_SIZEX, g_aBullet[nIdx].pos.y + BULLETA_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETA_SIZEX, g_aBullet[nIdx].pos.y - BULLETA_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETA_SIZEX, g_aBullet[nIdx].pos.y + BULLETA_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETA_SIZEX, g_aBullet[nIdx].pos.y - BULLETA_SIZEY, 0.0f);
	}
	else if (pBullet->nType == 1)
	{ // 弾B(ウェーブ)の場合
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETB_SIZEX, g_aBullet[nIdx].pos.y + BULLETB_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETB_SIZEX, g_aBullet[nIdx].pos.y - BULLETB_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETB_SIZEX, g_aBullet[nIdx].pos.y + BULLETB_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETB_SIZEX, g_aBullet[nIdx].pos.y - BULLETB_SIZEY, 0.0f);
	}
	else if (pBullet->nType == 2)
	{ // 弾C(ミサイル)の場合
		// ミサイルの各頂点座標
		if (g_aBullet[nIdx].move.x > 0)
		{
			g_rotMissile.z = -D3DX_PI / 6;
		}
		else if (g_aBullet[nIdx].move.x < 0)
		{
			g_rotMissile.z = D3DX_PI / 6;
		}

		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + sinf(-g_fAngleMissile + g_rotMissile.z) * g_fLengthMissile, g_aBullet[nIdx].pos.y + cosf(-g_fAngleMissile + g_rotMissile.z) * g_fLengthMissile, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + sinf(g_fAngleMissile - D3DX_PI + g_rotMissile.z) * g_fLengthMissile, g_aBullet[nIdx].pos.y + cosf(g_fAngleMissile - D3DX_PI + g_rotMissile.z) * g_fLengthMissile, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + sinf(g_fAngleMissile + g_rotMissile.z) * g_fLengthMissile, g_aBullet[nIdx].pos.y + cosf(g_fAngleMissile + g_rotMissile.z) * g_fLengthMissile, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + sinf(-g_fAngleMissile + D3DX_PI + g_rotMissile.z) * g_fLengthMissile, g_aBullet[nIdx].pos.y + cosf(-g_fAngleMissile + D3DX_PI + g_rotMissile.z) * g_fLengthMissile, 0.0f);		
	}
	else if (pBullet->nType == 3)
	{ // 弾D(リバースウェーブ)の場合
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETD_SIZEX, g_aBullet[nIdx].pos.y + BULLETD_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLETD_SIZEX, g_aBullet[nIdx].pos.y - BULLETD_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETD_SIZEX, g_aBullet[nIdx].pos.y + BULLETD_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLETD_SIZEX, g_aBullet[nIdx].pos.y - BULLETD_SIZEY, 0.0f);
	}
	else if (pBullet->nType == 4)
	{ // 敵の弾の場合
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - ENEMY_BULLETX, g_aBullet[nIdx].pos.y + ENEMY_BULLETY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - ENEMY_BULLETX, g_aBullet[nIdx].pos.y - ENEMY_BULLETY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + ENEMY_BULLETX, g_aBullet[nIdx].pos.y + ENEMY_BULLETY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + ENEMY_BULLETX, g_aBullet[nIdx].pos.y - ENEMY_BULLETY, 0.0f);
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//==============================================================================
// 弾の状態
//==============================================================================
SETBULLET GetState(void)
{
	return State;
}