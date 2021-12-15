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
#include "window_effect.h"
#include "polygon.h"
#include "sound.h"
#include "invasion.h"
#include "game.h"

// �ÓI�����o�ϐ��錾
CWindowEffect::EFFECT_STATE CWindowEffect::m_state;
bool CWindowEffect::m_bInvation = false;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CWindowEffect::CWindowEffect(int nPriority) : CScene2D(nPriority)
{
	// �G�t�F�N�g�̏��
	m_state = EFFECT_STATE_NONE;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CWindowEffect::~CWindowEffect()
{
}

//==============================================================================
// �{�[�_�[���C���̏���������
//==============================================================================
HRESULT CWindowEffect::Init(D3DXVECTOR3 pos)
{
	// �����o�ϐ��̏�����
	m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// ����
	CScene2D::Init(pos);
	CScene2D::SetSize(W_EFFECT_SIZEX, W_EFFECT_SIZEY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetCol(m_col);

	return S_OK;
}

//==============================================================================
// �{�[�_�[���C���̏I������
//==============================================================================
void CWindowEffect::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// ��ʃG�t�F�N�g�̍X�V����
//==============================================================================
void CWindowEffect::Update(void)
{
	// �G�t�F�N�g�̏��
	SetEffectState();

	CScene2D::Update();
}

//==============================================================================
// ��ʃG�t�F�N�g�̕`�揈��
//==============================================================================
void CWindowEffect::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// ��ʃG�t�F�N�g�̐�������
//==============================================================================
CWindowEffect *CWindowEffect::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CWindowEffect *pWindowEffect;
	pWindowEffect = new CWindowEffect;

	// ������
	pWindowEffect->Init(pos);

	// �e�N�X�`���̐ݒ�
	pWindowEffect->BindTexture(CTexture::TEXTYPE_WINDOWEFFECT);

	return pWindowEffect;
}

//==============================================================================
// ��ʃG�t�F�N�g�̐ݒ�
//==============================================================================
void CWindowEffect::SetEffectState(void)
{
	// ���C���̏�Ԃ��Ƃ̏���
	switch (m_state)
	{
	case EFFECT_STATE_NONE:
		// ����
		m_col = D3DXCOLOR(1.0f, 0.1f, 0.1f, 0.0f);

		break;

	case EFFECT_STATE_INVASION:

		if (m_bInvation == true)
		{
			// ���Z�b�g
			m_bAlpha = false;
		}

		if (m_bAlpha == false)
		{// ���l�̐ݒ�
			m_col.a = W_EFFECT_ALPHA_SET;
			m_bAlpha = true;
			m_bInvation = false;
		}

		// �N�����̃��C���F�ω�
		m_col.a -= W_EFFECT_ALPHA;

		// ���X�e�[�^�X�ֈڍs
		if (m_col.a <= 0.0f)
		{
			// ��ԕω�
			if (CInvasion::GetInvasion() >= (float)CGame::GetEndInvasion() / 10 * 8)
			{// �x�����
				m_state = EFFECT_STATE_CAUTION;				
			}
			else
			{// �ʏ��
				m_state = EFFECT_STATE_NONE;
			}

			m_bAlpha = false;
		}

		break;

	case EFFECT_STATE_CAUTION:
		// ���l�̑���
		float fAlpha = W_EFFECT_ALPHA;

		if (m_bAlpha == false)
		{// ������
			fAlpha *= -1;
		}

		m_col.a += fAlpha;

		if (m_col.a <= W_EFFECT_ALPHA_DOWN)
		{
			m_bAlpha = true;
		}
		else if (m_col.a >= W_EFFECT_ALPHA_UP)
		{
			m_bAlpha = false;
		}

		break;
	}

	// �F�̐ݒ�
	CScene2D::SetCol(m_col);
}