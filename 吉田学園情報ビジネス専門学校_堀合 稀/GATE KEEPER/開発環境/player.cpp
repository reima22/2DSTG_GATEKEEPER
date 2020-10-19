//==============================================================================
//
// プレイヤーの描画〔player.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "player.h"
#include "score.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "item.h"
#include "fade.h"
#include "sound.h"
#include "particle.h"
#include "bullet_ui.h"
#include "line.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define PLAYER_UP_REMIT		(90)	// プレイヤー移動の上限界
#define PLAYER_DOWN_REMIT	(588)	// プレイヤー移動の下限界
#define PLAYER_LEFT_REMIT	(20)	// プレイヤー移動の左限界
#define PLAYER_RIGHT_REMIT	(1260)	// プレイヤー移動の右限界
#define APPEAR_REMIT		(80)	// 出現状態カウンター上限
#define WAIT_REMIT			(30)	// 待機状態カウンター上限

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			// プレイヤーテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// バッファへのポインタ
PLAYER Player;										// プレイヤーの情報
int g_nShotCount;									// 弾発射間隔

//==============================================================================
// プレイヤーの初期化処理
//==============================================================================
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	Player.state = PLAYERSTATE_NORMAL;
	Player.colorPlayer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Player.nCounterDisp = 0;
	Player.nCounterState = 0;
	Player.nLife = PLAYER_LIFE;
	Player.nStock = PLAYER_STOCK;
	Player.nLineOver = 0;
	Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player.fLength = sqrtf(PLAYER_SIZEX * PLAYER_SIZEX + PLAYER_SIZEY * PLAYER_SIZEY);
	Player.fAngle = atan2f(PLAYER_SIZEX, PLAYER_SIZEY);
	Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	for (int nCnt = 0; nCnt < BULLET_PLUS; nCnt++)
	{
		Player.aBullet[nCnt] = false;
	}
	g_nShotCount = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/player000.png",
		&g_pTexturePlayer);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// プレイヤーの中心座標
	Player.pos = D3DXVECTOR3(POSPLAYERX, POSPLAYERY, 0.0f);

	// プレイヤーの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(Player.pos.x + sinf(-Player.fAngle) * Player.fLength, Player.pos.y + cosf(-Player.fAngle) * Player.fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Player.pos.x + sinf(Player.fAngle - D3DX_PI) * Player.fLength, Player.pos.y + cosf(Player.fAngle - D3DX_PI) * Player.fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Player.pos.x + sinf(Player.fAngle) * Player.fLength, Player.pos.y + cosf(Player.fAngle) * Player.fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Player.pos.x + sinf(-Player.fAngle + D3DX_PI) * Player.fLength, Player.pos.y + cosf(-Player.fAngle + D3DX_PI) * Player.fLength, 0.0f);
	
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

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

//==============================================================================
// プレイヤーの終了処理
//==============================================================================
void UninitPlayer(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	// テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

//==============================================================================
// プレイヤーの更新処理
//==============================================================================
void UpdatePlayer(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	
	if (Player.state == PLAYERSTATE_NORMAL)
	{ // ブースターパーティクルの設定
		SetEffect(D3DXVECTOR3(Player.pos.x, Player.pos.y + 25.0f, 0.0f), 0.009f, D3DXCOLOR(0.2f, 0.4f, 0.9f, 1.0f), 3.0f, 0.1f);
	}

	// 敵との接触時
	TouchEnemy();

	// プレイヤーのアイテム取得
	ItemGet();

	// プレイヤーの操作
	PlayerMove();

	// プレイヤーの状態
	switch (Player.state)
	{
	case PLAYERSTATE_APPEAR:	// プレイヤー出現時
		Player.nCounterDisp++;				// 出現状態カウントの増加

		if (Player.nCounterDisp % 2 == 0)
		{ // カウントが２の倍数で処理
			Player.colorPlayer.a = 1.0f;	// 各頂点透明度の設定
		}
		else
		{ // それ以外での処理
			Player.colorPlayer.a = 0.0f;	// 各頂点透明度の設定
		}

		if (Player.nCounterDisp >= APPEAR_REMIT)
		{ // カウンター上限時
			Player.state = PLAYERSTATE_NORMAL;	// 通常時への移行
			Player.nCounterDisp = 0;			// カウンターのリセット
		}

		break;

	case PLAYERSTATE_DAMAGE:	// プレイヤーがダメージを受ける時
		Player.nCounterState--;	// 被ダメージ状態のカウントの減少

		if (Player.nCounterState <= 0)
		{ // 被ダメージ状態の終了
			Player.state = PLAYERSTATE_APPEAR;						// 通常状態への移行
			Player.colorPlayer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 各頂点カラーの設定
		}

		break;

	case PLAYERSTATE_WAIT:		// プレイヤーの待機状態
		Player.nCounterDisp++;	// 待機カウントの増加

		if (Player.nCounterDisp >= WAIT_REMIT)
		{ // 待機終了時
			Player.state = PLAYERSTATE_APPEAR;	// 出現状態へ移行
			Player.nLife = PLAYER_LIFE;			// 体力のリセット
			Player.nCounterDisp = 0;			// カウントのリセット
		}

		break;

	case PLAYERSTATE_DEATH:		// プレイヤーの死亡
		Player.colorPlayer.a = 0;								// 各頂点カラーの設定
		Player.pos = D3DXVECTOR3(POSPLAYERX, POSPLAYERY, 0.0f);	// 配置位置のリセット
		Player.state = PLAYERSTATE_WAIT;						// 待機状態へ移行

		break;

	case PLAYERSTATE_GAMEOVER:	// ゲームオーバー
		Player.colorPlayer.a = 0;	// 各頂点カラーの設定

		break;
	}
	
	// 頂点バッファをロックする
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// 各頂点の色の更新
	pVtx[0].col = Player.colorPlayer;
	pVtx[1].col = Player.colorPlayer;
	pVtx[2].col = Player.colorPlayer;
	pVtx[3].col = Player.colorPlayer;

	// ポリゴンの頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(Player.pos.x + sinf(-Player.fAngle + Player.rot.z) * Player.fLength, Player.pos.y + cosf(-Player.fAngle + Player.rot.z) * Player.fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Player.pos.x + sinf(Player.fAngle - D3DX_PI + Player.rot.z) * Player.fLength, Player.pos.y + cosf(Player.fAngle - D3DX_PI + Player.rot.z) * Player.fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Player.pos.x + sinf(Player.fAngle + Player.rot.z) * Player.fLength, Player.pos.y + cosf(Player.fAngle + Player.rot.z) * Player.fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Player.pos.x + sinf(-Player.fAngle + D3DX_PI + Player.rot.z) * Player.fLength, Player.pos.y + cosf(-Player.fAngle + D3DX_PI + Player.rot.z) * Player.fLength, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==============================================================================
// プレイヤーの描画処理
//==============================================================================
void DrawPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	
	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,						// 描画を開始する頂点インデックス
		2);						// 描画するプリミティブ数
}

//==============================================================================
// プレイヤーの取得
//==============================================================================
PLAYER *GetPlayer(void)
{
	return &Player;
}

//==============================================================================
// 自機への命中とダメージ判定
//==============================================================================
bool HitPlayer(int nDamage)
{
	// ダメージ計算
	Player.nLife -= nDamage;

	// 残り体力による処理
	if (Player.nLife <= 0)
	{ // 撃破されたとき
		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_EXPLOSION003);

		// エフェクトの発生
		SetEffect(Player.pos, 0.3f, D3DXCOLOR(0.2f, 0.4f, 0.9f, 1.0f), 15.0f, 0.015f);

		// スコアの減少
		AddScore(-5000);

		// 死亡ステータスへ
		Player.state = PLAYERSTATE_DEATH;

		// 残機ありで撃墜された場合
		if (Player.nStock >= 0)
		{
			Player.nStock--;	// 残機の減少
		}
		
		// 残機なしで撃墜された場合
		if (Player.nStock < 0)
		{
			Player.nStock = 0;						// 残機の表示を「0」に保持
			Player.state = PLAYERSTATE_GAMEOVER;	// ゲームオーバーへの移行
		}

		return true;
	}
	else
	{ // ダメージを受けたとき
		Player.state = PLAYERSTATE_DAMAGE;	// 被ダメージ状態への変化
		Player.nCounterState = 9;			// カウントの設定

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_HIT001);

		// パーティクルの設定
		SetEffect(Player.pos, 0.05f, D3DXCOLOR(0.1f, 0.4f, 0.9f, 1.0f), 3.0f, 0.015f);

		// プレイヤーの色設定
		Player.colorPlayer = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		return false;
	}
}

//==============================================================================
// 敵との接触
//==============================================================================
void TouchEnemy(void)
{
	// ローカル変数宣言
	ENEMY *pEnemy;

	// 敵の取得
	pEnemy = GetEnemy();

	// 敵との接触判定
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (Player.pos.x + PLAYER_SIZEX >= pEnemy->pos.x - ENEMY_SIZEX &&
				Player.pos.x - PLAYER_SIZEX <= pEnemy->pos.x + ENEMY_SIZEX &&
				Player.pos.y - PLAYER_SIZEY <= pEnemy->pos.y + ENEMY_SIZEY &&
				Player.pos.y + PLAYER_SIZEY >= pEnemy->pos.y - ENEMY_SIZEY &&
				Player.state == PLAYERSTATE_NORMAL && pEnemy->nType != 5)
			{
				// 自機との接触処理(体力が1減少)
				HitPlayer(1);

				// スコアの減少
				AddScore(-10000);
			}
			else if (Player.pos.x + PLAYER_SIZEX >= pEnemy->pos.x - BIGENEMY_SIZEX &&
				Player.pos.x - PLAYER_SIZEX <= pEnemy->pos.x + BIGENEMY_SIZEX &&
				Player.pos.y - PLAYER_SIZEY <= pEnemy->pos.y + BIGENEMY_SIZEY &&
				Player.pos.y + PLAYER_SIZEY >= pEnemy->pos.y - BIGENEMY_SIZEY &&
				Player.state == PLAYERSTATE_NORMAL && pEnemy->nType == 5)
			{ // ボスとの接触
				// ボス敵との接触処理(体力全損)
				HitPlayer(Player.nLife);

				// スコアの減少
				AddScore(-10000);
			}
			else if (pEnemy->pos.y >= WINDOW_LOWEST)
			{ // ライン越え
				Player.nLineOver++;					// ライン越えのカウント増加
				AddScore(-3000);					// スコアの減少
				PlaySound(SOUND_LABEL_SE_LINEOVER);	// サウンドの再生

				// 一定数侵入
				if (Player.nLineOver >= LINE_REMIT)
				{
					Player.state = PLAYERSTATE_GAMEOVER;	// ゲームオーバーへの移行
				}
			}
		}
	}
}

//==============================================================================
// アイテム入手処理
//==============================================================================
void ItemGet(void)
{
	// ローカル変数宣言
	ITEM *pItem;

	// アイテムの取得
	pItem = GetItem();

	// 自機とアイテムの接触
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++, pItem++)
	{
		if (Player.pos.x + PLAYER_SIZEX >= pItem->pos.x - ITEM_SIZEX &&
			Player.pos.x - PLAYER_SIZEX <= pItem->pos.x + ITEM_SIZEX &&
			Player.pos.y - PLAYER_SIZEY <= pItem->pos.y + ITEM_SIZEY &&
			Player.pos.y + PLAYER_SIZEY >= pItem->pos.y - ITEM_SIZEY)
		{
			if (pItem->bUse == true)
			{ // アイテム出現時
				for (int nCntBullet = 0; nCntBullet < BULLET_PLUS; nCntBullet++)
				{
					if (pItem->nType == nCntBullet)
					{ // アイテムの種類によって取得する武装の判別
						if (Player.aBullet[nCntBullet] == false)
						{ // 追加弾の取得
							Player.aBullet[nCntBullet] = true;
						}
					}
				}

				// アイテムポリゴンの消滅
				pItem->bUse = false;

				// サウンドの再生
				PlaySound(SOUND_LABEL_SE_ITEM000);

				// スコアの追加
				AddScore(5000);
			}
		}
	}
}

//==============================================================================
// プレイヤーの操作処理
//==============================================================================
void PlayerMove(void)
{
	// ローカル変数宣言
	SETBULLET State;

	// 弾の装填情報の取得
	State = GetState();

	// 移動量の代入
	Player.pos.x += Player.move.x;
	Player.pos.y += Player.move.y;

	// ポリゴンの中心座標の更新（通常移動）
	if (Player.state == PLAYERSTATE_NORMAL || Player.state == PLAYERSTATE_DAMAGE || Player.state == PLAYERSTATE_APPEAR)
	{
		if (GetKeyboardPress(KEYINFO_UP) == true)
		{ // 上へ
			Player.move.y += cosf(D3DX_PI) * SPEEDUP;

			if (GetKeyboardPress(KEYINFO_LEFT) == true)
			{
				Player.move.x += sinf(D3DX_PI / -2) * SPEEDUP;
			}
			else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
			{
				Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
			}
			else if (GetKeyboardPress(KEYINFO_DOWN) == true)
			{
				Player.move.y += cosf(0.0f) * SPEEDUP;
			}
		}
		else if (GetKeyboardPress(KEYINFO_DOWN) == true)
		{ // 下へ
			Player.move.y += cosf(0.0f) * SPEEDUP;

			if (GetKeyboardPress(KEYINFO_LEFT) == true)
			{
				Player.move.x += sinf(D3DX_PI / -2) * SPEEDUP;
			}
			else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
			{
				Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
			}
		}
		else if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{ // 左へ
			Player.move.x += sinf(D3DX_PI / -2) * SPEEDUP;

			if (GetKeyboardPress(KEYINFO_RIGHT) == true)
			{
				Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
			}
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{ // 右へ
			Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
		}
	}

	// アイテム切り替えのサウンド再生
	if (Player.aBullet[0] == true || Player.aBullet[1] == true || Player.aBullet[2] == true)
	{
		if (GetKeyboardTrigger(KEYINFO_CHANGE) == true)
		{
			PlaySound(SOUND_LABEL_SE_ITEM001);
		}
	}

	// スペースキー入力(弾の発射)
	if (Player.state == PLAYERSTATE_NORMAL || Player.state == PLAYERSTATE_DAMAGE || Player.state == PLAYERSTATE_APPEAR)
	{
		if (GetKeyboardPress(KEYINFO_SHOT) == true && g_nShotCount == 0)
		{
			g_nShotCount = 8;

			if (State == SETBULLET_NORMAL)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_SHOT001);

				// 弾の設定
				SetBullet(Player.pos, D3DXVECTOR3(0.0f, -10.0f, 0.0f), 100, BULLETTYPE_PLAYER);
			}
			else if (State == SETBULLET_WAVE)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_SHOT003);

				// 弾の設定
				SetBullet(D3DXVECTOR3(Player.pos.x + 20.0f, Player.pos.y, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), 100, BULLETTYPE_PLAYER);
				SetBullet(D3DXVECTOR3(Player.pos.x - 20.0f, Player.pos.y, 0.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f), 100, BULLETTYPE_PLAYER);
			}
			else if (State == SETBULLET_MISSILE)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_SHOT002);

				// 弾の設定
				SetBullet(Player.pos, D3DXVECTOR3(-4.0f, -8.0f, 0.0f), 50, BULLETTYPE_PLAYER);
				SetBullet(Player.pos, D3DXVECTOR3(4.0f, -8.0f, 0.0f), 50, BULLETTYPE_PLAYER);
			}
			else if (State == SETBULLET_REVERSEWAVE)
			{
				// 音の再生
				PlaySound(SOUND_LABEL_SE_SHOT003);

				// 弾の設定
				SetBullet(Player.pos, D3DXVECTOR3(0.0f, 10.0f, 0.0f), 100, BULLETTYPE_PLAYER);
			}
		}
		else if (g_nShotCount > 0)
		{
			g_nShotCount--;
		}
	}

	// 加速後の減速処理
	Player.move.x += (0.0f - Player.move.x) * SPEEDDOWN;
	Player.move.y += (0.0f - Player.move.y) * SPEEDDOWN;

	// 画面上部限界で停止
	if (Player.pos.y <= PLAYER_UP_REMIT)
	{
		Player.pos.y = PLAYER_UP_REMIT;
	}

	// 画面下部限界で停止
	if (Player.pos.y >= PLAYER_DOWN_REMIT)
	{
		Player.pos.y = PLAYER_DOWN_REMIT;
	}

	// 画面左限界で停止
	if (Player.pos.x <= PLAYER_LEFT_REMIT)
	{
		Player.pos.x = PLAYER_LEFT_REMIT;
	}

	// 画面右限界で停止
	if (Player.pos.x >= PLAYER_RIGHT_REMIT)
	{
		Player.pos.x = PLAYER_RIGHT_REMIT;
	}
}