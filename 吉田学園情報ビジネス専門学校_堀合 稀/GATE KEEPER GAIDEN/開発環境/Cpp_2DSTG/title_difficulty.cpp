//==============================================================================
//
// ��Փx�I�������ktitle_difficulty.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "title.h"
#include "title_difficulty.h"
#include "polygon.h"
#include "input.h"
#include "gamepad.h"
#include "fade.h"
#include "sound.h"

// �ÓI�����o�ϐ��錾
CScene2D *CTitleDifficulty::m_apScene2D[DIFFICULTY_MAX] = {};	// Scene2D�N���X�̃|�C���^
CTitleDifficulty::DIFFICULTY CTitleDifficulty::m_difficulty;	// �I�𒆂̓�Փx
D3DXCOLOR CTitleDifficulty::m_col[DIFFICULTY_MAX];				// �I�𒆃J�[�\���̐F
bool CTitleDifficulty::m_bSelectDifficulty;						// �I���J�n

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTitleDifficulty::CTitleDifficulty(int nPriority) : CScene2D(nPriority)
{
	m_bSelectDifficulty = false;
	m_nBlinkCnt = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTitleDifficulty::~CTitleDifficulty()
{
}

//==============================================================================
// ��Փx�I���̏���������
//==============================================================================
HRESULT CTitleDifficulty::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = new CScene2D(PRIORITY_PAUSE);
		m_apScene2D[nCnt]->Init(D3DXVECTOR3(pos.x, pos.y - 100.0f + (nCnt * 110.0f), 0.0f));
		m_apScene2D[nCnt]->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_apScene2D[nCnt]->SetSize(T_DIFFICULT_X, T_DIFFICULT_Y);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_apScene2D[nCnt]->BindTexture((CTexture::TEXTYPE)(CTexture::TEXTYPE_EASY + nCnt));
	}

	// ������
	m_difficulty = DIFFICULTY_EASY;

	// �F�̐ݒ�
	m_col[DIFFICULTY_EASY] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);
	m_col[DIFFICULTY_NORMAL] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f);
	m_col[DIFFICULTY_HARD] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	m_bAlphaDown = true;

	return S_OK;
}

//==============================================================================
// ��Փx�I���̏I������
//==============================================================================
void CTitleDifficulty::Uninit(void)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			delete m_apScene2D[nCnt];
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}
}

//==============================================================================
// ��Փx�I���̍X�V����
//==============================================================================
void CTitleDifficulty::Update(void)
{
	SetMode();
	
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		// �X�V
		m_apScene2D[nCnt]->Update();
	}
}

//==============================================================================
// ��Փx�I���̕`�揈��
//==============================================================================
void CTitleDifficulty::Draw(void)
{
	for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ��Փx�I���̐�������
//==============================================================================
CTitleDifficulty *CTitleDifficulty::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CTitleDifficulty *pTitleDifficulty;
	pTitleDifficulty = new CTitleDifficulty;

	// ������
	pTitleDifficulty->Init(pos);

	return pTitleDifficulty;
}

//==============================================================================
// ��Փx���Ƃ̐ݒ�
//==============================================================================
void CTitleDifficulty::SetMode(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CGamepad *gamepad = CManager::GetInputGamepad();

	// �J�[�\���̖���
	if (m_bSelectDifficulty == false)
	{// �\�����Ă��Ȃ����
		for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
		{// ���l�[��
			m_col[nCnt].a = 0.0f;
		}
	}
	else
	{
		if (CTitle::GetStart() == false)
		{
			for (int nCnt = 0; nCnt < DIFFICULTY_MAX; nCnt++)
			{
				if (nCnt == m_difficulty)
				{// �I��
					if (m_bAlphaDown == true)
					{
						m_col[m_difficulty].a -= 0.01f;

						// �_�ł̐܂�Ԃ�
						if (m_col[m_difficulty].a <= 0.5f)
						{
							m_col[m_difficulty].a = 0.5f;
							m_bAlphaDown = false;
						}
					}
					else
					{
						m_col[m_difficulty].a += 0.01f;

						// �_�ł̐܂�Ԃ�
						if (m_col[m_difficulty].a >= 1.0f)
						{
							m_col[m_difficulty].a = 1.0f;

							m_bAlphaDown = true;
						}
					}

					m_apScene2D[nCnt]->SetCol(m_col[m_difficulty]);

				}
				else
				{// ����ȊO
					m_col[nCnt].a = 0.1f;
					m_apScene2D[nCnt]->SetCol(m_col[nCnt]);
				}
			}
		}
		else
		{
			m_nBlinkCnt++;

			if (m_nBlinkCnt % 2 == 0)
			{
				m_col[m_difficulty].a = 0.0f;
			}
			else
			{
				m_col[m_difficulty].a = 1.0f;
			}

			// �F�̐ݒ�
			m_apScene2D[m_difficulty]->SetCol(m_col[m_difficulty]);
		}
		// �J�[�\���̈ړ�
		if (keyboard->GetTrigger(CInput::KEYINFO_DOWN) == true || gamepad->GetTrigger(CInput::KEYINFO_DOWN) == true)
		{
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			m_difficulty = (DIFFICULTY)(m_difficulty + 1);
		}
		else if (keyboard->GetTrigger(CInput::KEYINFO_UP) == true || gamepad->GetTrigger(CInput::KEYINFO_UP) == true)
		{
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			m_difficulty = (DIFFICULTY)(m_difficulty - 1);
		}

		// �J�[�\���̕␳
		if (m_difficulty < 0)
		{
			m_difficulty = (DIFFICULTY)(DIFFICULTY_MAX - 1);
		}
		else if (m_difficulty >(DIFFICULTY_MAX - 1))
		{
			m_difficulty = DIFFICULTY_EASY;
		}

		// �I�������Ƃ̏���
		if (m_bSelectDifficulty == true && CTitle::GetStart() == false)
		{
			if (keyboard->GetTrigger(CInput::KEYINFO_OK) == true || gamepad->GetTrigger(CInput::KEYINFO_OK) == true)
			{
				// �Q�[���̊J�n
				CTitle::SetStart(true);

				// ���ʉ��̍Đ�
				CSound::Play(CSound::SOUND_LABEL_SE_START000);

				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_TUTORIAL);	// �V�[���J��
			}
		}
	}	
}