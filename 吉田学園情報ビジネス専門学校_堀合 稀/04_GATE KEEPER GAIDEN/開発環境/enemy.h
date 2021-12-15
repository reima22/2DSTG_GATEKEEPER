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
#define MOTHER_SET_POS	(D3DXVECTOR3(640.0f, -60.0f, 0.0f))	// 母艦の配置する位置 
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

// 前方宣言
class CChara3D;	// 3Dキャラクターポインタ

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
	static int m_nNumEnemy;				// 敵の総数

	ENEMYSTATE m_state;					// 状態
	int m_nStateCnt;					// 状態変化のカウント

	int m_nBulletCnt;					// 弾の発射カウント
	int m_nBulletSetDelay;				// 弾の発射間隔
	static int m_nSetEnemyCnt;			// 敵の設定カウント

	bool m_bUnderSet;					// 画面下に出現するか
	ENEMY_TYPE m_type;					// 敵の種類

	static BULLET_PATTERN m_patternSet;	// 保存用
	BULLET_PATTERN m_pattern;			// 弾発射パターン
};
#endif