//==============================================================================
//
// テキストデータ（敵）管理〔textdata_enemy.cpp〕
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_enemy.h"
#include "stdio.h"

// 静的メンバ変数宣言
int CTextDataEnemy::m_aEnemyData[ENEMYDATA_MAX];
float CTextDataEnemy::m_fSpeedData[CTitleDifficulty::DIFFICULTY_MAX];

//==============================================================================
// コンストラクタ
//==============================================================================
CTextDataEnemy::CTextDataEnemy()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTextDataEnemy::~CTextDataEnemy()
{

}

//==============================================================================
// テキストの読み込み
//==============================================================================
void CTextDataEnemy::LoadData(void)
{
	// ローカル変数宣言
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// 変数の初期化
	for (int nCnt = 0; nCnt < ENEMYDATA_MAX; nCnt++)
	{
		m_aEnemyData[nCnt] = 0;
	}

	// 速度データ
	for (int nCnt = 0; nCnt < CTitleDifficulty::DIFFICULTY_MAX; nCnt++)
	{
		m_fSpeedData[nCnt] = 0.0f;
	}

	// データの読み込み
	pFile = fopen(m_pFileName[TEXTDATA_ENEMY], "r");

	if (pFile != NULL)
	{ //ファイル展開可能

		while (strcmp("ENEMYDATA_END", pLoadText) != 0)
		{
			// 項目読み込み
			fscanf(pFile, "%s", pLoadText);

			// 項目に対応した変数への格納
			if (strcmp("SPEED_EASY", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_EASY]);
			}
			else if (strcmp("SPEED_NORMAL", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_NORMAL]);
			}
			else if (strcmp("SPEED_HARD", pLoadText) == 0)
			{// 移動速度
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_HARD]);
			}
			else if (strcmp("CREATE_MIN_EASY", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_EASY]);
			}
			else if (strcmp("CREATE_MAX_EASY", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_EASY]);
			}
			else if (strcmp("CREATE_MIN_NORMAL", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_NORMAL]);
			}
			else if (strcmp("CREATE_MAX_NORMAL", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_NORMAL]);
			}
			else if (strcmp("CREATE_MIN_HARD", pLoadText) == 0)
			{// 敵生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_HARD]);
			}
			else if (strcmp("CREATE_MAX_HARD", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_HARD]);
			}
			else if (strcmp("CREATE_BULLET_MIN", pLoadText) == 0)
			{// 弾生成カウント基準最小値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MIN]);
			}
			else if (strcmp("CREATE_BULLET_MAX", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MAX]);
			}
			else if (strcmp("CREATE_BULLET_MOTHER_MIN", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MIN]);
			}
			else if (strcmp("CREATE_BULLET_MOTHER_MAX", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MAX]);
			}
			else if (strcmp("MOTHER_LIFE_EASY", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_EASY]);
			}
			else if (strcmp("MOTHER_LIFE_NORMAL", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_NORMAL]);
			}
			else if (strcmp("MOTHER_LIFE_HARD", pLoadText) == 0)
			{// 敵生成カウント基準最大値
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_HARD]);
			}
		}

		fclose(pFile);
	}
	else
	{ // ファイル展開不可
	}
}

//==============================================================================
// カウントの設定
//==============================================================================
void CTextDataEnemy::CreateCntSet(void)
{
	// 敵生成カウントの差、難易度ごと
	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_EASY]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_EASY] - m_aEnemyData[ENEMYDATA_CREATE_MIN_EASY] + 1;

	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_NORMAL]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_NORMAL] - m_aEnemyData[ENEMYDATA_CREATE_MIN_NORMAL] + 1;

	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_HARD]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_HARD] - m_aEnemyData[ENEMYDATA_CREATE_MIN_HARD] + 1;

	// 弾生成カウントの差
	m_aEnemyData[ENEMYDATA_CREATE_BULLET_DIFFERENCE]
		= m_aEnemyData[ENEMYDATA_CREATE_BULLET_MAX] - m_aEnemyData[ENEMYDATA_CREATE_BULLET_MIN] + 1;

	// 母艦戦時弾生成カウントの差
	m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE]
		= m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MAX] - m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MIN] + 1;
}