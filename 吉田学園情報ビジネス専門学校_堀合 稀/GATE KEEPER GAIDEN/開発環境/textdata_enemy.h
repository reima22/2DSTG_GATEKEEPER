//=============================================================================
//
// �e�L�X�g�f�[�^�i�G�j�Ǘ� [textdata_enemy.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TEXTDATA_ENEMY_H_
#define _TEXTDATA_ENEMY_H_

#include "main.h"
#include "textdata.h"
#include "title_difficulty.h"

//==============================================================================
// �e�L�X�g�Ǘ��i�G�j�N���X
//==============================================================================
class CTextDataEnemy : public CTextData
{
public:
	// �G�l�~�[�f�[�^�̎��
	typedef enum
	{
		ENEMYDATA_CREATE_MIN_EASY = 0,				// �G���������J�E���g��ŏ��l�C�[�W�[
		ENEMYDATA_CREATE_MAX_EASY,					// �G���������J�E���g��ő�l�C�[�W�[
		ENEMYDATA_CREATE_DIFFERENCE_EASY,			// �G���������J�E���g�̍��C�[�W�[
		ENEMYDATA_CREATE_MIN_NORMAL,				// �G���������J�E���g��ŏ��l�m�[�}��
		ENEMYDATA_CREATE_MAX_NORMAL,				// �G���������J�E���g��ő�l�m�[�}��
		ENEMYDATA_CREATE_DIFFERENCE_NORMAL,			// �G���������J�E���g�̍��m�[�}��
		ENEMYDATA_CREATE_MIN_HARD,					// �G���������J�E���g��ŏ��l�n�[�h
		ENEMYDATA_CREATE_MAX_HARD,					// �G���������J�E���g��ő�l�n�[�h
		ENEMYDATA_CREATE_DIFFERENCE_HARD,			// �G���������J�E���g�̍��n�[�h

		ENEMYDATA_CREATE_BULLET_MIN,				// �e���������J�E���g��ŏ��l
		ENEMYDATA_CREATE_BULLET_MAX,				// �e���������J�E���g��ő�l
		ENEMYDATA_CREATE_BULLET_DIFFERENCE,			// �e���������J�E���g�̍�
		ENEMYDATA_CREATE_BULLET_MOTHER_MIN,			// ��͐펞�A�e���������J�E���g�ŏ��l
		ENEMYDATA_CREATE_BULLET_MOTHER_MAX,			// ��͐펞�A�e���������J�E���g�ő�l
		ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE,	// ��͐펞�A�e���������J�E���g�̍�

		ENEMYDATA_MOTHER_LIFE_EASY,					// ��̗͂̑̓C�[�W�[
		ENEMYDATA_MOTHER_LIFE_NORMAL,				// ��̗͂̑̓m�[�}��
		ENEMYDATA_MOTHER_LIFE_HARD,					// ��̗͂̑̓n�[�h
		ENEMYDATA_MAX
	}ENEMYDATA;

	CTextDataEnemy();
	~CTextDataEnemy();

	static void LoadData(void);

	static void CreateCntSet(void);

	// �f�[�^�̎擾
	static int SetData(int nDataNum) { return m_aEnemyData[nDataNum]; }

	// ���x�f�[�^�̎擾
	static float SetSpeed(CTitleDifficulty::DIFFICULTY difficulty) { return m_fSpeedData[difficulty]; }

private:
	static int m_aEnemyData[ENEMYDATA_MAX];
	static float m_fSpeedData[CTitleDifficulty::DIFFICULTY_MAX];
};

#endif
