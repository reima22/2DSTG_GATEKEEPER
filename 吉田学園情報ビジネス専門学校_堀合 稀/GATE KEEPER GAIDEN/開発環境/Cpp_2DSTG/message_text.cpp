//==============================================================================
//
// ���b�Z�[�W�e�L�X�g�`�揈���kmessage_text.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h" 
#include "fade.h"

// �ÓI�����o�ϐ��錾
CMessage::MESSAGE_START CMessageText::m_StartState;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMessageText::CMessageText(int nPriority) : CScene2D(nPriority)
{
	m_StartState = CMessage::MESSAGE_START_START;

	m_nStartCnt = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CMessageText::~CMessageText()
{
}

//==============================================================================
// ���b�Z�[�W�e�L�X�g�̏���������
//==============================================================================
HRESULT CMessageText::Init(D3DXVECTOR3 pos)
{
	// ������	
	CScene2D::Init(pos);
	CScene2D::SetSize(MESSAGE_SIZEX, MESSAGE_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);

	// �F�̏����ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// ���b�Z�[�W�̏I������
//==============================================================================
void CMessageText::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// ���b�Z�[�W�̍X�V����
//==============================================================================
void CMessageText::Update(void)
{
	SetState();

	CScene2D::Update();
}

//==============================================================================
// ���b�Z�[�W�̕`�揈��
//==============================================================================
void CMessageText::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// ���b�Z�[�W�̐�������
//==============================================================================
CMessageText *CMessageText::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CMessageText *pMessageText;
	pMessageText = new CMessageText;

	// ������
	pMessageText->Init(pos);

	pMessageText->BindTexture(CTexture::TEXTYPE_MESSAGE_TEXT);

	return pMessageText;
}

//==============================================================================
// ���b�Z�[�W�̏��
//==============================================================================
void CMessageText::SetState(void)
{
	// �t�F�[�h��Ԃ̎擾
	CFade::FADE fade = CFade::GetFade();

	// �E�B���h�E�̏�Ԏ擾
	CMessage::MESSAGE_START startWindow = CMessageWindow::GetState();

	if (fade == CFade::FADE_NONE)
	{
		switch (m_StartState)
		{
		case CMessage::MESSAGE_START_START:	// ���b�Z�[�W�\���J�n
			// �E�B���h�E���\������
			if (startWindow == CMessage::MESSAGE_START_ON)
			{
				// �t�F�[�h�C����
				m_StartState = CMessage::MESSAGE_START_IN;
				CLine::SetState(CLine::LINE_STATE_TUTORIAL);
			}

			break;

		case CMessage::MESSAGE_START_IN:		// ���b�Z�[�W�̃t�F�[�h�C��
			m_col.a += 0.01f;

			if (m_col.a >= 1.0f)
			{// �\������
				m_StartState = CMessage::MESSAGE_START_ON;
			}
			break;

		case CMessage::MESSAGE_START_ON:		// ���b�Z�[�W�̕\��
			// �\���Ԃ̃J�E���g
			m_nStartCnt++;

			if (m_nStartCnt > 180)
			{// ��3�b�o��
				m_StartState = CMessage::MESSAGE_START_OUT;
			}

			break;

		case CMessage::MESSAGE_START_OUT:		// ���b�Z�[�W�̃t�F�[�h�A�E�g
			m_col.a -= 0.01f;
			if (m_col.a <= 0.0f)
			{
				m_StartState = CMessage::MESSAGE_START_END;
				CLine::SetState(CLine::LINE_STATE_NONE);
			}

			break;

		case CMessage::MESSAGE_START_END:		// ���b�Z�[�W�I��

			break;
		}
	}

	// �F�̐ݒ�
	CScene2D::SetCol(m_col);
}