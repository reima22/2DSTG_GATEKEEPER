
//=============================================================================
//
// �Q�[�����[�h���� [game.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define LIFE_POS		(D3DXVECTOR3(40.0f, 40.0f, 0.0f))		// ���C�t�z�u�J�n�ʒu
#define SCORE_POS		(D3DXVECTOR3(1100.0f, 690.0f, 0.0f))	// �X�R�A�z�u�J�n�ʒu
#define DEFEAT_POS		(D3DXVECTOR3(730.0f, 690.0f, 0.0f))		// ���j���̔z�u�ʒu
#define INVASION_POS	(D3DXVECTOR3(870.0f, 690.0f, 0.0f))		// �N�����̔z�u�ʒu
#define STOCK_POS		(D3DXVECTOR3(600.0f, 680.0f, 0.0f))		// �c�@���̔z�u�ʒu
#define RESULT_GAMEEND	(D3DXVECTOR3(640.0f, 200.0f, 0.0f))		// �Q�[�����ʂ̕`��

// �O���錾
class CCharacter;
class CSceneX;
class CMeshfield;

//==============================================================================
// �Q�[�����[�h�N���X
//==============================================================================
class CGame : public CScene
{
public:
	CGame();
	~CGame();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

	static CGame *Create(void);
	static CCharacter *GetChara(void) { return m_pCharaPlayer; }
	static int GetEndInvasion(void) { return m_nEndInvasion; }
	static bool GetStartGame(void) { return m_bGameStart; }

	void SetEnemyPattern(int nType);
	static bool GetSetEnemy(void) { return m_bSetEnemy; }		// �G�̔z�u����
	static bool GetMotherEnemy(void) { return m_bMotherEnemy; }	// ��͐�ڍs��

	void Ending(void);						// �Q�[���I�����o
	void CntDownEnd(void);					// �Q�[���I���܂ł̃J�E���g�_�E��

	static CMeshfield *GetMeshfield(void) { return m_pMeshfield; }

private:
	static int m_nClearDefeat;				// �Q�[���N���A�̌��j��
	static int m_nEndInvasion;				// �Q�[���I������N����
	static CCharacter *m_pCharaPlayer;		// �v���C���[�L�����̃|�C���^
	static bool m_bGameStart;				// �Q�[���̊J�n����

	static int m_nPhase;					// �o�߂����t�F�[�Y��
	static int m_nSetType;					// �z�u���@�̎��
	static bool m_bSetEnemy;				// �G�z�u���@�̐؂�ւ�����
	bool m_bSetSE;							// SE����
	bool m_bSetMother;						// ��͐�BGM�̍Đ�����
	static bool m_bMotherEnemy;				// ��̓t�F�[�Y

	static int m_nCntDownEnd;			// �Q�[���I���܂ł̃J�E���g
	bool m_bSetLogo;

	static CMeshfield *m_pMeshfield;

	//static CSceneX *m_pSceneX;			// ���f��X�̃|�C���^
};

#endif