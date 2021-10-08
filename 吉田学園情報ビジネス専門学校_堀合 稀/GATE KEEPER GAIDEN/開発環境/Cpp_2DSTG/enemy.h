//=============================================================================
//
// 敵描画処理 [enemy.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "character.h"

// マクロ定義
#define ENEMYX_0		(20.0f)		// 敵0の大きさX
#define ENEMYY_0		(20.0f)		// 敵0の大きさY

#define ENEMYX_1		(22.5f)		// 敵1の大きさX
#define ENEMYY_1		(22.5f)		// 敵1の大きさY

#define ENEMYX_2		(27.5f)		// 敵2の大きさX
#define ENEMYY_2		(27.5f)		// 敵2の大きさY

#define ENEMYX_3		(30.0f)		// 敵3の大きさX
#define ENEMYY_3		(30.0f)		// 敵3の大きさY

#define MOTHER_ENEMY_X	(120.0f)	// 母艦の大きさX
#define MOTHER_ENEMY_Y	(80.0f)		// 母艦の大きさY
#define MOTHER_SET_POS	(D3DXVECTOR3(640.0f,-60.0f,0.0f))	// 母艦の配置する位置 
#define MOTHER_POS_Y	(120.0f)	// 母艦が停止する高さ

#define MOTHER_MOVE_0	(2.0f)		// フェーズごとの移動力
#define MOTHER_MOVE_1	(3.0f)		// フェーズごとの移動力
#define MOTHER_MOVE_2	(3.5f)		// フェーズごとの移動力
#define MOTHER_MOVE_3	(5.0f)		// フェーズごとの移動力

#define ENEMY_SET_POS_Y			(-50.0f)	// 敵の自動生成する高さ
#define ENEMY_SET_POS_Y_UNDER	(530.0f)	// 画面下部に出現する場合
#define ENEMY_SET_POS_X_UNDER_L	(-50.0f)	// 画面下部に出現する場合の左
#define ENEMY_SET_POS_X_UNDER_R	(1330.0f)	// 画面下部に出現する場合の右

#define MOTHER_PARTS_SIZE	(30.0f)			// パーツのサイズ
#define BULLET_AIM_SPEED		(4.0f)		// 照準弾の速度
#define MOTHER_PARTS_POS_0	(D3DXVECTOR3(-120.0f,0.0f,0.0f))	// パーツの位置
#define MOTHER_PARTS_POS_1	(D3DXVECTOR3(120.0f,0.0f,0.0f))		// パーツの位置
#define MOTHER_PARTS_POS_2	(D3DXVECTOR3(0.0f,80.0f,0.0f))		// パーツの位置

// 敵クラス
class CEnemy : public CCharacter
{
public:
	// 敵のステータス
	typedef enum
	{
		ENEMYSTATE_NORMAL = 0,	// 通常
		ENEMYSTATE_DAMAGE,		// 被弾
		ENEMYSTATE_DEATH,		// 撃墜された
		ENEMYSTATE_MAX
	}ENEMYSTATE;

	// 敵の種類
	typedef enum
	{
		ENEMY_TYPE_0 = 0,				// 通常敵0
		ENEMY_TYPE_1,					// 通常敵1
		ENEMY_TYPE_2,					// 通常敵2
		ENEMY_TYPE_3,					// 通常敵3
		ENEMY_TYPE_MOTHER,				// 母艦
		ENEMY_TYPE_MOTHER_PARTS_LEFT,	// 母艦パーツ左
		ENEMY_TYPE_MOTHER_PARTS_RIGHT,	// 母艦パーツ右
		ENEMY_TYPE_MOTHER_PARTS_CENTER,	// 母艦パーツ中央
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	// 敵の弾発射パターン
	typedef enum
	{
		BULLET_PATTERN_NORMAL = 0,	// 真下へ発射
		BULLET_PATTERN_AIM,			// プレイヤーを狙う
		BULLET_PATTERN_NONE,		// 弾を撃たない
		BULLET_PATTERN_MAX
	}BULLET_PATTERN;

	CEnemy();
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DamageLife(int nDamage,D3DXVECTOR3 pos);

	// 敵の生成
	static CEnemy *Create(D3DXVECTOR3 pos, 
		OBJTYPE objType, 
		float fSizeX, float fSizeY,
		CTexture::TEXTYPE type,
		D3DXVECTOR3 move,
		int nLife,
		int nBulletSetDelay,
		BULLET_PATTERN pattern,
		bool bUnder,
		ENEMY_TYPE enemyType);

	static void AutoSetCreate(void);			// ランダム個別配置
		
	ENEMY_TYPE GetEnemyType(void) { return m_type; }
		
	void SetState(ENEMYSTATE state) { m_state = state; }	// 状態変化設定
	void SetBulletDelay(int nBulletDelay) { m_nBulletSetDelay = nBulletDelay; }
	void EnemyState(void);

	// 弾の自動発射
	void AutoSetBullet(D3DXVECTOR3 pos, BULLET_PATTERN pattern);

	// 保存用変数の代入
	void SetBulletPattern(void) { m_pattern = m_patternSet; }

	static int GetNumEnemy(void) { return m_nNumEnemy; }		// 敵総数の取得
	static void UninitAll(void);								// 残存する敵の破棄

	void MoveMother(D3DXVECTOR3 pos);							// 母艦の挙動
	BULLET_PATTERN GetBulletPattern(void) { return m_pattern; }	// 弾の発射パターンの取得

private:
	static int m_nNumEnemy;				// 総数
	ENEMYSTATE m_state;					// 状態
	int m_nStateCnt;					// 状態変化のカウント
	int m_nBulletCnt;					// 弾の発射カウント
	int m_nBulletSetDelay;				// 弾の発射間隔
	static int m_nSetEnemyCnt;			// 敵の設定カウント

	bool m_bUnderSet;	// 画面下に出現するか
	ENEMY_TYPE m_type;					// 敵の種類

	static BULLET_PATTERN m_patternSet;	// 保存用
	BULLET_PATTERN m_pattern;			// 弾発射パターン
};

// 編隊生成クラス
class CEnemyFormation : public CEnemy
{
public:
	typedef enum
	{
		TEAM_PATTERN_RANDOM = 0,	// ランダム配置
		TEAM_PATTERN_SIDE,			// 両サイドへ出現
		TEAM_PATTERN_UNDER,			// 下を通る
		TEAM_PATTERN_MOTHER_0,		// 母艦からの配置
		TEAM_PATTERN_MOTHER_1,
		TEAM_PATTERN_MOTHER_2,
		TEAM_PATTERN_MOTHER_PARTS,	// 母艦パーツ左右から
		TEAM_PATTERN_MAX
	}TEAM_PATTERN;

	CEnemyFormation();
	~CEnemyFormation();

	static void SetFormationEnemy(		// 隊列編成配置処理	
		D3DXVECTOR3 pos,
		OBJTYPE objType,
		CTexture::TEXTYPE type,
		D3DXVECTOR3 moveRot,
		float fMove,
		int nSetNum,
		int nInterval,
		TEAM_PATTERN team);

	static void SetFormRandom(TEAM_PATTERN team);					// 隊列編成ランダム配置処理
	static void FormInterval(int nIntervalCnt);						// インターバルのカウント
	static bool GetIntervalBool(void) { return m_bFormSetInt; }

	static void SetInterval(int nInterval) { m_nInterval = nInterval; }

	// 一時中断の取得
	static bool GetLock(void) { return m_bLock; }

private:
	static int m_nFormNumInterval;		// 編成隊内の個別敵毎の生成カウント
	static int m_nFormSetInterval;		// 編隊生成のインターバル
	static bool m_bFormSetInt;			// インターバル判定

	static D3DXVECTOR3 m_pos;			// 配置する位置
	static D3DXVECTOR3 m_moveRot;		// 移動方向
	static float m_fMove;				// 移動力(基本1.0f)
	static CTexture::TEXTYPE m_type;	// 種類
	static int m_nInterval;				// 一体毎に生成する間隔
	static bool m_bSetParam;			// 生成パラメータ設定判定
	static bool m_bLock;				// 生成を一時中断
};

// 母艦生成クラス
class CEnemyMother : public CEnemy
{
public:
	// 母艦の出現状態
	typedef enum
	{
		MOTHER_STATE_NONE = 0,	// 母艦戦開始前の状態
		MOTHER_STATE_START,		// 出現開始演出
		MOTHER_STATE_ACTIVE,	// 活動、当たり判定有効化
		MOTHER_STATE_END,		// 母艦戦終了
		MOTHER_STATE_MAX
	}MOTHER_STATE;

	// 母艦戦闘のフェーズ
	typedef enum
	{
		MOTHER_PHASE_0 = 0,	// 第1フェーズ
		MOTHER_PHASE_1,		// 第2フェーズ
		MOTHER_PHASE_2,		// 第3フェーズ
		MOTHER_PHASE_3,		// 第4フェーズ
		MOTHER_PHASE_MAX
	}MOTHER_PHASE;

	// 母艦のパーツ
	typedef enum
	{
		MOTHER_PARTS_LEFT = 0,	// 左
		MOTHER_PARTS_RIGHT,		// 右
		MOTHER_PARTS_CENTER,	// 中央
		MOTHER_PARTS_MAX
	}MOTHER_PARTS;

	CEnemyMother();
	~CEnemyMother();

	static void MotherSetCreate(void);		// 母艦の生成

	// 状態の取得・設定
	static MOTHER_STATE GetMotherState(void) { return m_motherState; }
	static void SetMotherState(MOTHER_STATE state) { m_motherState = state; }

	// 母艦戦のフェーズ
	static MOTHER_PHASE GetPhase(void) { return m_phase; }
	static void SetPhase(MOTHER_PHASE phase) { m_phase = phase; }

	// 最大体力
	static int GetLife(void) { return m_nMotherStartLife; }
	static void SetLife(int nLife) { m_nMotherStartLife = nLife; }

	static void MoveParts(void);

private:
	static MOTHER_STATE m_motherState;				// 母艦の状態
	static MOTHER_PHASE m_phase;					// 母艦戦フェーズ
	static int m_nMotherStartLife;					// 開始時の体力
};

#endif