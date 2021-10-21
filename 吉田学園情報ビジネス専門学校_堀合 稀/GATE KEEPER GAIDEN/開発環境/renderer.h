//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

// �O���錾
class CPolygon;

// �}�N����`
#define FEEDBACK_NUM		(2)			// �t�B�[�h�o�b�N�G�t�F�N�g�Ɏg�p����|�C���^�̐�
#define FEEDBACK_LENGTH		(15.0f)		// �G�t�F�N�g�������Ƀ|���S�����g�k���钷��
#define FEEDBACK_FRAMECNT	(15)		// �t�B�[�h�o�b�N�G�t�F�N�g�̃t���[����
#define DEFAULT_ALPHA		(0.1f)		// �������l
#define FEEDBACK_ALPHA		(1.0f)		// �G�t�F�N�g�������̃|���S�����l


// �����_�����O�N���X
class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	//void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	//LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT; }

	// �t�B�[�h�o�b�N�G�t�F�N�g�̔���
	void SetFeedbackEffect(void);

	float GetAlpha(void) { return m_fAlpha; }
	float GetLength(void) { return m_fLength; }

private:
	void DrawFPS(void);

	LPDIRECT3D9 m_pD3D = NULL;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice = NULL;		// Direct3D�f�o�C�X�ւ̃|�C���^
	LPD3DXFONT m_pFont = NULL;					// �t�H���g�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_apTextureMT[FEEDBACK_NUM];	// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
	LPDIRECT3DSURFACE9 m_apRenderMT[FEEDBACK_NUM];	// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X
	LPDIRECT3DSURFACE9 m_apBuffMT[FEEDBACK_NUM];	// �e�N�X�`�������_�����O�pZ�o�b�t�@
	D3DVIEWPORT9 m_viewportMT;						// �e�N�X�`�������_�����O�p�r���[�|�[�g

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;			// �t�B�[�h�o�b�N�p�|���S�����_�o�b�t�@
	float m_fLength;
	float m_fAngle;
	float m_fAlpha;									// ���l
	int m_nAppealCnt;								// �G�t�F�N�g�����Ɏg���t���[���J�E���g
	bool m_bAppeal;									// �A�s�[�����
													//CPolygon *m_pPolygon;							// �|���S���|�C���^
};

#endif