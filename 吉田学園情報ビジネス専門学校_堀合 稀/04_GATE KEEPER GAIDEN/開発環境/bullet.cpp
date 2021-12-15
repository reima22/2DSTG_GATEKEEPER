//==============================================================================
//
// �e�`�揈���kbullet.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "explosion.h"
#include "sound.h"
#include "effect.h"

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CBullet::CBullet(int nPriority) : CScene2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = BULLET_LIFE;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CBullet::~CBullet()
{

}

//==============================================================================
// �e�̏���������
//==============================================================================
HRESULT CBullet::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,D3DXVECTOR3 move, CTexture::TEXTYPE type, D3DXVECTOR3 rot,int nAttack)
{
	CScene2D::Init(pos);
	CScene2D::SetRot(rot);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);
	m_move = move;
	m_Type = type;
	m_BulletType = (BULLET_TYPE)type;
	m_aAttack = nAttack;

	return S_OK;
}

//==============================================================================
// �e�̏I������
//==============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �e�̍X�V����
//==============================================================================
void CBullet::Update(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *keyboard = CManager::GetInputKeyboard();

	// �ʒu���̎擾
	D3DXVECTOR3 posBullet = CScene2D::GetPosition();
	int nNumAll = CScene::GetNumAll();

	// �e�̈ړ�
	posBullet += m_move;

	// �ʒu��񔻒�
	if (posBullet.y < 0 || posBullet.y > SCREEN_HEIGHT || posBullet.x < 0 || posBullet.x > SCREEN_WIDTH)
	{
		Uninit();
		return;
	}

	// �����̌���
	//m_nLife--;

	// �����ɂ�����
	if (m_nLife <= 0)
	{
		CExplosion::Create(posBullet, CScene::OBJTYPE_EXPLOSION, 50.0f, 50.0f);
		Uninit();
	}

	// �e�̋O��
	SetEffectBullet(posBullet, m_Type);

	// �ʒu���̐ݒ�
	CScene2D::SetPosition(posBullet);

	CScene2D::Update();
}

//==============================================================================
// �e�̕`�揈��
//==============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �e�̐�������
//==============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,D3DXVECTOR3 move, CTexture::TEXTYPE type, CBullet::BULLET_SIDE side,D3DXVECTOR3 rot, int nAttack)
{
	// ���[�J���ϐ��錾
	CBullet *pBullet;
	pBullet = new CBullet;

	// ������
	if (pBullet != NULL)
	{
		pBullet->Init(pos, objType, fSizeX, fSizeY, move, type,rot,nAttack);
	}

	// �e�N�X�`�����蓖��
	pBullet->BindTexture(type);
	
	// �G�������
	pBullet->m_Side = side;

	// ���ˉ�
	CSound::Play(CSound::SOUND_LABEL_SE_SHOT000);

	return pBullet;
}

//==============================================================================
// �g�p�e�ɉ����Đ�������e�̃p�^�[�����ω�
//==============================================================================
void CBullet::CreateType(D3DXVECTOR3 pos, int nUse)
{
	switch (nUse)
	{
	case 0:	// �ʏ�e
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_A_X, BULLET_A_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case 1:	// �E�F�[�u�e
		Create(
			D3DXVECTOR3(pos.x + 30.0f, pos.y, 0.0f),
			CScene::OBJTYPE_BULLET,
			BULLET_B_X, BULLET_B_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);

		Create(
			D3DXVECTOR3(pos.x - 30.0f, pos.y, 0.0f),
			CScene::OBJTYPE_BULLET,
			BULLET_B_X, BULLET_B_Y,
			D3DXVECTOR3(0.0f, -10.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case 2:	// �~�T�C���e
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_C_X, BULLET_C_Y,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2.0f), 3);

		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_C_X, BULLET_C_Y,
			D3DXVECTOR3(10.0f, 0.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 2.0f), 3);
		break;

	case 3:	// ����E�F�[�u�e
		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_D_X, BULLET_D_Y,
			D3DXVECTOR3(sinf(D3DX_PI / 6.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 6.0f), 1);

		Create(
			pos,
			CScene::OBJTYPE_BULLET,
			BULLET_D_X, BULLET_D_Y,
			D3DXVECTOR3(sinf(-D3DX_PI / 6.0f) * 5.0f, cosf(0.0f) * 5.0f, 0.0f),
			(CTexture::TEXTYPE)(CTexture::TEXTYPE_BULLET_0 + nUse), BULLET_SIDE_PLAYER, D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI / 6.0f), 1);
		break;

	default:
		break;
	}
}

//==============================================================================
// �O�ՃG�t�F�N�g�̐ݒ�
//==============================================================================
void CBullet::SetEffectBullet(D3DXVECTOR3 pos, CTexture::TEXTYPE type)
{
	switch (type)
	{
	case CTexture::TEXTYPE_BULLET_0:	// �ʏ�e
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_1:	// �E�F�[�u�e
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.1f), 20.0f, 0.005f,0);
		break;

	case CTexture::TEXTYPE_BULLET_2:	// �~�T�C���e
		CEffect::Create(pos, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_3:	// ����E�F�[�u�e
		CEffect::Create(pos, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 20.0f, 0.1f,0);
		break;

	case CTexture::TEXTYPE_BULLET_ENEMY:
		CEffect::Create(pos, 0.0f, D3DXCOLOR(0.9f, 0.9f, 0.5f, 1.0f), 10.0f, 0.1f, 0);
		break;

	default:
		break;
	}
}

//==============================================================================
// �e�̑S�폜
//==============================================================================
void CBullet::UninitAll(void)
{
	// ���[�J���ϐ��錾
	CScene *pScene = CScene::GetScene(PRIORITY_BULLET);
	CScene *pSceneNext = NULL;

	// �S�Ă̓G�̎��S�t���O����
	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		CBullet *pBullet = (CBullet*)pScene;

		if (pBullet->GetSide() == CBullet::BULLET_SIDE_ENEMY)
		{
			pScene->Uninit();
		}

		pScene = pSceneNext;
	}
}