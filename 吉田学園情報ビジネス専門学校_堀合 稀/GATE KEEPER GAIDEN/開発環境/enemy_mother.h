//=============================================================================
//
// 敵母艦処理 [enemy_mother.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_MOTHER_H_
#define _ENEMY_MOTHER_H_

#include "main.h"
#include "character.h"
#include "enemy.h"

//=============================================================================
// 母艦生成クラス
//=============================================================================
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

	static CChara3D *m_pMother3D;						// 本体3D
	static CChara3D *m_pSubParts[MOTHER_PARTS_MAX];		// パーツ3D
};
#endif
