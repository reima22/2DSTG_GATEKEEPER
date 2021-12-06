//=============================================================================
//
// �G��͏��� [enemy_mother.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_MOTHER_H_
#define _ENEMY_MOTHER_H_

#include "main.h"
#include "character.h"
#include "enemy.h"

//=============================================================================
// ��͐����N���X
//=============================================================================
class CEnemyMother : public CEnemy
{
public:
	// ��͂̏o�����
	typedef enum
	{
		MOTHER_STATE_NONE = 0,	// ��͐�J�n�O�̏��
		MOTHER_STATE_START,		// �o���J�n���o
		MOTHER_STATE_ACTIVE,	// �����A�����蔻��L����
		MOTHER_STATE_END,		// ��͐�I��
		MOTHER_STATE_MAX
	}MOTHER_STATE;

	// ��͐퓬�̃t�F�[�Y
	typedef enum
	{
		MOTHER_PHASE_0 = 0,	// ��1�t�F�[�Y
		MOTHER_PHASE_1,		// ��2�t�F�[�Y
		MOTHER_PHASE_2,		// ��3�t�F�[�Y
		MOTHER_PHASE_3,		// ��4�t�F�[�Y
		MOTHER_PHASE_MAX
	}MOTHER_PHASE;

	// ��͂̃p�[�c
	typedef enum
	{
		MOTHER_PARTS_LEFT = 0,	// ��
		MOTHER_PARTS_RIGHT,		// �E
		MOTHER_PARTS_CENTER,	// ����
		MOTHER_PARTS_MAX
	}MOTHER_PARTS;

	CEnemyMother();
	~CEnemyMother();

	static void MotherSetCreate(void);		// ��͂̐���

	// ��Ԃ̎擾�E�ݒ�
	static MOTHER_STATE GetMotherState(void) { return m_motherState; }
	static void SetMotherState(MOTHER_STATE state) { m_motherState = state; }

	// ��͐�̃t�F�[�Y
	static MOTHER_PHASE GetPhase(void) { return m_phase; }
	static void SetPhase(MOTHER_PHASE phase) { m_phase = phase; }

	// �ő�̗�
	static int GetLife(void) { return m_nMotherStartLife; }
	static void SetLife(int nLife) { m_nMotherStartLife = nLife; }

	static void MoveParts(void);

private:
	static MOTHER_STATE m_motherState;				// ��͂̏��
	static MOTHER_PHASE m_phase;					// ��͐�t�F�[�Y
	static int m_nMotherStartLife;					// �J�n���̗̑�

	static CChara3D *m_pMother3D;						// �{��3D
	static CChara3D *m_pSubParts[MOTHER_PARTS_MAX];		// �p�[�c3D
};
#endif
