//=============================================================================
//
// �v���C���[�`�揈�� [player.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "character.h"
#include "textdata_player.h"

// �}�N����`
#define PLAYER_POS			(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250.0f, 0.0f))	// �v���C���[�J�n�ʒu
#define PLAYERX				(20.0f)																// �v���C���[�̑傫��X
#define PLAYERY				(20.0f)																// �v���C���[�̑傫��Y
#define SPEED_UP			(3.0f)																// �����x
#define SPEED_DOWN			(0.2f)																// �����W��
#define MISSILE_INTERVAL	(30)																// �~�T�C�����ˊԊu

// �v���C���[�N���X
class CPlayer : public CCharacter
{
public:
	// �v���C�^�[�̃X�e�[�^�X
	typedef enum
	{
		PLAYERSTATE_APPEAR = 0,	// �o��
		PLAYERSTATE_NORMAL,		// �ʏ�
		PLAYERSTATE_DAMAGE,		// �_���[�W
		PLAYERSTATE_WAIT,		// �o���҂�
		PLAYERSTATE_DEATH,		// ���S
		PLAYERSTATE_GAMEOVER,	// �Q�[���I�[�o�[
		PLAYERSTATE_MAX
	}PLAYERSTATE;

	CPlayer();
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DamageLife(int nDamage,D3DXVECTOR3 pos);

	static CPlayer *Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY);

	void PlayerState(void);
	static void TransStock(int nTrans) { m_nStock += nTrans; }
	void PlayerMove(D3DXVECTOR3 pos);

	static int GetStcok(void) { return m_nStock; }
	static PLAYERSTATE GetState(void) { return m_state; }

	void MissileInterval(void);

private:
	int m_nBulletPress;				// �e�̃v���X���˃J�E���g
	D3DXVECTOR3 m_Move;				// �ړ���
	int m_nStateCnt;				// �X�e�[�^�X�̕ω��J�E���g
	static int m_nStock;			// �c�@��
	static PLAYERSTATE m_state;		// �v���C���[�̏��
	float m_fSpeed;					// �X�s�[�h
	static bool m_bMissileOff;		// �~�T�C���̃N�[���_�E������
	static int m_nMissileCnt;		// �~�T�C���̃C���^�[�o��
};

#endif