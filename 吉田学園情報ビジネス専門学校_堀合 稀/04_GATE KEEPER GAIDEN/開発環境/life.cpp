//==============================================================================
//
// ���C�t�`�揈���klife.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "life.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "textdata_player.h"

// �ÓI�����o�ϐ��錾
CPolygon **CLife::m_apPolygon;	// �|���S���|�C���^
int CLife::m_nLife;				// ���C�t���l
int CLife::m_nLifeMax;			// ���C�t�ő�l

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CLife::CLife(int nPriority) : CScene2D(nPriority)
{
	// ���C�t�l�ݒ�
	m_nLife = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_START);
	m_nLifeMax = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_MAX);

	// ���������I�m��
	m_apPolygon = new CPolygon*[m_nLifeMax];

	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		// ������
		m_apPolygon[nCnt] = NULL;
	}

	// �L�����N�^�[�|�C���^�擾
	m_pChara = CGame::GetChara();
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CLife::~CLife()
{
}

//==============================================================================
// ���C�t�̏���������
//==============================================================================
HRESULT CLife::Init(D3DXVECTOR3 pos)
{
	// �|���S���̐ݒ�
	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		if (m_apPolygon[nCnt] == NULL)
		{
			m_apPolygon[nCnt] = CPolygon::Create(
				D3DXVECTOR3(pos.x + (nCnt * 60.0f), pos.y, 0.0f), 30.0f, 30.0f, CTexture::TEXTYPE_LIFE);	

			m_apPolygon[nCnt]->SetTex(1, 1, 0,0,0.0f, 0.0f);
		}
	}

	// �J�n���̃��C�t�ݒ�
	m_nLife = CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_LIFE_START);

	return S_OK;
}

//==============================================================================
// ���C�t�̏I������
//==============================================================================
void CLife::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nLifeMax; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{// NULL�`�F�b�N��ɔj��
			m_apPolygon[nCnt]->Uninit();
			delete m_apPolygon[nCnt];
			m_apPolygon[nCnt] = NULL;
		}
	}

	Release();	// CLife�C���X�^���X�̃f���[�g
}

//==============================================================================
// ���C�t�̍X�V����
//==============================================================================
void CLife::Update(void)
{
	// ���C�t�l�̔��f
	m_nLife = m_pChara->GetLife(CCharacter::CHARA_TYPE_PLAYER);
}

//==============================================================================
// ���C�t�̕`�揈��
//==============================================================================
void CLife::Draw(void)
{
	// �i���o�[�̕`��
	for (int nCnt = 0; nCnt < m_nLife; nCnt++)
	{
		if (m_apPolygon[nCnt] != NULL)
		{
			m_apPolygon[nCnt]->Draw();
		}
	}
}

//==============================================================================
// ���C�t�̐�������
//==============================================================================
CLife *CLife::Create(D3DXVECTOR3 pos)
{
	// CLife�̃C���X�^���X����
	CLife *pLife;
	pLife = new CLife;

	// CLife�̏�����
	pLife->Init(pos);

	return pLife;
}