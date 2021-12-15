//=============================================================================
//
// プレイヤー描画処理 [player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "character.h"
#include "textdata_player.h"

// マクロ定義
#define PLAYER_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250.0f, 0.0f))	// プレイヤー開始位置
#define PLAYERX				(20.0f)																// プレイヤーの大きさX
#define PLAYERY				(20.0f)																// プレイヤーの大きさY
#define SPEED_UP			(3.0f)																// 加速度
#define SPEED_DOWN			(0.2f)																// 減衰係数
#define MISSILE_INTERVAL	(30)																// ミサイル発射間隔

// プレイヤークラス
class CPlayer : public CCharacter
{
public:
	// プレイターのステータス
	typedef enum
	{
		PLAYERSTATE_APPEAR = 0,	// 出現
		PLAYERSTATE_NORMAL,		// 通常
		PLAYERSTATE_DAMAGE,		// ダメージ
		PLAYERSTATE_WAIT,		// 出現待ち
		PLAYERSTATE_DEATH,		// 死亡
		PLAYERSTATE_GAMEOVER,	// ゲームオーバー
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer();
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DamageLife(int nDamage,D3DXVECTOR3 pos);

	static CPlayer *Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY);

	void PlayerState(void);
	static void TransStock(int nTrans) { m_nStock += nTrans; }
	void PlayerMove(D3DXVECTOR3 pos);

	static int GetStcok(void) { return m_nStock; }
	static PLAYERSTATE GetState(void) { return m_state; }

	void MissileInterval(void);

private:
	int m_nBulletPress;				// 弾のプレス発射カウント
	D3DXVECTOR3 m_Move;				// 移動量
	int m_nStateCnt;				// ステータスの変化カウント
	static int m_nStock;			// 残機数
	static PLAYERSTATE m_state;		// プレイヤーの状態
	float m_fSpeed;					// スピード
	static bool m_bMissileOff;		// ミサイルのクールダウン判定
	static int m_nMissileCnt;		// ミサイルのインターバル
};

#endif