//==============================================================================
//
// ���b�Z�[�W�E�B���h�E�`�揈���kmessage_window.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "polygon.h"
#include "message.h"
#include "message_text.h"
#include "message_window.h" 
#include "fade.h"

// �ÓI�����o�ϐ��錾
CMessage::MESSAGE_START CMessageWindow::m_StartState;
D3DXCOLOR CMessageWindow::m_col;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMessageWindow::CMessageWindow(int nPriority) : CScene2D(nPriority)
{
	m_StartState = CMessage::MESSAGE_START_START;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CMessageWindow::~CMessageWindow()
{
}

//==============================================================================
// ���b�Z�[�W�̏���������
//==============================================================================
HRESULT CMessageWindow::Init(D3DXVECTOR3 pos)
{
	// ������
	CScene2D::Init(pos);
	CScene2D::SetSize(MESSAGE_SIZEX + 10.0f, MESSAGE_SIZEY + 20.0f);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);

	// �F�̏����ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	return S_OK;
}

//==============================================================================
// ���b�Z�[�W�̏I������
//==============================================================================
void CMessageWindow::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// ���b�Z�[�W�̍X�V����
//==============================================================================
void CMessageWindow::Update(void)
{
	SetState();

	// �X�V
	CScene2D::Update();
}

//==============================================================================
// ���b�Z�[�W�̕`�揈��
//==============================================================================
void CMessageWindow::Draw(void)
{
	// �`��
	CScene2D::Draw();
}

//==============================================================================
// ���b�Z�[�W�̐�������
//==============================================================================
CMessageWindow *CMessageWindow::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CMessageWindow *pMessageWindow;
	pMessageWindow = new CMessageWindow;

	// ������
	pMessageWindow->Init(pos);

	pMessageWindow->BindTexture(CTexture::TEXTYPE_MESSAGE_WINDOW);

	return pMessageWindow;
}

//==============================================================================
// ���b�Z�[�W�̏��
//==============================================================================
void CMessageWindow::SetState(void)
{
	// �t�F�[�h��Ԃ̎擾
	CFade::FADE fade = CFade::GetFade();

	// ���b�Z�[�W�e�L�X�g��Ԃ̎擾
	CMessage::MESSAGE_START startText = CMessageText::GetState();

	if (fade == CFade::FADE_NONE)
	{
		switch (m_StartState)
		{
		case CMessage::MESSAGE_START_START:	// ���b�Z�[�W�\���J�n
			// �t�F�[�h�C����
			m_StartState = CMessage::MESSAGE_START_IN;

			break;

		case CMessage::MESSAGE_START_IN:		// ���b�Z�[�W�̃t�F�[�h�C��
			m_col.a += 0.01f;

			if (m_col.a >= 1.0f)
			{// �\������
				m_StartState = CMessage::MESSAGE_START_ON;
			}

			break;

		case CMessage::CMessage::MESSAGE_START_ON:		// ���b�Z�[�W�̕\��
			if (startText == CMessage::MESSAGE_START_END)
			{
				m_StartState = CMessage::MESSAGE_START_OUT;
			}
			break;

		case CMessage::MESSAGE_START_OUT:		// ���b�Z�[�W�̃t�F�[�h�A�E�g
			m_col.a -= 0.01f;

			if (m_col.a <= 0.0f)
			{// ��������
				m_StartState = CMessage::MESSAGE_START_END;
			}

			break;

		case CMessage::MESSAGE_START_END:		// ���b�Z�[�W�I��
			CMessage::ChangeOnEnd();
			break;
		}
	}

	// �F�̐ݒ�
	CScene2D::SetCol(m_col);
}