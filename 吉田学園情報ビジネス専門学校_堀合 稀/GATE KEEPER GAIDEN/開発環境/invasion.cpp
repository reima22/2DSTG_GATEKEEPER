//==============================================================================
//
// �N�����`�揈���kinvasion.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "score.h"
#include "number.h"
#include "invasion.h"

// �ÓI�����o�ϐ��錾
CNumber *CInvasion::m_apNumber[INVASION_NUM_MAX];
int CInvasion::m_nInvasion;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CInvasion::CInvasion(int nPriority) : CScene2D(nPriority)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		// �e�i���o�[�̏�����
		m_apNumber[nCnt] = NULL;
	}
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CInvasion::~CInvasion()
{
}

//==============================================================================
// �N�����`��̏���������
//==============================================================================
HRESULT CInvasion::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] == NULL)
		{
			m_apNumber[nCnt] = CNumber::Create(
				D3DXVECTOR3(pos.x + (nCnt * 25.0f), pos.y, 0.0f), 10.0f, 20.0f);
		}
	}

	m_nInvasion = 0;

	return S_OK;
}

//==============================================================================
// �N�����`��̏I������
//==============================================================================
void CInvasion::Uninit(void)
{
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber[nCnt] != NULL)
		{// NULL�`�F�b�N��ɔj��
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �N�����`��̍X�V����
//==============================================================================
void CInvasion::Update(void)
{
	// �X�R�A�̐ݒ�
	SetInvasion(m_nInvasion);

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		D3DXVECTOR2 size = m_apNumber[nCnt]->GetSize();

		if (size.x > INV_SIZE.x)
		{
			size.x -= 0.1f;
		}
		else
		{
			m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (size.y > INV_SIZE.y)
		{
			size.y -= 0.2f;
		}

		m_apNumber[nCnt]->SetSize(size);
		m_apNumber[nCnt]->Update();
	}
}

//==============================================================================
// �N�����`��̕`�揈��
//==============================================================================
void CInvasion::Draw(void)
{
	// �i���o�[�̕`��
	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		if (m_apNumber != NULL)
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//==============================================================================
// �N�����̐�������
//==============================================================================
CInvasion *CInvasion::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CInvasion *pInvasion;
	pInvasion = new CInvasion;

	// ������
	pInvasion->Init(pos);

	return pInvasion;
}

//==============================================================================
// �N�����ݒ�
//==============================================================================
void CInvasion::SetInvasion(int nInvasion)
{
	// �X�R�A�̐ݒ�
	m_nInvasion = nInvasion;

	int nData = (int)pow(10, (INVASION_NUM_MAX - 1));

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		// �e���̎Z�o
		int nNumber;
		nNumber = (m_nInvasion / nData) % 10;
		nData /= 10;

		// ���̐ݒ�
		m_apNumber[nCnt]->SetNumber(nNumber);
	}
}

//==============================================================================
// �N�����̉��Z
//==============================================================================
void CInvasion::AddInvasion(void)
{
	m_nInvasion++;

	for (int nCnt = 0; nCnt < INVASION_NUM_MAX; nCnt++)
	{
		m_apNumber[nCnt]->SetSize(D3DXVECTOR2(15.0f,30.0f));
		m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
	}
}