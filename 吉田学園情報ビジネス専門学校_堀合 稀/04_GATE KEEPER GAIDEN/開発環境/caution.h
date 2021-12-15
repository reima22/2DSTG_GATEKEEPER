//=============================================================================
//
// �x����ʕ`�揈�� [caution.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _CAUTION_H_
#define _CAUTION_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define CAUTION_CNT		(180)	// ���o�������
#define CAUTION_ALPHA	(0.03f)	// �ω����铧���x

// �O���錾
class CPolygon;

//==============================================================================
// �x����ʃN���X
//==============================================================================
class CCaution : public CScene
{
public:
	// ���C���̏��
	typedef enum
	{
		CAUTION_STATE_NONE = 0,		// ������
		CAUTION_STATE_ALERT,		// �A�N�e�B�u
		CAUTION_STATE_ALERT_END,	// �A���[�g�̏I��
		CAUTION_STATE_MAX
	}CAUTION_STATE;

	CCaution(int nPriority = PRIORITY_UI);
	~CCaution();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCaution *Create(D3DXVECTOR3 pos);

	void SetCautionState(void);
	static CAUTION_STATE GetCautionState(void) { return m_cautionState; }	// ��Ԃ̎擾

	D3DXVECTOR3 GetPosition(void);
	D3DXVECTOR3 GetSize(void);

private:
	CPolygon *m_pPolygon;					// �|���S���N���X�|�C���^
	static CAUTION_STATE m_cautionState;	// �x���̏��
	D3DXVECTOR3 m_pos;						// �ʒu
	static D3DXCOLOR m_col;					// �F
	int m_nCntState;						// ��ԕω��J�E���g
	int m_nCautionCnt;						// �댯���掞�ω��J�E���g

	bool m_bStartAlert;						// �A���[�g�̊J�n
	float m_fAlpha;							// �_�Ń��l
};
#endif