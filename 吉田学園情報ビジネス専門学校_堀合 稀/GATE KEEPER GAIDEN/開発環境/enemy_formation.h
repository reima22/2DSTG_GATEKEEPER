//=============================================================================
//
// �G�ґ��������� [enemy_formation.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_FORMATION_H_
#define _ENEMY_FORMATION_H_

#include "main.h"
#include "character.h"
#include "enemy.h"

//=============================================================================
// �ґ������N���X
//=============================================================================
class CEnemyFormation : public CEnemy
{
public:
	// �Ґ��p�^�[��
	typedef enum
	{
		TEAM_PATTERN_RANDOM = 0,	// �����_���z�u
		TEAM_PATTERN_SIDE,			// ���T�C�h�ɏo��
		TEAM_PATTERN_UNDER,			// ����ʂ�
		TEAM_PATTERN_MOTHER_0,		// ��͂���̔z�u
		TEAM_PATTERN_MOTHER_1,
		TEAM_PATTERN_MOTHER_2,
		TEAM_PATTERN_MOTHER_PARTS,	// ��̓p�[�c���E����
		TEAM_PATTERN_MAX
	}TEAM_PATTERN;

	CEnemyFormation();
	~CEnemyFormation();

	static void SetFormationEnemy(		// ����Ґ��z�u����	
		D3DXVECTOR3 pos,
		OBJTYPE objType,
		CTexture::TEXTYPE type,
		D3DXVECTOR3 moveRot,
		float fMove,
		int nSetNum,
		int nInterval,
		TEAM_PATTERN team);

	static void SetFormRandom(TEAM_PATTERN team);					// ����Ґ������_���z�u����
	static void FormInterval(int nIntervalCnt);						// �C���^�[�o���̃J�E���g
	static bool GetIntervalBool(void) { return m_bFormSetInt; }

	static void SetInterval(int nInterval) { m_nInterval = nInterval; }

	// �ꎞ���f�̎擾
	static bool GetLock(void) { return m_bLock; }

	static void SetParamReset(void) { m_bSetParam = false; }

private:
	static int m_nFormNumInterval;		// �Ґ������̌ʓG���̐����J�E���g
	static int m_nFormSetInterval;		// �ґ������̃C���^�[�o��
	static bool m_bFormSetInt;			// �C���^�[�o������

	static D3DXVECTOR3 m_pos;			// �z�u����ʒu
	static D3DXVECTOR3 m_moveRot;		// �ړ�����
	static float m_fMove;				// �ړ���(��{1.0f)
	static CTexture::TEXTYPE m_type;	// ���
	static int m_nInterval;				// ��̖��ɐ�������Ԋu
	static bool m_bSetParam;			// �����p�����[�^�ݒ蔻��
	static bool m_bLock;				// �������ꎞ���f
};

#endif
