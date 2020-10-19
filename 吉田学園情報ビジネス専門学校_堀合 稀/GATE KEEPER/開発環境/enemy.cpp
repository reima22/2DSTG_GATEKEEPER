//==============================================================================
//
// 敵の描画〔enemy.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "bullet.h"
#include "item.h"
#include "sound.h"
#include "particle.h"
#include "warning.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ENEMY_COUNT			(15000)		// 敵配置のカウント基準
#define LOW_SCORE				(150)		// スコア倍率低の高さ
#define MIDDLE_SCORE			(300)		// スコア倍率中の高さ
#define HIGH_SCORE				(450)		// スコア倍率高の高さ
#define FIRSTWAVE_SHOT_ONE		(20)		// 第1ウェーブの射撃パターン1
#define FIRSTWAVE_SHOT_TWO		(40)		// 第1ウェーブの射撃パターン2
#define FIRSTWAVE_SHOT_THREE	(60)		// 第1ウェーブの射撃パターン3
#define THIRDWAVE_SHOT			(120)		// 第3ウェーブの射撃パターン
#define FIRSTWAVE_END			(40)		// ゲーム開始時の敵の配置数・第1ウェーブ終了の撃破数
#define FIRSTWAVE_ONE			(20)		// 第1ウェーブの移動パターン変化1
#define FIRSTWAVE_TWO			(30)		// 第1ウェーブの移動パターン変化2
#define SECONDWAVE_END			(80)		// 第2ウェーブ終了時の撃破数
#define THIRDWAVE_END			(120)		// 第3ウェーブ終了時の撃破数
#define BIGENEMY_POSY			(120)		// ボスの配置する高さ
#define BIGENEMY_LIFE			(600)		// ボスの体力
#define BIGENEMY_LOSLIFE		(250)		// ボスの状態変化
#define BIGENEMY_SHOT			(60)		// ボスの射撃パターン
#define ITEM_PHASE				(2)			// アイテム配置の段階

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// 敵テクスチャーへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;				// 敵バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];									// 敵の情報
ENEMYINFO info;												// 敵全体の情報
int g_aItemRand[ITEM_PHASE][MAX_TYPE_ITEM];					// アイテムの配置場所

//==============================================================================
// 敵の初期化処理
//==============================================================================
HRESULT InitEnemy(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	// 乱数算出から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/enemy005.png",
		&g_apTextureEnemy[5]);

	// 変数の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++,pEnemy++)
	{
		pEnemy->pos = D3DXVECTOR3(0.0f, -30.0f, 0.0f);
		pEnemy->nType = NULL;
		pEnemy->bUse = false;
		pEnemy->state = ENEMYSTATE_NORMAL;
		pEnemy->nCounterState = 0;
		pEnemy->nLife = 0;
		pEnemy->move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
		pEnemy->nRandAngle = (rand() % 315);
		pEnemy->fAngle = ((float)pEnemy->nRandAngle / 100.0f);
		pEnemy->nShotCnt = rand() % 100;
	}
	info.nDestroy = 0;
	info.nVanish = 0;
	info.nShotCnt = 1;
	info.nSetCounter = 0;
	info.nUfoCnt = 0;
	info.nUfoSet = rand() % FIRSTWAVE_END + (FIRSTWAVE_END + 1);
	info.nBigEnemyCnt = 0;
	info.nStartCnt = 0;
	info.boss = BIGENEMY_NORMAL;

	// アイテムのランダム配置
	for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
	{
		while (1)
		{ // 数値の重複を除外
			g_aItemRand[0][nCnt] = rand() % FIRSTWAVE_END;
			if (g_aItemRand[0][nCnt] != g_aItemRand[0][nCnt - 1] && 
				g_aItemRand[0][nCnt] != g_aItemRand[0][nCnt - 2])
			{
				break;
			}
		}

		while (1)
		{ // 数値の重複を除外
			g_aItemRand[1][nCnt] = rand() % FIRSTWAVE_END + (FIRSTWAVE_END + 1);
			if (g_aItemRand[1][nCnt] != info.nUfoSet && 
				g_aItemRand[1][nCnt] != g_aItemRand[1][nCnt - 1] &&
				g_aItemRand[1][nCnt] != g_aItemRand[1][nCnt - 2])
			{
				break;
			}
		}
	}

	// 頂点情報の設定
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// 敵の中心座標の設定
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 敵の頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//==============================================================================
// 敵の終了処理
//==============================================================================
void UninitEnemy(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
	{
		if (g_apTextureEnemy[nCnt] != NULL)
		{
			g_apTextureEnemy[nCnt]->Release();
			g_apTextureEnemy[nCnt] = NULL;
		}
	}
}

//==============================================================================
// 敵の更新処理
//==============================================================================
void UpdateEnemy(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	PLAYER Player;
	PHASESTATE phase = GetPhase();	// 警告状態の取得
	int nShot = 0;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// プレイヤーの取得
	Player = *GetPlayer();
	
	// オープニングカウント
	if (info.nStartCnt <= OPEN_CNT)
	{
		info.nStartCnt++;
	}

	// オープニングカウント終了後
	if (info.nStartCnt > OPEN_CNT && info.nVanish < FIRSTWAVE_END)
	{
		info.nShotCnt++;	// ショットカウント開始
	}

	// 第3ウェーブ以降の弾発射カウント
	if (info.nDestroy >= SECONDWAVE_END)
	{
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].nShotCnt++;		// 弾発射のカウント
		}
	}
	
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
		{ // 敵機のX軸の波型移動
			g_aEnemy[nCnt].fAngle += 0.06f;

			if (g_aEnemy[nCnt].fAngle >= D3DX_PI)
			{
				g_aEnemy[nCnt].fAngle = -D3DX_PI;
			}
		}

		if (g_aEnemy[nCnt].bUse == true)
		{ // ライン越えの処理
			if (g_aEnemy[nCnt].pos.y >= WINDOW_LOWEST)
			{ // ライン越え
				if (info.nVanish < FIRSTWAVE_END)
				{
					info.nVanish++;	// 消滅カウント
				}
			
				g_aEnemy[nCnt].bUse = false;
			}
		}
	}

	// 第1ウェーブの弾の発射
	if (info.nVanish < FIRSTWAVE_END)
	{
		while (1)
		{
			// 初期の40機のみランダムに1機から発射
			nShot = rand() % FIRSTWAVE_END;

			if (g_aEnemy[nShot].bUse == true)
			{
				break;
			}
		}

		if (info.nShotCnt % FIRSTWAVE_SHOT_ONE == 0 && info.nVanish < FIRSTWAVE_ONE)
		{
			// 弾の発射
			if (g_aEnemy[nShot].bUse == true)
			{
				// 弾の設定
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 4.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
		else if (info.nShotCnt % FIRSTWAVE_SHOT_TWO == 0 && info.nVanish < FIRSTWAVE_TWO && info.nVanish >= FIRSTWAVE_ONE)
		{ // 第一ウェーブ間
			// 弾の発射
			if (g_aEnemy[nShot].bUse == true)
			{
				// 弾の設定
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
		else if (info.nShotCnt % FIRSTWAVE_SHOT_THREE == 0 && info.nVanish < FIRSTWAVE_END && info.nVanish >= FIRSTWAVE_TWO)
		{ // 第1ウェーブ間
			// 弾の発射
			if (g_aEnemy[nShot].bUse == true)
			{
				// 弾の設定
				SetBullet(g_aEnemy[nShot].pos, D3DXVECTOR3(0.0f, 6.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
	}

	for (int nCntShot = 0; nCntShot < MAX_ENEMY; nCntShot++)
	{
		if (g_aEnemy[nCntShot].nShotCnt % THIRDWAVE_SHOT == 0 && info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END && g_aEnemy[nCntShot].nType != 5)
		{ // 第3ウェーブ間
			if (g_aEnemy[nCntShot].bUse == true)
			{ // 出現する敵すべてから発射
				// 弾の設定
				SetBullet(g_aEnemy[nCntShot].pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}

		// 弾の設定
		if (g_aEnemy[nCntShot].nShotCnt % BIGENEMY_SHOT == 0 && g_aEnemy[nCntShot].pos.y == BIGENEMY_POSY && g_aEnemy[nCntShot].nType == 5)
		{ // ボスの射撃
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 80, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 60, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 60, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 90, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 90, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 120, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(5.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
			SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 120, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(-5.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);

			if (g_aEnemy[nCntShot].nLife <= BIGENEMY_LOSLIFE)
			{ // 体力の減少で弾増加
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 30, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 30, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 7.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 180, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 180, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3(0.0f, 5.0f, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x + 150, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 60, 0.0f), 200, BULLETTYPE_ENEMY);
				SetBullet(D3DXVECTOR3(g_aEnemy[nCntShot].pos.x - 150, g_aEnemy[nCntShot].pos.y, 0.0f),D3DXVECTOR3((Player.pos.x - g_aEnemy[nShot].pos.x) / 80, (Player.pos.y - g_aEnemy[nShot].pos.y) / 60, 0.0f), 200, BULLETTYPE_ENEMY);
			}
		}
	}
	
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++,pVtx = pVtx + 4)
	{
		if (pEnemy->bUse == true)
		{ // 敵が使用されていた場合
			// オープニングの敵の移動
			if (g_aEnemy[nCntEnemy].nType >= 0 && g_aEnemy[nCntEnemy].nType <= 3)
			{ // 通常の敵の撃破数ごとの移動
				SetEnemyMoveDes(nCntEnemy);
			}
			else if (g_aEnemy[nCntEnemy].nType == 4)
			{ // UFOの移動
  				SetEnemyMove(g_aEnemy[nCntEnemy].nType, nCntEnemy);
			}
			else if (g_aEnemy[nCntEnemy].nType == 5)
			{ // 大型敵の移動
				SetEnemyMove(g_aEnemy[nCntEnemy].nType, nCntEnemy);
			}

			// 位置の更新
			pEnemy->pos += pEnemy->move;

			// 頂点座標の更新
			SetVertexEnemy(nCntEnemy);

			// 敵の状態
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_DAMAGE:
				// カウント実行
				g_aEnemy[nCntEnemy].nCounterState--;

				// カウント終了
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					// 通常状態へ移行
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

					// 各頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;

			default:
				break;
			}
		}

		// 敵の撃破数ごとの配置
		SetEnemyDes();
	}

	if (info.nDestroy >= BIGENEMY_APPEAR && info.nBigEnemyCnt == 0)
	{ // 大型敵の配置
		SetBigEnemy();
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// 敵の描画処理
//==============================================================================
void DrawEnemy(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	ENEMY *pEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	// プリミティブの種類
				nCntEnemy * 4,			// 描画を開始する頂点インデックス
				2);						// 描画するプリミティブ数
		}
	}
}

//==============================================================================
// 敵の設定
//==============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife)
{
	// ローカル変数宣言
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			// 位置を設定
			pEnemy->pos = pos;

			// 頂点座標を設定
			SetVertexEnemy(nCntEnemy);

			// 種類の設定
			pEnemy->nType = nType;

			// 敵の体力
			pEnemy->nLife = nLife;

			pEnemy->bUse = true;
			break;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// 敵の撃破数ごとの配置設定
//==============================================================================
void SetEnemyDes(void)
{
	// ローカル変数宣言
	ENEMY *pEnemy;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 第2ウェーブの敵の配置
	if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
	{
		// 初期配置アイテムの無効化
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			g_aItemRand[0][nCnt] = -1;
		}

		// 敵配置カウント
		info.nSetCounter++;

		// 敵の配置
		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			int nPosx = rand() % 1080 + 100;
			int nType = rand() % 4;
			int nLife = 0;

			// タイプごとに体力の指定
			if (nType == 0)
			{
				nLife = 4;
			}
			else if (nType == 1)
			{
				nLife = 7;
			}
			else if (nType == 2)
			{
				nLife = 9;
			}
			else if (nType == 3)
			{
				nLife = 12;
			}

			// 敵の設定
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), nType, nLife);
		}
	}
	else if (info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END)
	{ // 第3ウェーブの敵の配置
		// 敵の設定カウント
		info.nSetCounter++;

		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			// 敵の配置
			int nPosx = rand() % 1240 + 20;
			int nType = rand() % 4;
			int nLife = 0;

			// タイプごとに体力の指定
			if (nType == 0)
			{
				nLife = 4;
			}
			else if (nType == 1)
			{
				nLife = 6;
			}
			else if (nType == 2)
			{
				nLife = 8;
			}
			else if (nType == 3)
			{
				nLife = 10;
			}

			// 敵の設定
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), nType, nLife);
		}
	}
	else if (info.nDestroy >= THIRDWAVE_END && info.nDestroy < BIGENEMY_APPEAR)
	{ // 第4ウェーブの敵の配置
	  // 敵の設定カウント
		info.nSetCounter++;

		if (info.nSetCounter % MAX_ENEMY_COUNT == 0)
		{
			// 敵の配置
			int nPosx = (rand() % 660 + 300);

			// 敵の設定
			SetEnemy(D3DXVECTOR3((float)nPosx, -20.0f, 0.0f), 3, 9);
		}
	}

	// UFOの配置
	if (info.nDestroy == info.nUfoSet && info.nUfoCnt == 0)
	{
		// UFOの設定
		SetEnemy(D3DXVECTOR3(-20.0f, 567.0f, 0.0f), 4, 5);
		info.nUfoCnt++;
	}
}

//==============================================================================
// 敵の頂点座標取得処理
//==============================================================================
void SetVertexEnemy(int nIdx)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点ポインタを4ずつ進める
	pVtx += nIdx * 4;

	pEnemy += nIdx;

	if (pEnemy->nType != 5)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZEX, g_aEnemy[nIdx].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZEX, g_aEnemy[nIdx].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZEX, g_aEnemy[nIdx].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZEX, g_aEnemy[nIdx].pos.y - ENEMY_SIZEY, 0.0f);
	}
	else
	{ // 大型の敵の頂点座標設定
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y + BIGENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y - BIGENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y + BIGENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + BIGENEMY_SIZEX, g_aEnemy[nIdx].pos.y - BIGENEMY_SIZEY, 0.0f);
	}
	
	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//==============================================================================
// 敵の移動力の変化
//==============================================================================
void SetEnemyMove(int nIdx,int nCnt)
{
	// ローカル変数宣言
	PHASESTATE phase = GetPhase();	// 警戒状態の取得

	// タイプごとに移動力の指定(第1ウェーブ以外)
	if (nIdx == 0)
	{
		g_aEnemy[nCnt].move.y = 3.0f;
	}
	else if (nIdx == 1)
	{
		g_aEnemy[nCnt].move.y = 2.5f;
	}
	else if (nIdx == 2)
	{
		g_aEnemy[nCnt].move.y = 2.0f;
	}
	else if (nIdx == 3)
	{
		g_aEnemy[nCnt].move.y = 1.5f;
	}
	else if (nIdx == 4)
	{ // UFOの移動量
		g_aEnemy[nCnt].move.x = 3.0f;
		g_aEnemy[nCnt].move.y = 0.0f;

		if (g_aEnemy[nCnt].pos.x >= 1300.0f)
		{
			g_aEnemy[nCnt].bUse = false;
		}
	}
	else if (nIdx == 5)
	{ // ボス敵の移動量
		if (g_aEnemy[nCnt].pos.y < BIGENEMY_POSY && phase != PHASESTATE_WARNING)
		{ // 登場演出移動
			g_aEnemy[nCnt].move.x = 0.0f;
			g_aEnemy[nCnt].move.y = 1.0f;
		}
		else if (phase == PHASESTATE_WARNING)
		{ // 警告中の停止
			g_aEnemy[nCnt].move.y = 0.0f;
		}
		else if (g_aEnemy[nCnt].pos.y > BIGENEMY_POSY)
		{ // 所定の位置で停止
			g_aEnemy[nCnt].pos.y = BIGENEMY_POSY;
			g_aEnemy[nCnt].move.x = 3.0f;
			g_aEnemy[nCnt].move.y = 0.0f;
		}

		if (g_aEnemy[nCnt].nLife <= BIGENEMY_LOSLIFE)
		{ // 体力低下による加速
			if (g_aEnemy[nCnt].move.x == -3.0f)
			{
				g_aEnemy[nCnt].move.x = -5.0f;
			}
			else if (g_aEnemy[nCnt].move.x == 3.0f)
			{
				g_aEnemy[nCnt].move.x = 5.0f;
			}
		}

		if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - BIGENEMY_SIZEX) || g_aEnemy[nCnt].pos.x <= BIGENEMY_SIZEX)
		{ // 画面端で折り返し
			g_aEnemy[nCnt].pos.x -= g_aEnemy[nCnt].move.x;
			g_aEnemy[nCnt].move.x *= -1;
		}
	}
}

//==============================================================================
// 撃破数ごとの敵の移動力の変化
//==============================================================================
void SetEnemyMoveDes(int nIdx)
{
	if (info.nVanish < 20 && info.nStartCnt == OPEN_CNT)
	{
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].move.x = 2.0f;
			g_aEnemy[nCnt].move.y = 0.0f;
		}
	}

	// 第1ウェーブの敵の移動
	if (info.nVanish < FIRSTWAVE_END)
	{
		if (g_aEnemy[nIdx].pos.x >= (SCREEN_WIDTH - ENEMY_SIZEX) || g_aEnemy[nIdx].pos.x <= ENEMY_SIZEX)
		{ // 画面端に到達
			for (int nCnt = 0; nCnt < FIRSTWAVE_END; nCnt++)
			{
				g_aEnemy[nCnt].pos.y += 20.0f;
				if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - ENEMY_SIZEX))
				{
					g_aEnemy[nCnt].pos.x += g_aEnemy[nCnt].move.x * 2;
				}
				g_aEnemy[nCnt].move.x *= -1;
				g_aEnemy[nCnt].pos.x += g_aEnemy[nCnt].move.x * 4;
			}
		}
	}

	if (info.nVanish >= 20 && info.nVanish < 40)
	{ // 初期配置の半数を撃破後
	  // 移動力の加速
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].move.x == 2.0f)
			{
				g_aEnemy[nCnt].move.x = 4.0f;
			}
			else if (g_aEnemy[nCnt].move.x == -2.0f)
			{
				g_aEnemy[nCnt].move.x = -4.0f;
			}
		}
	}
	else if (info.nVanish >= FIRSTWAVE_END && info.nDestroy < SECONDWAVE_END)
	{ // 第2ウェーブ
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{ // 敵の移動力
		  // 左右への波型移動
			g_aEnemy[nCnt].move.x = sinf(g_aEnemy[nCnt].fAngle) * SPEEDUP;

			// タイプごとに移動力の指定
			SetEnemyMove(g_aEnemy[nCnt].nType, nCnt);
		}
	}
	else if (info.nDestroy >= SECONDWAVE_END && info.nDestroy < THIRDWAVE_END)
	{ // 第3ウェーブの敵移動
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].move.x != -4.0f)
			{
				// 移動するX方向の指定
				g_aEnemy[nCnt].move.x = 4.0f;
			}
		}

		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			if (g_aEnemy[nCnt].pos.x >= (SCREEN_WIDTH - 20))
			{
				g_aEnemy[nCnt].move.x = -4.0f;
			}
			else if (g_aEnemy[nCnt].pos.x <= 20)
			{
				g_aEnemy[nCnt].move.x = 4.0f;
			}
		}
	}
	else if (info.nDestroy >= THIRDWAVE_END && info.nDestroy < BIGENEMY_APPEAR)
	{ // 第4ウェーブの敵移動
		for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
		{
			g_aEnemy[nCnt].move.x = 0.0f;
			g_aEnemy[nCnt].move.y = 2.5f;
		}
	}
}

//==============================================================================
// ボス敵の配置
//==============================================================================
void SetBigEnemy(void)
{
	// ローカル変数宣言
	ENEMY *pEnemy;
	PHASESTATE phase = GetPhase();	// 警戒状態の取得

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// 残存する敵の排除
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->nType != 5)
		{
			SetExplosion(pEnemy->pos);
			pEnemy->bUse = false;
		}
	}

	// サウンドの停止処理
	StopSound();

	// サウンドの再生
	PlaySound(SOUND_LABEL_SE_WARNING);

	// フェーズを通常に戻し、ボスの出現
	if (phase == PHASESTATE_NORMAL && g_aEnemy[0].bUse == false)
	{
		// 大型敵の設定
		SetEnemy(D3DXVECTOR3(640.0f, -60.0f, 0.0f), 5, BIGENEMY_LIFE);
		info.nBigEnemyCnt++;
	}
}

//==============================================================================
// 敵の取得
//==============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==============================================================================
// 敵の命中とダメージ判定
//==============================================================================
bool HitEnemy(int nIdx, int nDamage)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	ENEMY *pEnemy;
	int nScore = 0;

	// 敵の取得
	pEnemy = &g_aEnemy[0];

	// ダメージ計算
	g_aEnemy[nIdx].nLife -= nDamage;

	// 残り体力による処理
	if (g_aEnemy[nIdx].nLife <= 0)
	{
		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION000);

		// スコア計算
		for (int nCnt = 0; nCnt < MAX_TYPE_ENEMY; nCnt++)
		{
			if (g_aEnemy[nIdx].nType == nCnt)
			{
				if (nCnt == 0)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.9f, 0.4f, 0.1f, 1.0f), 15.0f, 0.015f);

					// スコアの設定
					nScore = 1000;
				}
				else if (nCnt == 1)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.8f, 0.3f, 0.8f, 1.0f), 15.0f, 0.015f);

					// スコアの設定
					nScore = 2000;
				}
				else if (nCnt == 2)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.5f, 0.9f, 0.4f, 1.0f), 15.0f, 0.015f);

					// スコアの設定
					nScore = 3000;
				}
				else if (nCnt == 3)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.2f, 0.9f, 0.2f, 1.0f), 15.0f, 0.015f);

					// スコアの設定
					nScore = 4000;
				}
				else if (nCnt == 4)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.05f, D3DXCOLOR(0.8f, 0.2f, 0.3f, 1.0f), 15.0f, 0.015f);

					// スコアの設定
					nScore = 12500;
				}
				else if (nCnt == 5)
				{
					// パーティクルの発生
					SetEffect(g_aEnemy[nIdx].pos, 0.2f, D3DXCOLOR(0.9f, 0.2f, 0.2f, 1.0f), 15.0f, 0.015f);

					// ボスの状態変化
					info.boss = BIGENEMY_DEATH;

					// スコアの設定
					nScore = 80000;
				}
			}
		}

		// 撃破した位置の高さによるスコアの倍率
		if (pEnemy->pos.y >= LOW_SCORE && pEnemy->pos.y < MIDDLE_SCORE)
		{
			nScore *= 2;
		}
		else if (pEnemy->pos.y >= MIDDLE_SCORE && pEnemy->pos.y < HIGH_SCORE)
		{
			nScore *= 3;
		}
		else if (pEnemy->pos.y >= HIGH_SCORE && pEnemy->pos.y < WINDOW_LOWEST)
		{
			nScore *= 4;
		}

		// 撃破数・消滅数の加算
		info.nDestroy++;
		if (info.nVanish < 40)
		{
			info.nVanish++;
		}
		
		// スコアの加算
		AddScore(nScore);

		// 敵の破棄
		g_aEnemy[nIdx].bUse = false;

		// 第1ウェーブのアイテム配置
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			if (nIdx == g_aItemRand[0][nCnt])
			{
				SetItem(g_aEnemy[nIdx].pos, nCnt);
			}
		}

		// 第2ウェーブのアイテム配置
		for (int nCnt = 0; nCnt < MAX_TYPE_ITEM; nCnt++)
		{
			if (g_aItemRand[1][nCnt] == info.nDestroy)
			{
				SetItem(g_aEnemy[nIdx].pos, nCnt);
			}
		}

		return true;
	}
	else
	{
		// 敵の状態の変化
		g_aEnemy[nIdx].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 9;

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION000);

		// パーティクルの発生
		SetEffect(g_aEnemy[nIdx].pos, 0.01f, D3DXCOLOR(0.9f, 0.1f, 0.1f, 1.0f), 3.0f, 0.015f);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nIdx * 4;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// 頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();

		return false;
	}
}

//==============================================================================
// 敵全体情報の取得
//==============================================================================
ENEMYINFO GetInfo(void)
{
	return info;
}