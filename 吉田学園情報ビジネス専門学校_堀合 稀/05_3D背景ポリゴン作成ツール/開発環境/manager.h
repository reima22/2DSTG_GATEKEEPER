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
class CCamera;			// �J�����N���X
class CLight;			// ���C�g�N���X
class CTextData;		// �e�L�X�g�f�[�^�N���X
class CTexture;			// �e�N�X�`���N���X
class CExe;				// ���s�N���X

//=============================================================================
// �}�l�[�W���N���X
//=============================================================================
class CManager
{
public:
	CManager();
	 ~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �N���X�|�C���^�̎擾
	static CRenderer *GetRenderer(void) { return m_pRenderer; };				// �����_���[�|�C���^�̎擾
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }	// �L�[�{�[�h�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; }						// �J�����|�C���^�̎擾	
	static CTexture *GetTexture(void) { return m_pTexture; }					// �e�N�X�`���|�C���^�̎擾
	static CTextData *GetTextData(void) { return m_pTextData; }

private:
	static CRenderer* m_pRenderer;				// �����_�����O�N���X
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�N���X
	static CCamera *m_pCamera;					// �J�����̃|�C���^
	static CLight *m_pLight;					// ���C�g�̃|�C���^
	static CTextData *m_pTextData;				// �e�L�X�g�f�[�^�N���X�̃|�C���^
	static CTexture *m_pTexture;				// �e�N�X�`���N���X�̃|�C���^

	static CExe *m_pExe;						// ���s�N���X
};

#endif