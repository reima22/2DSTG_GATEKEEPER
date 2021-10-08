//=============================================================================
//
// テキストデータ（敵）管理 [textdata_enemy.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_ENEMY_H_
#define _TEXTDATA_ENEMY_H_

#include "main.h"
#include "textdata.h"
#include "title_difficulty.h"

//==============================================================================
// テキスト管理（敵）クラス
//==============================================================================
class CTextDataEnemy : public CTextData
{
public:
	// エネミーデータの種類
	typedef enum
	{
		ENEMYDATA_CREATE_MIN_EASY = 0,				// 敵自動生成カウント基準最小値イージー
		ENEMYDATA_CREATE_MAX_EASY,					// 敵自動生成カウント基準最大値イージー
		ENEMYDATA_CREATE_DIFFERENCE_EASY,			// 敵自動生成カウントの差イージー
		ENEMYDATA_CREATE_MIN_NORMAL,				// 敵自動生成カウント基準最小値ノーマル
		ENEMYDATA_CREATE_MAX_NORMAL,				// 敵自動生成カウント基準最大値ノーマル
		ENEMYDATA_CREATE_DIFFERENCE_NORMAL,			// 敵自動生成カウントの差ノーマル
		ENEMYDATA_CREATE_MIN_HARD,					// 敵自動生成カウント基準最小値ハード
		ENEMYDATA_CREATE_MAX_HARD,					// 敵自動生成カウント基準最大値ハード
		ENEMYDATA_CREATE_DIFFERENCE_HARD,			// 敵自動生成カウントの差ハード

		ENEMYDATA_CREATE_BULLET_MIN,				// 弾自動生成カウント基準最小値
		ENEMYDATA_CREATE_BULLET_MAX,				// 弾自動生成カウント基準最大値
		ENEMYDATA_CREATE_BULLET_DIFFERENCE,			// 弾自動生成カウントの差
		ENEMYDATA_CREATE_BULLET_MOTHER_MIN,			// 母艦戦時、弾自動生成カウント最小値
		ENEMYDATA_CREATE_BULLET_MOTHER_MAX,			// 母艦戦時、弾自動生成カウント最大値
		ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE,	// 母艦戦時、弾自動生成カウントの差

		ENEMYDATA_MOTHER_LIFE_EASY,					// 母艦の体力イージー
		ENEMYDATA_MOTHER_LIFE_NORMAL,				// 母艦の体力ノーマル
		ENEMYDATA_MOTHER_LIFE_HARD,					// 母艦の体力ハード
		ENEMYDATA_MAX
	}ENEMYDATA;

	CTextDataEnemy();
	~CTextDataEnemy();

	static void LoadData(void);

	static void CreateCntSet(void);

	// データの取得
	static int SetData(int nDataNum) { return m_aEnemyData[nDataNum]; }

	// 速度データの取得
	static float SetSpeed(CTitleDifficulty::DIFFICULTY difficulty) { return m_fSpeedData[difficulty]; }

private:
	static int m_aEnemyData[ENEMYDATA_MAX];
	static float m_fSpeedData[CTitleDifficulty::DIFFICULTY_MAX];
};

#endif
