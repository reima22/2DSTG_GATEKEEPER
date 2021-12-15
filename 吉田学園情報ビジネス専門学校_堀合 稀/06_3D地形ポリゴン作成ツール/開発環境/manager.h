//=============================================================================
//
// �}�l�[�W�� [manager.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _MANAGER_H_
#define _MANAGER_H_

//=============================================================================
// �O���錾
//=============================================================================
class CRenderer;		// �����_���[�N���X
class CInputKeyboard;	// �L�[�{�[�h�N���X
class CGamepad;			// �Q�[���p�b�h�N���X
class CTitle;			// �^�C�g���N���X
class CTutorial;		// �`���[�g���A���N���X
class CGame;			// �Q�[���N���X
class CResult;			// ���U���g�N���X
class CRanking;			// �����L���O�N���X		
class CCamera;			// �J�����N���X
class CLight;			// ���C�g�N���X
class CTextData;		// �e�L�X�g�f�[�^�N���X
class CTexture;			// �e�N�X�`���N���X
class CCollision;		// �����蔻��N���X
class CExe;				// ���s�N���X
class CInputMouse;		// �}�E�X

//=============================================================================
// �}�l�[�W���N���X
//=============================================================================
class CManager
{
public:
	// �Q�[�����[�h�̗�
	typedef enum
	{
		MODE_TITLE = 0,	// �^�C�g��
		MODE_TUTORIAL,	// �`���[�g���A��
		MODE_GAME,		// �Q�[��
		MODE_RESULT,	// ���U���g
		MODE_RANKING,	// �����L���O
		MODE_MAX
	}MODE;

	CManager();
	 ~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �N���X�̎擾
	static CRenderer *GetRenderer(void) { return m_pRenderer; };
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CGamepad *GetInputGamepad(void) { return m_pGamepad; }

	// ���[�h�擾�E�ݒ�
	//static MODE GetMode(void) { return m_mode; }
	//static void SetMode(MODE mode);

	//// ���|�C���^�̎擾
	//static CSound *GetSound(void) { return m_pSound; }

	//// BGM�̍Đ��ݒ�
	//static bool GetBGMPlay(CSound::BGMTYPE type) { return m_bBGM[type]; }

	// �J�����|�C���^�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; }
	
	// �e�N�X�`���|�C���^�̎擾
	static CTexture *GetTexture(void) { return m_pTexture; }

	// �����蔻��̎擾
	static CCollision *GetCollision(void) { return m_pCollision; }

	// ���s���[�h�̎擾
	static CExe *GetExe(void) { return m_pExe; }
	
	// �}�E�X�̎擾
	static CInputMouse *GetInputMouse(void) { return m_pInputMouse; }

private:
	static CRenderer* m_pRenderer;				// �����_�����O�N���X
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�N���X
	static CGamepad *m_pGamepad;				// �Q�[���p�b�h�N���X

	static CCamera *m_pCamera;					// �J�����̃|�C���^
	static CLight *m_pLight;					// ���C�g�̃|�C���^

	static CTextData *m_pTextData;				// �e�L�X�g�f�[�^�N���X�̃|�C���^
	static CTexture *m_pTexture;				// �e�N�X�`���N���X�̃|�C���^
	static CCollision *m_pCollision;			// �����蔻��N���X�̃|�C���^
	static CExe *m_pExe;						// ���s�N���X
	static CInputMouse *m_pInputMouse;			// �}�E�X����
};

#endif