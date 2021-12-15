//==============================================================================
//
// �e�L�X�g�f�[�^�i�G�j�Ǘ��ktextdata_enemy.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "textdata.h"
#include "textdata_enemy.h"
#include "stdio.h"

// �ÓI�����o�ϐ��錾
int CTextDataEnemy::m_aEnemyData[ENEMYDATA_MAX];
float CTextDataEnemy::m_fSpeedData[CTitleDifficulty::DIFFICULTY_MAX];

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTextDataEnemy::CTextDataEnemy()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTextDataEnemy::~CTextDataEnemy()
{

}

//==============================================================================
// �e�L�X�g�̓ǂݍ���
//==============================================================================
void CTextDataEnemy::LoadData(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;
	char aLoadText[TEXT_NUM];
	char *pLoadText = &aLoadText[0];

	// �ϐ��̏�����
	for (int nCnt = 0; nCnt < ENEMYDATA_MAX; nCnt++)
	{
		m_aEnemyData[nCnt] = 0;
	}

	// ���x�f�[�^
	for (int nCnt = 0; nCnt < CTitleDifficulty::DIFFICULTY_MAX; nCnt++)
	{
		m_fSpeedData[nCnt] = 0.0f;
	}

	// �f�[�^�̓ǂݍ���
	pFile = fopen(m_pFileName[TEXTDATA_ENEMY], "r");

	if (pFile != NULL)
	{ //�t�@�C���W�J�\

		while (strcmp("ENEMYDATA_END", pLoadText) != 0)
		{
			// ���ړǂݍ���
			fscanf(pFile, "%s", pLoadText);

			// ���ڂɑΉ������ϐ��ւ̊i�[
			if (strcmp("SPEED_EASY", pLoadText) == 0)
			{// �ړ����x
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_EASY]);
			}
			else if (strcmp("SPEED_NORMAL", pLoadText) == 0)
			{// �ړ����x
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_NORMAL]);
			}
			else if (strcmp("SPEED_HARD", pLoadText) == 0)
			{// �ړ����x
				fscanf(pFile, "%s %f", &aLoadText[0], &m_fSpeedData[CTitleDifficulty::DIFFICULTY_HARD]);
			}
			else if (strcmp("CREATE_MIN_EASY", pLoadText) == 0)
			{// �G�����J�E���g��ŏ��l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_EASY]);
			}
			else if (strcmp("CREATE_MAX_EASY", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_EASY]);
			}
			else if (strcmp("CREATE_MIN_NORMAL", pLoadText) == 0)
			{// �G�����J�E���g��ŏ��l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_NORMAL]);
			}
			else if (strcmp("CREATE_MAX_NORMAL", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_NORMAL]);
			}
			else if (strcmp("CREATE_MIN_HARD", pLoadText) == 0)
			{// �G�����J�E���g��ŏ��l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MIN_HARD]);
			}
			else if (strcmp("CREATE_MAX_HARD", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_MAX_HARD]);
			}
			else if (strcmp("CREATE_BULLET_MIN", pLoadText) == 0)
			{// �e�����J�E���g��ŏ��l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MIN]);
			}
			else if (strcmp("CREATE_BULLET_MAX", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MAX]);
			}
			else if (strcmp("CREATE_BULLET_MOTHER_MIN", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MIN]);
			}
			else if (strcmp("CREATE_BULLET_MOTHER_MAX", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MAX]);
			}
			else if (strcmp("MOTHER_LIFE_EASY", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_EASY]);
			}
			else if (strcmp("MOTHER_LIFE_NORMAL", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_NORMAL]);
			}
			else if (strcmp("MOTHER_LIFE_HARD", pLoadText) == 0)
			{// �G�����J�E���g��ő�l
				fscanf(pFile, "%s %d", &aLoadText[0], &m_aEnemyData[ENEMYDATA_MOTHER_LIFE_HARD]);
			}
		}

		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}

//==============================================================================
// �J�E���g�̐ݒ�
//==============================================================================
void CTextDataEnemy::CreateCntSet(void)
{
	// �G�����J�E���g�̍��A��Փx����
	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_EASY]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_EASY] - m_aEnemyData[ENEMYDATA_CREATE_MIN_EASY] + 1;

	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_NORMAL]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_NORMAL] - m_aEnemyData[ENEMYDATA_CREATE_MIN_NORMAL] + 1;

	m_aEnemyData[ENEMYDATA_CREATE_DIFFERENCE_HARD]
		= m_aEnemyData[ENEMYDATA_CREATE_MAX_HARD] - m_aEnemyData[ENEMYDATA_CREATE_MIN_HARD] + 1;

	// �e�����J�E���g�̍�
	m_aEnemyData[ENEMYDATA_CREATE_BULLET_DIFFERENCE]
		= m_aEnemyData[ENEMYDATA_CREATE_BULLET_MAX] - m_aEnemyData[ENEMYDATA_CREATE_BULLET_MIN] + 1;

	// ��͐펞�e�����J�E���g�̍�
	m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE]
		= m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MAX] - m_aEnemyData[ENEMYDATA_CREATE_BULLET_MOTHER_MIN] + 1;
}