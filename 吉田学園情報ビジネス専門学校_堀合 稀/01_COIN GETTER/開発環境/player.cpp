//==============================================================================
//
// プレイヤーの描画〔player.cpp〕
// AUTHOR : MARE HORIAI
//
//==============================================================================
#include "player.h"
#include "game.h"
#include "enemy.h"
#include "block.h"
#include "input.h"
#include "item.h"
#include "score.h"
#include "particle.h"
#include "sound.h"
#include "timer.h"
#include "gamepad.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define PLAYER_LEFT_REMIT	(-20)	// プレイヤー移動の左側ループ地点
#define PLAYER_RIGHT_REMIT	(1300)	// プレイヤー移動の右側ループ地点
#define APPEAR_REMIT		(80)	// 出現状態カウンター上限
#define WAIT_REMIT			(30)	// 待機状態カウンター上限

//==============================================================================
// グローバル変数
//==============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			// プレイヤーテクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// バッファへのポインタ
PLAYER Player;										// プレイヤーの情報

//==============================================================================
// プレイヤーの初期化処理
//==============================================================================
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;
	pItem = GetItem();

	// デバイスの取得
	pDevice = GetDevice();

	// 情報の初期化
	Player.fWidth = PLAYER_SIZEX;
	Player.fHeight = PLAYER_SIZEY;
	Player.colorPlayer = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Player.state = PLAYERSTATE_NORMAL;
	Player.nCounterState = 0;
	Player.nCounterDisp = 0;
	Player.nLife = PLAYER_LIFE;
	Player.nStock = 0;
	Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player.nCntAnime = 1;
	Player.bRight = true;
	Player.bJump = true;
	Player.fAnime = 0.0f;
	Player.nGetCoin = 0;
	Player.nTrueCoin = 0;
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++, pItem++)
	{
		if (pItem->bUse == true)
		{
			Player.nTrueCoin++;
		}
	}

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

	// プレイヤーの初期配置中心座標
	SetPlayer();

	// プレイヤーの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(Player.pos.x - Player.fWidth, Player.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Player.pos.x - Player.fWidth, Player.pos.y - Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Player.pos.x + Player.fWidth, Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Player.pos.x + Player.fWidth, Player.pos.y - Player.fHeight, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / 4.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(1.0f / 4.0f, 0.0f);

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
	// 前回の位置を保存
	Player.posOld = Player.pos;

	// アイテム入手
	ItemGet();

	// 敵との接触
	TouchEnemy();

	// プレイヤーの移動
	MovePlayer();

	// プレイヤーのテクスチャアニメーション
	PlayerAnim();

	// プレイヤーの状態
	PlayerState(Player.state);
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
// アイテム入手処理
//==============================================================================
void ItemGet(void)
{
	// ローカル変数宣言
	ITEM *pItem;
	int nStage = GetStage();

	// アイテムの取得
	pItem = GetItem();

	// 自機とアイテムの接触
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++, pItem++)
	{
		if (Player.pos.x + PLAYER_SIZEX >= pItem->pos.x - ITEM_SIZEX / 2 &&
			Player.pos.x - PLAYER_SIZEX <= pItem->pos.x + ITEM_SIZEX / 2 &&
			Player.pos.y - PLAYER_SIZEY <= pItem->pos.y + ITEM_SIZEY / 2 &&
			Player.pos.y >= pItem->pos.y - ITEM_SIZEY / 2)
		{
			if (pItem->bUse == true)
			{ // アイテム出現時

				// アイテムポリゴンの消滅
				pItem->bUse = false;

				// スコアの追加
				if (pItem->col.g == 0.0f)
				{
					// エフェクトの設定
					SetEffect(D3DXVECTOR3(pItem->pos.x, pItem->pos.y, 0.0f), 0.02f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.05f, 30);

					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_HIGHCOIN);
					AddScore(50000);
				}
				else
				{
					// エフェクトの設定
					SetEffect(D3DXVECTOR3(pItem->pos.x, pItem->pos.y, 0.0f), 0.02f, D3DXCOLOR(1.0f, 0.9f, 0.1f, 1.0f), 10.0f, 0.05f, 30);

					// サウンドの再生
					PlaySound(SOUND_LABEL_SE_COIN);
					AddScore(5000);
				}

				Player.nGetCoin++;

				// ステージのクリア条件
				if (Player.nGetCoin == Player.nTrueCoin)
				{
					Player.state = PLAYERSTATE_CLEAR;
				}
			}
		}
	}
}

//==============================================================================
// 自機へのダメージ判定
//==============================================================================
bool HitPlayer(int nDamage)
{
	// ダメージ計算
	Player.nLife -= nDamage;
	int nScore = GetScore();

	// 残り体力による処理
	if (Player.nLife <= 0)
	{ // 撃破されたとき
	  // サウンドの再生
		PlaySound(SOUND_LABEL_SE_STOCK_LOST);

		// スコアの減算
		if (nScore >= 2000)
		{
			AddScore(-2000);
		}
		else if (nScore < 2000)
		{
			AddScore(-nScore);
		}

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
			UpdateScore();							// スコア表示の更新
			Player.state = PLAYERSTATE_GAMEOVER;	// ゲームオーバーへの移行
		}

		return true;
	}
	else
	{ // ダメージを受けたとき
		Player.state = PLAYERSTATE_DAMAGE;	// 被ダメージ状態への変化
		Player.nCounterState = 9;			// カウントの設定

		// サウンドの再生
		PlaySound(SOUND_LABEL_SE_HIT000);

		// スコアの減算
		if (nScore >= 2000)
		{
			AddScore(-2000);
		}
		else if (nScore < 2000)
		{
			AddScore(-nScore);
		}

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
			if (Player.pos.x + PLAYER_FUTSIZEX >= pEnemy->pos.x - ENEMY_SIZEX &&
				Player.pos.x - PLAYER_FUTSIZEX <= pEnemy->pos.x + ENEMY_SIZEX &&
				Player.pos.y - PLAYER_SIZEY <= pEnemy->pos.y &&
				Player.pos.y >= pEnemy->pos.y - ENEMY_SIZEY &&
				Player.state == PLAYERSTATE_NORMAL)
			{
				// 自機との接触処理(体力が1減少)
				HitPlayer(1);
			}
		}
	}
}

//==============================================================================
// プレイヤーの移動
//==============================================================================
void MovePlayer(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;

	// プレイヤーの操作
	if (Player.bJump == false)
	{ // ジャンプの処理
		if (GetKeyboardTrigger(KEYINFO_JUMP) == true || IsButtonDown(KEYINFO::KEYINFO_OK) == true)
		{
			Player.move.y = JUMP_POWER;
			// サウンドの再生
			PlaySound(SOUND_LABEL_SE_JUMP);
		}
	}

	// プレイヤーの移動
	if (GetKeyboardPress(KEYINFO_LEFT) == true || IsButtonPush(KEYINFO::KEYINFO_LEFT) == true)
	{ // 左へ
		Player.move.x += sinf(D3DX_PI / -2) * SPEEDUP;
		Player.bRight = false;		// 左に向く
		if (Player.bJump == false)
		{
			Player.nCntAnime++;
		}

		if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
		{// 逆移動量の加算
			Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
		}
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true || IsButtonPush(KEYINFO::KEYINFO_RIGHT) == true)
	{ // 右へ
		Player.move.x += sinf(D3DX_PI / 2) * SPEEDUP;
		Player.bRight = true;
		if (Player.bJump == false)
		{
			Player.nCntAnime++;
		}
	}
	else
	{ // 静止状態のアニメーションカウント
		Player.nCntAnime = 1;
		Player.fAnime = 0.0f;
	}

	// 移動量の代入
	Player.pos.x += Player.move.x;
	Player.pos.y += Player.move.y;

	// 加速後の減速処理(慣性)
	Player.move.x += (0.0f - Player.move.x) * SPEEDDOWN;

	// 重力の発生
	if (Player.move.y <= MAX_GRAVITY && Player.bJump == true)
	{
		Player.move.y += DROP_SPEEDUP;
	}

	// ブロックとの接触判定
	if (CollisionBlock(
		&Player.pos, &Player.posOld,
		&Player.move, PLAYER_FUTSIZEX,
		Player.fHeight, ONTYPE_PLAYER) == false)
	{
		Player.bJump = true;
	}
	else
	{
		Player.bJump = false;
	}

	// 画面左でループ(右側へ移動)
	if (Player.pos.x < PLAYER_LEFT_REMIT)
	{
		Player.pos.x = PLAYER_RIGHT_REMIT;
	}

	// 画面右でループ(左側へ移動)
	if (Player.pos.x > PLAYER_RIGHT_REMIT)
	{
		Player.pos.x = PLAYER_LEFT_REMIT;
	}

	// 画面下部で落下判定
	if (Player.pos.y > 760.0f)
	{
		Player.pos = POS3_PLAYER;
		HitPlayer(1);
	}

	// 頂点バッファをロックする
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの頂点座標の更新
	pVtx[0].pos = D3DXVECTOR3(Player.pos.x - Player.fWidth, Player.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Player.pos.x - Player.fWidth, Player.pos.y - Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Player.pos.x + Player.fWidth, Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Player.pos.x + Player.fWidth, Player.pos.y - Player.fHeight, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==============================================================================
// プレイヤーの状態
//==============================================================================
void PlayerState(PLAYERSTATE state)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	int nTimer = GetTimer();	// タイマーの取得
	int nStage = GetStage();	// ステージの取得

	// 時間切れの時
	if (nTimer <= 0 && state != PLAYERSTATE_CLEAR && nStage != MAX_STAGE)
	{
		Player.state = PLAYERSTATE_GAMEOVER;
	}

	switch (state)
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
		Player.colorPlayer.a = 0;				// 各頂点カラーの設定
		Player.state = PLAYERSTATE_WAIT;		// 待機状態へ移行

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

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==============================================================================
// プレイヤーテクスチャのアニメーション
//==============================================================================
void PlayerAnim(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	float fAnime = 0.0f;
	float fRight = 0.0f;

	// テクスチャ左右の切り替え
	if (Player.bRight == false)
	{
		fRight = 0.5f;
	}
	else
	{
		fRight = 0.0f;
	}

	// 移動中のアニメーション
	if (Player.nCntAnime % 8 == 0)
	{
		Player.fAnime += 1.0f;	// テクスチャの変化
	}

	// ジャンプ状態のアニメーション固定
	if (Player.bJump == true)
	{
		Player.fAnime = 1.0f;
	}

	// 頂点バッファをロックする
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (Player.fAnime * 0.25f), 0.5f + fRight);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (Player.fAnime * 0.25f), 0.0f + fRight);
	pVtx[2].tex = D3DXVECTOR2(0.25f + (Player.fAnime * 0.25f), 0.5f + fRight);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (Player.fAnime * 0.25f), 0.0f + fRight);

	// 頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//==============================================================================
// ステージごとのプレイヤーの初期配置
//==============================================================================
void SetPlayer(void)
{
	// ローカル変数宣言
	int nStage = GetStage();

	// ステージごとの初期配置
	switch (nStage)
	{
	case 0:	// ステージ1
		Player.pos = POS1_PLAYER;
		Player.posOld = POS1_PLAYER;

		break;

	case 1:	// ステージ2
		Player.pos = POS2_PLAYER;
		Player.posOld = POS2_PLAYER;

		break;

	case 2:	// ステージ3
		Player.pos = POS3_PLAYER;
		Player.posOld = POS3_PLAYER;

		break;

	default:
		break;
	}
}