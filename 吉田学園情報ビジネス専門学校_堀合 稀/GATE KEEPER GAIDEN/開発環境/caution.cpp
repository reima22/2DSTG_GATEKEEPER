//==============================================================================
//
// �{�X�o���x���`�揈���kcaution.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "caution.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "effect.h"
#include "game.h"

// �ÓI�����o�ϐ��錾
CCaution::CAUTION_STATE CCaution::m_cautionState = CAUTION_STATE_NONE;		// �x���̏��
D3DXCOLOR CCaution::m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);				// �{�[�_�[���C���̐F

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CCaution::CCaution(int nPriority) : CScene(nPriority)
{
	// �|���S���̏�����
	m_pPolygon = NULL;

	// ��ԕω��J�E���g
	m_nCntState = 0;
	m_nCautionCnt = 0;

	m_cautionState = CAUTION_STATE_NONE;
	m_bStartAlert = false;

	m_fAlpha = CAUTION_ALPHA;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CCaution::~CCaution()
{
}

//==============================================================================
// �x����ʂ̏���������
//==============================================================================
HRESULT CCaution::Init(D3DXVECTOR3 pos)
{
	// �����o�ϐ��̏�����
	m_pos = pos;

	if (m_pPolygon == NULL)
	{
		m_pPolygon = CPolygon::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
			SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f,
			CTexture::TEXTYPE_CAUTION);

		
		m_pPolygon->SetTex(1, 1, 0, 0, 0.0f, 0.0f);
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		m_pPolygon->SetCol(m_col);
	}

	return S_OK;
}

//==============================================================================
// �x����ʂ̏I������
//==============================================================================
void CCaution::Uninit(void)
{
	if (m_pPolygon != NULL)
	{// NULL�`�F�b�N��ɔj��
		m_pPolygon->Uninit();
		m_pPolygon = NULL;
	}

	Release();	// �C���X�^���X�̃f���[�g
}

//==============================================================================
// �x����ʂ̍X�V����
//==============================================================================
void CCaution::Update(void)
{
	// ���C���̏��
	SetCautionState();
}

//==============================================================================
// �x����ʂ̕`�揈��
//==============================================================================
void CCaution::Draw(void)
{
	// ���C���̕`��
	if (m_pPolygon != NULL)
	{
		m_pPolygon->Draw();
	}
}

//==============================================================================
// �x����ʂ̐�������
//==============================================================================
CCaution *CCaution::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CCaution *pCaution;
	pCaution = new CCaution;

	// ������
	pCaution->Init(pos);

	return pCaution;
}

//==============================================================================
// �x����ʂ̐ݒ�
//==============================================================================
void CCaution::SetCautionState(void)
{
	// ���C���̏�Ԃ��Ƃ̏���
	switch (m_cautionState)
	{
	case CAUTION_STATE_NONE:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// ��͐�ֈڍs
		if (CGame::GetMotherEnemy() == true && m_bStartAlert == false)
		{
			m_cautionState = CAUTION_STATE_ALERT;
			m_bStartAlert = true;
		}
		
		break;

	case CAUTION_STATE_ALERT:

		// �J�E���g���Z
		m_nCautionCnt++;

		// �F�̓_�ŕω�
		if (m_nCautionCnt <= 180)
		{			
			if (m_col.a <= 0.0f)
			{
				m_fAlpha = CAUTION_ALPHA;
			}
			else if (m_col.a >= 1.0f)
			{
				m_fAlpha = -CAUTION_ALPHA;
			}

			// �����x�ω�
			m_col.a += m_fAlpha;
		}
		else
		{// �A���[�g�I���A������
			m_col.a -= CAUTION_ALPHA;
			if (m_col.a <= 0.0f)
			{
				m_cautionState = CAUTION_STATE_ALERT_END;
			}
		}

		break;
	}

	m_pPolygon->SetCol(m_col);
}

//==============================================================================
// �ʒu�̎擾(���z�֐�)
//==============================================================================
D3DXVECTOR3 CCaution::GetPosition(void)
{
	D3DXVECTOR3 pos;
	return pos;
}

//==============================================================================
// �T�C�Y�̎擾
//==============================================================================
D3DXVECTOR3 CCaution::GetSize(void)
{
	D3DXVECTOR3 size;
	return size;
}