//==============================================================================
//
// �eUI�`�揈���kbullet_UI.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "UI.h"
#include "bullet_UI.h"
#include "polygon.h"
#include "input.h"
#include "gamepad.h"
#include "sound.h"
#include "item.h"
#include "textdata_player.h"
#include "effect.h"

// �ÓI�����o�ϐ��錾
CScene2D *CBulletUI::m_apScene2D[BUITYPE_MAX];
int CBulletUI::m_nUse = 0;
bool CBulletUI::m_bTake[BUITYPE_MAX];
D3DXVECTOR2 CBulletUI::m_size[BUITYPE_MAX];

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CBulletUI::CBulletUI(int nPriority) : CScene2D(nPriority)
{
	// �|���S���̏�����
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt] = NULL;
		m_aPattern[nCnt] = 0;
	}

	// �A�C�e���ɂ��擾�o����e
	m_bTake[1] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_WAVE);
	m_bTake[2] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_MISSILE);
	m_bTake[3] = (bool)CTextDataPlayer::SetData(CTextDataPlayer::PLAYERDATA_UNLOCK_BULLET_REVERSE);

	// �ʏ�e
	m_nUse = 0;
	m_aPattern[0] = 0;
	m_bTake[0] = true;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CBulletUI::~CBulletUI()
{
}

//==============================================================================
// �eUI�̏���������
//==============================================================================
HRESULT CBulletUI::Init(D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_size[nCnt] = D3DXVECTOR2(BUI_SIZE_X, BUI_SIZE_Y);

		// �C���X�^���X����
		m_apScene2D[nCnt] = new CScene2D;
	
		m_apScene2D[nCnt]->Init(D3DXVECTOR3(pos.x - 592.0f + nCnt * 80.0f, pos.y, 0.0f));
		m_apScene2D[nCnt]->SetSize(m_size[nCnt].x, m_size[nCnt].y);
		m_apScene2D[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apScene2D[nCnt]->BindTexture((CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_UI_0 + nCnt));
	}

	return S_OK;
}

//==============================================================================
// �eUI�̏I������
//==============================================================================
void CBulletUI::Uninit(void)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{// NULL�`�F�b�N��ɔj��
			delete m_apScene2D[nCnt];
			m_apScene2D[nCnt]->Uninit();
			m_apScene2D[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �eUI�̍X�V����
//==============================================================================
void CBulletUI::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CGamepad *gamepad = CManager::GetInputGamepad();

	// �A�C�e���̐؂�ւ�
	if (keyboard->GetTrigger(CInput::KEYINFO_CHANGE_RIGHT) == true || gamepad->GetTrigger(CInput::KEYINFO_CHANGE_RIGHT) == true)
	{// �e�̕ύX
		if (m_bTake[1] == true || m_bTake[2] == true || m_bTake[3] == true)
		{
			// �T�E���h�Đ�
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);
			
			// �e�̕ω�
			BulletChangeRight();
		}
	}

	if (keyboard->GetTrigger(CInput::KEYINFO_CHANGE_LEFT) == true || gamepad->GetTrigger(CInput::KEYINFO_CHANGE_LEFT) == true)
	{
		if (m_bTake[1] == true || m_bTake[2] == true || m_bTake[3] == true)
		{
			// �T�E���h�Đ�
			CSound::Play(CSound::SOUND_LABEL_SE_ITEM001);

			// �e�̕ω�
			BulletChangeLeft();
		}
	}

	BulletStatus();

	// �ʏ�eUI
	m_apScene2D[0]->SetTex(2,1, m_aPattern[0],0, 0.0f, 0.0f);

	// �ǉ��eUI
	for (int nCnt = 1; nCnt < BUITYPE_MAX; nCnt++)
	{
		m_apScene2D[nCnt]->SetTex(3,1, m_aPattern[nCnt],0, 0.0f, 0.0f);
	}
}

//==============================================================================
// �eUI�̕`�揈��
//==============================================================================
void CBulletUI::Draw(void)
{
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (m_apScene2D[nCnt] != NULL)
		{
			m_apScene2D[nCnt]->Draw();
		}
	}
}

//==============================================================================
// �eUI�̐�������
//==============================================================================
CBulletUI *CBulletUI::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CBulletUI *pBulletUI;
	pBulletUI = new CBulletUI;

	// ������
	pBulletUI->Init(pos);

	return pBulletUI;
}

//==============================================================================
// �e�̕ύX
//==============================================================================
void CBulletUI::BulletChangeRight(void)
{
	// �v���C���[�̈ʒu
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	D3DXVECTOR3 posPlayer = pScene->GetPosition();

	// m_nUse�̊Ǘ�
	switch (m_nUse)
	{// �g�p���Ă���e�ƕۗL���Ă���e�ɂ���Ď��̒e������
	case 0:	// �ʏ�e
		if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f,1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}
			
			m_nUse = 1;
			break;
		}
		else if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �~�T�C���e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �t�E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}

	case 1:	// �ǉ��e1
		if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �~�T�C���e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �t�E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �ʏ�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 2:	// �ǉ��e2
		if (m_bTake[3] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �t�E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �ʏ�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 3:	// �ǉ��e3(�ʏ�e�ֈڍs�̂�)
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{// �ʏ�e�ω��G�t�F�N�g
			CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
		}

		m_nUse = 0;
		break;

	default:
		break;
	}
}

//==============================================================================
// �e�̕ύX
//==============================================================================
void CBulletUI::BulletChangeLeft(void)
{
	// �v���C���[�̈ʒu
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	D3DXVECTOR3 posPlayer = pScene->GetPosition();

	// m_nUse�̊Ǘ�
	switch (m_nUse)
	{// �g�p���Ă���e�ƕۗL���Ă���e�ɂ���Ď��̒e������
	case 0:	// �ʏ�e
		if (m_bTake[3] == true)
		{// �t�E�F�[�u
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �t�E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 3;
			break;
		}
		else if (m_bTake[2] == true)
		{// �~�T�C��
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �~�T�C���e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[1] == true)
		{// �E�F�[�u
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}

	case 1:	// �ǉ��e1
		for (int nCnt = 0; nCnt < 100; nCnt++)
		{// �ʏ�e�ω��G�t�F�N�g
			CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
		}

		m_nUse = 0;
		break;

	case 2:	// �ǉ��e2
		if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �ʏ�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}

	case 3:	// �ǉ��e3(�ʏ�e�ֈڍs�̂�)
		if (m_bTake[2] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �~�T�C���e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 2;
			break;
		}
		else if (m_bTake[1] == true)
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �E�F�[�u�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 1.0f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 1;
			break;
		}
		else
		{
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{// �ʏ�e�ω��G�t�F�N�g
				CEffect::Create(posPlayer, 0.009f, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 10.0f, 0.03f, 0);
			}

			m_nUse = 0;
			break;
		}
		

	default:
		break;
	}
}

//==============================================================================
// �e�̏�ԂɑΉ�����e�N�X�`���̐ݒ�
//==============================================================================
void CBulletUI::BulletStatus(void)
{
	switch(m_nUse)
	{
		case 0:	// �ʏ�e
			m_aPattern[0] = 0;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}

			break;

		case 1:	// �ǉ��e1
			m_aPattern[0] = 1;
			m_aPattern[1] = 2;

			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}

			break;

		case 2:	// �ǉ��e2
			m_aPattern[0] = 1;
			m_aPattern[2] = 2;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[3] == true)
			{
				m_aPattern[3] = 1;
			}
			break;

		case 3:	// �ǉ��e3
			m_aPattern[0] = 1;
			m_aPattern[3] = 2;

			if (m_bTake[1] == true)
			{
				m_aPattern[1] = 1;
			}
			if (m_bTake[2] == true)
			{
				m_aPattern[2] = 1;
			}
			break;

		default:
			break;
	}

	// �g�p��Ԃɉ�����UI�̃T�C�Y��ω�
	for (int nCnt = 0; nCnt < BUITYPE_MAX; nCnt++)
	{
		if (nCnt == m_nUse)
		{// �I��
			if (m_size[nCnt].x <= BUI_SIZE_X)
			{
				m_size[nCnt].x += 0.32f;
			}
			else if (m_size[nCnt].x >= BUI_SIZE_X)
			{
				m_size[nCnt].x = BUI_SIZE_X;
			}

			if (m_size[nCnt].y <= BUI_SIZE_Y)
			{
				m_size[nCnt].y += 0.34f;
			}
			else if (m_size[nCnt].y >= BUI_SIZE_Y)
			{
				m_size[nCnt].y = BUI_SIZE_Y;
			}
		}
		else
		{// �I���O
			if (m_size[nCnt].x >= BUI_SIZE_X / 2)
			{
				m_size[nCnt].x -= 0.32f;
			}
			else if (m_size[nCnt].x <= BUI_SIZE_X / 2)
			{
				m_size[nCnt].x = BUI_SIZE_X / 2;
			}

			if (m_size[nCnt].y >= BUI_SIZE_Y / 2)
			{
				m_size[nCnt].y -= 0.34f;
			}
			else if (m_size[nCnt].y <= BUI_SIZE_Y / 2)
			{
				m_size[nCnt].y = BUI_SIZE_Y / 2;
			}
		}	

			m_apScene2D[nCnt]->SetSize(m_size[nCnt].x, m_size[nCnt].y);
		
	}
}

//==============================================================================
// �����̒ǉ�
//==============================================================================
void CBulletUI::AddBullet(CItem::ITEM_TYPE type)
{
	switch (type)
	{
	case CItem::ITEM_TYPE_0:
		if (m_bTake[1] == false)
		{
			m_bTake[1] = true;
		}
		break;

	case CItem::ITEM_TYPE_1:
		if (m_bTake[2] == false)
		{
			m_bTake[2] = true;
		}
		break;

	case CItem::ITEM_TYPE_2:
		if (m_bTake[3] == false)
		{
			m_bTake[3] = true;
		}
		break;
	}
}