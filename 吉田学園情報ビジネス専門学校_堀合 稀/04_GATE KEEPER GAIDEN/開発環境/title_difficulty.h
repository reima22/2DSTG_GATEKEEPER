//=============================================================================
//
// ��Փx�I�������� [title_difficulty.h]
// Author : Mare Horiai
//
//============================================================================= 
#ifndef _TITLE_DIFFICULTY_H_
#define _TITLE_DIFFICULTY_H_

#include "main.h"
#include "scene2D.h"

#define T_DIFFICULT_POS	(D3DXVECTOR3(640.0f, 480.0f, 0.0f))
#define T_DIFFICULT_X	(200.0f)
#define T_DIFFICULT_Y	(50.0f)

//==============================================================================
// ��Փx�I�����N���X
//==============================================================================
class CTitleDifficulty : public CScene2D
{
public:
	// �I����
	typedef enum
	{
		DIFFICULTY_EASY = 0,	// �ȒP
		DIFFICULTY_NORMAL,		// ����
		DIFFICULTY_HARD,		// ���
		DIFFICULTY_MAX
	}DIFFICULTY;

	CTitleDifficulty(int nPriority = CScene::PRIORITY_UI);
	~CTitleDifficulty();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitleDifficulty *Create(D3DXVECTOR3 pos);
	void SetMode(void);

	// �I���J�n�̉�
	static bool GetSelect(void) { return m_bSelectDifficulty; }
	static void SetSelect(bool bSelect) { m_bSelectDifficulty = bSelect; }

	// �I��������Փx�̎擾
	static DIFFICULTY GetDifficulty(void) { return m_difficulty; }

private:
	static CScene2D *m_apScene2D[DIFFICULTY_MAX];	// Scene2D�N���X
	static DIFFICULTY m_difficulty;					// �I�𒆂̃J�[�\��
	static D3DXCOLOR m_col[DIFFICULTY_MAX];			// �J�[�\���F
	static bool m_bSelectDifficulty;				// �I���J�n			
	bool m_bAlphaDown;								// �I�𒆃J�[�\���_�ŗp����
	int m_nBlinkCnt;								// �_�ŃJ�E���g
};
#endif