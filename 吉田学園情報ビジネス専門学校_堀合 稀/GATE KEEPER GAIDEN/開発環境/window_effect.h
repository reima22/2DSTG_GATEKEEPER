//=============================================================================
//
// ��ʃG�t�F�N�g�`�揈�� [window_effect.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _WINDOW_EFFECT_H_
#define _WINDOW_EFFECT_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define W_EFFECT_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 315.0f, 0.0f))	// �ʒu
#define W_EFFECT_SIZEX	(SCREEN_WIDTH / 2)								// ��
#define W_EFFECT_SIZEY	(315.0f)										// ����
#define W_EFFECT_ALPHA	(0.02f)											// �������郿�l
#define W_EFFECT_ALPHA_SET	(0.7f)										// �\������ۂ̃��l
#define W_EFFECT_ALPHA_DOWN	(0.1f)										// ���Ŏ��̍ŏ����l
#define W_EFFECT_ALPHA_UP	(0.5f)										// ���Ŏ��̍ő僿�l

// �O���錾
class CPolygon;

//==============================================================================
// ��ʃG�t�F�N�g�N���X
//==============================================================================
class CWindowEffect : public CScene2D
{
public:
	// ���C���̏��
	typedef enum
	{
		EFFECT_STATE_NONE = 0,	// ���펞
		EFFECT_STATE_INVASION,	// �N����
		EFFECT_STATE_CAUTION,	// �N�����댯���掞
		EFFECT_STATE_MAX
	}EFFECT_STATE;

	CWindowEffect(int nPriority = PRIORITY_UI);
	~CWindowEffect();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CWindowEffect *Create(D3DXVECTOR3 pos);

	// ��Ԃ̐ݒ�
	static void SetState(EFFECT_STATE state) { m_state = state; }
	void SetEffectState(void);

	// ���j���ɉ��������
	static void SetInvationEvent(void) { m_bInvation = !m_bInvation; }

private:
	static EFFECT_STATE m_state;	// �E�B���h�E�̏��
	D3DXCOLOR m_col;				// �F
	bool m_bAlpha;					// ���l�����̔���
	static bool m_bInvation;		// �N���G�t�F�N�g��
};
#endif
