//==============================================================================
//
// �`��D���G�t�F�N�g�`�揈���keffect.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "effect_on.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEffectOn::CEffectOn(int nPriority) : CScene2D(nPriority)
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEffectOn::~CEffectOn()
{

}

//==============================================================================
// �G�t�F�N�g�̏���������
//==============================================================================
HRESULT CEffectOn::Init(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern)
{
	// �������ݒ�
	CScene2D::Init(pos);
	CScene2D::SetSize(fRadius, fRadius);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(OBJTYPE_EFFECT);
	CScene2D::SetCol(col);
	m_effectType = (EFFECT_TYPE)nPattern;

	// �傫��
	m_fRadius = fRadius;

	// �����x����
	m_fAlphaDown = fAlphaDown;

	// �F���
	m_col = col;

	// �p�[�e�B�N���̊p�x�Ɠ���
	float fAngle;

	// �p�^�[������
	if (m_effectType == 0)
	{
		// �S���ʊg�U
		fAngle = (float)((rand() % 628) - 314) / 100.0f;
	}
	else if (m_effectType == 1)
	{
		// ����p�[�e�B�N��
		fAngle = (float)((rand() % 115 + 200) / 100.0f);
		int nPlus = rand() % 2;
		if (nPlus == 0)
		{// ���E���]����
			fAngle *= -1;
		}
	}
	else if(m_effectType == 2)
	{
		fAngle = 0.0f;
		fMove = 0.0f;
	}
	else
	{
		int nLeft = rand() % 2;
		fAngle = rand() % 30 / 100.0f;
		if (nLeft == 0)
		{
			fAngle *= -1;
		}
	}

	// �G�t�F�N�g�̈ړ��ݒ�
	m_Move.x = (sinf(fAngle) * (float)(rand() % 500 + 1) * fMove);
	m_Move.y = (cosf(fAngle) * (float)(rand() % 500 + 1) * fMove);

	return S_OK;
}

//==============================================================================
// �G�t�F�N�g�̏I������
//==============================================================================
void CEffectOn::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �G�t�F�N�g�̍X�V����
//==============================================================================
void CEffectOn::Update(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 posEffect = CScene2D::GetPosition();

	// �p�[�e�B�N���ړ�����
	posEffect += m_Move;

	// �G�t�F�N�g�̔��a�ω�
	if (m_effectType != EFFECT_TYPE_WAVE)
	{
		m_fRadius -= 0.02f;						// �傫���̌���
	}
	else
	{
		m_fRadius += 3.5f;
	}

	CScene2D::SetSize(m_fRadius, m_fRadius);	// �ω��𔽉f

												// �G�t�F�N�g�̐F���ω�
	m_col.a -= m_fAlphaDown;	// ���X�ɓ�����
	CScene2D::SetCol(m_col);	// �ω��𔽉f

								// ����������ŏI��
	if (m_col.a < 0.0f)
	{
		Uninit();
		return;
	}

	// �ʒu���̐ݒ�
	CScene2D::SetPosition(posEffect);

	CScene2D::Update();
}

//==============================================================================
// �G�t�F�N�g�̕`�揈��
//==============================================================================
void CEffectOn::Draw(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���Z�����ɂ��A���t�@�u�����f�B���O
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �`��
	CScene2D::Draw();

	// �ʏ�̃A���t�@�u�����f�B���O
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================================
// �G�t�F�N�g�̐�������
//==============================================================================
CEffectOn *CEffectOn::Create(D3DXVECTOR3 pos, float fMove, D3DXCOLOR col, float fRadius, float fAlphaDown, int nPattern)
{
	// ���[�J���ϐ��錾
	CEffectOn *pEffectOn;
	pEffectOn = new CEffectOn;

	// ������
	if (pEffectOn != NULL)
	{
		pEffectOn->Init(pos, fMove, col, fRadius, fAlphaDown, nPattern);
	}

	// �e�N�X�`�����蓖��
	pEffectOn->BindTexture(CTexture::TEXTYPE_EFFECT);

	return pEffectOn;
}