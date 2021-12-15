//==============================================================================
//
// �{�[�_�[���C���`�揈���kline.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "line.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "effect.h"
#include "game.h"
#include "window_effect.h"

// �ÓI�����o�ϐ��錾
CLine::LINE_STATE CLine::m_lineState = LINE_STATE_NONE;		// �{�[�_�[���C���̏��
D3DXCOLOR CLine::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �{�[�_�[���C���̐F

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CLine::CLine(int nPriority) : CScene2D(nPriority)
{
	// ��ԕω��J�E���g
	m_nCntState = 0;
	m_nCautionCnt = 0;
	m_nTutoCnt = 0;

	m_lineState = LINE_STATE_NONE;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CLine::~CLine()
{
}

//==============================================================================
// �{�[�_�[���C���̏���������
//==============================================================================
HRESULT CLine::Init(D3DXVECTOR3 pos)
{
	// ������
	CScene2D::Init(pos);
	CScene2D::SetSize(LINE_SIZEX, LINE_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(OBJTYPE_UI);
	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//==============================================================================
// �{�[�_�[���C���̏I������
//==============================================================================
void CLine::Uninit(void)
{
	// �I������
	CScene2D::Uninit();
}

//==============================================================================
// �{�[�_�[���C���̍X�V����
//==============================================================================
void CLine::Update(void)
{
	// ���[�J���ϐ��錾
	CScene *pScene;
	pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;
	CRenderer *pRenderer = CManager::GetRenderer();	// �����_���[�̎擾

	// ���C���̈ʒu
	D3DXVECTOR3 pos = CScene2D::GetPosition();

	// ���C���̓����蔻��
	while (pScene)
	{		
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene != NULL)
		{
			// �T�C�Y�̎擾
			float fSizeX = pScene->GetSize().x;
			float fSizeY = pScene->GetSize().y;

			// �I�u�W�F�N�g�̎��
			CScene::OBJTYPE objType;
			objType = pScene->GetObjType();

			if (objType == OBJTYPE_ENEMY)
			{// �G�̐N������
				D3DXVECTOR3 posEnemy;
				posEnemy = pScene->GetPosition();

				if (pos.x + LINE_SIZEX >= posEnemy.x - fSizeX &&
					pos.x - LINE_SIZEX <= posEnemy.x + fSizeX &&
					pos.y + LINE_SIZEY >= posEnemy.y - fSizeY &&
					pos.y - LINE_SIZEY <= posEnemy.y + fSizeY)
				{
					// �T�E���h�Đ�
					CSound::Play(CSound::SOUND_LABEL_SE_LINEOVER);

					// �N�����̉��Z
					CInvasion::AddInvasion();

					// �G�t�F�N�g�̔���
					for (int nCnt = 0; nCnt < 100; nCnt++)
					{
						CEffect::Create(posEnemy, 0.05f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 10.0f, 0.02f, 1);
					}

					// ��ԕω��J�E���g�ݒ�
					m_nCntState = 9;

					// ��ԕω�
					m_lineState = LINE_STATE_INVASION;

					// �t�B�[�h�o�b�N�G�t�F�N�g�̔��� 
					pRenderer->SetFeedbackEffect();

					// ��ʃG�t�F�N�g�̎��s
					CWindowEffect::SetState(CWindowEffect::EFFECT_STATE_INVASION);
					CWindowEffect::SetInvationEvent();

					pScene->Uninit();

					return;
				}
			}
		}

		// ���̃|�C���^�̑��
		pScene = pSceneNext;
	}

	// ���C���̏��
	SetLineState();

	// �X�V
	CScene2D::Update();
}

//==============================================================================
// �{�[�_�[���C���̕`�揈��
//==============================================================================
void CLine::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �{�[�_�[���C���̐�������
//==============================================================================
CLine *CLine::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CLine *pLine;
	pLine = new CLine;

	// ������
	pLine->Init(pos);

	pLine->BindTexture(CTexture::TEXTYPE_LINE);

	return pLine;
}

//==============================================================================
// �{�[�_�[���C���̐ݒ�
//==============================================================================
void CLine::SetLineState(void)
{
	// ���C���̏�Ԃ��Ƃ̏���
	switch (m_lineState)
	{
	case LINE_STATE_NONE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �J�E���^�[���Z
		if (m_nCntState >= 0)
		{
			m_nCntState--;
		}

		// ���X�e�[�^�X�ֈڍs
		if (m_nCntState <= 0)
		{
			// ��ԕω�
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{
				m_lineState = LINE_STATE_CAUTION;
			}
			else
			{
				m_lineState = LINE_STATE_NONE;
			}			
		}

		break;

	case LINE_STATE_INVASION:

		// �N�����̃��C���F�ω�
		m_col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

		// �J�E���^�[���Z
		if (m_nCntState >= 0)
		{
			m_nCntState--;
		}

		// ���X�e�[�^�X�ֈڍs
		if (m_nCntState <= 0)
		{
			// ��ԕω�
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{
				m_lineState = LINE_STATE_CAUTION;
			}
			else
			{
				m_lineState = LINE_STATE_NONE;
			}
		}

		break;

	case LINE_STATE_CAUTION:

		// �J�E���g���Z
		m_nCautionCnt++;

		// �F�̓_�ŕω�
		if (m_nCautionCnt % 60 == 0)
		{
			m_col = D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f);

			// �T�E���h�Đ�
			CSound::Play(CSound::SOUND_LABEL_SE_LINE_CAUTION);
		}
		else if (m_nCautionCnt % 30 == 0 && m_nCautionCnt % 60 != 0)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		break;

	case LINE_STATE_TUTORIAL:
		
		// �J�E���g���Z
		m_nTutoCnt++;

		// �F�̓_�ŕω�
		if (m_nTutoCnt % 60 == 0)
		{
			m_col = D3DXCOLOR(1.0f, 0.0f, 0.6f, 1.0f);
		}
		else if (m_nTutoCnt % 30 == 0 && m_nTutoCnt % 60 != 0)
		{
			m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		break;
	}



	CScene2D::SetCol(m_col);
}