//==============================================================================
//
// �G�`�揈���kenemy.cpp�l
// Author : Mare Horiai
//
//==============================================================================
#include "main.h"
#include "scene2D.h"
#include "enemy.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "renderer.h"
#include "texture.h"
#include "item.h"
#include "score.h"
#include "defeat.h"
#include "effect.h"
#include "effect_on.h"
#include "game.h"
#include "textdata_enemy.h"
#include "caution.h"
#include "sound.h"
#include "enemy_formation.h"
#include "enemy_mother.h"

// �ÓI�����o�ϐ��錾
int CEnemy::m_nNumEnemy = 0;						// ����
int CEnemy::m_nSetEnemyCnt = 0;						// ���������ݒ�J�E���g
CEnemy::BULLET_PATTERN CEnemy::m_patternSet = 
BULLET_PATTERN_NORMAL;								// �e�̔��˃p�^�[��

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CEnemy::CEnemy()
{
	m_nNumEnemy++;		// �����̉��Z
	m_nBulletCnt = 0;
	m_nBulletSetDelay = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CEnemy::~CEnemy()
{

}

//==============================================================================
// �G�̏���������
//==============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY,int nBulletSetDelay)
{
	// �������ݒ�
	CScene2D::Init(pos);
	CScene2D::SetSize(fSizeX, fSizeY);
	CScene2D::SetTex(1, 1, 0, 0, 0.0f, 0.0f);
	CScene2D::SetObjType(objType);

	// �e���˃p�^�[���̐ݒ�
	SetBulletPattern();

	// ���ˊԊu�̐ݒ�
	m_nBulletSetDelay = nBulletSetDelay;

	// ��Ԑݒ�
	m_state = ENEMYSTATE_NORMAL;

	return S_OK;
}

//==============================================================================
// �G�̏I������
//==============================================================================
void CEnemy::Uninit(void)
{
	// �����̌���
	m_nNumEnemy--;

	CScene2D::Uninit();
}

//==============================================================================
// �G�̍X�V����
//==============================================================================
void CEnemy::Update(void)
{
	// �L�������̎擾
	D3DXVECTOR3 posEnemy = CScene2D::GetPosition();
	float fSizeCharaX = CScene2D::GetSize().x;
	float fSizeCharaY = CScene2D::GetSize().y;

	// �ړ�����
	posEnemy += m_Move;

	if (m_type == ENEMY_TYPE_MOTHER)
	{
		CEnemy::MoveMother(posEnemy);
	}

	CEnemyMother::MoveParts();

	// �e���˃J�E���g
	m_nBulletCnt++;

	// �e�̎�������
	if ( m_nBulletSetDelay != 0 && m_nBulletCnt % m_nBulletSetDelay == 0)
	{	
		AutoSetBullet(posEnemy, m_pattern);
	}

	// �e�Ƃ̓����蔻��
	CollisionBullet(posEnemy, fSizeCharaX, fSizeCharaY);

	// ��ʉ��[�ڐG�ɂ��ړ��̐܂�Ԃ�	
	if (m_bUnderSet == false)
	{// X���̔���L��
		if (posEnemy.x < fSizeCharaX || posEnemy.x > SCREEN_WIDTH - fSizeCharaX)
		{
			m_Move.x *= -1;
		}

		if (CGame::GetMotherEnemy() == true)
		{
			if (m_type == ENEMY_TYPE_1)
			{
				if (posEnemy.x < fSizeCharaX + 10.0f || posEnemy.x > SCREEN_WIDTH - fSizeCharaX - 10.0f)
				{
					m_Move.x = 0.0f;
					m_Move.y = 1.0f;
				}
			}
		}
	}
	else
	{// �܂�Ԃ������̏ꍇ�A����ʒu�ō~��
		if (m_Move.x > 0 && posEnemy.x >= 1240.0f)
		{// �E�ړ��̏ꍇ
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}
		else if (m_Move.x < 0 && posEnemy.x <= 40.0f)
		{// ���ړ��̏ꍇ
			m_Move = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		}		
	}

	// �X�e�[�^�X���
	EnemyState();

	// �ʒu�̐ݒ�
	CScene2D::SetPosition(posEnemy);

	// �X�V
	CScene2D::Update();
}

//==============================================================================
// �G�̕`�揈��
//==============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// �G�̐�������
//==============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, OBJTYPE objType, float fSizeX, float fSizeY, CTexture::TEXTYPE type, D3DXVECTOR3 move,int nLife,int nBulletSetDelay,BULLET_PATTERN pattern,bool bUnder,ENEMY_TYPE enemyType)
{
	// ���[�J���ϐ��錾
	CEnemy *pEnemy;
	pEnemy = new CEnemy;

	// �e���˃p�^�[���̈ꎞ�ۑ�
	m_patternSet = pattern;

	// ������
	if (pEnemy != NULL)
	{
		pEnemy->Init(pos, objType, fSizeX, fSizeY,nBulletSetDelay);
	}

	// �e�N�X�`�����蓖��
	pEnemy->BindTexture(type);

	// �L�����N�^�[�p�����[�^
	pEnemy->SetMove(move);
	pEnemy->SetLife(CHARA_TYPE_ENEMY,nLife);
	pEnemy->CharaSetType(CHARA_TYPE_ENEMY);

	pEnemy->m_bUnderSet = bUnder;
	pEnemy->m_type = enemyType;

	return pEnemy;
}

//==============================================================================
// �ʎ�����������
//==============================================================================
void CEnemy::AutoSetCreate(void)
{
	// �����_���ݒu�J�E���g��l
	int nSetMin = 0;
	int nSetDif = 0;

	float fSpeed = 0.0f;

	switch (CTitleDifficulty::GetDifficulty())
	{
	case CTitleDifficulty::DIFFICULTY_EASY:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_EASY);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_EASY);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_EASY);
		break;

	case CTitleDifficulty::DIFFICULTY_NORMAL:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_NORMAL);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_NORMAL);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_NORMAL);
		break;

	case CTitleDifficulty::DIFFICULTY_HARD:
		nSetMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_MIN_HARD);
		nSetDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_DIFFERENCE_HARD);
		fSpeed = CTextDataEnemy::SetSpeed(CTitleDifficulty::DIFFICULTY_HARD);
		break;
	}

	// �ݒu�J�E���g�̑���
	m_nSetEnemyCnt++;

	// �G�̎�������
	if (m_nSetEnemyCnt % nSetMin == 0)
	{
		// �z�u����X���̐ݒ�
		float fRandX = (float)(rand() % 1200 + 60);

		// ��������G�̃T�C�Y
		float fSizeX;
		float fSizeY;

		ENEMY_TYPE enemyType;

		// �G�̎�ސݒ�
		int nType = rand() % 4;

		fSpeed -= (float)nType / 10.0f;

		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeed, 0.0f);

		// �e�����_�������J�E���g��l
		int nSetBulletMin = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_MIN);
		int nSetBulletDif = CTextDataEnemy::SetData(CTextDataEnemy::ENEMYDATA_CREATE_BULLET_DIFFERENCE);

		// �e�̔��ˊԊu
		int nBulletCntSet = rand() % nSetBulletDif + nSetBulletMin;

		// �G�̃N���G�C�g
		switch (nType)
		{// �T�C�Y�̕���
		case 0:
			fSizeX = ENEMYX_0;
			fSizeY = ENEMYY_0;
			enemyType = ENEMY_TYPE_0;
			break;

		case 1:
			fSizeX = ENEMYX_1;
			fSizeY = ENEMYY_1;
			enemyType = ENEMY_TYPE_1;
			break;

		case 2:
			fSizeX = ENEMYX_2;
			fSizeY = ENEMYY_2;
			enemyType = ENEMY_TYPE_2;
			break;

		case 3:
			fSizeX = ENEMYX_3;
			fSizeY = ENEMYY_3;
			enemyType = ENEMY_TYPE_3;
			break;
		}

		// �G�̐���
		CEnemy::Create(D3DXVECTOR3(fRandX, ENEMY_SET_POS_Y, 0.0f),
			CScene::OBJTYPE_ENEMY,
			fSizeX, fSizeY, (CTexture::TEXTYPE)(CTexture::TEXTYPE_ENEMY_0 + nType),
			move, nType + 1, nBulletCntSet, BULLET_PATTERN_NORMAL, false,enemyType);

	}
}

//==============================================================================
// ��_���[�W�̏���
//==============================================================================
void CEnemy::DamageLife(int nDamage,D3DXVECTOR3 pos)
{
	// �_���[�W�v�Z
	m_nLife[CHARA_TYPE_ENEMY] -= nDamage;

	// ����
	if (m_nLife[CHARA_TYPE_ENEMY] <= 0)
	{// ���j
		// ���j�X�e�[�^�X��
		SetState(ENEMYSTATE_DEATH);
	}
	else
	{// ��_���[�W
		// �T�E���h�Đ�
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// ��_���[�W�X�e�[�^�X��
		SetState(ENEMYSTATE_DAMAGE);
	}
}

//==============================================================================
// STATE���̏���
//==============================================================================
void CEnemy::EnemyState(void)
{
	// �G�̈ʒu
	D3DXVECTOR3 posEnemy = GetPosition();

	m_nStateCnt++;

	switch (m_state)
	{
	case ENEMYSTATE_NORMAL:	// �ʏ펞
		CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nStateCnt = 0;
		break;

	case ENEMYSTATE_DAMAGE:	// ��_���[�W
		if (m_nStateCnt <= 9)
		{
			CScene2D::SetCol(D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_state = ENEMYSTATE_NORMAL;
			m_nStateCnt = 0;
		}
		break;

	case ENEMYSTATE_DEATH:	// ���C�t�r��		
		m_nStateCnt = 0;

		// ������
		CSound::Play(CSound::SOUND_LABEL_SE_EXPLOSION000);

		// �X�R�A�̉��Z
		if (m_type == ENEMY_TYPE_MOTHER)
		{// ��͂̌��j
			for (int nCnt = 0; nCnt < 100; nCnt++)
			{
				CEffect::Create(posEnemy, 0.08f, D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f), 20.0f, 0.01f, 0);
			}

			CScore::AddScore(30000);
			
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_END);
		}
		else
		{// ���G�̌��j
			CScore::AddScore(10000);

			// ���j���̉��Z
			CDefeat::AddDefeat();

			// �A�C�e������
			CItem::RandCreate(posEnemy);

			for (int nCnt = 0; nCnt < 50; nCnt++)
			{
				CEffect::Create(posEnemy, 0.05f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 0.02f, 0);
			}
		}
		
		Uninit();
		break;

	default:
		break;
	}
}

//==============================================================================
// �G�̒e��������
//==============================================================================
void CEnemy::AutoSetBullet(D3DXVECTOR3 pos,BULLET_PATTERN pattern)
{
	// ���˂������
	switch (pattern)
	{
	case BULLET_PATTERN_NONE:	//	�����Ȃ�
		break;

	case BULLET_PATTERN_NORMAL:	// �^���֌���
		CBullet::Create(pos, CScene::OBJTYPE_BULLET, 
			BULLET_A_X, BULLET_A_Y, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1);
		break;

	case BULLET_PATTERN_AIM:	// �v���C���[��_��
		// �v���C���[�̈ʒu�擾
		D3DXVECTOR3 posPlayer = CScene::GetScene(PRIORITY_CHARA)->GetPosition();

		// �v���C���[�ւ̕���
		D3DXVECTOR3 vecPlayer = posPlayer - pos;

		// ���������߂�
		float fLength = (float)sqrt((vecPlayer.x * vecPlayer.x + vecPlayer.y * vecPlayer.y));

		// �p�x
		float fAngle = atan2f(vecPlayer.x, vecPlayer.y);

		// �����𒲐�
		vecPlayer.x = vecPlayer.x / fLength;
		vecPlayer.y = vecPlayer.y / fLength;

		// �������w��
		vecPlayer.x *= BULLET_AIM_SPEED;
		vecPlayer.y *= BULLET_AIM_SPEED;

		CBullet::Create(pos, CScene::OBJTYPE_BULLET,
			BULLET_A_X, BULLET_A_Y, vecPlayer,
			CTexture::TEXTYPE_BULLET_ENEMY, CBullet::BULLET_SIDE_ENEMY,
			D3DXVECTOR3(0.0f, 0.0f, fAngle), 1);
		break;
	}
}

//==============================================================================
// �c������G�̑S�j��
//==============================================================================
void CEnemy::UninitAll(void)
{
	// ���[�J���ϐ��錾
	CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
	CScene *pSceneNext = NULL;

	// �S�Ă̓G�̎��S�t���O����
	while (pScene)
	{
		pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetObjType() == OBJTYPE_ENEMY)
		{
			pScene->Uninit();
		}

		pScene = pSceneNext;
	}
}

//==============================================================================
// ��͂̋���
//==============================================================================
void CEnemy::MoveMother(D3DXVECTOR3 pos)
{
	// ��͂̏o�����
	CEnemyMother::MOTHER_STATE state = CEnemyMother::GetMotherState();

	// ��Ԃɂ��ω�
	switch (state)
	{
	case CEnemyMother::MOTHER_STATE_NONE:
		// �x���̏I��
		if (CCaution::GetCautionState() == CCaution::CAUTION_STATE_ALERT_END)
		{
			// ��Ԃ̕ω�
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_START);
		}

		break;

	case CEnemyMother::MOTHER_STATE_START:
		// �ړ��͂̐ݒ�
		m_Move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		if (pos.y >= MOTHER_POS_Y)
		{
			// ��Ԃ̕ω�
			m_Move = D3DXVECTOR3(MOTHER_MOVE_0, 0.0f, 0.0f);			
			CEnemyFormation::SetParamReset();	// ������̃��Z�b�g
			CEnemyMother::SetMotherState(CEnemyMother::MOTHER_STATE_ACTIVE);
		}
		break;

	case CEnemyMother::MOTHER_STATE_ACTIVE:
		if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() * 3.0f / 4.0f)
		{
			// �����C���^�[�o��
			CEnemyFormation::SetInterval(100);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_0);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 2.0f)
		{// �̗�50%�܂�
			// �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_0)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_1);

				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_1;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_1;
				}
			}

			CEnemyFormation::SetInterval(110);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_1);
		}
		else if (m_nLife[CHARA_TYPE_ENEMY] >= CEnemyMother::GetLife() / 4.0f)
		{// �̗�25%�܂�
		 // �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_1)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_2);

				// �ړ��͂̉���
				if (m_Move.x > 0)
				{
					m_Move.x = MOTHER_MOVE_2;
				}
				else
				{
					m_Move.x = -MOTHER_MOVE_2;
				}
			}

			// �C���^�[�o���̐ݒ�
			CEnemyFormation::SetInterval(120);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_2);
		}
		else
		{// �̗�25%�ȍ~
		 // �t�F�[�Y�V�t�g
			if (CEnemyMother::GetPhase() == CEnemyMother::MOTHER_PHASE_2)
			{
				CEnemyMother::SetPhase(CEnemyMother::MOTHER_PHASE_3);
			}

			CScene *pScene = CScene::GetScene(PRIORITY_CHARA);
			CScene *pSceneNext = NULL;

			while (pScene)
			{
				pSceneNext = CScene::GetSceneNext(pScene);

				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{
					CEnemy *pEnemy = (CEnemy*)pScene;

					if (pEnemy->GetEnemyType() == ENEMY_TYPE_MOTHER)
					{
						if (pEnemy->GetPosition().x < 650.0f && pEnemy->GetPosition().x > 630.0f)
						{
							pEnemy->SetPosition(D3DXVECTOR3(640.0f, pEnemy->GetPosition().y, 0.0f));
							pEnemy->m_Move.x = 0.0f;
						}					
					}
				}

				pScene = pSceneNext;
			}

			// �C���^�[�o���̐ݒ�
			CEnemyFormation::SetInterval(130);
			CEnemyFormation::SetFormRandom(CEnemyFormation::TEAM_PATTERN_MOTHER_PARTS);
		}

		break;
	}
}