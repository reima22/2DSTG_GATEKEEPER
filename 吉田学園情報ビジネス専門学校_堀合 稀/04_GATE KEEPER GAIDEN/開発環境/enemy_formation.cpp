//==============================================================================
//
// �G�ґ����������kenemy_formation.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "game.h"
#include "enemy.h"
#include "enemy_formation.h"
#include "textdata_enemy.h"
#include "sound.h"
#include "effect.h"

// �ÓI�����o�ϐ��錾
int CEnemyFormation::m_nFormNumInterval = 0;		// ���񐶐��̌ʊԂ̊Ԋu
int CEnemyFormation::m_nFormSetInterval = 0;		// ���񂻂̂��̂̐����Ԋu
bool CEnemyFormation::m_bFormSetInt = false;		// ����𐶐����邩
D3DXVECTOR3 CEnemyFormation::m_pos;					// ����z�u�J�n�ʒu
D3DXVECTOR3 CEnemyFormation::m_moveRot;				// ����z�u�ړ�����
float CEnemyFormation::m_fMove;						// ����ړ���
CTexture::TEXTYPE CEnemyFormation::m_type;			// ����̎��
int CEnemyFormation::m_nInterval = 0;				// �������銴�o
bool CEnemyFormation::m_bSetParam = false;			// �p�����[�^��ݒ肷�邩
bool CEnemyFormation::m_bLock = false;				// �����̒��f

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemyFormation::CEnemyFormation()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemyFormation::~CEnemyFormation()
{

}

//==============================================================================
// �G�̕ґ���Ԑ���
//==============================================================================
void CEnemyFormation::SetFormationEnemy(D3DXVECTOR3 pos, OBJTYPE objType, CTexture::TEXTYPE type, D3DXVECTOR3 moveRot, float fMove, int nSetNum, int nInterval, TEAM_PATTERN team)
{
	// �ړ��{��
	moveRot.x *= fMove;
	moveRot.y *= fMove;

	// �e�����_�������J�E���g��l
	int nSetBulletMin;
	int nSetBulletDif;

	// ��͐펞
	if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2)
	{
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MOTHER_DIFFERENCE);
	}
	else
	{// �ʏ�
		nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);
	}

	// ���i�ړ��p�^�[��
	bool bUnder = false;
	if (team == TEAM_PATTERN_UNDER)
	{
		bUnder = true;
	}

	// ���[�J���ϐ��錾
	int nNumMax = 1;	// ��x�̍ő�z�u��

						// �z�u�p�^�[���ɂ��ݒ�
	if (team == TEAM_PATTERN_SIDE || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_PARTS)
	{
		nNumMax = 2;
	}
	else if (team == TEAM_PATTERN_MOTHER_2)
	{
		nNumMax = 3;
	}

	m_nFormNumInterval++;

	if (m_nFormNumInterval % nInterval == 0)
	{
		// ���[�J���ϐ��錾
		float fSizeX;
		float fSizeY;
		int nLife;
		ENEMY_TYPE enemyType;
		int nBulletDelay = rand() % nSetBulletDif + nSetBulletMin;

		// �e�p�[�c
		CEnemy *pLeft = NULL;
		CEnemy *pRight = NULL;
		CEnemy *pCenter = NULL;

		// ��ނ��Ƃ̐ݒ�
		switch (type)
		{
		case CTexture::TEXTYPE_ENEMY_0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			nLife = 1;
			enemyType = ENEMY_TYPE_0;
			break;

		case CTexture::TEXTYPE_ENEMY_1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			nLife = 2;
			enemyType = ENEMY_TYPE_1;
			break;

		case CTexture::TEXTYPE_ENEMY_2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			nLife = 3;
			enemyType = ENEMY_TYPE_2;
			break;

		case CTexture::TEXTYPE_ENEMY_3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			nLife = 4;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// ���[�J���ϐ��錾
		D3DXVECTOR3 posLeft;
		D3DXVECTOR3 posRight;
		D3DXVECTOR3 posCenter;

		int nLifeLeft = 0;
		int nLifeRight = 0;
		int nLifeCenter = 0;

		// ��͐�A�G�����G�t�F�N�g
		if (team == TEAM_PATTERN_MOTHER_0 || team == TEAM_PATTERN_MOTHER_1 || team == TEAM_PATTERN_MOTHER_2 || team == TEAM_PATTERN_MOTHER_PARTS)
		{
			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// �G�ł���
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_LEFT)
					{// ���p�[�c
					 // �ʒu�̎擾
						pLeft = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_RIGHT)
					{// �E�p�[�c
						pRight = pEnemy;
					}

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER_PARTS_CENTER)
					{// �����p�[�c
						pCenter = pEnemy;
					}
				}

				// ���̃|�C���^
				pScene = pSceneNext;
			}

			if (pLeft != NULL)
			{
				nLifeLeft = pLeft->GetLife(CHARA_TYPE_ENEMY);
				posLeft = pLeft->GetPosition();
			}

			if (pRight != NULL)
			{
				nLifeRight = pRight->GetLife(CHARA_TYPE_ENEMY);
				posRight = pRight->GetPosition();
			}

			if (pCenter != NULL)
			{
				nLifeCenter = pCenter->GetLife(CHARA_TYPE_ENEMY);
				posCenter = pCenter->GetPosition();
			}
		}

		// �����ݒ�
		if (team == TEAM_PATTERN_SIDE)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1)
				{// 1�̖�
					CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
				else
				{// 2�̖�
					CEnemy::Create(D3DXVECTOR3(pos.x + 1200.0f, pos.y, 0.0f), objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_0)
		{// �ʏ�ݒ�
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// �g��^�G�t�F�N�g
					CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_1)
		{
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

						for (int nCnt = 0; nCnt < 20; nCnt++)
						{
							// �g��^�G�t�F�N�g
							CEffect::Create(posCenter, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x + 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
							CEffect::Create(D3DXVECTOR3(posCenter.x - 10.0f, posCenter.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
						}
					}
					else
					{// 2�̖�

						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_2)
		{
			if (pCenter != NULL)
			{
				// ������
				CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

				for (int nCnt = 0; nCnt < 20; nCnt++)
				{
					// �g��^�G�t�F�N�g
					CEffect::Create(pos, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x + 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
					CEffect::Create(D3DXVECTOR3(pos.x - 10.0f, pos.y, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_UNDER);
				}

				for (int nNum = 0; nNum < nNumMax; nNum++)
				{
					if (nNum < 1)
					{// 1�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else if (nNum < 2)
					{// 2�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(0.0f, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
					else
					{// 3�̖�
						CEnemy::Create(posCenter, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, moveRot.z), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
					}
				}
			}
		}
		else if (team == TEAM_PATTERN_MOTHER_PARTS)
		{
			for (int nNum = 0; nNum < nNumMax; nNum++)
			{
				if (nNum < 1 && pLeft != NULL)
				{
					// ������
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posLeft, objType, fSizeX, fSizeY, type, D3DXVECTOR3(-moveRot.x, moveRot.y, 0.0f), nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// �g��^�G�t�F�N�g
						CEffect::Create(posLeft, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y + 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
						CEffect::Create(D3DXVECTOR3(posLeft.x, posLeft.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_LEFT);
					}
				}
				else if (nNum < 2 && pRight != NULL)
				{
					// ������
					CSound::Play(CSound::SOUND_LABEL_SE_MOTHER_CREATE);

					CEnemy::Create(posRight, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);

					for (int nCnt = 0; nCnt < 20; nCnt++)
					{
						// �g��^�G�t�F�N�g
						CEffect::Create(posRight, 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y + 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);
						CEffect::Create(D3DXVECTOR3(posRight.x, posRight.y - 10.0f, 0.0f), 0.009f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 5.0f, 0.01f, CEffect::EFFECT_TYPE_RIGHT);

					}
				}
			}
		}
		else
		{// �P��
			CEnemy::Create(pos, objType, fSizeX, fSizeY, type, moveRot, nLife, nBulletDelay, BULLET_PATTERN_AIM, bUnder, enemyType);
		}
	}

	if (m_nFormNumInterval >= nInterval * nSetNum || CGame::GetSetEnemy() == false)
	{
		// �Ґ������̃C���^�[�o����
		m_bFormSetInt = true;

		// ���������߂�
		m_bSetParam = false;

		// �J�E���g���Z�b�g
		m_nFormNumInterval = 0;
	}
}

//==============================================================================
// �G�̕ґ�������񃉃��_����
//==============================================================================
void CEnemyFormation::SetFormRandom(TEAM_PATTERN team)
{
	// ���[�J���ϐ��錾
	CEnemy *pEnemy = NULL;
	// ��͂̈ʒu�擾
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	switch (team)
	{
	case TEAM_PATTERN_RANDOM: // �����_���z�u

		if (m_bSetParam == false)
		{
			// �ʒu�̐ݒ�
			m_pos = D3DXVECTOR3((float)(rand() % 1200 + 40), ENEMY_SET_POS_Y, 0.0f);

			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// �ړ������E�͂̐ݒ�
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);

		break;

	case TEAM_PATTERN_SIDE:	// ���T�C�h�z�u

		if (m_bSetParam == false)
		{
			// �ʒu�̐ݒ�
			m_pos = D3DXVECTOR3(40.0f, ENEMY_SET_POS_Y, 0.0f);

			// ��ނ̐ݒ�
			m_type = CTexture::TEXTYPE_ENEMY_1;

			// �ړ������E�͂̐ݒ�
			m_moveRot = D3DXVECTOR3(0.0f, cosf(0.0f), 0.0f);
			m_fMove = 1.0f;

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u(���E)
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval, team);

		break;

	case TEAM_PATTERN_UNDER:

		if (m_bSetParam == false)
		{
			// ���E�̂ǂ���ɏo�邩
			bool bLeft = false;

			int nRand = rand() % 2;

			if (nRand == 0)
			{
				bLeft = true;
			}

			if (bLeft == true)
			{
				// �ʒu�̐ݒ�
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_L, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// �ړ������E�͂̐ݒ�
				m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}
			else
			{
				// �ʒu�̐ݒ�
				m_pos = D3DXVECTOR3(ENEMY_SET_POS_X_UNDER_R, ENEMY_SET_POS_Y_UNDER, 0.0f);

				// �ړ������E�͂̐ݒ�
				m_moveRot = D3DXVECTOR3(sinf(-D3DX_PI / 2), 0.0f, 0.0f);
				m_fMove = (float)(rand() % 10 + 10) / 10.0f;
			}

			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// ��������Ԋu
			m_nInterval = 60;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_0:

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

		// �ʒu�̐ݒ�
		m_pos = pos;

		if (m_bSetParam == false)
		{
			// ��ނ̐ݒ�
			m_type = (CTexture::TEXTYPE)(rand() % 4 + (int)(CTexture::TEXTYPE_ENEMY_0));

			// �ړ������E�͂̐ݒ�
			float fSinAngle = ((float)(rand() % 314 - 157) / 100.0f);
			m_moveRot = D3DXVECTOR3(sinf(fSinAngle), cosf(0.0f), 0.0f);
			m_fMove = (float)(rand() % 10 + 10) / 10.0f;

			// �ݒ芮��
			m_bSetParam = true;
		}

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 5, m_nInterval, team);
		break;

	case TEAM_PATTERN_MOTHER_1:
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

		// �ʒu�̐ݒ�
		m_pos = pos;

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_3;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), cosf(0.0f), 0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_2:

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

		// �ʒu�̐ݒ�
		m_pos = pos;

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_2;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2), cosf(0.0f), 0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 1, m_nInterval, team);

		break;

	case TEAM_PATTERN_MOTHER_PARTS:

		// ��ނ̐ݒ�
		m_type = CTexture::TEXTYPE_ENEMY_1;

		// ���ꂼ��̈ړ���
		m_moveRot = D3DXVECTOR3(sinf(D3DX_PI / 2) * 4.0f, 0.0f, 0.0f);

		// �ґ��z�u
		SetFormationEnemy(m_pos, OBJTYPE_ENEMY, m_type, m_moveRot, m_fMove, 3, m_nInterval, team);

		break;
	}

}

//==============================================================================
// �C���^�[�o���J�E���g
//==============================================================================
void CEnemyFormation::FormInterval(int nIntervalCnt)
{
	// �J�E���g���Z
	m_nFormSetInterval++;

	if (m_nFormSetInterval >= nIntervalCnt)
	{
		m_nFormSetInterval = 0;
		m_bFormSetInt = false;
	}
}