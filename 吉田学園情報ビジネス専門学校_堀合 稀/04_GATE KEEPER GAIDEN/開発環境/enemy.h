//=============================================================================
//
// �G�`�揈�� [enemy.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "character.h"

// �}�N����`
#define ENEMYX_0		(20.0f)		// �G0�̑傫��X
#define ENEMYY_0		(20.0f)		// �G0�̑傫��Y

#define ENEMYX_1		(22.5f)		// �G1�̑傫��X
#define ENEMYY_1		(22.5f)		// �G1�̑傫��Y

#define ENEMYX_2		(27.5f)		// �G2�̑傫��X
#define ENEMYY_2		(27.5f)		// �G2�̑傫��Y

#define ENEMYX_3		(30.0f)		// �G3�̑傫��X
#define ENEMYY_3		(30.0f)		// �G3�̑傫��Y

#define MOTHER_ENEMY_X	(120.0f)	// ��͂̑傫��X
#define MOTHER_ENEMY_Y	(80.0f)		// ��͂̑傫��Y
#define MOTHER_SET_POS	(D3DXVECTOR3(640.0f, -60.0f, 0.0f))	// ��͂̔z�u����ʒu 
#define MOTHER_POS_Y	(120.0f)	// ��͂���~���鍂��

#define MOTHER_MOVE_0	(2.0f)		// �t�F�[�Y���Ƃ̈ړ���
#define MOTHER_MOVE_1	(3.0f)		// �t�F�[�Y���Ƃ̈ړ���
#define MOTHER_MOVE_2	(3.5f)		// �t�F�[�Y���Ƃ̈ړ���
#define MOTHER_MOVE_3	(5.0f)		// �t�F�[�Y���Ƃ̈ړ���

#define ENEMY_SET_POS_Y			(-50.0f)	// �G�̎����������鍂��
#define ENEMY_SET_POS_Y_UNDER	(530.0f)	// ��ʉ����ɏo������ꍇ
#define ENEMY_SET_POS_X_UNDER_L	(-50.0f)	// ��ʉ����ɏo������ꍇ�̍�
#define ENEMY_SET_POS_X_UNDER_R	(1330.0f)	// ��ʉ����ɏo������ꍇ�̉E

#define MOTHER_PARTS_SIZE	(30.0f)			// �p�[�c�̃T�C�Y
#define BULLET_AIM_SPEED		(4.0f)		// �Ə��e�̑��x
#define MOTHER_PARTS_POS_0	(D3DXVECTOR3(-120.0f,0.0f,0.0f))	// �p�[�c�̈ʒu
#define MOTHER_PARTS_POS_1	(D3DXVECTOR3(120.0f,0.0f,0.0f))		// �p�[�c�̈ʒu
#define MOTHER_PARTS_POS_2	(D3DXVECTOR3(0.0f,80.0f,0.0f))		// �p�[�c�̈ʒu

// �O���錾
class CChara3D;	// 3D�L�����N�^�[�|�C���^

// �G�N���X
class CEnemy : public CCharacter
{
public:
	// �G�̃X�e�[�^�X
	typedef enum
	{
		ENEMYSTATE_NORMAL = 0,	// �ʏ�
		ENEMYSTATE_DAMAGE,		// ��e
		ENEMYSTATE_DEATH,		// ���Ă��ꂽ
		ENEMYSTATE_MAX
	}ENEMYSTATE;

	// �G�̎��
	typedef enum
	{
		ENEMY_TYPE_0 = 0,				// �ʏ�G0
		ENEMY_TYPE_1,					// �ʏ�G1
		ENEMY_TYPE_2,					// �ʏ�G2
		ENEMY_TYPE_3,					// �ʏ�G3
		ENEMY_TYPE_MOTHER,				// ���
		ENEMY_TYPE_MOTHER_PARTS_LEFT,	// ��̓p�[�c��
		ENEMY_TYPE_MOTHER_PARTS_RIGHT,	// ��̓p�[�c�E
		ENEMY_TYPE_MOTHER_PARTS_CENTER,	// ��̓p�[�c����
		ENEMY_TYPE_MAX
	}ENEMY_TYPE;

	// �G�̒e���˃p�^�[��
	typedef enum
	{
		BULLET_PATTERN_NORMAL = 0,	// �^���֔���
		BULLET_PATTERN_AIM,			// �v���C���[��_��
		BULLET_PATTERN_NONE,		// �e�������Ȃ�
		BULLET_PATTERN_MAX
	}BULLET_PATTERN;

	CEnemy();
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DamageLife(int nDamage,D3DXVECTOR3 pos);

	// �G�̐���
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

	static void AutoSetCreate(void);			// �����_���ʔz�u
		
	ENEMY_TYPE GetEnemyType(void) { return m_type; }
		
	void SetState(ENEMYSTATE state) { m_state = state; }	// ��ԕω��ݒ�
	void SetBulletDelay(int nBulletDelay) { m_nBulletSetDelay = nBulletDelay; }
	void EnemyState(void);

	// �e�̎�������
	void AutoSetBullet(D3DXVECTOR3 pos, BULLET_PATTERN pattern);

	// �ۑ��p�ϐ��̑��
	void SetBulletPattern(void) { m_pattern = m_patternSet; }

	static int GetNumEnemy(void) { return m_nNumEnemy; }		// �G�����̎擾

	static void UninitAll(void);								// �c������G�̔j��

	void MoveMother(D3DXVECTOR3 pos);							// ��͂̋���
	BULLET_PATTERN GetBulletPattern(void) { return m_pattern; }	// �e�̔��˃p�^�[���̎擾

private:
	static int m_nNumEnemy;				// �G�̑���

	ENEMYSTATE m_state;					// ���
	int m_nStateCnt;					// ��ԕω��̃J�E���g

	int m_nBulletCnt;					// �e�̔��˃J�E���g
	int m_nBulletSetDelay;				// �e�̔��ˊԊu
	static int m_nSetEnemyCnt;			// �G�̐ݒ�J�E���g

	bool m_bUnderSet;					// ��ʉ��ɏo�����邩
	ENEMY_TYPE m_type;					// �G�̎��

	static BULLET_PATTERN m_patternSet;	// �ۑ��p
	BULLET_PATTERN m_pattern;			// �e���˃p�^�[��
};
#endif