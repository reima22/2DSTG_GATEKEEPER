//==============================================================================
//
// �A�C�e���`�揈���kitem.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "score.h"
#include "texture.h"
#include "item.h"
#include "bullet_UI.h"
#include "effect.h"
#include "defeat.h"
#include "textdata_system.h"
#include "character.h"
#include "title_difficulty.h"
#include "player.h"

// �ÓI�����o�ϐ��錾
int CItem::m_nSetHealCnt = 0;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CItem::CItem(int nPriority) : CScene2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nBlinkCnt = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CItem::~CItem()
{

}

//==============================================================================
// �A�C�e���̏���������
//==============================================================================
HRESULT CItem::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, D3DXVECTOR3 move, CTexture::TEXTYPE type)
{
	// �������ݒ�
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);
	m_move = move;
	m_bChase = false;
	SetType(type);

	return S_OK;
}

//==============================================================================
// �A�C�e���̏I������
//==============================================================================
void CItem::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �A�C�e���̍X�V����
//==============================================================================
void CItem::Update(void)
{
	// �ʒu���̎擾
	D3DXVECTOR3 posItem = CScene2D::GetPosition();

	// �v���C���[�����S����
	if (CPlayer::GetState() == CPlayer::PLAYERSTATE_DEATH && m_bChase == true)
	{
		m_bChase = false;
	}

	// �ړ����@�̎w��
	SetChaseMove(posItem);

	// �e�̈ړ�
	posItem += m_move;

	// �ʒu���̐ݒ�
	CScene2D::SetPosition(posItem);

	// �_�ŏ���
	SetBlink();

	// Scene2D�̍X�V
	CScene2D::Update();

	// ��ʊO�̈ʒu��񔻒�
	if (posItem.y < 0 || posItem.y > SCREEN_HEIGHT)
	{
		Uninit();
	}
}

//==============================================================================
// �A�C�e���̕`�揈��
//==============================================================================
void CItem::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �A�C�e���̐�������
//==============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, D3DXVECTOR3 move, CTexture::TEXTYPE type)
{
	// ���[�J���ϐ��錾
	CItem *pItem;
	pItem = new CItem;

	// ������
	if (pItem != NULL)
	{
		pItem->Init(pos, objType, fSizeX, fSizeY, move, type);
	}

	// �e�N�X�`�����蓖��
	pItem->BindTexture(type);

	return pItem;
}

//==============================================================================
// �A�C�e���̃����_������
//==============================================================================
void CItem::RandCreate(D3DXVECTOR3 pos)
{
	// ���[�J���ϐ��錾
	int nDefeat = CDefeat::GetDefeat();		// ���j���̎擾

	// �񕜃A�C�e���ݒ�J�E���g
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nSetHealCnt = CTextDataSystem::SetData(CTextDataSystem::SYSTEMDATA_HEALITEM_CNT_HARD);
		break;
	}

	if (nDefeat % m_nSetHealCnt == 0)
	{
		Create(pos, CScene::OBJTYPE_ITEM, ITEM_X, ITEM_Y,
			D3DXVECTOR3(0.0f, 0.5f, 0.0f),
			CTexture::TEXTYPE_ITEM_3);

		return;
	}
	else
	{
		int nRandCreate = rand() % 3;	// �A�C�e����������

										// ��������ꍇ
		if (nRandCreate == 0)
		{
			int nRandType = rand() % 3;	// �A�C�e���̎��

			Create(pos, CScene::OBJTYPE_ITEM, ITEM_X, ITEM_Y,
				D3DXVECTOR3(0.0f, 0.5f, 0.0f),
				(CTexture::TEXTYPE)(CTexture::TEXTYPE_ITEM_0 + nRandType));

			return;
		}
		else
		{
			return;
		}
	}
}

//==============================================================================
// ��ނ̐ݒ�
//==============================================================================
void CItem::SetType(CTexture::TEXTYPE texType)
{
	switch (texType)
	{
	case CTexture::TEXTYPE_ITEM_0:
		m_Type = ITEM_TYPE_0;
		break;

	case CTexture::TEXTYPE_ITEM_1:
		m_Type = ITEM_TYPE_1;
		break;
	
	case CTexture::TEXTYPE_ITEM_2:
		m_Type = ITEM_TYPE_2;
		break;

	case CTexture::TEXTYPE_ITEM_3:
		m_Type = ITEM_TYPE_3;
		break;
	}
}

//==============================================================================
// �_�ŏ���
//==============================================================================
void CItem::SetBlink(void)
{
	switch (m_bBlinkUp)
	{
	case false:	// ���l�̌��Z

		m_col.a -= 0.02f;

		if (m_col.a <= 0.5f)
		{
			m_bBlinkUp = true;
		}

		break;

	case true:	// ���l�̉��Z

		m_col.a += 0.02f;

		if (m_col.a >= 1.0f)
		{
			m_bBlinkUp = false;
		}

		break;
	}

	// �F���̐ݒ�
	CScene2D::SetCol(m_col);
}

// �Ǐ]�ړ��͔��f
void CItem::SetChaseMove(D3DXVECTOR3 posItem)
{
	if (m_bChase == true)
	{
		// ���[�J���ϐ��錾
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_CHARA);

		// �v���C���[�̈ʒu
		D3DXVECTOR3 posPlayer = pScene->GetPosition();

		// �v���C���[�ւ̕���
		D3DXVECTOR3 vecPlayer = posPlayer - posItem;

		// ���������߂�
		float fLength = (float)sqrt((vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y));

		// �p�x
		float fAngle = atan2f(vecPlayer.x, vecPlayer.y);

		// �����𒲐�
		vecPlayer.x = vecPlayer.x / fLength;
		vecPlayer.y = vecPlayer.y / fLength;

		// �������w��
		vecPlayer.x *= 5.0f;
		vecPlayer.y *= 5.0f;

		m_move = vecPlayer;
	}
	else
	{
		m_move = D3DXVECTOR3(0.0f, 0.5f, 0.0f);
	}
}