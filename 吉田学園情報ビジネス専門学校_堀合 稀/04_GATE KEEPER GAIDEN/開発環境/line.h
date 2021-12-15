//=============================================================================
//
// �{�[�_�[���C���`�揈�� [line.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _LINE_H_
#define _LINE_H_

#include "main.h"
#include "scene.h"

// �}�N����`
#define LINE_POS	(D3DXVECTOR3(SCREEN_WIDTH / 2, 620.0f, 0.0f))	// �ʒu
#define LINE_SIZEX	(SCREEN_WIDTH / 2)								// ��
#define LINE_SIZEY	(10.0f)											// ����
#define LINE_STATE_CNT	(9)											// ��ԕω��J�E���g�l

// �O���錾
class CPolygon;

//==============================================================================
// �{�[�_�[���C���N���X
//==============================================================================
class CLine : public CScene2D
{
public:
	// ���C���̏��
	typedef enum
	{
		LINE_STATE_NONE = 0,	// ���펞
		LINE_STATE_INVASION,	// �N����
		LINE_STATE_CAUTION,		// �N�����댯���掞
		LINE_STATE_TUTORIAL,	// �J�n�����b�Z�[�W�\��
		LINE_STATE_MAX
	}LINE_STATE;

	CLine(int nPriority = PRIORITY_UI);
	~CLine();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CLine *Create(D3DXVECTOR3 pos);

	void SetLineState(void);

	static void SetState(LINE_STATE state) { m_lineState = state; }

private:
	static LINE_STATE m_lineState;	// ���C���̏��
	static D3DXCOLOR m_col;			// �F
	int m_nCntState;				// ��ԕω��J�E���g
	int m_nCautionCnt;				// �댯���掞�ω��J�E���g
	int m_nTutoCnt;					// ���b�Z�[�W�\�����J�E���g
};
#endif