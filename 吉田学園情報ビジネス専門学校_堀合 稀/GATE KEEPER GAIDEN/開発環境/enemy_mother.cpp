//==============================================================================
//
// �G��͏����kenemy_mother.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "game.h"
#include "enemy.h"
#include "enemy_mother.h"
#include "textdata_enemy.h"
#include "sound.h"
#include "effect.h"
#include "chara3D.h"

// �ÓI�����o�ϐ��錾
CEnemyMother::MOTHER_STATE CEnemyMother::m_motherState = MOTHER_STATE_NONE;	// ��͂̏��
CEnemyMother::MOTHER_PHASE CEnemyMother::m_phase = MOTHER_PHASE_0;			// ��͐�̃t�F�[�Y
int CEnemyMother::m_nMotherStartLife;										// ��͂̍ő�̗�
CChara3D *CEnemyMother::m_pMother3D = NULL;									// �{�̂�3D���f��
CChara3D *CEnemyMother::m_pSubParts[MOTHER_PARTS_MAX] = {};					// �p�[�c��3D���f��

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemyMother::CEnemyMother()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemyMother::~CEnemyMother()
{

}

//==============================================================================
// ��͂̐���
//==============================================================================
void CEnemyMother::MotherSetCreate(void)
{
	// ��Փx���Ƃ̕�̗͑�
	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		m_nMotherStartLife = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_MOTHER_LIFE_HARD);
		break;
	}

	D3DXVECTOR3 pos = MOTHER_SET_POS;

	// �p�[�c�̗̑�
	int nLifeParts = 50;

	// �{��
	CEnemy::Create(pos, CScene::OBJTYPE_ENEMY, MOTHER_ENEMY_X, MOTHER_ENEMY_Y,
		CTexture::TEXTYPE_ENEMY_4_MOTHER,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_nMotherStartLife,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER);

	// ��
	CEnemy::Create(D3DXVECTOR3(pos.x - 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_LEFT);

	// �E
	CEnemy::Create(D3DXVECTOR3(pos.x + 120.0f, pos.y, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_RIGHT);

	// ����
	CEnemy::Create(D3DXVECTOR3(pos.x, pos.y + 80.0f, 0.0f), CScene::OBJTYPE_ENEMY, MOTHER_PARTS_SIZE, MOTHER_PARTS_SIZE,
		CTexture::TEXTYPE_MOTHER_PARTS,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		nLifeParts,
		0, CEnemy::BULLET_PATTERN_NONE, false, ENEMY_TYPE_MOTHER_PARTS_CENTER);

	// 3D���f��
	//m_pMother3D = CChara3D::Create(D3DXVECTOR3(0.0f, 90.0f, 0.0f),D3DXVECTOR3(0.0f,-0.2f,0.0f),0);

	//m_pSubParts[MOTHER_PARTS_LEFT] = CChara3D::Create(D3DXVECTOR3(-30.0f, 90.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
	//m_pSubParts[MOTHER_PARTS_RIGHT] = CChara3D::Create(D3DXVECTOR3(30.0f, 90.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
	//m_pSubParts[MOTHER_PARTS_CENTER] = CChara3D::Create(D3DXVECTOR3(0.0f, 70.0f, 0.0f), D3DXVECTOR3(0.0f, -0.2f, 0.0f), 0);
}

//==============================================================================
// ��̓p�[�c�̋���
//==============================================================================
void CEnemyMother::MoveParts(void)
{
	// ���[�J���ϐ��錾
	D3DXVECTOR3 posLeft = MOTHER_PARTS_POS_0;
	D3DXVECTOR3 posRight = MOTHER_PARTS_POS_1;
	D3DXVECTOR3 posCenter = MOTHER_PARTS_POS_2;

	// ��͂̏��
	// ��͂̈ʒu�擾
	D3DXVECTOR3 pos;
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	while (pScene)
	{
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// �G�ł���
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER)
			{// ��͂ł���
			 // �ʒu�̎擾
				pos = pEnemy->GetPosition();
				break;
			}
		}

		// ���̃|�C���^
		pScene = pSceneNext;
	}

	// �|�C���^�C��
	pScene = CScene::GetScene(PRIORITY_CHARA);
	pSceneNext = NULL;

	while (pScene)
	{
		// ���̃|�C���^
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{// �G�ł���
			CEnemy *pEnemy = (CEnemy*)pScene;

			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_LEFT)
			{
				posLeft = pos + posLeft;
				pEnemy->SetPosition(posLeft);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_RIGHT)
			{
				posRight = pos + posRight;
				pEnemy->SetPosition(posRight);
			}
			else if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_MOTHER_PARTS_CENTER)
			{
				posCenter = pos + posCenter;
				pEnemy->SetPosition(posCenter);
			}
		}

		// ���̃|�C���^
		pScene = pSceneNext;
	}
}